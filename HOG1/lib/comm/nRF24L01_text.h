//*************************************************************************************
/** \file nRF24L01_text.h
 *    This file contains a class which operates a Nordic nRF24L01 based radio module
 *    in text mode. The radio is attached to an AVR processor through the AVR's SPI 
 *    port. In this context, "text mode" means that the radio will send characters and 
 *    character strings as one would send strings through a serial line to a terminal, 
 *    using the emstream class structure with its "<<" operator, as opposed to 
 *    sending packets of binary data with error checking. 
 *
 *  Revised:
 *    \li 04-22-2007 MNL Original file by Minh-Nhat Le
 *    \li 04-26-2007 MNL Added a bunch of functions. Transfer verified working
 *    \li 05-09-2007 MNL Changed so that setup() doesn't have to be called by user
 *    \li 11-15-2007 SCH Overhauled to properly control radios and increase speed
 *    \li 01-26-2007 SCH Fixed more major flaws
 *    \li 02-15-2008 JRR Text based version, written for use on ME405 boards
 *    \li 02-17-2008 JRR nRF24L01_base split off from nRF24L01_text
 *    \li 05-21-2008 JRR Added code for second revision of ME405 boards (V0.60)
 *    \li 10-11-2008 JRR Added code for the Swoop sensor board with ATmega324PV
 *    \li 10-11-2008 JRR Added control for enhanced/broadcast mode, long addresses
 *    \li 10-11-2008 JRR Changed SPI port from bit-banged to SPI hardware
 *    \li 11-04-2012 JRR Made compatible with latest ME405/ME507 and FreeRTOS stuff
 * 
 *  License:
 *    This file is copyright 2012 by JR Ridgely and released under the Lesser GNU 
 *    Public License, version 2. It intended for educational use only, but its use
 *    is not limited thereto. */
/*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 *    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 *    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUEN-
 *    TIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 *    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 *    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 *    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */
//*************************************************************************************

// These defines prevent this file from being included more than once in a *.cpp file
#ifndef _NRF24L01_TEXT_H_
#define _NRF24L01_TEXT_H_

#include <avr/io.h>                         // Definitions of SFR's and such

#include "circ_buffer.h"                    // Template header for circular buffer
#include "emstream.h"                       // Header for base serial devices
#include "nRF24L01_base.h"                  // Header for base nRF24L01 radio driver


/** This define specifies a character which causes everything in the transmission 
 *  buffer to be sent immediately. A common example would be a newline character.
 */
#define nRF_SEND_NOW_CODE	'\n'


/** This is the size of the queue which holds characters received from the radio. The
 *  radio's maximum payload size is 32 bytes, so this queue should be larger than 
 *  that; if packets are expected to come in quickly much larger might be a good idea.
 */
const uint8_t nRF_QUEUE_SIZE = 36;


/** \cond NO_DOX_HERE This portion of the code doesn't need to be in the user manual.
 *  This define chooses a version of the ME405 board to use. The older V0.5A boards
 *  with one motor driver need ME405_BOARD_V05, and the newer V0.60 boards with dual
 *  motor drivers are signified by ME405_BOARD_V06. Choose only one of these defines.
 */
#if (defined ME405_BOARD_V05)
	#define nRF24_IRQ_VECT  INT7_vect       ///< The radio RX interrupt vector
	#define nRF24_IRQ_CBIT  ISC71           ///< Bit sets IRQ in falling edge mode
	#define nRF24_IRQ_MASK  INT7            ///< Mask to enable radio interrupts
	#define nRF24_EICR      EICRB           ///< Interrupt mode control register
	#define nRF24_EIMSK     EIMSK           ///< External interrupt mask name
#elif (defined ME405_BOARD_V06)
	#define nRF24_IRQ_VECT  INT0_vect       ///< The radio RX interrupt vector
	#define nRF24_IRQ_CBIT  ISC01           ///< Bit sets IRQ in falling edge mode
	#define nRF24_IRQ_MASK  INT0            ///< Mask to enable radio interrupts
	#define nRF24_EICR      EICRA           ///< Interrupt mode control register
	#define nRF24_EIMSK     EIMSK           ///< External interrupt mask name
#elif (defined SWOOP_BOARD)                 // With an ATmega324P or 644P or 1284P
	#define nRF24_IRQ_VECT  INT2_vect       ///< The radio RX interrupt vector
	#define nRF24_IRQ_CBIT  ISC21           ///< Bit sets IRQ in falling edge mode
	#define nRF24_IRQ_MASK  INT2            ///< Mask to enable radio interrupts
	#define nRF24_EICR      EICRA           ///< Interrupt mode control register
	#define nRF24_EIMSK     EIMSK           ///< External interrupt mask name
#else // (defined ME405_BREADBOARD)         // With an old leftover ATmega32
	#define nRF24_IRQ_VECT  INT2_vect       ///< The radio RX interrupt vector
	#define nRF24_IRQ_MASK  INT2            ///< Mask to enable radio interrupts
	#ifdef EIMSK                            // In case there's a Mega644P there
		#define nRF24_IRQ_CBIT  ISC21       ///< Bit sets IRQ in falling edge mode
		#define nRF24_EICR      EICRA       ///< Interrupt mode control register
		#define nRF24_EIMSK     EIMSK       ///< External interrupt mask name
	#else                                   // The ATmega32 again
		#define nRF24_IRQ_CBIT  8           ///< Do nothing -> falling edge mode
		#define nRF24_EICR      MCUCR       ///< Interrupt mode control register
		#define nRF24_EIMSK     GICR        ///< External interrupt mask name
	#endif
#endif

/// @endcond NO_DOX_HERE


//-------------------------------------------------------------------------------------
/** \brief This class operates a radio module based on a Nordic nRF24L01 chip in text 
 *  mode.
 * 
 *  \details In this mode, the radio sends and receives character strings, acting as 
 *  similarly as possible to a simple serial cable connected between two endpoints. 
 *
 *  Lines which are connected to the radio on the ME405 board, in addition to the 
 *  usual Vcc and ground, are as follows. Pin names are on the radio first, the 
 *  CPU pin second; CPU pins are listed by board, ME405 V0.5A/ME405 V0.60/Swoop:
 *    \li MISO  - MISO      - SPI data, AVR to radio
 *    \li MOSI  - MOSI      - SPI data, radio to AVR
 *    \li SCK   - SCK       - SPI clock line
 *    \li CSN   - SS        - SPI slave select
 *    \li CE    - E7/D1/B3  - Chip enable selects transmit or receive mode
 *    \li IRQ   - E6/E0/B2  - Interrupt (radio interrupts CPU when data is received)
 * 
 *  \section Usage
 *  This radio driver is designed to be, as much as possible, a drop-in replacement
 *  for a serial port. In order to make it work that way, one must first instantiate
 *  an instance of class nRF24L01_text, then reset the radio and configure it:
 *  \code
 *  nRF24L01_text* p_my_radio = new nRF24L01_text (p_serial);
 *  p_my_radio->reset ();
 *  p_my_radio->set_RX_address (0xA4, 0x05, 0x01, 0);
 *  p_my_radio->set_TX_address (0xA4, 0x05, 0x01);
 *  \endcode
 *  If the parameter for the \c nRF24L01_text constructor is omitted, the radio driver
 *  will not use a serial port to print debugging information if something goes wrong,
 *  but it will otherwise operate normally. The TX and RX addresses of the sending and
 *  receiving radios must match for transmissions to be received by the receiver. 
 * 
 *  Data is then transmitted in the usual way for a descendent of \c emstream:
 *  \code
 *  *p_my_radio << "Hi, I'm a radio and I'm " << age << " years old." << endl;
 *  \endcode
 *  In this example, we used the \c new operator, so \c my_radio is a pointer. If one
 *  just creates a radio object without using new, the pointer isn't needed:
 *  \code
 *  nRF24L01_text a_radio (p_serial_port);
 *  ...
 *  a_radio << "Hello" << endl;
 *  \endcode
 *  The following example shows how complex it often \b isn't to use this class. Put
 *  somewhere in a task loop, it watches for characters which have come in through the
 *  radio and relays those characters directly to another serial device, probably a
 *  regular USB-serial port of class \c rs232. Of course, \c p_serial could be any
 *  descendent of \c emstream, so it could be an SD card logging radio chatter,
 *  another radio through which information is being relayed, an LCD display, or 
 *  whatever. 
 *  \code
 *  if (my_radio->check_for_char ())
 *  {
 *      char_in = my_radio->getchar ();
 *      p_serial->putchar (char_in);
 *  }
 *  \endcode
 */

class nRF24L01_text: public nRF24L01_base, public emstream
{
	// Private data and methods are accessible only from within this class and 
	// cannot be accessed from outside -- even from descendents of this class
	private:

	// Protected data and methods are accessible from this class and its descendents
	protected:

	// Public methods can be called from anywhere in the program where there is a 
	// pointer or reference to an object of this class
	public:
		// The constructor sets up the radio interface
		nRF24L01_text (emstream* = NULL);

		bool putchar (char);                // Write one character to serial port
		bool check_for_char (void);         // Check if a character is in the buffer
		int16_t getchar (void);             // Get a character; wait if none is ready
};

#endif // _NRF24L01_TEXT_H_


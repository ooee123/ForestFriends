//*************************************************************************************
/** \file rs232int.h
 *    This file contains a class which allows the use of a serial port on an AVR 
 *    microcontroller. This version of the class uses the serial port receiver 
 *    interrupt and a buffer to allow characters to be received in the background.
 *    The port is used in "text mode"; that is, the information which is sent and 
 *    received is expected to be plain ASCII text, and the set of overloaded left-shift 
 *    operators "<<" in emstream.* can be used to easily send all sorts of data 
 *    to the serial port in a manner similar to iostreams (like "cout") in regular C++. 
 *
 *  Revised:
 *    \li 04-03-2006 JRR For updated version of compiler
 *    \li 06-10-2006 JRR Ported from C++ to C for use with some C-only projects; also
 *        serial_avr.h now has defines for compatibility among lots of AVR variants
 *    \li 08-11-2006 JRR Some bug fixes
 *    \li 03-02-2007 JRR Ported back to C++. I've had it with the limitations of C.
 *    \li 04-16-2007 JO  Added write (unsigned long)
 *    \li 07-19-2007 JRR Changed some character return values to bool, added m324p
 *    \li 01-12-2008 JRR Added code for the ATmega128 using USART number 1 only
 *    \li 02-14-2008 JRR Split between emstream and rs232 files
 *    \li 05-31-2008 JRR Changed baud calculations to use CPU_FREQ_MHz from Makefile
 *    \li 06-01-2008 JRR Added getch_tout() because it's needed by 9Xstream modems
 *    \li 07-05-2008 JRR Changed from 1 to 2 stop bits to placate finicky receivers
 *    \li 12-22-2008 JRR Split off stuff in base232.h for efficiency
 *    \li 06-30-2009 JRR Received data interrupt and buffer added
 *
 *  License:
 *		This file is copyright 2012 by JR Ridgely and released under the Lesser GNU 
 *		Public License, version 2. It intended for educational use only, but its use
 *		is not limited thereto. */
/*		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *		AND	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * 		IMPLIED 	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * 		ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * 		LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUEN-
 * 		TIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 * 		OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * 		CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * 		OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * 		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */
//*************************************************************************************

/// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _RS232_H_
#define _RS232_H_

#include <avr/interrupt.h>					// Header for AVR interrupt programming
#include "base232.h"						// Grab the base RS232-style header file
#include "emstream.h"				// Pull in the base class header file


// If the chip for which we are compiling has dual serial ports, define two character
// received interrupts, one for each port. This is for ATmega128 and 324P and similar
// chips. Otherwise, only define one character received interrupt; the other one won't
// be compiled (see near the end of rs232int.cpp). 
#if defined USART_RXC_vect
	#define RSI_CHAR_RECV_INT_0 USART_RXC_vect
#elif defined USART_RX_vect
	#define RSI_CHAR_RECV_INT_0 USART_RX_vect
#elif defined USART_RXC0_vect
	#define RSI_CHAR_RECV_INT_0 USART_RXC0_vect
#elif defined USART0_RX_vect
	#define RSI_CHAR_RECV_INT_0 USART0_RX_vect 
#else
	#error Unable to determine character received interrupt vector for this chip
#endif

#if defined UCSR1A
	#if defined USART1_RX_vect
		#define RSI_CHAR_RECV_INT_1 USART1_RX_vect
	#elif defined USART1_RXC_vect
		#define RSI_CHAR_RECV_INT_1 USART1_RXC_vect
	#endif
#endif

/** This is the size of the buffer which holds characters received by the serial port.
 *  It is usually set to something fairly large (~100 bytes) so that we don't miss
 *  incoming characters. However, when run on an AVR with very little RAM such as an
 *  ATmega8, ATmega32, ATmega324P or similar, it should usually be set smaller, for
 *  example 20 ~ 30 bytes or so. 
 */
#define RSINT_BUF_SIZE		32


//-------------------------------------------------------------------------------------
/** \brief This class controls a UART (Universal Asynchronous Receiver Transmitter), 
 *  a common asynchronous serial interface used in microcontrollers, and allows the
 *  use of an overloaded \c << operator to print things in \c cout style. 
 * 
 *  \details It talks to old-style RS232 serial ports (through a voltage converter chip 
 *  such as a MAX232) or through a USB to serial converter such as a FT232RL chip. The 
 *  UART (or USART on a newer AVR chip) is also sometimes used to communicate directly 
 *  with other microcontrollers, sensors, or wireless modems. 
 * 
 *  In the version in files \c rs232int.* this class installs an interrupt service
 *  routine (ISR) for receiving characters. When characters arrive in the UART, they
 *  are placed in a buffer whose size is configurable with the macro \c RSINT_BUF_SIZE.
 *  Calls to \c getchar() will check the buffer for received characters. This method,
 *  as opposed to polling the receiver without using interrupts, allows much higher
 *  data rates to be reliably supported in a multitasking program. Sending of 
 *  characters is currently not interrupt based. 
 * 
 *  \section Usage
 *  To create and use a serial port driver object requires only code such as the
 *  following:
 *  \code
 *  rs232 ser_port (9600, 0);
 *  ser_port << "Hello from test program number " << prog_num << endl;
 *  \endcode
 *  The first parameter to the \c rs232 constructor is the baud rate; 9600 is by far 
 *  the most commonly used. The second parameter is the USART number; use 0 for AVR's
 *  which have only one UART or USART. On AVR's which have a second USART, it is
 *  possible to have two \c rs232 objects, one on USART 0 and one on USART 1. 
 *  The third USART on some AVR's is currently not supported until the author gets a
 *  three-USART chip on which to test new code. 
 */

class rs232 : public emstream, public base232
{
	// This protected data can only be accessed from this class or its descendents
	protected:
		uint8_t port_num;					///< The USART number, 0 or 1

	// Public methods can be called from anywhere in the program where there is a 
	// pointer or reference to an object of this class
	public:
		// The constructor sets up the UART, saving its baud rate and port number
		rs232 (uint16_t = 9600, uint8_t = 0);

		// This method writes one character to the serial port.
		bool putchar (char);

		bool check_for_char (void);			// Check if a character is in the buffer
		int16_t getchar (void);				// Get a character; wait if none is ready
		void clear_screen (void);			// Send the 'clear display screen' code
};

#endif  // _RS232_H_

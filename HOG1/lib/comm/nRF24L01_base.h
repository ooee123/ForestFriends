//*************************************************************************************
/** \file nRF24L01_base.h
 *    This file contains a class full of utility functions to work with a Nordic 
 *    nRF24L01 based radio module. One wouldn't want to instantiate an object of this
 *    class; it's meant to be used as a base class for class nRT24L01_text and maybe 
 *    a packet based class to be used in the future. 
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
 *    \li 11-04-2012 JRR Made compatible with latest ME405/ME507 and FreeRTOS stuff;
 *                       moved pin/port specification to defines instead of variables
 *    \li 11-30-2012 JRR Changed address width to 3 bytes; 16.7 million is enough
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

/// These defines prevent this file from being included more than once in a *.cc file
#ifndef _NRF24L01_BASE_H_
#define _NRF24L01_BASE_H_

#include <stdlib.h>                         // Standard C/C++ library stuff
#include "emstream.h"                       // Header for base serial devices


/** This constant sets the number of bytes in the address being used by the radio. It
 *  can be 3, 4, or 5.
 */
#define nRF24_ADDR_WIDTH    3

#if (nRF24_ADDR_WIDTH < 3 || nRF24_ADDR_WIDTH > 5)
	#error Address width for nRF24L01 radio must be 3, 4, or 5 bytes
#endif


/** @cond NOT_TO_DOCUMENT The following defines don't need to be in Doxygen documents
 *                        because they're only used internally within this class
 */

/*  These defines choose a version of the ME405 board to use. The old ME405 boards
 *  with one motor driver need ME405_BOARD_V05, and the newer ME405 boards with dual
 *  motor drivers are signified by ME405_BOARD_V06. A Swoop data acquisition board or
 *  a breadboard layout using the same pins as the Swoop can also be chosen. Only one
 *  of these board defines may be used at any time. 
 */
#if (defined ME405_BOARD_V05)
	/// This is the port output latch register for the SPI port pins.
	#define nRF_PORT_SPI    PORTB

	/// This is the data direction register for the SPI port pins. 
	#define nRF_DDR_SPI     DDRB

	/// This is the number of the MISO pin within the I/O port whose pin it shares.
	#define nRF_MASK_MISO   (1 << 3)

	/// This is the number of the MISO pin within the I/O port whose pin it shares.
	#define nRF_MASK_MOSI   (1 << 2)

	/// This is the pin number of the SCK pin on the I/O port whose pin it shares.
	#define nRF_MASK_SCK    (1 << 1)

	/// This is the I/O port for the Slave Select line (SS on CPU, CSN on radio).
	#define nRF_PORT_SS     PORTB

	/// This is the Data Direction Register for the Slave Select line.
	#define nRF_DDR_SS      DDRB

	/// This is a bitmask for the Slave Select (SS on CPU, CSN on the radio) bit.
	#define nRF_MASK_SS     (1 << 0)

	/// This is the I/O port for the Chip Enable (CE) line
	#define nRF_PORT_CE     PORTE

	/// This is the Data Direction Register for the Chip Enable line.
	#define nRF_DDR_CE      DDRE

	/// This bitmask has a 1 for the Chip Enable (CE) line.
	#define nRF_MASK_CE     (1 << 6)

#elif (defined ME405_BOARD_V06)
	/// This is the port output latch register for the SPI port pins.
	#define nRF_PORT_SPI    PORTB

	/// This is the data direction register for the SPI port pins. 
	#define nRF_DDR_SPI     DDRB

	/// This is the number of the MISO pin within the I/O port whose pin it shares.
	#define nRF_MASK_MISO   (1 << 3)

	/// This is the number of the MISO pin within the I/O port whose pin it shares.
	#define nRF_MASK_MOSI   (1 << 2)

	/// This is the pin number of the SCK pin on the I/O port whose pin it shares.
	#define nRF_MASK_SCK    (1 << 1)

	/// This is the I/O port for the Slave Select line (SS on CPU, CSN on radio).
	#define nRF_PORT_SS     PORTB

	/// This is the Data Direction Register for the Slave Select line.
	#define nRF_DDR_SS      DDRB

	/// This is a bitmask for the Slave Select (SS on CPU, CSN on the radio) bit.
	#define nRF_MASK_SS     (1 << 0)

	/// This is the I/O port for the Chip Enable (CE) line
	#define nRF_PORT_CE     PORTD

	/// This is the Data Direction Register for the Chip Enable line.
	#define nRF_DDR_CE      DDRD

	/// This bitmask has a 1 for the Chip Enable (CE) line.
	#define nRF_MASK_CE     (1 << 1)

#else // We're assuming (defined SWOOP_BOARD || defined ME405_BREADBOARD)
	/// This is the port output latch register for the SPI port pins.
	#define nRF_PORT_SPI    PORTB

	/// This is the data direction register for the SPI port pins. 
	#define nRF_DDR_SPI     DDRB

	/// This is the number of the MISO pin within the I/O port whose pin it shares.
	#define nRF_MASK_MISO   (1 << 6)

	/// This is the number of the MISO pin within the I/O port whose pin it shares.
	#define nRF_MASK_MOSI   (1 << 5)

	/// This is the pin number of the SCK pin on the I/O port whose pin it shares.
	#define nRF_MASK_SCK    (1 << 7)

	/// This is the I/O port for the Slave Select line (SS on CPU, CSN on radio).
	#define nRF_PORT_SS     PORTB

	/// This is the Data Direction Register for the Slave Select line.
	#define nRF_DDR_SS      DDRB

	/// This is a bitmask for the Slave Select (SS on CPU, CSN on the radio) bit.
	#define nRF_MASK_SS     (1 << 4)

	/// This is the I/O port for the Chip Enable (CE) line
	#define nRF_PORT_CE     PORTB

	/// This is the Data Direction Register for the Chip Enable line.
	#define nRF_DDR_CE      DDRB

	/// This bitmask has a 1 for the Chip Enable (CE) line.
	#define nRF_MASK_CE     (1 << 3)

#endif


// This value determines how long the CPU waits for a successful transmission
const uint8_t nRF24_SEND_TIMEOUT = 250;

// These codes mask the interrupt sources in the nRF24L01
const uint8_t nRF24_INT_RX   = 0b01000000;   // Mask (stop) RX data arrived interrupt
const uint8_t nRF24_INT_TX   = 0b00100000;   // Mask data successfully send interrupt
const uint8_t nRF24_INT_M_RT = 0b00010000;   // Mask send-has-failed interrupt

// These constants are bitmasks used to turn CRC's on and choose 1 or 2 byte CRC's
const uint8_t nRF24_CRC_ON     = 0b00001000;
const uint8_t nRF24_CRC_OFF    = 0b00000000;
const uint8_t nRF24_CRC_1_BYTE = 0b00000000;
const uint8_t nRF24_CRC_2_BYTE = 0b00000100;

// These constants can be used to set the radio's data rate and power output
const uint8_t nRF24_DR_1M      = 0b00000000;
const uint8_t nRF24_DR_2M      = 0b00001000;
const uint8_t nRF24_RF_POW_0   = 0b00000110;
const uint8_t nRF24_RF_POW_N6  = 0b00000100;
const uint8_t nRF24_RF_POW_N12 = 0b00000010;
const uint8_t nRF24_RF_POW_N18 = 0b00000000;
const uint8_t nRF24_LNA_HCURR  = 0b00000001;

// These are commands to be sent to the radio
const uint8_t nRF24_RD_REG       = 0b00000000;   // Command to read a register
const uint8_t nRF24_WR_REG       = 0b00100000;   // Command to write a register
const uint8_t nRF24_RD_PLD       = 0b01100001;   // Command to read payload buffer
const uint8_t nRF24_WR_PLD       = 0b10100000;   // Command to write payload buffer
const uint8_t nRF24_FLUSH_TX     = 0b11100001;   // Command flushes transmitter buffer
const uint8_t nRF24_FLUSH_RX     = 0b11100010;   // Command to flush receiver buffer
const uint8_t nRF24_REUSE_TX_PLD = 0b11100011;   // Command to resend transmitted data
const uint8_t nRF24_NOP          = 0b11111111;   // NOP command is used to get status

// REG_ADDR - Addresses of registers to which to send commands
const uint8_t nRF24_REG_CONF        = 0x00;  // Address of configuration register
const uint8_t nRF24_REG_EN_AA       = 0x01;  // Register enables Auto-Ack for pipes
const uint8_t nRF24_REG_EN_RXADDR   = 0x02;  // Register enables RX addresses (pipes)
const uint8_t nRF24_REG_SETUP_AW    = 0x03;  // Address width (3 - 5 bytes) register
const uint8_t nRF24_REG_SETUP_RETR  = 0x04;  // Setup retransmit register
const uint8_t nRF24_REG_RF_CH       = 0x05;  // RF Channel register
const uint8_t nRF24_REG_RF_SETUP    = 0x06;  // RF Setup register
const uint8_t nRF24_REG_STATUS      = 0x07;  // Status register always returned by SPI
const uint8_t nRF24_REG_OBS_TX      = 0x08;  // Observe transmission register
const uint8_t nRF24_REG_CD          = 0x09;  // Carrier Detect (1 bit) status register
const uint8_t nRF24_REG_RX_ADDR_P0  = 0x0A;  // Address for Pipe 0 (5 bytes)
const uint8_t nRF24_REG_RX_ADDR_P1  = 0x0B;  // Address for Pipe 1 (5 bytes)
const uint8_t nRF24_REG_RX_ADDR_P2  = 0x0C;  // Address for Pipe 2 (1 byte)
const uint8_t nRF24_REG_RX_ADDR_P3  = 0x0D;  // Address for Pipe 3 (1 byte)
const uint8_t nRF24_REG_RX_ADDR_P4  = 0x0E;  // Address for Pipe 4 (1 byte)
const uint8_t nRF24_REG_RX_ADDR_P5  = 0x0F;  // Address for Pipe 5 (1 byte)
const uint8_t nRF24_REG_TX_ADDR     = 0x10;  // Address to which data is transmitted
const uint8_t nRF24_REG_PW_P0       = 0x11;  // Data size in FIFO register for Pipe 0
const uint8_t nRF24_REG_PW_P1       = 0x12;  // Data size in FIFO register for Pipe 1
const uint8_t nRF24_REG_PW_P2       = 0x13;  // Data size in FIFO register for Pipe 2
const uint8_t nRF24_REG_PW_P3       = 0x14;  // Data size in FIFO register for Pipe 3
const uint8_t nRF24_REG_PW_P4       = 0x15;  // Data size in FIFO register for Pipe 4
const uint8_t nRF24_REG_PW_P5       = 0x16;  // Data size in FIFO register for Pipe 5
const uint8_t nRF24_REG_FIFO_STATUS = 0x17;  // Register holds status of FIFO's

// These bitmasks are used to isolate bits in the NRF24L01's status register
const uint8_t nRF24_RX_DR   = (1 << 6);     // Data is ready to be read
const uint8_t nRF24_TX_DS   = (1 << 5);     // Data has been successfully sent 
const uint8_t nRF24_MAX_RT  = (1 << 4);     // Maximum retries have been retried
const uint8_t nRF24_RX_P_NO = 0b00001110;   // Pipe number where data came in
const uint8_t nRF24_TX_FULL = 0b00000001;   // Transmitter FIFO is already full

// Miscellaneous bits in the configuration register
const uint8_t nRF24_EN_CRC  = (1 << 3);     // Bitmask to enable CRC
const uint8_t nRF24_CRCO    = (1 << 2);     // Bitmask for CRC encoding, 1 or 2 bit
const uint8_t nRF24_PWR_UP  = (1 << 1);     // Bitmask for power-up bit
const uint8_t nRF24_PRIM_RX = (1 << 0);     // Bitmask for receive or transmit mode

/*  This configuration puts the radio in receive mode with only RX interrupts enabled
 *  (the transmitter empty and max retries used interrupts are disabled). Power is on
 *  and 2 byte CRC's are chosen. CRC's in general are not forced on, as this is done
 *  when 'enhanced shockburst' mode is enabled. 
 */
const uint8_t nRF24_RECV_MODE = nRF24_EN_CRC | nRF24_PWR_UP | nRF24_PRIM_RX \
                              | nRF24_CRCO | nRF24_INT_TX | nRF24_INT_M_RT;

/*  This configuration puts the radio in transmit mode with only RX interrupts enabled
 *  (the transmitter empty and max retries used interrupts are disabled). Power is on
 *  and 2 byte CRC's are chosen. CRC's in general are not forced on, as this is done
 *  when 'enhanced shockburst' mode is enabled. 
 */
const uint8_t nRF24_XMIT_MODE = nRF24_EN_CRC | nRF24_PWR_UP | nRF24_INT_TX \
                              | nRF24_CRCO | nRF24_INT_M_RT;

/*  This configuration puts the radio in sleep mode with the same set of interrupt
 *  masks and CRC configuration bits as it will use when it is awakened.
 */
const uint8_t nRF24_SPI_MODE = nRF24_INT_TX | nRF24_INT_M_RT | nRF24_EN_CRC \
                             | nRF24_CRCO;

/// @endcond NOT_TO_DOCUMENT


//-------------------------------------------------------------------------------------
/** \brief This is a base class for drivers for a radio module based on a Nordic 
 *  Semiconductor nRF24L01 chip. 
 * 
 *  \details As a base class, this class is intended to be extended; its descendents 
 *  will implement text-based connections (see \c nRF24L01_text) and packet-based 
 *  connections. 
 *
 *  Lines which are connected to the radio on the ME405 board, in addition to the 
 *  usual Vcc and ground (radio pin first, CPU pin second):
 *    \li MISO - MISO   - SPI data, AVR to radio
 *    \li MOSI - MISO   - SPI data, radio to AVR
 *    \li SCK  - SCK    - SPI clock line
 *    \li CSN  - SS/PB0 - SPI slave select
 *    \li CE   - PE7    - Chip enable selects transmit or receive mode
 *    \li IRQ  - PE6    - Interrupt pin (radio can interrupt CPU when data is received)
 */

class nRF24L01_base
{
	// Protected data and methods are accessible from this class and its descendents
	protected:
		/// This is a pointer to a serial port object which is used for debugging the
		/// radio modem code. Left blank, it defaults to NULL and no debugging info
		emstream* p_serport;

	// Public methods can be called from anywhere in the program where there is a 
	// pointer or reference to an object of this class
	public:
		// The constructor sets up the radio interface
		nRF24L01_base (emstream* = NULL);

		bool ready_to_send (void);          // Check if the port is ready to transmit
		void reset (void);                  // Reset radio module to starting state

		// Methods which set addresses for the transmitter and receiver pipes 0 and 1. 
		// Depending on the value of nRF24_ADDR_WIDTH, a different number of 
		// parameters is needed by each
		#if (nRF24_ADDR_WIDTH == 3)
			void set_TX_address (uint8_t, uint8_t, uint8_t);
			void set_RX_address (uint8_t, uint8_t, uint8_t, uint8_t);
		#elif (nRF24_ADDR_WIDTH == 4)
			void set_TX_address (uint8_t, uint8_t, uint8_t, uint8_t);
			void set_RX_address (uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
		#elif (nRF24_ADDR_WIDTH == 5)
			void set_TX_address (uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
			void set_RX_address (uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
		#else
			#error Invalid address width specified in nRF24_ADDR_WIDTH
		#endif

		// Method to set addresses for receiver pipes 2 thorugh 5
		void set_RX_address (uint8_t, uint8_t);

		// Method to display the contents of the radio chip's registers
		void dump_registers (emstream*);

		// Set the width of the payload which the receiver will expect
		void set_payload_width (uint8_t, uint8_t = 0);

		// Put the radio into a standard transmission mode
		void set_transmit_mode (void);
		
		// Put the radio into a standard receiving mode
		void set_receive_mode (void);

		// Method to transmit a packet of data via the radio
		bool transmit (uint8_t*);

		// This method checks if there is a packet of data available
		bool data_ready (void);
};

// This function transfers information through the SPI port to the radio and receives 
// the radio's reply. It's declared C style because it doesn't need anything from the
// class and it's easier for the data received ISR to call this way
bool nRF24_spi_transfer (uint8_t*, uint8_t);

#endif // _NRF24L01_BASE_H_

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
 *    \li 10-11-2008 JRR Added control for enhanced/regular mode, long addresses
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

#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>

#include "nRF24L01_base.h"


//-------------------------------------------------------------------------------------
/** This constructor sets up the nRF24L01 radio for communications. It configures bits
 *  in I/O ports which are used for communication with the radio module, and it saves
 *  ports and bitmasks so that other functions can talk to the radio later. 
 *  @param debug_port A serial port, usually RS-232, for debugging text (default: NULL)
 */

nRF24L01_base::nRF24L01_base (emstream* debug_port)
{
	p_serport = debug_port;                 // Save pointer to serial port

	// Set up the SPI port and make its bits inputs or outputs as needed
	nRF_DDR_SPI &= ~nRF_MASK_MISO;                // Set MISO as an input
	nRF_PORT_SPI |= nRF_MASK_MISO;                // Turn on MISO pullup
	nRF_DDR_SPI |= nRF_MASK_MOSI | nRF_MASK_SCK;  // Set MOSI and SCK as outputs
	nRF_DDR_SS |= nRF_MASK_SS;                    // Set SS as an output

	// Enable the SPI port, set it as master, and set clock to f_osc/8 with
	// the SPI clock prescaler set up in double-speed mode
	SPCR |= (1 << MSTR) | (1 << SPE) | (1 << SPR0);
	SPSR = 0x01;

	reset ();                               // Set radio module to starting state
}


//-------------------------------------------------------------------------------------
/** This method sets the number of bytes of payload the receiver will be expecting. 
 *  For first versions, we'll be setting this at 32 bytes for all uses. 
 *  @param bytes The number of bytes to be expected in the payload
 *  @param pipe The number of the pipe whose payload width is to be set (default 0)
 */

void nRF24L01_base::set_payload_width (uint8_t bytes, uint8_t pipe)
{
	uint8_t cmd[2];                         // Temporary storage for commands & data

	// Make sure the pipe size and number of bytes to send are legal
	if ((pipe < 6) && (bytes > 0) && (bytes <= 32))
	{
		cmd[0] = nRF24_WR_REG | (nRF24_REG_PW_P0 + pipe);
		cmd[1] = bytes;
		nRF24_spi_transfer (cmd, 2);
	}
}


//-------------------------------------------------------------------------------------
/** This method puts the radio into a standard transmission mode. Generally this will
 *  be done just long enough to send a packet, then things go back to receive mode, or
 *  sometimes it's nap time. 
 */

void nRF24L01_base::set_transmit_mode (void)
{
	uint8_t cmd[2];                         // Temporary storage for commands & data

	cmd[0] = nRF24_WR_REG | nRF24_REG_CONF;
	cmd[1] = nRF24_XMIT_MODE;
	nRF24_spi_transfer (cmd, 2);

	nRF_PORT_CE &= ~nRF_MASK_CE;            // Turn the receiver off by dropping CE
}


//-------------------------------------------------------------------------------------
/** This method puts the radio into a standard receiving mode. It will wait for 
 *  packets to arrive and alert the microcontroller using the IRQ line when one has 
 *  arrived. Other interrupt sources (transmitter empty and too many retries) are
 *  masked so they won't cause interrupts. 
 */

void nRF24L01_base::set_receive_mode (void)
{
	uint8_t cmd[2];                         // Temporary storage for commands & data

	cmd[0] = nRF24_WR_REG | nRF24_REG_CONF;
	cmd[1] = nRF24_RECV_MODE;
	nRF24_spi_transfer (cmd, 2);

	nRF_PORT_CE |= nRF_MASK_CE;             // Receiver needs CE high to be on
}


//-------------------------------------------------------------------------------------
/** This method checks if there's data available to be read.
 *  @return True if there's data available in the buffer and false if not
 */

bool nRF24L01_base::data_ready (void)
{
	uint8_t cmd[2];                         // Temporary space for commands & data

	cmd[0] = nRF24_RD_REG | nRF24_REG_STATUS;
	nRF24_spi_transfer (cmd, 2);

	return ((cmd[0] & nRF24_RX_DR) != 0);
}


//-------------------------------------------------------------------------------------
/** This method transmits a packet of data using the radio. Data sent to this method
 *  should be in an array of 33 uint8_tacters; the first element in the array
 *  will be discarded (it will be used to hold the Write Payload command for the radio)
 *  and the other 32 elements contain data to be sent over the radio. 
 *  @param buffer A pointer to an array of 33 uint8_ts, the first one expendable
 *  @return True if the data was successfully sent, false if not
 */

bool nRF24L01_base::transmit (uint8_t* buffer)
{
	uint8_t cmd[2];                         // Smaller buffer for commands & configs

	// Make sure the radio's ready to send some data...somehow...how?

	// The transmitter doesn't seem to work reliably unless the radio is powered down
	// and then powered up again in transmit mode. I have no idea why this is, but
	// since this kludge seems to work pretty well, we'll go with it
	cmd[0] = nRF24_WR_REG | nRF24_REG_CONF;
	cmd[1] = nRF24_SPI_MODE;
	nRF24_spi_transfer (cmd, 2);

// 	// While the radio is powered down, check if it's ready to send something yet
// 	do
// 	{
// 		cmd[0] = nRF24_NOP;
// 		nRF24_spi_transfer (cmd, 1);
// 	}
// 	while (cmd[0] & nRF24_TX_FULL);

	// OK, here's where we power the radio back up in transmit mode
	cmd[0] = nRF24_WR_REG | nRF24_REG_CONF;
	cmd[1] = nRF24_XMIT_MODE;
	nRF24_spi_transfer (cmd, 2);

	// Flush transmitter buffer, then put the data to send in the buffer
	cmd[0] = nRF24_FLUSH_TX;
	cmd[1] = 0x00;
	nRF24_spi_transfer (cmd, 2);

	buffer[0] = nRF24_WR_PLD;
	nRF24_spi_transfer (buffer, 33);

	// Pulse the CE line to begin the transmission; the pulse must be >10 us long
	nRF_PORT_CE |= nRF_MASK_CE;
	for (volatile uint8_t blah = 0; blah < 100; blah++);
	nRF_PORT_CE &= ~nRF_MASK_CE;

	uint8_t timeout = 0;
	do
	{
		// Check the status byte to see if we have sent the packet successfully
		cmd[0] = nRF24_NOP;
		nRF24_spi_transfer (cmd, 1);
		cmd[0] = nRF24_RD_REG | nRF24_REG_STATUS;
		cmd[1] = 0x00;
		nRF24_spi_transfer (cmd, 2);
	}
	while (((cmd[1] & (nRF24_TX_DS | nRF24_MAX_RT)) == 0) 
			&& (++timeout < nRF24_SEND_TIMEOUT));

	// Save an indication of whether the transmission was successful
	bool success = (cmd[0] & nRF24_TX_DS) != 0;

	// If something went wrong, the TX_DS bit will be 0; complain about it
	if (((cmd[0] & nRF24_TX_DS) == 0) && p_serport)
	{
		*p_serport << PMS ("TX error: ") << bin << cmd[0] << dec << endl;
	}

	// Clear the TX_DS bit
	cmd[0] = nRF24_WR_REG | nRF24_REG_STATUS;
	cmd[1] = nRF24_TX_DS;
	nRF24_spi_transfer (cmd, 2);

	// Turn the receiver back on again
	set_receive_mode ();

	// If the last status byte indicated a successful transmission, we'll return true
	return (success);
}


//-------------------------------------------------------------------------------------
/** This method resets the radio. It can be used if the radio gets into an unknown
 *  state. It sets the following configuration:
 *    \li Only the data received interrupt is used; others are masked off
 *    \li Auto-acknowledgement and re-transmission are disabled for all pipes
 *    \li Automatic retrying of sending is turned off
 *    \li Set send and receive payload widths to 32 bytes
 */

void nRF24L01_base::reset (void)
{
	uint8_t cmd[2];                         // Temporary storage for commands/data

	// Mask off unneeded interrupts and put radio in receive mode
	cmd[0] = nRF24_WR_REG | nRF24_REG_CONF;
	cmd[1] = nRF24_RECV_MODE;
	nRF24_spi_transfer (cmd, 2);

	// Disable auto-acknowledgement for text mode
	cmd[0] = nRF24_WR_REG | nRF24_REG_EN_AA;
	cmd[1] = 0x00;                          // Changed 02-20-08: Ack turned off
	nRF24_spi_transfer (cmd, 2);

	// Turn on/off automatic retries
	cmd[0] = nRF24_WR_REG | nRF24_REG_SETUP_RETR;
	cmd[1] = 0x00;                          // Changed 02-20-08: Retries turned off
	nRF24_spi_transfer (cmd, 2);

	// Set address width to 3 bytes
	cmd[0] = nRF24_WR_REG | nRF24_REG_SETUP_AW;
	cmd[1] = (nRF24_ADDR_WIDTH - 2);
	nRF24_spi_transfer (cmd, 2);

	// Set RF setup register: Max power, high data rate, LNA (Low Noise Amp) gain 1
	cmd[0] = nRF24_WR_REG | nRF24_REG_RF_SETUP;
	cmd[1] = nRF24_DR_2M | nRF24_RF_POW_0 | nRF24_LNA_HCURR;
	nRF24_spi_transfer (cmd, 2);

	// Set receiver payload width for Pipe 0 to 32 bytes
	set_payload_width (32, 0);

	// Clear all the interrupt sources
	cmd[0] = nRF24_WR_REG | nRF24_REG_STATUS;
	cmd[1] = nRF24_TX_DS | nRF24_RX_DR | nRF24_MAX_RT | nRF24_TX_FULL;
	nRF24_spi_transfer (cmd, 2);

	// Flush the transmit and receive buffers
	cmd[0] = nRF24_FLUSH_TX;
	nRF24_spi_transfer (cmd, 1);

	cmd[0]= nRF24_FLUSH_RX;
	nRF24_spi_transfer (cmd, 1);

	// Leave the CE line high to keep the receiver on
	set_receive_mode ();
}


//-------------------------------------------------------------------------------------
/** This method sets the transmitter's address. The number of bytes used in a radio
 *  address is specified in the define \c nRF24_ADDR_WIDTH, which must be 3, 4, or 5.
 *  @param addr0 The first byte of the address to be set
 *  @param addr1 The second byte of the address to be set
 *  @param addr2 The third byte of the address to be set
 */

#if (nRF24_ADDR_WIDTH == 3)
	void nRF24L01_base::set_TX_address (uint8_t addr0, uint8_t addr1, uint8_t addr2)
#elif (nRF24_ADDR_WIDTH == 4)
	void nRF24L01_base::set_TX_address (uint8_t addr0, uint8_t addr1, uint8_t addr2,
									   uint8_t addr3)
#elif (nRF24_ADDR_WIDTH == 5)
	void nRF24L01_base::set_TX_address (uint8_t addr0, uint8_t addr1, uint8_t addr2,
									   uint8_t addr3, uint8_t addr4)
#endif
{
	uint8_t bytes[nRF24_ADDR_WIDTH + 1];    // Array to be sent via SPI to radio

	// The first byte in the array gets a write-register command
	bytes[0] = nRF24_WR_REG | nRF24_REG_TX_ADDR;

	// Now fill in the correct amount of data for the address - 3, 4, or 5 bytes.
	// The data goes LSB first into the array of stuff to be sent over SPI
	#if (nRF24_ADDR_WIDTH == 3)
		bytes[1] = addr2;
		bytes[2] = addr1;
		bytes[3] = addr0;
	#elif (nRF24_ADDR_WIDTH == 4)
		bytes[1] = addr3;
		bytes[2] = addr2;
		bytes[3] = addr1;
		bytes[4] = addr0;
	#elif (nRF24_ADDR_WIDTH == 5)
		bytes[1] = addr4;
		bytes[2] = addr3;
		bytes[3] = addr2;
		bytes[4] = addr1;
		bytes[5] = addr0;
	#endif

	// Call the SPI transfer function to do the sending
	nRF24_spi_transfer (bytes, (nRF24_ADDR_WIDTH + 1));
}


//-------------------------------------------------------------------------------------
/** This method sets the receiver address for Pipe 0 or 1. The number of bytes used in 
 *  a radio address is specified in the define \c nRF24_ADDR_WIDTH, which must be 3, 
 *  4, or 5. 
 *  @param addr0 The first byte of the address to be set
 *  @param addr1 The second byte of the address to be set
 *  @param addr2 The third byte of the address to be set
 *  @param pipe The number of the pipe whose address will be set
 */

#if (nRF24_ADDR_WIDTH == 3)
	void nRF24L01_base::set_RX_address (uint8_t addr0, uint8_t addr1, uint8_t addr2,
									   uint8_t pipe)
#elif (nRF24_ADDR_WIDTH == 4)
	void nRF24L01_base::set_RX_address (uint8_t addr0, uint8_t addr1, uint8_t addr2,
									   uint8_t addr3, uint8_t pipe)
#elif (nRF24_ADDR_WIDTH == 5)
	void nRF24L01_base::set_RX_address (uint8_t addr0, uint8_t addr1, uint8_t addr2,
									   uint8_t addr3, uint8_t addr4, uint8_t pipe)
#endif
{
	uint8_t bytes[nRF24_ADDR_WIDTH + 1];    // Array to be sent via SPI to radio

	// This command is only valid if the pipe number is 0 or 1
	if (pipe <= 1)
	{
		// Fill the array with write-register command and the address data
		bytes[0] = nRF24_WR_REG | (nRF24_REG_RX_ADDR_P0 + pipe);

		// Now fill in the correct amount of data for the address - 3, 4, or 5 bytes.
		// The data goes LSB first into the array of stuff to be sent over SPI
		#if (nRF24_ADDR_WIDTH == 3)
			bytes[1] = addr2;
			bytes[2] = addr1;
			bytes[3] = addr0;
		#elif (nRF24_ADDR_WIDTH == 4)
			bytes[1] = addr3;
			bytes[2] = addr2;
			bytes[3] = addr1;
			bytes[4] = addr0;
		#elif (nRF24_ADDR_WIDTH == 5)
			bytes[1] = addr4;
			bytes[2] = addr3;
			bytes[3] = addr2;
			bytes[4] = addr1;
			bytes[5] = addr0;
		#endif

		// Call the SPI transfer function to do the sending
		nRF24_spi_transfer (bytes, (nRF24_ADDR_WIDTH + 1));
	}
}


//-------------------------------------------------------------------------------------
/** This method sets the receiver address for Pipe 2 through 5. These pipes use only
 *  one unique byte each and share the rest of their addresses with Pipe 1. 
 *  @param addr The byte which is unique to this pipe's address
 *  @param pipe The number of the pipe whose address will be set
 */

void nRF24L01_base::set_RX_address (uint8_t addr, uint8_t pipe)
{
	uint8_t bytes[nRF24_ADDR_WIDTH + 1];    // Array to be sent via SPI to radio

	// This command is only valid if the pipe number is between 2 and 5 inclusive
	if ((pipe >= 2) && (pipe <= 5))
	{
		// Fill the array with write-register command and the address data
		bytes[0] = nRF24_WR_REG | (nRF24_REG_RX_ADDR_P0 + pipe);

		// Now fill in the correct amount of data for the address - 3, 4, or 5 bytes.
		// The data goes LSB first into the array of stuff to be sent over SPI
		bytes[1] = addr;

		// Call the SPI transfer function to do the sending
		nRF24_spi_transfer (bytes, 2);
	}
}


//-------------------------------------------------------------------------------------
/** This method checks if the radio transmitter is ready to send data. But since the
 *  sending of data blocks until the data has been sent and the radio is ready to
 *  send more data, this method always returns true. 
 *  @return True if the radio is ready to send, and it should always be
 */

bool nRF24L01_base::ready_to_send (void)
{
	return (true);
}


//-------------------------------------------------------------------------------------
/** This method displays the contents of the nRF24L01's registers on the given serial
 *  device in a convenient, easy to read (for nerds) format.
 *  @param p_serial A pointer to the serial port device on which to display registers
 */

void nRF24L01_base::dump_registers (emstream* p_serial)
{
	uint8_t reg_data[6];                    // Stuff sent and received over SPI
	uint8_t item;                           // Counts items within an array

	*p_serial << PMS ("Registers in nRF24L01:") << bin << endl;

	reg_data[0] = nRF24_RD_REG | nRF24_REG_CONF;
	reg_data[1] = 0x00;
	nRF24_spi_transfer (reg_data, 2);
	*p_serial << PMS ("00 Config:   ") << reg_data[1];

	reg_data[0] = nRF24_RD_REG | nRF24_REG_RF_CH;
	reg_data[1] = 0x00;
	nRF24_spi_transfer (reg_data, 2);
	*p_serial << PMS ("\t05 RF Chan:  ") << reg_data[1] << endl;

	reg_data[0] = nRF24_RD_REG | nRF24_REG_EN_AA;
	reg_data[1] = 0x00;
	nRF24_spi_transfer (reg_data, 2);
	*p_serial << PMS ("01 Auto Ack: ") << reg_data[1];

	reg_data[0] = nRF24_RD_REG | nRF24_REG_RF_SETUP;
	reg_data[1] = 0x00;
	nRF24_spi_transfer (reg_data, 2);
	*p_serial << PMS ("\t06 RF Setup: ") << reg_data[1] << endl;

	reg_data[0] = nRF24_RD_REG | nRF24_REG_EN_RXADDR;
	reg_data[1] = 0x00;
	nRF24_spi_transfer (reg_data, 2);
	*p_serial << PMS ("02 Pipes En: ") << reg_data[1];

	reg_data[0] = nRF24_RD_REG | nRF24_REG_STATUS;
	reg_data[1] = 0x00;
	nRF24_spi_transfer (reg_data, 2);
	*p_serial << PMS ("\t07 Status:   ") << reg_data[1] << endl;

	reg_data[0] = nRF24_RD_REG | nRF24_REG_SETUP_AW;
	reg_data[1] = 0x00;
	nRF24_spi_transfer (reg_data, 2);
	*p_serial << PMS ("03 Addr Wid: ") << reg_data[1];

	reg_data[0] = nRF24_RD_REG | nRF24_REG_OBS_TX;
	reg_data[1] = 0x00;
	nRF24_spi_transfer (reg_data, 2);
	*p_serial << PMS ("\t08 TX Errs:  ") << reg_data[1] << endl;

	reg_data[0] = nRF24_RD_REG | nRF24_REG_SETUP_RETR;
	reg_data[1] = 0x00;
	nRF24_spi_transfer (reg_data, 2);
	*p_serial << PMS ("04 Retry:    ") << reg_data[1];

	reg_data[0] = nRF24_RD_REG | nRF24_REG_CD;
	reg_data[1] = 0x00;
	nRF24_spi_transfer (reg_data, 2);
	*p_serial << PMS ("\t09 Carrier:  ") << reg_data[1] << endl;

	for (uint8_t reggie = 0; reggie < 6; reggie++)
	{
		reg_data[0] = (nRF24_RD_REG | nRF24_REG_RX_ADDR_P0) + reggie;
		reg_data[1] = 0x00;
		nRF24_spi_transfer (reg_data, (nRF24_ADDR_WIDTH + 1));
		*p_serial << PMS ("0") << ascii << ('A' + reggie) << numeric << dec 
				  << PMS (" P") << reggie << PMS (" Addr:  ") << hex;
		if (reggie < 2)
		{
			for (item = nRF24_ADDR_WIDTH; item > 1; item--)
			{
			*p_serial << reg_data[item];
			p_serial->putchar ('.');
			}
			*p_serial << reg_data[item];
		}
		else
		{
			for (item = 3; item < (nRF24_ADDR_WIDTH * 3); item++)
			{
				p_serial->putchar ('.');
			}
			*p_serial << reg_data[1];
		}
		*p_serial << endl;
	}

	reg_data[0] = nRF24_RD_REG | nRF24_REG_TX_ADDR;
	reg_data[1] = 0x00;
	nRF24_spi_transfer (reg_data, (nRF24_ADDR_WIDTH + 1));
	*p_serial << PMS ("10 TX Addr:  ");
	for (item = nRF24_ADDR_WIDTH; item > 1; item--)
	{
		*p_serial << reg_data[item];
		p_serial->putchar ('.');
	}
	*p_serial << reg_data[item] << endl << bin;

	reg_data[0] = nRF24_RD_REG | nRF24_REG_PW_P0;
	reg_data[1] = 0x00;
	nRF24_spi_transfer (reg_data, 6);
	*p_serial << PMS ("11 P0 Width: ") << reg_data[1] << " (" << dec << reg_data[1] 
		<< ")" << endl << bin;

	reg_data[0] = nRF24_RD_REG | nRF24_REG_FIFO_STATUS;
	reg_data[1] = 0x00;
	nRF24_spi_transfer (reg_data, 6);
	*p_serial << PMS ("17 FIFO St:  ") << reg_data[1] << endl;

	*p_serial << dec << endl;
}


//-------------------------------------------------------------------------------------
/** This function sends a string of bytes to the radio through the SPI port. As the bytes
 *  are transferred to the radio in series, the radio sends back bytes which are stored
 *  in the same string. 
 *  @param p_bytes A pointer to the array of bytes to be sent to the radio
 *  @param num_bytes The number of bytes to be sent and received
 *  @return False if the transfer was successful and true if there was a timeout
 */

bool nRF24_spi_transfer (uint8_t* p_bytes, uint8_t num_bytes)
{
	uint8_t timeout_count;

	// Set the slave select pin for the radio low to get its attention
	nRF_PORT_SS &= ~nRF_MASK_SS;

	// Send each byte from the character array and replace it with what's recieved
	for (uint8_t index = 0; index < num_bytes; index++)
	{
		SPDR = p_bytes[index];
		for (timeout_count = 0; !(SPSR & (1 << SPIF)); timeout_count++)
		{
			if (timeout_count == 255)		// This means a timeout
			{
				nRF_PORT_SS |= nRF_MASK_SS;
				return (true);
			}
		}
		p_bytes[index] = SPDR;              // Save what was sent back by the radio
	}

	nRF_PORT_SS |= nRF_MASK_SS;             // De-select the radio's slave select line
	return (false);
}


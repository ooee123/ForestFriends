//*************************************************************************************
/** \file nRF24L01_text.cpp
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

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "FreeRTOS.h"                       // Header for the RTOS in which this runs
#include "nRF24L01_text.h"                  // Header for this file


/** This circular buffer holds characters received from the radio. The characters can
 *  be read by calls to getchar(). */
circ_buffer<uint8_t, nRF_QUEUE_SIZE>* g_RX_queue;


//-------------------------------------------------------------------------------------
/** This constructor sets up the nRF24L01 radio for communications. It doesn't have to
 *  do much work itself, because the constructors of its ancestors (the base nRF24L01 
 *  and base text serial port) do all the work. 
 *  @param debug_port A serial port, usually RS-232, for debugging text (default: NULL)
 */

nRF24L01_text::nRF24L01_text (emstream* debug_port)
    : nRF24L01_base (debug_port), emstream ()
{
	// Enable External Interrupt, connected to the radio's IRQ pin, on falling edges
	nRF24_EICR |= (1 << nRF24_IRQ_CBIT);
	nRF24_EIMSK |= (1 << nRF24_IRQ_MASK);

	// Allocate the queue which holds received characters
	g_RX_queue = new circ_buffer<uint8_t, nRF_QUEUE_SIZE>;
	#ifdef SERIAL_DEBUG
		if (g_RX_queue == NULL)
		{
			DBG (debug_port, "Buffer memory problem for nRF24L01_text" << endl);
		}
	#endif
}


//-------------------------------------------------------------------------------------
/** This method sends one character to the radio object's transmit buffer. Because 
 *  characters are usually sent as part of strings and we don't want to send single
 *  character packets all the time, the character is just stored in the buffer unless
 *  it causes the buffer to reach 31 bytes in length or someone has requested immediate
 *  transmission, in which case the buffer is sent. 
 *  @param chout The character to be sent out
 *  @return True if everything was OK and false if there was a timeout
 */

bool nRF24L01_text::putchar (char chout)
{
	uint8_t payload[33];                    // Buffer holds payload sent to radio
	uint8_t count;                          // Counts its way through the bytes

	// Form a buffer to be sent out via the radio module, with element 0 empty
	payload[1] = (uint8_t)chout;
	for (count = 2; count < 33; count++)
	{
		payload[count] = '\0';
	}

	// The transmit() method returns true for success
	return (transmit (payload));
}


//-------------------------------------------------------------------------------------
/** This method gets one character from the serial port, if one is there.  If not, it
 *  returns (-1) as a subtle hint that one shouldn't have called it when one did. One 
 *  should almost always use \c check_for_char() to ensure that there's data available 
 *  first, and if there is data available call this method. 
 *  @return The character which was found in the serial port receive buffer, or (-1)
 *          if something went all wonky and nothing was found
 */

int16_t nRF24L01_text::getchar (void)
{
    if (g_RX_queue->is_empty ())
	{
		return (-1);
	}
	else
	{
		return (g_RX_queue->get ());
    }
}


//-------------------------------------------------------------------------------------
/** This function checks if there is a character in the serial port's receiver buffer.
 *  It returns 1 if there's a character available, and 0 if not. 
 *  @return True for character available, false for no character available
 */

bool nRF24L01_text::check_for_char (void)
{
	// Check the receiver queue first; if there's something there, we're done
	return (!(g_RX_queue->is_empty ()));
}


//--------------------------------------------------------------------------------------
/** This is the interrupt service routine which is called whenever the nRF23L01 radio
 *  module drops its interrupt pin low. This even occurs when there's data which has
 *  arrived into the receiver. The ISR gets the data from the radio and puts it into
 *  the receiving queue. 
 */

ISR (nRF24_IRQ_VECT)
{
	static uint8_t buffer[34];              // Buffer holds data from radio
	uint8_t index;                          // Index into the buffer

	// Get data out from the buffer
	buffer[0] = nRF24_RD_PLD;
	nRF24_spi_transfer (buffer, 33);

	// Put data from the buffer into the queue until we hit a '\0'
	for (index = 1; (index <= 33) && (buffer[index]); index++)
	{
		g_RX_queue->put (buffer[index]);
	}

	// Flush the buffer
	buffer[0]= nRF24_FLUSH_RX;
	nRF24_spi_transfer (buffer, 1);

	// Clear the interrupt sources in the nRF24L01 radio 
	buffer[0] = nRF24_WR_REG | nRF24_REG_STATUS;
	buffer[1] = nRF24_RX_DR | nRF24_TX_DS | nRF24_MAX_RT;
	nRF24_spi_transfer (buffer, 2);
}

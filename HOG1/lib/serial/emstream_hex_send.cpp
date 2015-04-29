//*************************************************************************************
/** \file emstream_hex_send.cpp
 *    This file contains a method which converts an arbitrary data item, either Plain
 *    Old Data (POD) or a compound data type such as an array or an object of a class,
 *    into a stream of printable hex numbers that can be sent over a serial 
 *    communication interface. 
 *
 *  Revised:
 *    \li 12-02-2012 JRR Split this file off from the main \c emstream.cpp to allow
 *                       smaller machine code if stuff in this file isn't used
 *
 *  License:
 *    This file released under the Lesser GNU Public License, version 2. This program
 *    is intended for educational use only, but it is not limited thereto. This code
 *    incorporates elements from Xmelkov's ftoa_engine.h, part of the avr-libc source,
 *    and users must accept and comply with the license of ftoa_engine.h as well. See
 *    emstream.h for a copy of the relevant license terms. */
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

#include <stdlib.h>                         // Standard C/C++ library for AVR's
#include <util/crc16.h>                     // Header for cyclic redundancy checks

#include "FreeRTOS.h"                       // Header for FreeRTOS in general
#include "task.h"                           // Header for FreeRTOS tasks in particular

#include "emstream.h"                       // Header for the emstream class


//-------------------------------------------------------------------------------------
/** This method converts a data item into a stream of printable hex characters, then
 *  sends that stream over the serial interface. The stream of characters includes a
 *  type identifier, the length of the data in bytes, and a cyclic redundancy check 
 *  (CRC) which can be used to check for errors in the transmission of the data. When
 *  the stream is received at the other end of the communication channel, a data item
 *  of the same type as the one being sent can be filled with data identical to that
 *  which was in the transmitted item. You end up with a "remote copy" of the item. 
 *  Typical usage of this function is as follows:
 *  \code
 *  some_serial_device my_ser_dev;
 *  uint16_t some_readings[10];
 *  ...
 *  my_ser_dev.hex_send (some_readings, sizeof (some_readings), hex_send_type_16_bit);
 *  \endcode
 *  It is also possible to send a run time sized portion of an array as long as you 
 *  can figure out the size of the data to be sent. For example, if one only needs to
 *  send the first 5 bytes of \c some_readings, one can use:
 *  \code
 *  my_ser_dev.hex_send (some_readings, 5 * sizeof (uint16_t), hex_send_type_16_bit);
 *  \endcode
 *  @param p_item A pointer to the data item (POD, array, or object) to be sent
 *  @param size The size of the item in bytes. This is \b not necessarily the number
 *              of items in an array; using \c sizeof() gives the correct size
 *  @param type An 8-bit code signifying the type of data
 */

void emstream::hex_send (void* p_item, size_t size, uint8_t type)
{
	// Make a really cheap semaphore to ensure that two items don't get hex-sent by
	// two tasks at the same time. Anyone who wants to send needs to grab the
	// semaphore; if it's not available, some waiting is in order. We eschew the use
	// of a proper mutex because those things take up a lot of RAM
	static bool cheap_semaphore = false;

	// Try to grab the semaphore. If it's false (meaning nobody has taken it), set it
	// to true, meaning nobody else gets to use this function until we're done with it
	bool good_to_go = false;
	do
	{
		taskENTER_CRITICAL ();
		if (cheap_semaphore == false)
		{
			cheap_semaphore = true;
			good_to_go = true;
		}
		taskEXIT_CRITICAL ();

		if (good_to_go == false)            // Someone else has the semaphore, so wait
		{                                   // a little while before trying again
			vTaskDelay (2);
		}
	}
	while (good_to_go == false);

	uint16_t my_crc = 0xFFFF;               // The cyclic redundancy check number

	*this << hex << (uint8_t)type << (uint8_t)(size);

	for (uint8_t index = 0; index < size; index++)
	{
		// Get the byte to be printed from the data belonging to the object
		*this << *((uint8_t*)(p_item));

		// Update a cyclic reduncancy check (fancy checksum) for the data
		my_crc = _crc_ccitt_update (my_crc, *((uint8_t*)p_item));
		p_item = (uint8_t*)p_item + 1;
	}
	*this << my_crc << dec;

	// Release the semaphore so that other tasks can use this function
	cheap_semaphore = false;
}


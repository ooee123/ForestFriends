//*************************************************************************************
/** \file frt_text_queue.cpp
 *    This file contains a class which uses the class \c emstream operators to make 
 *    writing formatted text to a FreeRTOS character queue very easy. 
 *
 *  Revised:
 *    \li 10-21-2012 JRR Original file
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


#include "frt_text_queue.h"                 // Pull in the base class header file


//-------------------------------------------------------------------------------------
/** This constructor creates a \c frt_text_queue object. The queue size should be 
 *  large enough so that if the writing task gets ahead of the reading task for a 
 *  little while, there's room in the queue to hold the extra characters; if not, 
 *  writing to the queue will block until the reading task has read characters from 
 *  the queue and done something with them. 
 *  @param queue_size The number of characters which can be stored in the queue
 *  @param a_wait_time How long, in RTOS ticks, to wait for a full queue to become
 *                     empty before a character can be sent. The default value of
 *                     portMAX_DELAY causes a send to block indefinitely
 *  @param p_ser_dev A pointer which points to a serial device which can be used for
 *                   diagnostic logging or printing
 */

frt_text_queue::frt_text_queue (uint16_t queue_size, emstream* p_ser_dev,
							   portTickType a_wait_time)
{
	// Save the pointer to the serial device which is used for debugging
	p_serial = p_ser_dev;

	// Create a FreeRTOS queue object which holds the given number of characters
	the_queue = xQueueCreate (queue_size, sizeof (char));

	// Store the wait time; it will be used when writing to the queue
	ticks_to_wait = a_wait_time;
}


//-------------------------------------------------------------------------------------
/** This method writes one character to the queue. If the second constructor parameter
 *  wasn't given, the write operation will block until there is room in the queue for
 *  the character being written. Otherwise, the write will block for the given number
 *  of RTOS ticks waiting for an empty space in the queue, then if the queue has not
 *  become empty, give up in frustration and return false. 
 *  @param a_char The character to be sent to the queue
 *  @return True if the character was successfully sent, false if something went wrong
 */

inline bool frt_text_queue::putchar (char a_char)
{
	// If the data is successfully put in the queue, return true
	if (xQueueSendToBack (the_queue, &a_char, ticks_to_wait))
	{
		return (true);
	}

	// If we get here, something went wrong (probably a timeout), so return false
	return (false);
}


//-------------------------------------------------------------------------------------
/** This method writes a string to the queue. It just calls putchar() lots of times
 *  until a null character ('\0') is encountered. 
 */

inline void frt_text_queue::puts (char const* p_str)
{
	while (*p_str)
	{
		putchar (*p_str++);
	}
}


//-------------------------------------------------------------------------------------
/** This method checks if there is a character in the queue. It just calls the FreeRTOS
 *  function uxQueueMessagesWaiting(); if there's anything in the queue, the return 
 *  value from that function will be greater than zero. 
 *  @return True for character available, false for no character available
 */

inline bool frt_text_queue::check_for_char (void)
{
	if (uxQueueMessagesWaiting (the_queue) == 0)
	{
		return (false);
	}
	return (true);
}


//-------------------------------------------------------------------------------------
/** This method reads a character from the queue. If there isn't one conveniently in
 *  the queue, it blocks until a character is received.
 *  @return The character which was received from the queue
 */

inline int16_t frt_text_queue::getchar (void)
{
	char recv_char;							// Character read from the queue

	// If xQueueReceive doesn't return pdTrue, nothing was found in the queue
	if (xQueueReceive (the_queue, &recv_char, portMAX_DELAY) != pdTRUE)
	{
		return (-1);
	}

	// OK, we got good data from the queue, so return it
	return (recv_char);
}

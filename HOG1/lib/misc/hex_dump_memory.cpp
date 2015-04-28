//*************************************************************************************
/** \file hex_dump_memory.cpp
 *    This file contains a function which prints a hex(adecimal) dump of a section of
 *    the processor's memory to a serial device which is a descendent of the 
 *    \c emstream class. The hex dump is handy for viewing a FreeRTOS task's
 *    stack to see if the available stack space has been used up. 
 *
 *  Revisions
 *    \li 04-12-2008 JRR Original file, material from source above
 *    \li 09-30-2012 JRR Added code to make memory allocation work with FreeRTOS
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

#include "mechutil.h"                       // Include header for this function
#include "ansi_terminal.h"                  // Header for ANSI terminal escape codes


#define DUMP_BYTES_PER_LINE 20              ///< The number of bytes per hex-dump line

//-------------------------------------------------------------------------------------
/** This function displays a bunch of binary data from the SRAM of an AVR processor 
 *  as a hexadecimal data dump. 
 *  @param start_address The memory address at which to begin the dump
 *  @param end_address The address which is one byte past the last one to be dumped
 *  @param p_ser_dev Pointer to the serial device on which data will be shown
 *  @param highlight True if we want to highlight FreeRTOS task stack which has been 
 *      used
 *  @param empty_code This is used for FreeRTOS task stack dumps. If highlighting is
 *      enabled, bytes equal to the given number are displayed less brightly until the 
 *      first byte which isn't the given value is found. This highlights the used part 
 *      of a task's stack memory space. 
 */

void hex_dump_memory (uint8_t* start_address, uint8_t* end_address,
					  emstream* p_ser_dev, bool highlight, uint8_t empty_code)
{
	uint8_t byte_count;                     // Counts through the bytes in a line
	uint8_t temp_byte;                      // Holds a byte temporarily and obviously
	bool found_changes = false;             // Becomes true when changed memory found

	// Find the end address for the dump. Since C++ doesn't allow us to add two
	// pointers together, we convert the pointers into equivalent integers (this is
	// non-portable and may need to be changed for other processors!), add those
	// together, then convert back into a pointer

	// Print DUMP_BYTES_PER_LINE bytes on each line
	*p_ser_dev << hex;
	while (start_address < end_address)
	{
		// Print the line address 
		*p_ser_dev << (size_t)(start_address) << PMS ("  ");

		// Show a line full of data in hexadecimal format. If the memory has not been
		// changed from its original value, show it non-highlighted
		if (highlight && found_changes)
		{
			*p_ser_dev << ATERM_BOLD;
		}
		for (byte_count = 0; byte_count < DUMP_BYTES_PER_LINE; byte_count++)
		{
			if (highlight && (!found_changes) && ((*start_address) != empty_code))
			{
				found_changes = true;
				*p_ser_dev << ATERM_BOLD;
			}
			*p_ser_dev << (uint8_t)(*start_address++) << (char)(' ');
		}

		// Show the same line full of data in text format
		if (highlight)
		{
			*p_ser_dev << ATERM_NORM_INT << (char)(' ');
		}
		for (start_address -= DUMP_BYTES_PER_LINE; byte_count > 0; byte_count--)
		{
			temp_byte = *start_address++;
			if (temp_byte >= ' ' && temp_byte <= '~')
				p_ser_dev->putchar (temp_byte);
			else
				p_ser_dev->putchar ('.');
		}

		// Put a return at the end of the line
		*p_ser_dev << endl;
	}
	*p_ser_dev << dec;
}


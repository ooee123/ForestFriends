//**************************************************************************************
/** \file func_get_run_time_counter.cpp
 *    This file contains a function which can be used by the FreeRTOS run time counter
 *    to find the real time using the mechanism which is used by the \c time_stamp
 *    class. It's only used if the run time counter is being used to debug a FreeRTOS
 *    program's tasks. 
 *
 *  Revisions:
 *    \li 12-02-2012 JRR Split off from time_stamp.cpp to save memory in machine file
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
//**************************************************************************************

#include <stdlib.h>                         // Used for itoa()
#include <string.h>                         // Header for character string functions
#include <avr/interrupt.h>                  // For using interrupt service routines

#include "FreeRTOS.h"                       // Main header for FreeRTOS 
#include "task.h"                           // The FreeRTOS task functions header
#include "emstream.h"                       // Base for text-type serial port objects
#include "time_stamp.h"                     // Header for this file


//---------------------------------------------------------------------------------
/** This funtion does the work of finding the current real time to high resolution
	*  as measured by combining RTOS scheduler ticks with the hardware timer's count.
	*  @return A measurement of the current real time in a 32-bit number
	*/
uint32_t func_get_run_time_counter (void)
{
	portTickType tick_count;                // The number of RTOS ticks in the time
	HW_CTR_TYPE hardware_count;             // The count from the timer/counter SFR

	portENTER_CRITICAL ();                  // Disable interrupts while getting counts

	// Grab the hardware timer count. The tick count can't be updated, even if the
	// hardware timer overflows, because interrupts are disabled
	#if (defined TIMER5_COMPA_vect)
		hardware_count = TCNT3;
	#elif (defined TIMER3_COMPA_vect)
		hardware_count = TCNT3;
	#else
		hardware_count = TCNT1;
	#endif

	// Now get the tick count (interrupts are still disabled)
	tick_count = xTaskGetTickCount ();

	// Re-enable interrupts here; if the tick count is incremented now, that's fine
	portEXIT_CRITICAL ();

	// Compute the 32-bit number which is the current time in hardware timer ticks
	uint32_t return_this = (tick_count * TMR_MAX_CT) + hardware_count;
	return (return_this);
}


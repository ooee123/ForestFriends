//**************************************************************************************
/** \file motor_task.h
 *    This file contains the header for a task class that controls the motor PWM, 
 *    direction and mode of operation. This task controls a motor driver.
 *
 *  Revisions:
 *    \li 09-30-2012 JRR Original file was a one-file demonstration with two tasks
 *    \li 10-05-2012 JRR Split into multiple files, one for each task
 *    \li 10-25-2012 JRR Changed to a more fully C++ version with class task_sender
 *    \li 10-27-2012 JRR Altered from data sending task into LED blinking class
 *    \li 11-04-2012 JRR Altered again into the multi-task monstrosity
 *    \li 12-13-2012 JRR Yet again transmogrified; now it controls LED brightness
 *    \li 04-22-2014 Brad F., Darren C., Brandon H., Christian B. --> Rewrote the 
 * 	  program for alternate functionality related to motor control.
 *	  \li 06-4-2014 Brad F., Darren C., Brandon H., Christian B. --> Rewrote the 
 * 	  program to include a system task setup for the pongship project.
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

// This define prevents this .h file from being included multiple times in a .cpp file
#ifndef _Z_MOTOR_TASK_H_
#define _Z_MOTOR_TASK_H_

#include <stdlib.h>                         // Prototype declarations for I/O functions
#include <avr/io.h>                         // Header for special function registers

#include "FreeRTOS.h"                       // Primary header for FreeRTOS
#include "queue.h"                          // FreeRTOS inter-task communication queues

#include "frt_task.h"                       // ME405/507 base task class
#include "time_stamp.h"                     // Class to implement a microsecond timer
#include "frt_queue.h"                      // Header of wrapper for FreeRTOS queues
#include "frt_shared_data.h"                // Header for thread-safe shared data

#include "rs232int.h"                       // ME405/507 library for serial comm.

#include "shares.h"                         // Global ('extern') queue declarations
#include "motor_driver.h"
#include "motor_task.h"
#include "encoder_driver.h"
#include "read_serial_driver.h"

//-------------------------------------------------------------------------------------
/** \brief This task controls the direction and duty cycle of a motor driver. This task
 *  controls a motor driver on the ME405 board.
 *  \details The motor driver is run using a driver in files \c motor_driver.h and 
 *  \c motor_driver.cpp.
 */

class z_motor_task : public motor_task
{
private:
	// No private variables or methods for this class

protected:
	// No protected variables or methods for this class

public:
	// This constructor creates a task for controlling motor 1.
	z_motor_task (const char*, unsigned portBASE_TYPE, size_t, emstream*, motor_driver*, encoder_driver*, int16_t *desired_in, volatile uint8_t* limitDDR_in, volatile uint8_t* limitPORT_in, volatile uint8_t* limitPIN_in, uint8_t limitPinNum_in, volatile State* state_in, bool* zReady_in);

	// This method is called by the RTOS once to run the task loop for ever and ever.
	void run (void);
};

#endif

//**************************************************************************************
/** \file encoder_task.cpp
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
#include "frt_text_queue.h"                 // Header for text queue class
#include "shares.h"
#include "motor_driver.h"
#include "encoder_task.h"                 // Header for this task
#include <util/delay.h>
#define CPR 1000


//-------------------------------------------------------------------------------------
/** This constructor creates a task which controls the direction and PWM of a motor. 
 *  The main job of this constructor is to call the constructor of parent class 
 *  (\c frt_task ); the parent's constructor the work.
 *  @param a_name A character string which will be the name of this task
 *  @param a_priority The priority at which this task will initially run (default: 0)
 *  @param a_stack_size The size of this task's stack in bytes 
 *                      (default: configMINIMAL_STACK_SIZE)
 *  @param p_ser_dev Pointer to a serial device (port, radio, SD card, etc.) which can
 *                   be used by this task to communicate (default: NULL)
 */

encoder_task::encoder_task (const char* a_name, 
								 unsigned portBASE_TYPE a_priority, 
								 size_t a_stack_size,
								 emstream* p_ser_dev,
                         encoder_driver* encoder_in 
								)
	:
   frt_task (a_name, a_priority, a_stack_size, p_ser_dev)
{
   encoder = encoder_in;
   encoder->setSerial(p_serial);
	// Nothing is done in the body of this constructor. All the work is done in the
	// call to the frt_task constructor on the line just above this one
}

//-------------------------------------------------------------------------------------
/** This method is called once by the RTOS scheduler. Each time around the for (;;)
 *  loop, it reads the shared variables and uses the result to control the mode of
 *  operation of the motor. This essentially sorts the list of commands to be sent to
 *  the motor so that it can be controlled from a high-level user interface. This task
 *  can control multiple motors.
 */

void encoder_task::run (void)
{
  	portTickType previousTicks = xTaskGetTickCount ();
	
	// power is turned off. The task continuously reads the shared variable, mode1 (which
	// determines the mode of operation for a motor driver and reads the shared variable
	// power_level1 which determines the duty cycle and direction of the motor.	
	
	// Enable global interrupts
	
	for (;;)
	{	
		runs++;
		
		delay_from_to (previousTicks, configMS_TO_TICKS (10));
	}
}


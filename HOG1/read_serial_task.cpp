//**************************************************************************************
/** \file motor_task.cpp
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
#include "read_serial_driver.h"
#include "read_serial_task.h"                 // Header for this task
#include <util/delay.h>
#include "state.h"
#include "pinLayout.h"
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

read_serial_task::read_serial_task (
                         const char* a_name, 
								 unsigned portBASE_TYPE a_priority, 
								 size_t a_stack_size,
								 emstream* p_ser_dev,
                         read_serial_driver* serial_in,
                         uint16_t* desiredX_in,
                         uint16_t* desiredY_in,
                         uint16_t* desiredZ_in,
                         encoder_driver* xEncoder_in,
                         encoder_driver* yEncoder_in,
                         encoder_driver* zEncoder_in,
                         State* state_in
								)
	:
   frt_task (a_name, a_priority, a_stack_size, p_ser_dev)
{
   serial = serial_in;
   desiredX = desiredX_in;
   desiredY = desiredY_in;
   desiredZ = desiredZ_in;
   xEncoder = xEncoder_in;
   yEncoder = yEncoder_in;
   zEncoder = zEncoder_in;
   state = state_in;

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

bool read_serial_task::isWithinTolerance(uint16_t actual, uint16_t expected)
{
   return abs(actual - expected) <= EPSILON;
}

void read_serial_task::run (void)
{
  	portTickType previousTicks = xTaskGetTickCount ();
	for (;;)
	{	
		runs++;
      
      if (*state == HOME)
      {
         // If both limit switches are activated
         if (!getXLimitSwitch() && !getYLimitSwitch())
         {
            *p_serial << "LIMIT";
            // Then we're ready to proceed to normal operation
            *state = NORMAL;
            xEncoder->reset();
            yEncoder->reset();
            // And then take up the first coordinate
            *desiredX = serial->read_uint16_t();
            *desiredY = serial->read_uint16_t();
            *desiredZ = serial->read_uint16_t();
            *p_serial << "Got data HOME";
         }
      }
      else if (*state == NORMAL)
      {
         // Check if all axis are within tolerance
         if (isWithinTolerance(xEncoder->getPosition(), *desiredX) &&
             isWithinTolerance(yEncoder->getPosition(), *desiredY) &&
             1)
             //isWithinTolerance(zEncoder->getPosition(), *desiredZ))
         {
            // Notify Pi for next command
            *p_serial << "Z";
            // Get the next command
            // Update desired
            *desiredX = serial->read_uint16_t();
            *desiredY = serial->read_uint16_t();
            *desiredZ = serial->read_uint16_t();
            *p_serial << "Got data NORMAL";
            // If all points say 0, return home
            if (*desiredX == 0 && *desiredY == 0 && *desiredZ == 0)
            {
               *state = HOME;
            }
         }
      }
		delay_from_to (previousTicks, configMS_TO_TICKS (100));
	}
}

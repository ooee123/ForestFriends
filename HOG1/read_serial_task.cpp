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
#include "shares.h"
#include "pinLayout.h"
#include "constants.h"
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
                         int32_t* desiredX_in,
                         int32_t* desiredY_in,
                         int32_t* desiredZ_in,
                         encoder_driver* xEncoder_in,
                         encoder_driver* yEncoder_in,
                         encoder_driver* zEncoder_in,
                         volatile State* state_in,
                         bool* zReady_in,
                         uint16_t boardOffset_in
								)
	:
   frt_task (a_name, a_priority, a_stack_size, p_ser_dev)
{
   serial = serial_in;
   desiredX = desiredX_in;
   desiredY = desiredY_in;
   desiredZ = desiredZ_in;
   /*
   xEncoder = xEncoder_in;
   yEncoder = yEncoder_in;
   zEncoder = zEncoder_in;
   */
   state = state_in;
   zReady = zReady_in;
   boardOffset = boardOffset_in;

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

void read_serial_task::run (void)
{
  	portTickType previousTicks = xTaskGetTickCount ();
	for (;;)
	{	
		runs++;
      #if defined(DEBUG) || defined(MOTOR_DEBUG)
         while (print_ser_queue.check_for_char())
         {
            p_serial->putchar(print_ser_queue.getchar());
         }
      #endif
      #ifdef CURRENT_SENSOR
      uint8_t sense = ADMUX & 0x111;
      if (_getBit(ADCSR, ADIF))
      {
         #ifdef DEBUG
            *p_serial << "CUR ";
            *p_serial << sense;
            *p_serial << ":";
            *p_serial << ADCH;
            *p_serial << "\n";
         #endif
         if (!isWithinTolerance(ADCH, 128, ADC_MAX))
         {
            // Shut down machine!
            *p_serial << "CURRENT SENSOR TRIPPED\n";
            shutdown();
         }
         ADMUX &= 0b11111000;
         ADMUX |= (sense + 1) % NUM_CURRENT_SENSORS;
         _setBit(ADCSRA, ADSC);
      }
      #endif

      // If ANY of the limit switches are triggered
      if (!getXMaxLimitSwitch() || !getYMaxLimitSwitch() || !getZMaxLimitSwitch())
      {
         *p_serial << "*ERROR*\n";
         *p_serial << "MAX SWITCH ACTIVATED\n";
         *p_serial << "*ERROR*\n";
         shutdown();
      }
      if (*state == HOME)
      {
         // If all limit switches are activated
         if (!getXLimitSwitch() && !getYLimitSwitch() && !getZLimitSwitch())
         {
         #ifdef DEBUG
            *p_serial << "LIMIT";
         #endif
            // Then we're ready to proceed to normal operation
            *p_serial << AT_HOME; 
            *state = NORMAL;
            turnOnEncoders();
            xEncoder.reset();
            yEncoder.reset();
            zEncoder.reset();
            xAxis.brake();
            yAxis.brake();
            zAxis.brake();
            // And then take up the first coordinate
            getNextCoordinate();
         }
      }
      else if (*state == NORMAL)
      {
         // Check if all axis are within tolerance
         if (isWithinTolerance(xEncoder.getPosition(), *desiredX, TOLERANCE) &&
             isWithinTolerance(yEncoder.getPosition(), *desiredY, TOLERANCE) &&
             isWithinTolerance(zEncoder.getPosition(), *desiredZ, Z_AXIS_TOLERANCE))
         {
            xAxis.brake();
            yAxis.brake();
            zAxis.brake();
            // Notify Pi for next command
            *p_serial << NEXT_COMMAND;
            // Get the next command
            getNextCoordinate();
         }
      }
		delay_from_to (previousTicks, configMS_TO_TICKS (100));
	}
}

void read_serial_task::getNextCoordinate(void)
{
   *desiredX = serial->read_int32_t();
   *desiredY = serial->read_int32_t();
   *desiredZ = serial->read_int32_t();
   *zReady = false;
   if (*desiredX == 0 && *desiredY == 0 && (*desiredZ == 0 || *desiredZ == HOME_THREE_QUARTER_BOARD || *desiredZ == HOME_ONE_POINT_FIVE_BOARD))
   {
      *state = HOME;
      turnOffEncoders();
      if (*desiredZ == HOME_THREE_QUARTER_BOARD)
      {
         boardOffset = INCH * 3 / 4;
      }
      else if (*desiredZ == HOME_ONE_POINT_FIVE_BOARD)
      {
         boardOffset = 0;
      }
      // boardOffset is the thickness of the board
   }
   else
   {
      #ifndef NO_Z_CODE_CONVERSION
         int32_t desiredHeight = *desiredZ;
         if (*desiredZ == START || *desiredZ == MOVE)
         {
            desiredHeight = DISTANCE_1_5 + boardOffset - HOVER_HEIGHT;
            //desiredHeight = DISTANCE_1_5 + boardOffset - HOVER_HEIGHT;
         }
         else if (*desiredZ == LINE)
         {
            //desiredHeight = DISTANCE_1_5 + boardOffset + ROUTING_DEPTH;
            desiredHeight = DISTANCE_1_5 + +boardOffset + ROUTING_DEPTH;
         }
         *desiredZ = desiredHeight;
      #endif
   }
   #ifdef DEBUG
      *p_serial << "X:";
      *p_serial << *desiredX;
      *p_serial << "Y:";
      *p_serial << *desiredY;
      *p_serial << "Z:";
      *p_serial << *desiredZ;
   #endif
}

void read_serial_task::shutdown(void)
{
   *state = NORMAL;
   xAxis.brake();
   yAxis.brake();
   zAxis.brake();
   *desiredX = xEncoder.getPosition();
   *desiredY = yEncoder.getPosition();
   *desiredZ = zEncoder.getPosition();
   turnOffEncoders();
   _clearBit(SOLID_STATE_PORT, SOLID_STATE_PIN_NUM);
   for (;;);
}

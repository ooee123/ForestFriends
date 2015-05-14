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
#include "motor_driver.h"
#include "motor_task.h"                 // Header for this task
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

motor_task::motor_task (const char* a_name, 
								 unsigned portBASE_TYPE a_priority, 
								 size_t a_stack_size,
								 emstream* p_ser_dev,
                         motor_driver* motor_in,
                         encoder_driver* encoder_in,
                         uint16_t* desired_in
								)
	:
   frt_task (a_name, a_priority, a_stack_size, p_ser_dev)
{
   motor = motor_in;
   encoder = encoder_in;
   desired = desired_in;
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

void motor_task::run (void)
{
  	portTickType previousTicks = xTaskGetTickCount ();
    
	// Motor Drivers: PC0 = mode select A, PC1 = mode select B, PC2 = Output enable
	
   /* Kevin Notes 
   Here's how I interpret the parameters

   p_serial?
   Direction Register of MOT_X (DDRD)
   Direction Register of inA and inB
   Direction Register of PWM pins
   Output Pin for MOT_X. The PORTX value should match the letter of MOT_X
   Output Register for inA and inB
   Pin number for MOT_X
   Pin number for inA
   Pin number for inB
   Pin number for PWM
   COM1A1? "Compare output mode bits 1/0, how it's connected to Timer1
   OCR1A? "Generate interupts after num of clock ticks to it
   */

	// pan motor
	//motor_driver* yaw_motor = new motor_driver (p_serial, &DDRD, &DDRC, &DDRB, &PORTD, &PORTC, PD7, PC3, PC2, PB5, COM1A1, &OCR1A);
	// motor2 
	//motor_driver* motor2 = new motor_driver (p_serial, &DDRC, &DDRC, &DDRB, &PORTC, &PORTC, PC0, PC5, PC4, PB6, COM1B1, &OCR1B);
	// motor3
	//motor_driver* motor3 = new motor_driver (p_serial, &DDRC, &DDRC, &DDRB, &PORTC, &PORTC, PC0, PC5, PC4, PB6, COM1A1, &OCR1A);
	// tilt motor
	
	
	//motor_driver* p_motor_2 = new my_motor_driver (p_serial, &DDRD, PB5, PD5, PD6, PD7, COM1A1, &PORTD, &OCR1A, true);
	//sim motors (two motors using one control pin)
	//motor_driver* sim_motors = new my_motor_driver (p_serial, &DDRC, PB7, PC0, PC1, PC2, COM1C1, &PORTC, &OCR1C, false);
	
	// power is turned off. The task continuously reads the shared variable, mode1 (which
	// determines the mode of operation for a motor driver and reads the shared variable
	// power_level1 which determines the duty cycle and direction of the motor.	
	
	// Enable global interrupts
	
	for (;;)
	{	
		runs++;
      


      //motor->set_power(0b1010100111);
      motor->set_power(motor->move(encoder.getPosition() - desired));
      //p_serial->puts("HELLO");
   //dump_stack(p_serial);
      //motor->set_power(30000);
      PORTA = 1;
		//delay_from_to (previousTicks, configMS_TO_TICKS (600));
      //motor->brake();
		delay_from_to (previousTicks, configMS_TO_TICKS (600));
	}
}


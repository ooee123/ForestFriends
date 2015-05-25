//======================================================================================
/** \file encoder_driver.h
 *   This file contains a motor driver header file. The functions that come with this 
 *    motor driver class include set_power and brake. The set_power function first obtains 
 *    a reading from the task_user power_level1 and power_level2 values and converts it 
 *    from an unsigned 10 bit integer to a signed 10 bit integer. The sign of the input 
 *    determines ccw or cw rotation of motor and converts signed integter into an unsigned
 *    integer which is interpreted by the motor controllers a pwm signal or duty cycle. 
 *    The brake function sets the mode select bits A & B to the same value. 
 *    
 *
 *  Revisions:
 *    \li 01-15-2008 JRR Original (somewhat useful) file
 *    \li 10-11-2012 JRR Less original, more useful file with FreeRTOS mutex added
 *    \li 10-12-2012 JRR There was a bug in the mutex code, and it has been fixed
 *    \li 4-16-2014 Christian Bundschu, Darren Chan, Brad Funsten, and Brandon Hilgers
 *	  code was re-written to produce a motor driver 
 *    \li 4-29-2014 Christian Bundschu, Darren Chan, Brad Funsten, and Brandon Hilgers
 *	  code was re-written to include interrupts for encoding.
 *	  \li 06-4-2014 Brad F., Darren C., Brandon H., Christian B. --> Code rewritten for
 *    PI control for the pongship project.
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
//======================================================================================

// This define prevents this .H file from being included multiple times in a .CPP file
#ifndef _Z_ENCODER_driver_H_
#define _Z_ENCODER_driver_H_


#include "emstream.h"                       // Header for serial ports and devices
#include "FreeRTOS.h"                       // Header for the FreeRTOS RTOS
#include "task.h"                           // Header for FreeRTOS task functions
#include "queue.h"                          // Header for FreeRTOS queues
#include "semphr.h"                         // Header for FreeRTOS semaphores

#include "shares.h"
#include "constants.h"
//-------------------------------------------------------------------------------------
/** \brief This class should run the motor driver on an AVR processor. 
 *  \details my_motor_driver class holds the member functions in order to output a PWM signal 
 *  to the the motor in decimal form pwm.
 *  @param ptr_to_serial The my_motor_driver class uses this pointer to the serial port to print desired data
 *  @param PORT initializes the data direction registers for the motor
 *  @param INA mode select for bit A of the motor
 *  @param INB mode select for bit B of the motor
 *  @param EN output enable and diagnostics
 *  @param OCR the pwm control register 
 */

class z_encoder_driver : public encoder_driver
{
	protected:
		//instance/global variables
      volatile Direction* direction;
		
	public:
		// The constructor sets up the motor driver. The "= NULL" part is a
		// default parameter, meaning that if that parameter isn't given on the line
		// where this constructor is called, the compiler will just fill in "NULL".
		// In this case that has the effect of turning off diagnostic printouts.
		// This follows for the "= 0" part.
      z_encoder_driver(volatile uint8_t* DDR_en, volatile uint8_t* PIN_en, volatile uint8_t* PORT_EN, uint8_t Abit, uint8_t Bbit, volatile Direction* increasing_in);
		
      void updatePosition(void);
}; // end of class my_motor_driver

#endif // _AVR_my_motor_driver_H_

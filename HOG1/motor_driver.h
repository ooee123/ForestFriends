//======================================================================================
/** \file motor_driver.h
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
#ifndef _motor_driver_H_
#define _motor_driver_H_


#include "emstream.h"                       // Header for serial ports and devices
#include "FreeRTOS.h"                       // Header for the FreeRTOS RTOS
#include "task.h"                           // Header for FreeRTOS task functions
#include "queue.h"                          // Header for FreeRTOS queues
#include "semphr.h"                         // Header for FreeRTOS semaphores

#include "shares.h"
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

class motor_driver
{
	protected:
		//instance/global variables
		emstream* ptr_to_serial;

		volatile uint8_t* DDR_DIR;
		volatile uint8_t* DDR_EN;
		volatile uint8_t* DDR_PWM;
		
		volatile uint8_t* PORT_EN;
		volatile uint8_t* PORT_DIR;
		
		uint8_t COMTIMER;
		
		uint8_t INA; 
		uint8_t INB; 
		uint8_t EN;
		volatile uint16_t* OCR;
		uint8_t PWM;
		
		bool motor_trans;
		
	public:
		// The constructor sets up the motor driver. The "= NULL" part is a
		// default parameter, meaning that if that parameter isn't given on the line
		// where this constructor is called, the compiler will just fill in "NULL".
		// In this case that has the effect of turning off diagnostic printouts.
		// This follows for the "= 0" part.
		motor_driver (emstream* p_serial_port, volatile uint8_t* DDR_en, volatile uint8_t* DDR_dir, volatile uint8_t* DDR_pwm, volatile uint8_t* PORT_en, volatile uint8_t* PORT_dir, uint8_t ENbit, uint8_t INAbit, uint8_t INBbit, uint8_t PWMbit,uint8_t COMtimer, volatile uint16_t* OCRbit);
		
		// The set_power function first obtains 
   		// a reading from the power variables. 
		// The sign of the input determines ccw or cw rotation of motor and converts 
    		// signed integter into an unsigned integer which is interpreted by the motor controllers
     		// a pwm signal or duty cycle.
		void set_power (double power);

		// The brake function sets the mode select bits A & B to the same value.   
		void brake (void);
		
		void move_cw(void);
		
		
		void PI(void);

		
}; // end of class my_motor_driver

#endif // _AVR_my_motor_driver_H_

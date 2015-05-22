//*************************************************************************************
/** \file motor_driver.cpp
 
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

#include <stdlib.h>                         // Include standard library header files
#include <avr/io.h>
#include <math.h>
#include <avr/interrupt.h>

#include "rs232int.h"                       // Include header for serial port class
#include "motor_driver.h"                // Include header for the A/D class
#include "pinLayout.h"
#include "constants.h"

#define STOP_CONST 25

//---------------------		//p_motor_1->set_power(control1);----------------------------------------------------------------
/** \brief This constructor initializes the motor driver. 
 *  \details The motor driver is made ready so that when a function such as \c set_power() is called the motor runs.
 *  when the \c brake() function is called it stops the motor if running. 
 *  @param ptr_to_serial The my_motor_driver class uses this pointer to the serial port to print desired data
 *  @param PORT initializes the data direction registers for the motor
 *  @param INA mode select for bit A of the motor
 *  @param INB mode select for bit B of the motor
 *  @param EN output enable and diagnostics
 *  @param OCR the pwm control register 
 */
	//motor_driver* p_motor_1 = new my_motor_driver (p_serial, &DDRD, &DDRC, &DDRB, &PORTD, &PORTC, PD7, PC3, PC2, PB5, COM1B1, &OCR1B);
//Initialize my_motor_driver
motor_driver::motor_driver (volatile uint8_t* DDR_en, volatile uint8_t* DDR_dir, volatile uint8_t* DDR_pwm, volatile uint8_t* PORT_en, volatile uint8_t* PORT_dir, uint8_t ENbit, uint8_t INAbit, uint8_t INBbit, uint8_t PWMbit, uint8_t COMtimer, volatile uint16_t* OCRtimer)
{
	DDR_DIR = DDR_dir;
	DDR_EN = DDR_en;
	DDR_PWM = DDR_pwm;
	
	PORT_EN = PORT_en;
	PORT_DIR = PORT_dir;
	
	COMTIMER = COMtimer;
	
	INA = INAbit; 
	INB = INBbit; 
	EN = ENbit;
	OCR = OCRtimer;
	PWM = PWMbit;
	
   /*
   _setBit(*DDR_EN, EN);
   _setBit(*DDR_DIR, INA);
   _setBit(*DDR_DIR, INB);
   _setBit(*DDR_PWM, PWM);
   _setBit(*PORT_EN, EN);
   _setBit(*PORT_DIR, INA);
   _setBit(*PORT_DIR, INB);
*/
	*DDR_EN |= (1 << EN); // Output enable motors 1 and 2
	*DDR_DIR |= (1 << INA) | (1 << INB); // Output enable motors 1 and 2
	*DDR_PWM |= (1 << PWM); // Turn on PWM for motors 1 and 2 as outputs

	*PORT_EN |= (1 << EN); // Initialize mode for motor
	*PORT_DIR |= (1 << INA) | (1 << INB);
	
	// To set 10-bit fast PWM mode we must set bits WGM30 and WGM32, which are in two
	// different registers (ugh). We use COM3B1 and Com3B0 to set up the PWM so that
	// the pin output will have inverted sense, that is, a 0 is on and a 1 is off; 
	// this is needed because the LED connects from Vcc to the pin. 
	
   // The line below is true for whatever ATmega they were using */
	TCCR1A |= (1 << WGM10) | (1 << WGM11) | (1 << COMTIMER);
   
   // For ATmega128 To activate 10 bit fast PWM mode...
   // Actually I don't know with confidence
   // To activate 10 bit fast PWM mode on a 16 bit timer/counter
   //TCCR1A |= (1 << WGM10) | (1 << WGM11 ) (1 << COMTIMER);

	// The CS11 bit sets the prescaler for this timer/counter to run the
	// timer at F_CPU / 8
	
	TCCR1B |= (1 << WGM12) | (1 << CS11); // 10-bit PWM and prescale to 8	
}

void motor_driver::setSerial(emstream* stream)
{
   ptr_to_serial = stream;
}

//------------------------------------------------------------------------------------------
/** \brief The set_power function first obtains a reading from the global power variable and converts 
 *  \brief it from an unsigned 10 bit integer to a signed 10 bit integer. 
 *  \details The sign of the input determines ccw or cw rotation of motor and converts 
 *   signed integter into an unsigned integer which is interpreted by the motor controllers
 *   a pwm signal or duty cycle.
 *  @param  power dictates the value of the PWM
 */

void motor_driver::set_power (double power)
{	
   if (power == 0)
   {
      *OCR = 0;
   }
   
   // Counter clockwise
   else if (power < 0)
   {
      *PORT_DIR &= ~(1 << INA);
      *PORT_DIR |= (1 << INB) | (1 << EN);
      
      *OCR = uint16_t(1.0*abs(power));
   }
   
   // Clockwise
   else if(power > 0)
   {
      *PORT_DIR |= (1 << INA) | (1 << EN);
      *PORT_DIR &= ~(1 << INB);
      
      *OCR = uint16_t(1.0*abs(power));
      
   }	
   // freewheeling
   else
   {
      *OCR = 0;
   }
}

//-------------------------------------------------------------------------------------
/** \brief This function stops the motor.
 *  \details The brake function sets the mode select bits A & B to the same value.    
 */

void motor_driver::brake (void)
{
   *PORT_DIR |= (1 << INA) | (1 << INB);
   *PORT_EN |= (1 << EN); // Initialize mode for motor
}

//-------------------------------------------------------------------------------------
/** \brief This function sets the position of the motor.
 *  \details The brake function sets the mode select bits A & B to the same value.    
 */
void motor_driver::move_cw (void)
{
    //*OCR = 10000;
    *PORT_EN |= (1 << EN); // Initialize mode for motor
    if(motor_trans)
    {
     *PORT_DIR |= (1 << INA);
     *PORT_DIR &= ~(1 << INB);
     motor_trans = false;
    }
    else{
     *PORT_DIR |= (1 << INB);
     *PORT_DIR &= ~(1 << INA);
     motor_trans = true;
    }
}

void motor_driver::move(int16_t delta)
{	
	if(delta > 0)
	{
		set_power(PI(delta)); //go clockwise
	}
	
	else if(delta < 0)
	{
		set_power(-PI(delta)); // go counterclockwise
	}
}

double motor_driver::PI(uint16_t error)
{
	//Porportional error variables
	double pTerm = PCONSTANT + PGAIN * abs(error);
	
   /* Integral part
	//Integral error variables
	double iTerm = 0.0000005;
	uint32_t iState = 0;
	uint32_t iMax = 72000000;
	uint8_t iMin = 0;
	double iGain = 0.0;
	
	
	if(error <= TOLERANCE)
	{
	  iState = 0;
	}
	
	else
	{
	  iState += position_error.get();
	  
	  if( iState > iMax )
	  {
		iState = iMax;
	  }
	  else if( iState < iMin )
	  {
		iState = iMin;
	  }
	  
	  iTerm = iGain * iState;
	}
   End Integral */

	return pTerm;
}



//*************************************************************************************
/** \file my_motor_driver.cpp
 
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
//#include "shares.h"                // Include header for the A/D class
//#include "z_encoder_driver.h"                // Include header for the A/D class
#include "encoder_driver.h"                // Include header for the A/D class
#include "z_encoder_driver.h"                // Include header for the A/D class
#define STOP_CONST 25

//---------------------		//p_motor_1->set_power(control1);----------------------------------------------------------------
/** \brief This constructor initializes the motor driver. 
 *  \details The motor driver is made ready so that when a function such as \c set_power() is called the motor runs.
 *  when the \c brake() function is called it stops the motor if running. 
 *  @param PORT initializes the data direction registers for the motor
 *  @param INA mode select for bit A of the motor
 *  @param INB mode select for bit B of the motor
 *  @param EN output enable and diagnostics
 *  @param OCR the pwm control register 
 */
	//motor_driver* p_motor_1 = new my_motor_driver (p_serial, &DDRD, &DDRC, &DDRB, &PORTD, &PORTC, PD7, PC3, PC2, PB5, COM1B1, &OCR1B);
//Initialize my_motor_driver
encoder_driver::encoder_driver(volatile uint8_t* DDR_en, volatile uint8_t* PIN_en, volatile uint8_t* PORT_EN, uint8_t Abit, uint8_t Bbit, volatile Direction* direction_in, bool locatedAtZero_in)
{
	
	DDR_EN = DDR_en;
	
	PIN = PIN_en;

   direction = direction_in;
	
   if (locatedAtZero_in)
   {
      INA = Abit; 
      INB = Bbit; 
   }
   else
   {
      INA = Bbit;
      INB = Abit;
   }

	*DDR_EN &= ~((1 << INA) | (1 << INB)); // Input enable for Encoder Pin A and B

   *PORT_EN |= (1 << INA) | (1 << INB); // Activate pull up resister
   reset();
}

void encoder_driver::updatePosition(void)
{
   uint8_t newA = (*PIN >> INA) & 0x1;
   uint8_t newB = (*PIN >> INB) & 0x1;
   
   uint8_t sum = (newA << 1) | newB;

   if (sum != prevSum)
   {
      if (prevA != newA)
      {
         if (prevSum == 1 || prevSum == 2)
         {
            position++;
         }
         else
         {
            position--;
         }
      }
      else
      {
         if (prevSum == 0 || prevSum == 3)
         {
            position++;
         }
         else
         {
            position--;
         }
      }
      prevA = newA;
      prevSum = sum;
   }
}

int16_t encoder_driver::getPosition(void)
{
   return position;
}

void encoder_driver::reset(void)
{
   position = 0;
   prevA = (*PIN >> INA) & 1;
   prevSum = prevA << 1 | ((*PIN >> INB) & 1);
}

void encoder_driver::setSerial(emstream* p_serial_port)
{
	ptr_to_serial = p_serial_port;
}

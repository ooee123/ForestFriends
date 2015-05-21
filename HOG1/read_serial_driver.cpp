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
#include <ctype.h>

#include "rs232int.h"                       // Include header for serial port class
#include "read_serial_driver.h"                // Include header for the A/D class
#define STOP_CONST 25
//#define BINARY_SERIAL

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
//Initialize my_motor_driver
read_serial_driver::read_serial_driver(rs232 *serial_in)
{
   serial = serial_in;
}

uint16_t read_serial_driver::read_uint16_t()
{
   uint16_t num;
   #ifdef BINARY_SERIAL
      num = serial->getchar();
      return (num << 8) | serial->getchar();
   #else
      num = serial->getchar() - '0';
      num = num * 10 + serial->getchar() - '0';
      num = num * 10 + serial->getchar() - '0';
      return num;
   #endif
}

uint8_t read_serial_driver::read_uint8_t()
{
   #ifdef BINARY_SERIAL
      return serial->getchar();
   #else
      return serial->getchar() - '0';
   #endif
}

//*************************************************************************************
/*  License:
 *		This file is copyright 2012 by JR Ridgely and released under the Lesser GNU 
 *		Public License, version 2. It intended for educational use only, but its use
 *		is not limited thereto.*/
/*		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *		AND	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * 		IMPLIED 	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * 		ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * 		LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUEN-
 * 		TIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 * 		OR SERVICES; LOSS OF USE, DATA
 * , OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * 		CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * 		OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * 		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */
//*************************************************************************************


#include <stdlib.h>                         // Prototype declarations for I/O functions
#include <avr/io.h>                         // Port I/O for SFR's
#include <avr/wdt.h>                        // Watchdog timer header
#include <string.h>                         // Functions for C string handling

#include "FreeRTOS.h"                       // Primary header for FreeRTOS
#include "task.h"                           // Header for FreeRTOS task functions
#include "queue.h"                          // FreeRTOS inter-task communication queues
#include "croutine.h"                       // Header for co-routines and such

#include "rs232int.h"                       // ME405/507 library for serial comm.
#include "time_stamp.h"                     // Class to implement a microsecond timer
#include "frt_task.h"                       // Header of wrapper for FreeRTOS tasks
#include "frt_text_queue.h"                 // Wrapper for FreeRTOS character queues
#include "frt_queue.h"                      // Header of wrapper for FreeRTOS queues
#include "frt_shared_data.h"                // Header for thread-safe shared data
#include "shares.h"                         // Global ('extern') queue declarations
#include "motor_driver.h"  
#include "encoder_driver.h"  
#include "motor_task.h"  
#include "z_encoder_driver.h"  
#include "z_motor_task.h"
#include "read_serial_driver.h"  
#include "read_serial_task.h"  

#include <util/delay.h>			     // Delay 
#include "pinLayout.h"
#include "constants.h"


// Declare the queues which are used by tasks to communicate with each other here. 
// Each queue must also be declared 'extern' in a header file which will be read 
// by every task that needs to use that queue. The format for all queues except 
// the serial text printing queue is 'frt_queue<type> name (size)', where 'type' 
// is the type of data in the queue and 'size' is the number of items (not neces-
// sarily bytes) which the queue can hold

/** This is a print queue, descended from \c emstream so that things can be printed 
 *  into the queue using the "<<" operator and they'll come out the other end as a 
 *  stream of characters. It's used by tasks that send things to the user interface 
 *  task to be printed. 
 */

//frt_text_queue* print_ser_queue = new frt(text_queue;

/** This queue sends data from the source task to the sink task (both of these tasks
 *  have been removed in this revision).
 */

/*
frt_queue<uint16_t> queue_x (20);
frt_queue<uint16_t> queue_y (20);
frt_queue<uint16_t> queue_z (20);
*/

frt_text_queue print_ser_queue(32, NULL, 10);
/** This shared data item allows communication between the motor task and the task user.
 *  In this way, we can make a user interface which can update the motor operation in
 *  realtime as the tasks are called pseudo simultaneously in the RTOS.
 */
//shared_data<uint16_t> power_level; // motor power level

//=====================================================================================
/** The main function sets up the RTOS.  Some test tasks are created. Then the 
 *  scheduler is started up; the scheduler runs until power is turned off or there's a 
 *  reset.
 *  @return This is a real-time microcontroller program which doesn't return. Ever.
 */

//motor_driver* xAxis = new motor_driver (&DDRD, &DDRC, &DDRB, &PORTD, &PORTC, PD7, PC3, PC2, PB5, COM1A1, &OCR1A, false, X_PGAIN, X_PCONSTANT, X_POWERMIN, X_POWERMAX);
//motor_driver* yAxis = new motor_driver (&DDRC, &DDRC, &DDRB, &PORTC, &PORTC, PC0, PC5, PC4, PB6, COM1B1, &OCR1B, true, Y_PGAIN, Y_PCONSTANT, Y_POWERMIN, Y_POWERMAX);
//motor_driver* zAxis = new motor_driver (&DDRC, &DDRC, &DDRB, &PORTC, &PORTC, PC1, PC7, PC6, PB7, COM1C1, &OCR1C, true, Z_PGAIN, Z_PCONSTANT, Z_POWERMIN, Z_POWERMAX);
motor_driver xAxis(&DDRD, &DDRC, &DDRB, &PORTD, &PORTC, PD7, PC3, PC2, PB5, COM1A1, &OCR1A, true, X_PGAIN, X_PCONSTANT, X_POWERMIN, X_POWERMAX
#ifdef INTEGRAL
, X_IGAIN, X_IMIN, X_IMAX
#endif
);
motor_driver yAxis(&DDRC, &DDRC, &DDRB, &PORTC, &PORTC, PC0, PC5, PC4, PB6, COM1B1, &OCR1B, false, Y_PGAIN, Y_PCONSTANT, Y_POWERMIN, Y_POWERMAX
#ifdef INTEGRAL
, Y_IGAIN, Y_IMIN, Y_IMAX
#endif
);
motor_driver zAxis(&DDRC, &DDRC, &DDRB, &PORTC, &PORTC, PC1, PC7, PC6, PB7, COM1C1, &OCR1C, false, Z_PGAIN, Z_PCONSTANT, Z_POWERMIN, Z_POWERMAX
#ifdef INTEGRAL
, Z_IGAIN, Z_IMIN, Z_IMAX
#endif
);

encoder_driver xEncoder(&X_ENCODER_DDR, &X_ENCODER_PIN, &X_ENCODER_PORT, X_ENCODER_PINA, X_ENCODER_PINB, xAxis.getDirection(), true);
encoder_driver yEncoder(&Y_ENCODER_DDR, &Y_ENCODER_PIN, &Y_ENCODER_PORT, Y_ENCODER_PINA, Y_ENCODER_PINB, yAxis.getDirection(), false);
z_encoder_driver zEncoder(&Z_ENCODER_DDR, &Z_ENCODER_PIN, &Z_ENCODER_PORT, Z_ENCODER_PINA, Z_ENCODER_PINB, zAxis.getDirection(), false);
	// Configure a serial port which can be used by a task to print debugging infor-
	// mation, or to allow user interaction, or for whatever use is appropriate.  The
	// serial port will be used by the user interface task after setup is complete and
	// the task scheduler has been started by the function vTaskStartScheduler()
rs232 ser_port (9600, 0);
int32_t desiredX = 0;
int32_t desiredY = 0;
int32_t desiredZ = 0;
volatile State state = NORMAL;
bool zReady = false;

// Enc Z A SDA
ISR(INT0_vect)
{
   zEncoder.updatePosition();
   if (!getZLimitSwitch() || !getZMaxLimitSwitch())
   {
      zAxis.brake();
   }
   else
   {
      if (!zReady && state == NORMAL)
      {
         //zAxis.move(zEncoder.getPosition() - desiredZ);
         zAxis.move(desiredZ - zEncoder.getPosition());
      }
      else
      {
         zAxis.brake();
      }
   }
   #ifdef DEBUG
      if (zEncoder.getPosition() % 64 == 0)
      {
         ser_port << zEncoder.getPosition();
         ser_port << "\n";
      }
   #endif
}

// Enc Z B SCL
ISR(INT1_vect, ISR_ALIASOF(INT0_vect));

// Enc X A PE4
ISR(INT4_vect)
{
   xEncoder.updatePosition();
   if (!getXLimitSwitch() || !getXMaxLimitSwitch())
   {
      xAxis.brake();
   }
   else
   {
      if (zReady && state == NORMAL)
      {
         //xAxis.move(xEncoder.getPosition() - desiredX);
         xAxis.move(desiredX - xEncoder.getPosition());
      }
      else
      {
         xAxis.brake();
      }
   }
   #ifdef DEBUG
      if (xEncoder.getPosition() % 64 == 0)
      {
         ser_port << xEncoder.getPosition();
         ser_port << "\n";
      }
   #endif
}

// Enc X B INT
ISR(INT5_vect, ISR_ALIASOF(INT4_vect));

// Enc Y A PE6
ISR(INT6_vect)
{
   yEncoder.updatePosition();
   if (!getYLimitSwitch() || !getYMaxLimitSwitch())
   {
      yAxis.brake();
   }
   else
   {
      if (zReady && state == NORMAL)
      {
         //yAxis.move(yEncoder.getPosition() - desiredY);
         yAxis.move(desiredY - yEncoder.getPosition());
      }
      else
      {
         yAxis.brake();
      }
   }
   #ifdef DEBUG
      if ((yEncoder.getPosition() % 64) == 0)
      {
         ser_port << yEncoder.getPosition();
         ser_port << "\n";
      }
   #endif
}

// Enc Y B PE7
ISR(INT7_vect, ISR_ALIASOF(INT6_vect));

int main (void)
{
   /* Kevin's testing stuff. Test for pin output */
         
	// Disable the watchdog timer unless it's needed later. This is important because
	// sometimes the watchdog timer may have been left on...and it tends to stay on
	wdt_disable ();
   EICRA = 0b00001111; // Set Int_0-1 to activate on rising edge
   #ifdef FAST_ENCODER
      EICRB = 0b11111111; // Set Int_4-7 to activate on rising edge 
      EIMSK = 0b01010001; // Turn on Int_0, 4, 6
   #else
      EICRB = 0b01010101; // Set Int_4-7 to activate on pin toggle
      EIMSK = 0b11110011; // Turn on Int_0-1, Int_4-7
   #endif
   setupLimitSwitch(X_LIMIT_DDR, X_LIMIT_PORT, X_MAX_LIMIT_PIN_NUM);
   setupLimitSwitch(Y_LIMIT_DDR, Y_LIMIT_PORT, Y_MAX_LIMIT_PIN_NUM);
   setupLimitSwitch(Z_LIMIT_DDR, Z_LIMIT_PORT, Z_MAX_LIMIT_PIN_NUM);
   #ifdef CURRENT_SENSOR
      
      _setBit(CURRENT_DDR, X_CURRENT_PIN_POWER);
      _setBit(CURRENT_DDR, Y_CURRENT_PIN_POWER);
      _setBit(CURRENT_DDR, Z_CURRENT_PIN_POWER);
      _clearBit(CURRENT_DDR, X_CURRENT_PIN_NUM);
      _clearBit(CURRENT_DDR, Y_CURRENT_PIN_NUM);
      _clearBit(CURRENT_DDR, Z_CURRENT_PIN_NUM);
      _setBit(CURRENT_PORT, X_CURRENT_PIN_POWER);
      _setBit(CURRENT_PORT, Y_CURRENT_PIN_POWER);
      _setBit(CURRENT_PORT, Z_CURRENT_PIN_POWER);

      ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescalar to 128 - 125KHz sample rate @ 16MHz

      ADMUX |= (1 << REFS0); // Set ADC reference to AVCC
      ADMUX |= (1 << ADLAR); // Left adjust ADC result to allow easy 8 bit reading

      // No MUX values needed to be changed to use ADC0

      ADCSRA |= (1 << ADEN);  // Enable ADC
      ADCSRA |= (1 << ADSC);  // Start A2D Conversions

   #endif
   sei();

	// task that controls motors
   xEncoder.setSerial(&ser_port);
	new motor_task ("X", task_priority (2), 280, &ser_port, &xAxis, &xEncoder, &desiredX, X_CALIBRATE_SPEED, &X_LIMIT_DDR, &X_LIMIT_PORT, &X_LIMIT_PIN, X_ZERO_LIMIT_PIN_NUM, X_MAX_LIMIT_PIN_NUM, &state, &zReady);
   yEncoder.setSerial(&ser_port);
	new motor_task ("Y", task_priority (2), 280, &ser_port, &yAxis, &yEncoder, &desiredY, Y_CALIBRATE_SPEED, &Y_LIMIT_DDR, &Y_LIMIT_PORT, &Y_LIMIT_PIN, Y_ZERO_LIMIT_PIN_NUM, Y_MAX_LIMIT_PIN_NUM, &state, &zReady);
   zEncoder.setSerial(&ser_port);
   new z_motor_task ("Z", task_priority (2), 280, &ser_port, &zAxis, &zEncoder, &desiredZ, Z_CALIBRATE_SPEED, &Z_LIMIT_DDR, &Z_LIMIT_PORT, &Z_LIMIT_PIN, Z_ZERO_LIMIT_PIN_NUM, Z_MAX_LIMIT_PIN_NUM, &state, &zReady);
   // task that reads incoming serial data
   read_serial_driver* serial = new read_serial_driver(&ser_port);
   new read_serial_task("S", task_priority (1), 280, &ser_port, serial, &desiredX, &desiredY, &desiredZ, &xEncoder, &yEncoder, &zEncoder, &state, &zReady, 0);
	// Here's where the RTOS scheduler is started up. It should never exit as long as
	// power is on and the microcontroller isn't rebooted

   while (!getXMaxLimitSwitch())
   {
      ser_port << "X";
      xAxis.move(X_CALIBRATE_SPEED);
   }
   xAxis.brake();

   while (!getYMaxLimitSwitch())
   {
      ser_port << "Y";
      yAxis.move(Y_CALIBRATE_SPEED);
   }
   yAxis.brake();
   while (!getZMaxLimitSwitch())
   {
      ser_port << "Z";
      zAxis.move(Z_CALIBRATE_SPEED);
   }
   zAxis.brake();
   //xAxis.move(550);
   //yAxis.move(150);
   //zAxis.move(128);
   //zAxis.move(165);

	vTaskStartScheduler ();
}

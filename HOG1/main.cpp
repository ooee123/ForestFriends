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
#include "motor_task.h"  
#include "encoder_driver.h"  
#include "read_serial_driver.h"  
#include "read_serial_task.h"  

#include <util/delay.h>			     // Delay 
#include "pinLayout.h"
#include "constants.h"
#include "state.h"
#include "z_motor_task.h"


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

//encoder_driver* xEncoder = new encoder_driver(&DDRE, &PINE, &PORTE, PE4, PE5);
//encoder_driver* yEncoder = new encoder_driver(&DDRE, &PINE, &PORTE, PE6, PE7);
//encoder_driver* zEncoder = new encoder_driver(&DDRE, &PINE, &PORTE, PA1, PA0);
encoder_driver* xEncoder = new encoder_driver(&X_ENCODER_DDR, &X_ENCODER_PIN, &X_ENCODER_PORT, X_ENCODER_PINA, X_ENCODER_PINB);
encoder_driver* yEncoder = new encoder_driver(&Y_ENCODER_DDR, &Y_ENCODER_PIN, &Y_ENCODER_PORT, Y_ENCODER_PINA, Y_ENCODER_PINB);
encoder_driver* zEncoder = new encoder_driver(&Z_ENCODER_DDR, &Z_ENCODER_PIN, &Z_ENCODER_PORT, Z_ENCODER_PINA, Z_ENCODER_PINB);
motor_driver* xAxis = new motor_driver (&DDRD, &DDRC, &DDRB, &PORTD, &PORTC, PD7, PC3, PC2, PB5, COM1A1, &OCR1A, X_PGAIN, X_PCONSTANT, X_POWERMIN, X_POWERMAX);
motor_driver* yAxis = new motor_driver (&DDRC, &DDRC, &DDRB, &PORTC, &PORTC, PC0, PC5, PC4, PB6, COM1B1, &OCR1B, Y_PGAIN, Y_PCONSTANT, Y_POWERMIN, Y_POWERMAX);
motor_driver* zAxis = new motor_driver (&DDRC, &DDRC, &DDRB, &PORTC, &PORTC, PC1, PC7, PC6, PB7, COM1C1, &OCR1C, Z_PGAIN, Z_PCONSTANT, Z_POWERMIN, Z_POWERMAX);
rs232 ser_port (9600, 0);
uint16_t desiredX = 0;
uint16_t desiredY = 0;
uint16_t desiredZ = 0;
uint16_t adcValue = 0;
volatile State state = NORMAL;
// X Current Switch SCL
ISR(INT0_vect)
{
   state = HOME;
   desiredX = 0;
   desiredY = 0;
   desiredZ = 0;
   _clearBit(SOLID_STATE_PORT, SOLID_STATE_PIN_NUM);
}

// Y Current Switch SDA
ISR(INT1_vect)
{
   state = HOME;
   desiredX = 0;
   desiredY = 0;
   desiredZ = 0;
   _clearBit(SOLID_STATE_PORT, SOLID_STATE_PIN_NUM);
}

// Enc X A PE4
ISR(INT4_vect)
{
   xEncoder->updatePosition();
   xAxis->move(desiredX - xEncoder->getPosition());
}

// Enc X B INT
ISR(INT5_vect)
{
   xEncoder->updatePosition();
   xAxis->move(desiredX - xEncoder->getPosition());
}

// Enc Y A PE6
ISR(INT6_vect)
{
   yEncoder->updatePosition();
   yAxis->move(desiredY - yEncoder->getPosition());
}

// Enc Y B PE7
ISR(INT7_vect)
{
   yEncoder->updatePosition();
   yAxis->move(desiredY - yEncoder->getPosition());
}

int main (void)
{
   /* Kevin's testing stuff. Test for pin output */
         
	// Disable the watchdog timer unless it's needed later. This is important because
	// sometimes the watchdog timer may have been left on...and it tends to stay on
	wdt_disable ();
   sei();
   EICRB = 0b01010101; // Set Int_4-7 to activate on pin toggle
   EIMSK = 0b11110000; // Turn on Int_4-7
   DDRA = 1;
   #ifdef Z_AXIS
      bool zReady = false;
   #else
      bool zReady = true;
   #endif
   #ifdef CURRENT_SENSOR

   ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescalar to 128 - 125KHz sample rate @ 16MHz

   ADMUX |= (1 << REFS0); // Set ADC reference to AVCC
   ADMUX |= (1 << ADLAR); // Left adjust ADC result to allow easy 8 bit reading

   // No MUX values needed to be changed to use ADC0

   //ADCSRA |= (1 << ADFR);  // Set ADC to Free-Running Mode
   ADCSRA |= (1 << ADEN);  // Enable ADC
   ADCSRA |= (1 << ADSC);  // Start A2D Conversions

   #endif


	// Configure a serial port which can be used by a task to print debugging infor-
	// mation, or to allow user interaction, or for whatever use is appropriate.  The
	// serial port will be used by the user interface task after setup is complete and
	// the task scheduler has been started by the function vTaskStartScheduler()


	// task that controls motors
   xEncoder->setSerial(&ser_port);
	new motor_task ("X", task_priority (2), 280, &ser_port, xAxis, xEncoder, &desiredX, &X_LIMIT_DDR, &X_LIMIT_PORT, &X_LIMIT_PIN, X_ZERO_LIMIT_PIN_NUM, &state, &zReady);
   yEncoder->setSerial(&ser_port);
	new motor_task ("Y", task_priority (2), 280, &ser_port, yAxis, yEncoder, &desiredY, &Y_LIMIT_DDR, &Y_LIMIT_PORT, &Y_LIMIT_PIN, Y_ZERO_LIMIT_PIN_NUM, &state, &zReady);
   #ifdef Z_AXIS
      zEncoder->setSerial(&ser_port);
      new z_motor_task ("Z", task_priority (2), 280, &ser_port, zAxis, zEncoder, &desiredZ, &Z_LIMIT_DDR, &Z_LIMIT_PORT, &Z_LIMIT_PIN, Z_ZERO_LIMIT_PIN_NUM, &state, &zReady);
   #endif
   read_serial_driver* serial = new read_serial_driver(&ser_port);
   new read_serial_task("S", task_priority (1), 280, &ser_port, serial, &desiredX, &desiredY, &desiredZ, xEncoder, yEncoder, zEncoder, &state, &zReady, 0);
   // task that reads incoming serial data
	// Here's where the RTOS scheduler is started up. It should never exit as long as
	// power is on and the microcontroller isn't rebooted

	vTaskStartScheduler ();
}

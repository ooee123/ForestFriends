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
#include "read_serial_driver.h"  

#include <util/delay.h>			     // Delay 

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

frt_text_queue print_ser_queue (32, NULL, 10);

/** This queue sends data from the source task to the sink task (both of these tasks
 *  have been removed in this revision).
 */

frt_queue<uint32_t> queue_1 (20);

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

// Enc 1 A SCL
ISP(INT0_vect)
{
}

// Enc 1 B SDA
ISP(INT1_vect)
{
}

// Enc 2 A PE4
ISP(INT4_vect)
{
}

// Enc 2 B INT
ISP(INT5_vect)
{
}

// Enc 3 A PE6
ISP(INT6_vect)
{
}

// Enc 3 A PE7
ISP(INT7_vect)
{
}

int main (void)
{
   /* Kevin's testing stuff. Test for pin output */
   /*
   EICRB = 0b10101010;
   EIMSK = 0b11110011;

   DDRA |= 0b00000001;
   
   DDRE = 0;
   PORTE = ~0;
   */
   /*
   for (;;)
   {
      PORTA = 0;
      if ((PINE >> 7) & 1)
      //if ((PINE >> 4) & 1)
      {
         PORTA |= (1 << 0);
      }
      else
      {
         PORTA &= ~(1 << 0);
      }
   }
   */
         
	// Disable the watchdog timer unless it's needed later. This is important because
	// sometimes the watchdog timer may have been left on...and it tends to stay on
	wdt_disable ();
	// Configure a serial port which can be used by a task to print debugging infor-
	// mation, or to allow user interaction, or for whatever use is appropriate.  The
	// serial port will be used by the user interface task after setup is complete and
	// the task scheduler has been started by the function vTaskStartScheduler()
	rs232 ser_port (9600, 0);

   uint16_t desiredX;
   uint16_t desiredY;
   uint16_t desiredZ;

   sei();
   /*
   ser_port.putchar('t');
   ser_port << ser_port.check_for_char();
   ser_port.putchar(ser_port.getchar());
   ser_port.putchar('s');
   */


   read_serial_driver* serial; // = new read_serial_driver(&ser_port, &desiredX, &desiredY, &desiredZ);
      
	// task that controls motors
	motor_driver* xAxis = new motor_driver (&DDRD, &DDRC, &DDRB, &PORTD, &PORTC, PD7, PC3, PC2, PB5, COM1A1, &OCR1A);
   encoder_driver* xEncoder = new encoder_driver(&DDRA, &PINA, &PORTA, PA5, PA4);
   xEncoder->setSerial(&ser_port);
	new motor_task ("X", task_priority (1), 280, &ser_port, xAxis, xEncoder, &desiredX);

   /*
	motor_driver* yAxis = new motor_driver (&DDRC, &DDRC, &DDRB, &PORTC, &PORTC, PC0, PC5, PC4, PB6, COM1B1, &OCR1B);
   encoder_driver* yEncoder = new encoder_driver(&DDRA, &PINA, &PORTA, PA3, PA2);
   yEncoder->setSerial(&ser_port);
	new motor_task ("Y", task_priority (1), 280, &ser_port, yAxis, yEncoder, &desiredY);
	motor_driver* zAxis = new motor_driver (&DDRC, &DDRC, &DDRB, &PORTC, &PORTC, PC1, PC7, PC6, PB7, COM1C1, &OCR1C);
   encoder_driver* zEncoder = new encoder_driver(&DDRA, &PINA, &PORTA, PA1, PA0);
   zEncoder->setSerial(&ser_port);
	new motor_task ("Z", task_priority (1), 280, &ser_port, zAxis, zEncoder, &desiredZ);
   */
   // task that reads incoming serial data
	// Here's where the RTOS scheduler is started up. It should never exit as long as
	// power is on and the microcontroller isn't rebooted

	vTaskStartScheduler ();
}

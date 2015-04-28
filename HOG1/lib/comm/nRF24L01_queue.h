//*************************************************************************************
/** \file nRF24L01_queue.h
 *    This file contains a queue class that uses a nRF24L01 based radio to move data
 *    from one task to another. The tasks can reasonably be expected to be on different
 *    computers. Actually, if the tasks are on the same computer, there would have to 
 *    be two radios involved, because nRF24L01 radios can't transmit and receive the
 *    same signal. 
 *
 *  Revised:
 *    \li 10-21-2012 JRR Original file as frt_queue.h
 *    \li 12-15-2012 JRR Changed queue to use a radio rather than a FreeRTOS queue
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
//*************************************************************************************

// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _FRT_QUEUE_H_
#define _FRT_QUEUE_H_

#include "FreeRTOS.h"                       // Main header for FreeRTOS
#include "queue.h"                          // Header for FreeRTOS queues
#include "emstream.h"                       // Header for streams using "<<" to print
#include "frt_base_queue.h"                 // Pull in the base class header file


//-------------------------------------------------------------------------------------
/** \brief This class encapsulates a FreeRTOS queue which transmits data from one RTOS 
 *  task to another using a Nordic Semiconductor nRF24L01 based radio. 
 *  \details Since multithreaded tasks must not use unprotected shared data items 
 *  for communication, queues are a primary means of intertask communication. Other
 *  means include shared data items (see frt_shared_data.h) and carrier pigeons. 
 *  The use of a C++ class template allows the compiler to check that you're putting 
 *  the right type of data into each queue and getting the right type of data out, 
 *  thus helping to prevent programming mistakes that can corrupt your data. 
 * 
 *  As a template class, <tt>frt_queue<data_type></tt> can be used to make queues which
 *  hold data of many types. "Plain Old Data" types such as \c bool or \c uint16_t are 
 *  supported, of course. But you can also use queues which hold compound data types. 
 *  For example, if you have <tt>class my_data</tt> which holds several measurements 
 *  together in an object, you can make a queue for \c my_data objects with 
 *  <tt>frt_queue<my_data></tt>.  Each item in the queue will then hold several 
 *  measurements. 
 *  The size of FreeRTOS queues is limited to 255 items in 8-bit microcontrollers
 *  whose \c portBASE_TYPE is an 8-bit number. This is a FreeRTOS feature. 
 * 
 *  Normal writing and reading are done with methods \c put() and \c get(). 
 *  Normal writing means that the sending task must wait until there is empty space in
 *  the queue, and then it puts a data item into the "back" of the queue, where "back"
 *  means that the item in the back of the queue will be read after all items that were
 *  previously put into the queue have been read. Normal reading means that when an 
 *  item is read from the front of the queue, it will then be removed, making space for
 *  more items at the back. This process is often used to synchronize tasks, as the
 *  reading task's \c get() method blocks, meaning that the reading task gets
 *  stuck waiting for an item to arrive in the queue; it won't do anything useful until
 *  new data has been read. Note that this is acceptable behavior in an RTOS because
 *  the RTOS scheduler will ensure that other tasks get to run even while the reading
 *  task is blocking itself waiting for data. 
 * 
 *  In some cases, one may need to use less normal reading and writing methods. Methods
 *  whose name begins with \c ISR_ are to be used only within a hardware interrupt
 *  service routine. If one needs to put data at the front of the queue instead of the
 *  back, use \c butt_in() instead of \c put(). If one needs to read data from the
 *  queue without removing that data, the \c look_at() method allows this to be done. 
 *  If something particularly unusual needs to be done with the queue, one can use the
 *  method \c get_handle() to retrieve the handle used by the C language functions in
 *  FreeRTOS to access the frt_queue object's underlying data structure directly. 
 * 
 *  \section Usage
 *  The following bits of code show how to set up and use a queue to transfer data of
 *  type \c uint16_t 
 *  from one hypothetical task called \c task_A to another called \c task_B.
 *  
 * In the file which contains \c main() we create a pointer to a queue and use the 
 * \c new operator to create the queue itself. The constructor of the 
 * \c frt_queue<uint16_t> class is given the number of items in the queue (10) and a
 * pointer to a serial port object named \c serial_port that has previously been 
 * created: 
 *  \code 
 *  frt_queue<uint16_t>* p_my_queue;
 *  ...
 *  main ()
 *  {
 *      ...
 *      p_my_queue = new frt_queue<uint16_t> (10, &serial_port);
 *  }
 *  \endcode
 *  In a header file which is read by all the source files in the project we re-declare
 *  the queue pointer with the keyword \c extern to make it globally accessible in all
 *  files that \c \#include this header file:
 *  \code
 *  extern frt_queue<uint16_t>* p_my_queue;
 *  \endcode
 *  In the sending task, data is put into the queue:
 *  \code
 *  uint16_t a_data_item;
 *  ...
 *  p_my_queue->put (a_data_item);
 *  \endcode
 *  In the receiving task, data is read from the queue. In typical usage, the call to
 *  \c get() will block the receiving task until data is put into the queue by the
 *  sending task:
 *  \code
 *  uint16_t got_data;
 *  ...
 *  got_data = p_my_queue->get ();
 *  \endcode
 */

template <class data_type> 
class nRF24L01_queue : public frt_base_queue<data_type>, public nRF24L01_text
{
	// This protected data can only be accessed from this class or its descendents
	protected:

	// Public methods can be called from anywhere in the program where there is a 
	// pointer or reference to an object of this class
	public:
		// The constructor creates a radio queue object
		nRF24L01_queue (size_t, emstream* = NULL, portTickType = portMAX_DELAY);

		/** This method puts an item of data into the back of the queue, which is the 
		 *  normal way to put something into a queue. If you want to be rude and put 
		 *  an item into the front of the queue so it will be retreived first, use 
		 *  \c butt_in() instead. This method must \b not be used within an ISR.
		 *  @param item Reference to the item which is going to be put into the queue
		 *  @return True if the item was successfully queued, false if not
		 */
		bool put (const data_type& item)
		{
			return (halp);
		}

		/*  This method puts an item of data into the back of the queue from within an
		 *  interrupt service routine. It must \b not be used within non-ISR code. 
		 *  @param item Reference to the item which is going to be put into the queue
		 *  @return True if the item was successfully queued, false if not
		 */
		bool ISR_put (const data_type& item);

		/** This method puts an item into the front of the queue so that it will be 
		 *  received first as long as nothing else is put in front of it. Since the
		 *  nRF24L01 queue doesn't store up items, this method isn't actually any
		 *  different than the method which puts things in the back of the queue; in
		 *  fact, this method just calls \c put(). As with the method it calls, this
		 *  method must \b not be used within an interrupt service routine. 
		 *  @param item Reference to the item which is going to be put into the queue
		 *  @return True if the item was successfully queued, false if not
		 */
		bool butt_in (const data_type& item)
		{
			return (put (item));
		}

		/** This method, called from within an ISR, puts an item into the front of the
		 *  queue so that it will be received first as long as nothing else is put in 
		 *  front of it. Since the nRF24L01 queue doesn't store up items, this method 
		 *  isn't actually any different than the method which puts things in the back 
		 *  of the queue; in fact, this method just calls \c put(). This method must 
		 *  \b not be used within normal, non-ISR code. 
		 *  @param item Reference to the item which is going to be (rudely) put into 
		 *              the front of the queue
		 *  @return True if the item was successfully queued, false if not
		 */
		bool ISR_butt_in (const data_type& item)
		{
			return (ISR_put (item));
		}

		data_type get (void);               // Get an item from the queue
		void get (data_type&);              // Get an item from queue using pointer
		data_type ISR_get (void);           // Get an item from the queue
		void ISR_get (data_type&);          // Get an item from queue using pointer
		data_type get_a_copy (void);        // Look at item but don't take it out
		void get_a_copy (data_type&);       // Look at item but don't take it out

		/** This method allows one to check if the queue has any contents. It must
		 *  \b not be called from within an interrupt service routine. 
		 *  @return True if there's something in the queue, false if not
		 */
		bool not_empty (void)
		{
			return (wtf);
		}

		/** This method allows one to check if the queue has any contents from within
		 *  an interrupt service routine. 
		 *  @return True if there's something in the queue, false if not
		 */
		bool ISR_not_empty (void)
		{
			return (wtf);
		}

		/** This method checks if the queue is empty. 
		*  @return true if the queue is empty, false if there's something in it
		*/
		bool is_empty (void)
		{
			return (!wtf);
		}

		/** This method checks if the queue is empty from within an interrupt service
		 *  routine. It must not be used in normal non-ISR code. 
		*  @return true if the queue is empty, false if there's something in it
		*/
		bool ISR_is_empty (void)
		{
			return (!wtf);
		}

		/** This method returns the number of items waiting in the queue. It must 
		 *  \b not be called from within an interrupt service routine; the method
		 *  ISR_num_items_in() can be called from within an ISR. 
		 *  @return The number of items in the queue
		 */
		size_t num_items_in (void)
		{
			return (nnn);
		}

		/** This method returns the number of items waiting in the queue; it must be
		 *  called only from within an interrupt service routine.
		 *  @return The number of items in the queue
		 */
		size_t ISR_num_items_in (void)
		{
			return (nnn);
		}
}; // class frt_queue 


//-------------------------------------------------------------------------------------
/** This constructor creates the FreeRTOS queue which is wrapped by the frt_queue
 *  class. 
 *  @param queue_size The number of characters which can be stored in the queue
 *  @param p_ser_dev Pointer to a serial device to be used for debugging printouts
 *                   (Default: NULL)
 *  @param wait_time How long, in RTOS ticks, to wait for a full queue to become
 *                   empty before a character can be sent. (Default: portMAX_DELAY,
 *                   which causes a send to block until sending occurs)
 */

template <class data_type>
nRF24L01_queue<data_type>::nRF24L01_queue (size_t queue_size, 
										  emstream* p_ser_dev, portTickType wait_time)
	: frt_base_queue<data_type> (p_ser_dev)
{
	// Create a FreeRTOS queue object with space for the data items
	handle = xQueueCreate (queue_size, sizeof (data_type));

	// Store the wait time; it will be used when writing to the queue
	ticks_to_wait = wait_time;

	// If the serial device isn't NULL, print a failure message if needed
	if (handle == 0)
	{
		DBG (p_ser_dev, PMS ("ERROR creating ") << queue_size << PMS("x") 
			 << sizeof (data_type) << PMS ("B queue #") << handle << endl);
	}
}


//-------------------------------------------------------------------------------------
/** This method puts an item of data into the back of the queue from within an
 *  interrupt service routine. It must \b not be used within non-ISR code. 
 *  @param item Reference to the item which is going to be put into the queue
 *  @return True if the item was successfully queued, false if not
 */

template <class data_type>
inline bool nRF24L01_queue<data_type>::ISR_put (const data_type& item)
{
	// This value is set to true if a context switch should occur due to this data
	signed portBASE_TYPE shouldSwitch = pdFALSE;

	bool return_value;						// Value returned from this method

	// Call the FreeRTOS function and save its return value
	return_value = (bool)(xQueueSendToBackFromISR (handle, &item, &shouldSwitch));

	// If the queue said that putting something into it has un-blocked a higher
	// priority task than the one currently running, ask for a context switch
	// BUG: There doesn't seem to be a taskYIELD_FROM_ISR() for the AVR port
// 	if (shouldSwitch)
// 	{
// 		taskYIELD_FROM_ISR ();
// 	}

	// Return the return value saved from the call to xQueueSendToBackFromISR()
	return (return_value);
}



//-------------------------------------------------------------------------------------
/** This method puts an item into the front of the queue from within an ISR.
 *  It must \b not be used within normal, non-ISR code. 
 *  @param item Reference to the item which is going to be (rudely) put into 
 *              the front of the queue
 *  @return True if the item was successfully queued, false if not
 */

template <class data_type>
bool nRF24L01_queue<data_type>::ISR_butt_in (const data_type& item)
{
	// This value is set to true if a context switch should occur due to this data
	signed portBASE_TYPE shouldSwitch = pdFALSE;

	bool return_value;						// Value returned from this method

	// Call the FreeRTOS function and save its return value
	return_value = (bool)(xQueueSendToFrontFromISR (handle, &item, &shouldSwitch));

	// If the queue said that putting something into it has un-blocked a higher
	// priority task than the one currently running, ask for a context switch
	// BUG: There doesn't seem to be a taskYIELD_FROM_ISR() for the AVR port
// 	if (shouldSwitch)
// 	{
// 		taskYIELD_FROM_ISR ();
// 	}

	// Return the return value saved from the call to xQueueSendToBackFromISR()
	return (return_value);
}


//-------------------------------------------------------------------------------------
/** This method returns the item at the head of the queue. If there's nothing in the
 *  queue, this method waits (blocking) for a very long time or until something shows
 *  up. 
 *  @return The data retrieved from the queue
 */

template <class data_type>
inline data_type nRF24L01_queue<data_type>::get (void)
{
	data_type recv_item;						// Data item read from the queue

	// If xQueueReceive doesn't return pdTrue, nothing was found in the queue, so
	// we'll return the item as created by its default constructor
	xQueueReceive (handle, &recv_item, portMAX_DELAY);
	return (recv_item);
}


//-------------------------------------------------------------------------------------
/** This method gets the item at the head of the queue, using a pointer to an object
 *  of the correct data type into which the data will be placed. This is a bit faster
 *  and uses a little less memory than calling the simpler \c get() method. If there's 
 *  nothing in the queue, this method waits (blocking) for a very long time or until 
 *  something shows up. This method must not be called from within an interrupt
 *  service routine. 
 *  @param item A reference to the data item into which received data will go
 */

template <class data_type>
inline void nRF24L01_queue<data_type>::get (data_type& item)
{
	xQueueReceive (handle, &item, portMAX_DELAY);
}


//-------------------------------------------------------------------------------------
/** This method returns the item at the head of the queue from within an interrupt
 *  service routine. This method must not be called from within normal non-ISR code. 
 *  @return The data retrieved from the queue
 */

template <class data_type>
inline data_type nRF24L01_queue<data_type>::ISR_get (void)
{
	data_type recv_item;						// Data item read from the queue
	portBASE_TYPE task_awakened;				// Checks if a task will wake up

	// If xQueueReceive doesn't return pdTrue, nothing was found in the queue, so
	// we'll return the item as created by its default constructor
	xQueueReceiveFromISR (handle, &recv_item, &task_awakened);
	return (recv_item);
}


//-------------------------------------------------------------------------------------
/** This method gets the item at the head of the queue, using a pointer to an object
 *  of the correct data type into which the data will be placed, from within an 
 *  interrupt service routine. This method must not be called from outside an interrupt
 *  service routine. 
 *  @param item A reference to the data item into which received data will go
 */

template <class data_type>
inline void nRF24L01_queue<data_type>::ISR_get (data_type& item)
{
	portBASE_TYPE task_awakened;				// Checks if a task will wake up

	// If xQueueReceive doesn't return pdTrue, nothing was found in the queue, and
	// nothing will actually be copied
	xQueueReceiveFromISR (handle, &item, &task_awakened);
}


//-------------------------------------------------------------------------------------
/** This method returns the item at the head of the queue without removing that item
 *  from the queue. If there's nothing in the queue, this method waits (blocking) for 
 *  a very long time or until something shows up. This method must \b not be called 
 *  from within an interrupt service routine. 
 *  @return The data retrieved from the queue; the data is set to 0 in the correct
 *          data type if we couldn't get any data from the queue and the attempt
 *          timed out
 */

template <class data_type>
inline data_type nRF24L01_queue<data_type>::get_a_copy (void)
{
	data_type recv_item;						// Data item read from the queue

	// If xQueueReceive doesn't return pdTrue, nothing was found in the queue, so
	// we'll return the item as created by its default constructor
	xQueuePeek (handle, &recv_item, portMAX_DELAY);
	return (recv_item);
}


//-------------------------------------------------------------------------------------
/** This method returns the item at the head of the queue without removing that item
 *  from the queue. This version puts the data into the item whose reference is given;
 *  receiving data this way is a little faster and uses a little less memory than
 *  returning the item as method \c data_type \c get_a_copy(void) does. If there's 
 *  nothing in the queue, this method waits (blocking) for a very long time or until 
 *  something shows up. This method must \b not be called from within an interrupt 
 *  service routine; FreeRTOS 7.1.X doesn't seem to have a function such as 
 *  \c xQueuePeekFromISR() which would allow copying items from a queue in an ISR. 
 *  @param item A reference to the data item into which received data will go
 */

template <class data_type>
inline void nRF24L01_queue<data_type>::get_a_copy (data_type& item)
{
	xQueuePeek (handle, &item, portMAX_DELAY);
}


#endif  // _FRT_QUEUE_H_

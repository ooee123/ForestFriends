//*************************************************************************************
/** \file frt_base_queue.h
 *    This file contains a base class for thread-safe RTOS queues. This design allows
 *    queues to be polymorphic: any descendent of this class uses the same mechanism 
 *    to transfer data between tasks, whether they're running in the computer or not.
 *    Descendents of this class include \c frt_queue, which is just a C++ wrapper for
 *    FreeRTOS queues, and other queues that use communication devices to allow
 *    computers to communicate data with one another. 
 *
 *  Revised:
 *    \li 10-21-2012 JRR Original file
 *    \li 01-30-2013 JRR Changed \c get(...) methods to use pointers, not references
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
#ifndef _FRT_BASE_QUEUE_H_
#define _FRT_BASE_QUEUE_H_

#include "FreeRTOS.h"                       // Main header for FreeRTOS
#include "queue.h"                          // Header for FreeRTOS queues


//-------------------------------------------------------------------------------------
/** \brief This is a base class for inter-task communication queues. 
 *  \details These queues transfer information from one task to another in the 
 *  FreeRTOS environment. The most commonly used descendent of this class is class 
 *  \c frt_queue, which is a wrapper on the FreeRTOS queue class. However, other 
 *  descendents use communications devices to transfer information between tasks. 
 */

template <class data_type> 
class frt_base_queue
{
	// These private functions can't be accessed from outside this class. The two
	// methods here can't be used to construct anything; this is intentional, as
	// it prevents an object of this class from being copied. One shouldn't copy a
	// task object; it's unnatural and can lead one to moral instability and ennui
	private:
		/** This copy constructor is poisoned by being declared private so that it 
		 *  can't be used. One should not copy a queue object. If one needs a second
		 *  object with the same parameters, one should construct another similarly.
		 *  @param that_clod A reference to a task which ought not be copied
		 */
		frt_base_queue (const frt_base_queue& that_clod);

		/** This assignment operator is poisoned by being declared private so that it
		 *  can't be used to copy the contents of one task into another. There is no
		 *  legitimate, morally acceptable reason to copy tasks that way. 
		 *  @param that_clod A reference to a task which ought not be copied
		 */
		frt_base_queue& operator= (const frt_base_queue& that_clod);

	// This protected data can only be accessed from this class or its descendents
	protected:
		/// This is a pointer to a serial device which can print debugging information.
		emstream* p_serial;

	// Public methods can be called from anywhere in the program where there is a 
	// pointer or reference to an object of this class
	public:
		// The constructor creates a queue base object. This constructor should never
		// be called from application code, only from descendent class constructors
		frt_base_queue (emstream* = NULL);

		/** This method puts an item of data into the back of the queue, which is the 
		 *  normal way to put something into a queue. If you want to be rude and put 
		 *  an item into the front of the queue so it will be retreived first, use 
		 *  butt_in() instead. This method must \b not be used within an ISR.
		 * 
		 *  This is a pure virtual method in this base class; every descendent class
		 *  must implement \c put() in a useful way. 
		 *  @param item Reference to the item which is going to be put into the queue
		 *  @return True if the item was successfully queued, false if not
		 */
		virtual bool put (const data_type& item) = 0;

		/** This method puts an item of data into the back of the queue from within
		 *  an interrupt service routine. It must \b not be used within non-ISR code. 
		 * 
		 *  This is a pure virtual method in this base class; every descendent class
		 *  must implement \c ISR_put() in a useful way, even if the descendent method 
		 *  just throws an error because things shouldn't be queued from within an ISR.
		 *  @param item Reference to the item which is going to be put into the queue
		 *  @return True if the item was successfully queued, false if not
		 */
		virtual bool ISR_put (const data_type& item) = 0;

		/** This method puts an item into the front of the queue so that it will be 
		 *  received first as long as nothing else is put in front of it. This is not 
		 *  the normal way to put things into a queue; using put() to put items into 
		 *  the back of the queue is. If you always use this method, you're really 
		 *  making a stack rather than a queue, you weirdo. Note that this method must
		 *  \b not be used within an interrupt service routine. 
		 * 
		 *  The \c butt_in() method for this base class doesn't do anything except
		 *  return \c false because it never sends anything anywhere. However, some 
		 *  descendent classes won't want to implement this method, so it is given an
		 *  implementation here. 
		 *  @param item Reference to the item which is going to be (rudely) put into 
		 *              the front of the queue
		 *  @return True if the item was successfully queued, false if not
		 */
		virtual bool butt_in (const data_type& item)
		{
			(void) item;                    // This just shuts up a compiler warning
			return (false);
		}

		/** This method puts an item into the front of the queue from within an ISR.
		 *  It must \b not be used within normal, non-ISR code. 
		 * 
		 *  The \c butt_in() method for this base class doesn't do anything except
		 *  return \c false because it never sends anything anywhere. However, some 
		 *  descendent classes won't want to implement this method, so it is given an
		 *  implementation here. 
		 *  @param item Reference to the item which is going to be (rudely) put into 
		 *              the front of the queue
		 *  @return True if the item was successfully queued, false if not
		 */
		virtual bool ISR_butt_in (const data_type& item)
		{
			(void) item;                    // This just shuts up a compiler warning
			return (false);
		}

		/** This method gets an item from the queue. Once the item has been gotten,
		 *  it's no longer in the queue and can't be re-gotten, so it can be thought
		 *  of as having been for-gotten. This method must not be called from within
		 *  an interrupt service routine; we have \c ISR_get() for that.
		 * 
		 *  This is a pure virtual method in this base class; every descendent class 
		 *  must implement \c get() in a useful way. 
		 * 
		 *  If it turns out that the same code can be used in a descendent class to 
		 *  get data from a queue within an ISR and outside of ISR's, it's reasonable
		 *  to create an \c ISR_get() method, then write a method \c get() which just
		 *  calls the \c ISR_get() method. This avoids unnecessary code duplication.
		 *  In any case, it is important to ensure that both methods are thread safe
		 *  if there is any possibility of one interrupting another. 
		 */
		virtual data_type get (void) = 0;

		/** This method gets data from the queue and puts it into the object whose
		 *  address is given. This uses less memory than \c data_type \c get() for 
		 *  large objects and runs a little more quickly. 
		 * 
		 *  This is a pure virtual method in this base class; every descendent class 
		 *  must implement \c get() in a useful way. 
		 *  @param p_item A pointer to the item into which the received data goes
		 */
		virtual void get (data_type* p_item) = 0;

		/** This method gets an item from the queue. It must only be used within an 
		 *  interrupt service routine. 
		 * 
		 *  This is a pure virtual method in this base class; every descendent class 
		 *  must implement \c ISR_is_empty() in a useful way, even if the descendent 
		 *  method just throws an error because it shouldn't be used within an ISR.
		 */
		virtual data_type ISR_get (void) = 0;

		/** This method gets data from the queue and puts it into the object pointed 
		 *  to by the given pointer. This uses less memory than \c get() for large
		 *  objects and runs a little more quickly. This method must only be called
		 *  from within an interrupt service routine.
		 * 
		 *  This is a pure virtual method in this base class; every descendent class 
		 *  must implement \c ISR_is_empty() in a useful way, even if the descendent 
		 *  method just throws an error because it shouldn't be used within an ISR.
		 * @param p_item A reference to the data item into which received data will go
		 */
		virtual void ISR_get (data_type* p_item) = 0;

		/** This method gets an item from the queue \b without causing the item to be
		 *  removed from the queue. 
		 * 
		 *  This is a pure virtual method in this base class; every descendent class 
		 *  must implement \c look_at() in a useful way.
		 */
		virtual data_type get_a_copy (void) = 0;

		/** This method gets an item from the queue \b without causing the item to be
		 *  removed from the queue. It puts the data ino the object whose reference is
		 *  given; this is a little faster and uses a little less memory than returning
		 *  the object as \c data_type \c look_at(void) does. 
		 * 
		 *  This is a pure virtual method in this base class; every descendent class 
		 *  must implement \c look_at() in a useful way.
		 * @param item A reference to the data item into which received data will go
		 */
		virtual void get_a_copy (data_type& item) = 0;

		/** This method checks if the queue is empty. 
		 * 
		 *  This is a pure virtual method in this base class; every descendent class 
		 *  must implement \c is_empty() in a useful way.
		*  @return true if the queue is empty, false if there's something in it
		*/
		virtual bool is_empty (void) = 0;

		/** This method checks if the queue is empty from within an interrupt service
		 *  routine. It must not be used in normal non-ISR code. 
		 * 
		 *  This is a pure virtual method in this base class; every descendent class 
		 *  must implement \c ISR_is_empty() in a useful way, even if the descendent 
		 *  method just throws an error because it shouldn't be used within an ISR.
		*  @return true if the queue is empty, false if there's something in it
		*/
		virtual bool ISR_is_empty (void) = 0;

		/** This method allows one to check if the queue has any contents. It must
		 *  \b not be called from within an interrupt service routine. 
		 * 
		 *  This is a pure virtual method in this base class; every descendent class 
		 *  must implement \c has_items() in a useful way.
		 *  @return True if there's something in the queue, false if not
		 */
		virtual bool not_empty (void) = 0;

		/** This method allows one to check if the queue has any contents from within
		 *  an interrupt service routine. 
		 * 
		 *  This is a pure virtual method in this base class; every descendent class 
		 *  must implement \c ISR_has_items() in a useful way.
		 *  @return True if there's something in the queue, false if not
		 */
		virtual bool ISR_not_empty (void) = 0;

		/** This method returns the number of items waiting in the queue. It must 
		 *  \b not be called from within an interrupt service routine; the method
		 *  \c ISR_num_items_in() can be called from within an ISR. 
		 * 
		 *  This is a pure virtual method in this base class; every descendent class 
		 *  must implement \c num_items_in() in a useful way.
		 *  @return True if there's something in the queue, false if not
		 *  @return The number of items in the queue
		 */
		virtual size_t num_items_in (void) = 0;

		/** This method returns the number of items waiting in the queue; it must be
		 *  called only from within an interrupt service routine.
		 * 
		 *  This is a pure virtual method in this base class; every descendent class 
		 *  must implement \c ISR_num_items_in() in a useful way.
		 *  @return The number of items in the queue
		 */
		virtual size_t ISR_num_items_in (void) = 0;
}; // class frt_queue 


//-------------------------------------------------------------------------------------
/** This constructor creates the base queue. All it does it save a copy of a pointer 
 *  to the serial device which is to be used for printing debugging messages.  
 *  @param p_ser_dev Pointer to a serial device to be used for debugging printouts
 *                   (Default: NULL)
 */

template <class data_type>
frt_base_queue<data_type>::frt_base_queue (emstream* p_ser_dev)
{
	// Save the pointer to a serial devide so it may be used for debugging
	p_serial = p_ser_dev;
}


#endif  // _FRT_BASE_QUEUE_H_

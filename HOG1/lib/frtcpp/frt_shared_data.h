//*************************************************************************************
/** \file frt_shared_data.h
 *    This file contains a template class for data which is to be shared between
 *    classes. The data must be protected against damage due to context switches, so
 *    it can be protected by a mutex or by making transfers take place inside critical
 *    sections which are protected from being interrupted. 
 *
 *  Revised:
 *    \li 10-29-2012 JRR Original file
 *    \li 01-21-2013 JRR Removed semaphore version; inlined all the methods
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
#ifndef _FRT_SHARED_DATA_H_
#define _FRT_SHARED_DATA_H_

#include "FreeRTOS.h"                       // Main header for FreeRTOS


//-------------------------------------------------------------------------------------
/** \brief This class implements an item of data which can be shared between tasks
 *  without the synchronization or overhead associated with queues. 
 *  \details The data is protected by using critical code sections (see the FreeRTOS 
 *  documentation of \c portENTER_CRITICAL() ) so that tasks can't interrupt each 
 *  other when reading or writing the data is taking place. This prevents data 
 *  corruption due to thread switching. The C++ template mechanism is used to ensure 
 *  that only data of the correct type is put into or taken from a shared data item. 
 *  A \c shared_data<data_type> object keeps its own separate copy of the data. This 
 *  uses some memory, but it is necessary to reliably prevent data corruption; it 
 *  prevents possible side effects from causing the sender's copy of the data from 
 *  being inadvertently changed. 
 */

template <class data_type> class shared_data
{
	protected:
		data_type the_data;					///< Holds the data to be shared

	public:
		/** This default constructor for a shared data item doesn't do much
		 *  because there isn't any particular setup required. 
		 */
		shared_data<data_type> (void)
		{
		}

		//-----------------------------------------------------------------------------
		/** This method is used to write data into the shared data item. It's declared
		*  inline so that instead of a regular function call at the assembly language
		*  level, "my_item.put (x);" will result in the code within this function
		*  being inserted directly into the calling function. This is faster than 
		*  doing a regular function call with pushing the program counter on the
		*  stack, jumping, jumping back and popping the program counter, yawn, zzz...
		*  @param new_data The data which is to be written
		*/
		void put (data_type new_data)
		{
			portENTER_CRITICAL ();
			the_data = new_data;
			portEXIT_CRITICAL ();
		}

		//-----------------------------------------------------------------------------
		/** This method writes data from an ISR into the shared data item. It must only
		*  be called from within a hardware interrupt, not a normal task. This is 
		*  because critical section protection isn't used here. This is OK, assuming 
		*  that an interrupt can't be interrupted by another interrupt, which is the 
		*  default case on most small microcontrollers. 
		*  @param new_data The data which is to be written into the shared data item
		*/
		void ISR_put (data_type new_data)
		{
			the_data = new_data;
		}

		//-----------------------------------------------------------------------------
		/** This method is used to read data from the shared data item. 
		*  @return The current value of the shared data item
		*/
		data_type get (void)
		{
			// It's necessary to make an extra, temporary copy of the data so that the
			// temporary copy can be returned. We can't call return() from within the
			// critical section for reasons that are obvious if you think about it
			data_type temporary_copy;

			portENTER_CRITICAL ();
			temporary_copy = the_data;
			portEXIT_CRITICAL ();

			return (temporary_copy);
		}

		//-----------------------------------------------------------------------------
		/** This method is used to enable an ISR to read data from the shared data 
		*  item. It must only be called from within a hardware interrupt, not a normal
		*  task. This is because critical section protection isn't used here, which is
		*  OK, assuming that an interrupt can't be interrupted by another interrupt, 
		*  which is the case on most small microcontrollers. 
		*  @return The current value of the shared data item
		*/
		data_type ISR_get (void)
		{
			return (the_data);
		}

		/** This is a slightly faster version of the \c get() method which saves
		 *  the data which has just been gotten into a variable to which the given
		 *  pointer points. This method must \b not be used within an ISR. 
		 *  @param p_item A pointer to the variable where what we got is put
		 */
		void get (data_type* p_item)
		{
			portENTER_CRITICAL ();
			*p_item = the_data;
			portEXIT_CRITICAL ();
		}

		/** This is a slightly faster version of the \c ISR_get() method which 
		 *  saves the retreived data into the variable to which the given pointer
		 *  is pointing. This method must \b only be used within an ISR. 
		 *  @param p_item A pointer to the variable where what we got is put
		 */
		void ISR_get (data_type* p_item)
		{
			// Since this is within an ISR, it shouldn't be interrupted, so just copy
			// the data from the protected space into the receiving variable
			*p_item = the_data;
		}

}; // class shared_data<data_type>

#endif  // _FRT_SHARED_DATA_H_


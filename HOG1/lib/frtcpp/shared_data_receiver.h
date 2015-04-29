//*************************************************************************************
/** \file shared_data_receiver.h
 *    This file contains a template class for data which is to be shared between
 *    computers. This is a test version; it doesn't use a radio or other communication
 *    medium. Instead, data is just converted from its base form into a test stream,
 *    "sent" as characters, and reconstructed to try to make a copy of the original
 *    data. This test version exists only to help develop useful classes that tramsmit
 *    and receive data through various communications channels. 
 *
 *  Revised:
 *    \li 10-29-2012 JRR Original file, \c frt_shared_data.h
 *    \li 12-15-2012 JRR Made into test version of sender and receiver tasks
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
#ifndef _SHARED_DATA_RECEIVER_H_
#define _SHARED_DATA_RECEIVER_H_

#include "FreeRTOS.h"                       // Main header for FreeRTOS
#include "base_data_receiver.h"             // The header for this class's parent class


//-------------------------------------------------------------------------------------
/** \brief This class is only a test, used to develop a convenient way to share data
 *  between computers. 
 *  \details In this class, the data is changed to a stream of printable characters. 
 *  The characters are then sent to a receiver which reconstructs the original data
 *  in a copy of the original object. The copy is double-buffered so that any task can 
 *  read the latest value of the data which has been received. Reading of the data is 
 *  protected by using critical code sections (see the FreeRTOS documentation of 
 *  \c portENTER_CRITICAL() ). The C++ template mechanism is used to ensure that only 
 *  data of the correct type is put into or taken from a shared data item. A 
 *  \c shared_data<data_type> object keeps its own separate copy of the data. This uses 
 *  some memory, but it is necessary to reliably prevent data corruption; it prevents 
 *  possible side effects from causing the sender's copy of the data from being 
 *  inadvertently changed. 
 */

template <class data_type> 
class shared_data_receiver : public base_data_receiver
{
	protected:
		/** This object holds the data as it is being reconstructed from a stream of 
		 *  characters. When filled, it will be copied to an extra buffer which can
		 *  be read at any old time. 
		 */
		data_type* p_received_data;

		/** This object holds a copy of the last good data which has been received. It
		 *  can be read out by a receiving program at one's leisure. 
		 */
		data_type* p_readable_data;

	public:
		// This constructor sets up the receiver with its serial device
		shared_data_receiver (uint8_t, emstream*, emstream* = NULL);

		// This method is used to read data from the shared data item
		data_type get (void);

		// This method is used to read data from within an ISR only
		data_type ISR_get (void);

		// This method transfers data from the receiving buffer to the readable buffer
		void transfer (void);
};


//-------------------------------------------------------------------------------------
/** This constructor creates a shared data receiver. It calls its parent's constructor,
 *  which attaches this object to a serial device descended from class \c emstream and 
 *  uses the serial device's hex receiver to get data from the communication channel 
 *  and store it in the appropriate place.
 *  @param code The data type and ID code; this is a unique identifier for each item
 *              of data being shared between two computers
 *  @param p_data_str Pointer to the stream through which hex data is sent
 *  @param p_dbg_str A pointer to a serial device which is used for debugging printouts
 */

template <class data_type>
shared_data_receiver<data_type>::shared_data_receiver (uint8_t code, 
													  emstream* p_data_str, 
													  emstream* p_dbg_str)
	: base_data_receiver (p_dbg_str)
{
	// Create the buffer in which received data will be assembled
	p_received_data = new data_type;

	// Create a place to store the readable copy of the data
	p_readable_data = new data_type;

	// If something went awry, complain
	#ifdef SERIAL_DEBUG
		if ((p_received_data == NULL) || (p_readable_data == NULL))
		{
			DBG (p_dbg_str, "new() failed in shared_data_receiver ctr" << endl);
		}
	#endif

	// Put this object into the list of objects kept by the serial input device
	p_data_str->hex_receiver_setup (code, (void*)p_received_data, this);
}


//-------------------------------------------------------------------------------------
/** This method is used to read data from the shared data item. It reads from the
 *  readable copy of the data (the one which has been double buffered, so that its
 *  data can be read while another copy is being received). 
 *  @return The current value of the shared data item
 */

template <class data_type>
inline data_type shared_data_receiver<data_type>::get (void)
{
	return (*p_readable_data);
}


//-------------------------------------------------------------------------------------
/** This method is used to enable an ISR to read data from the shared data 
 *  item. It must only be called from within a hardware interrupt, not a normal
 *  task. This is because critical section protection isn't used here, which is
 *  OK, assuming that an interrupt can't be interrupted by another interrupt, 
 *  which is the case on most small microcontrollers. 
 *  @return The current value of the shared data item
 */

template <class data_type>
inline data_type shared_data_receiver<data_type>::ISR_get (void)
{
	return (*p_readable_data);
}


//-------------------------------------------------------------------------------------
/** This method transfers data from the buffer in which it was reconstructed to an
 *  object which is readable on the receiving computer. This readable object is made
 *  type-safe to read by doing the transfer within a critical section of code. This
 *  method is to be called only by the method \c emstream::hex_receiver_loop() which 
 *  belongs to the serial device which received the characters. 
 */

template <class data_type>
void shared_data_receiver<data_type>::transfer (void)
{
	// Prevent this transfer from being interrupted by pretty much anything
	portENTER_CRITICAL ();

	// Copy the data
	*p_readable_data = *p_received_data;

	// We're done copying; re-enable interrupts and context switches
	portEXIT_CRITICAL ();

	// We've copied some good data, so set the good data variable
	good_data = true;
}

#endif  // _SHARED_DATA_RECEIVER_H_


//*************************************************************************************
/** \file shared_data_sender.h
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
#ifndef _SHARED_DATA_SENDER_H_
#define _SHARED_DATA_SENDER_H_

#include "FreeRTOS.h"                       // Main header for FreeRTOS
#include "emstream.h"                       // Base class for serial devices


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
class shared_data_sender
{
	protected:
		/// This pointer points to the serial device on which the data will be sent.
		emstream* p_data_dev;

		/** This is a unique type identifier which will be sent along with the data.
		 *  It will be used by the receiver to tell what kind of data is being sent
		 *  and where it should go. 
		 */
		uint8_t data_ID;

		/// This pointer points to a serial device used only for debugging printouts.
		emstream* p_serial;

	public:
		// The constructor sets up the data sender
		shared_data_sender<data_type> (emstream*, uint8_t, emstream* = NULL);

		// This method is used to write data into the shared data item
		void put (data_type);

		// This method is used to write data from within an ISR only
		void ISR_put (data_type);

}; // class shared_data<data_type>


//-------------------------------------------------------------------------------------
/** This constructor creates a new shared data test sender which just puts characters
 *  of a hex format packet into a buffer. 
 *  @param p_data_str Pointer to the serial port object to which data will be sent
 *  @param type_ID A user-specified ID number for this type and item of data
 *  @param p_ser_dev Pointer to a serial device to be used for debugging printouts
 *                   (Default: NULL, so no debugging information will be printed)
 */

template <class data_type> 
shared_data_sender<data_type>::shared_data_sender (emstream* p_data_str, 
												uint8_t type_ID, emstream* p_ser_dev)
{
	p_data_dev = p_data_str;                // Save pointers to needed devices
	data_ID = type_ID;
	p_serial = p_ser_dev;
}


//-------------------------------------------------------------------------------------
/** This method is used to write data into the shared data item. It's declared inline 
 *  so that instead of a regular function call at the assembly language level, 
 *  \c an_object.put \c (x); will result in the code within this function being 
 *  inserted directly into the calling function. This is faster than doing a regular 
 *  function call with pushing the program counter on the stack, jumping, jumping back 
 *  and popping the program counter, yawn, zzz...
 *  @param new_data The data which is to be written
 */

template <class data_type>
inline void shared_data_sender<data_type>::put (data_type new_data)
{
	p_data_dev->hex_send ((void*)(&new_data), sizeof (data_type), data_ID);
}


//-------------------------------------------------------------------------------------
/** This method doesn't write data from an ISR into the shared data item. It should 
 *  not be called because it doesn't do anything except print an error message. 
 *  @param new_data The data which is to be written into the shared data item
 */

template <class data_type>
void shared_data_sender<data_type>::ISR_put (data_type new_data)
{
	(void)new_data;
	*p_serial << "ERROR: Should not call shared_data<xxx>::ISR_put()" << endl;
}

#endif  // _SHARED_DATA_SENDER_H_


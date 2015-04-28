//*************************************************************************************
/** \file base_data_receiver.h
 *    This file contains a base class for classes which share data between computers. 
 *    Its most important feature is a virtual method called \c transfer(), which is a
 *    sort of callback function that is executed by an \c emstream descendent class 
 *    that implements a stream (serial device for characters). When the stream's
 *    hex receiver has received good data, \c transfer() is called to move that data
 *    from a buffer in which it was reconstituted into a readable output buffer. This
 *    double buffering ensures that there's always a good copy of the shared data to 
 *    be read on the receiving computer. 
 *
 *  Revised:
 *    \li 10-29-2012 JRR Original file, \c frt_shared_data.h
 *    \li 12-15-2012 JRR Made into test version of sender and receiver tasks
 *    \li 12-16-2012 JRR Split off base receiver class
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
#ifndef _BASE_DATA_RECEIVER_H_
#define _BASE_DATA_RECEIVER_H_

#include "FreeRTOS.h"                       // Main header for FreeRTOS
#include "emstream.h"                       // Header for streams which receive data


//-------------------------------------------------------------------------------------
/** \brief This is a base class for classes which receive data as a stream of 
 *  printable hex characers through a serial device and reassemble the data into 
 *  objects. 
 *  \details Child classes include queues and shared data items that operate between
 *  computers. In another class, data is changed to a stream of printable characters. 
 *  The characters are then sent to a receiver which reconstructs the original data
 *  in a copy of the original object. The copy is double-buffered so that any task can 
 *  read the latest value of the data which has been received. 
 */

class base_data_receiver
{
	protected:
		/// This pointer points to a serial device used only for debugging printouts.
		emstream* p_serial;

		/// This Boolean tells whether good data has been received.
		bool good_data;

	public:
		// This constructor sets up the receiver with its serial device
		base_data_receiver (emstream* = NULL);

		/** This method, when implemented in a descendent class, transfers received
		 *  data from the buffer in which received bytes were assembled to the readable
		 *  output buffer. 
		 */
		virtual void transfer (void) = 0;

		/** This Boolean operator causes the shared data item to become true if good
		 *  data has been received.
		 */
		operator bool () const
		{
			return (good_data);
		}

};

#endif  // _BASE_DATA_RECEIVER_H_


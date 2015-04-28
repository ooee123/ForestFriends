//*************************************************************************************
/** \file base_data_receiver.cpp
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

#include "base_data_receiver.h"             // The header for this class's parent class


//-------------------------------------------------------------------------------------
/** This constructor creates a shared data receiver. It attaches itself to a serial
 *  device descended from class \c emstream and uses the serial device's hex receiver
 *  to get data from the communication channel and store it in the appropriate place.
 *  @param p_dbg_str A pointer to a serial device which is used for debugging printouts
 */

// template <class data_type>
base_data_receiver::base_data_receiver (emstream* p_dbg_str)
{
	// Set the debugging serial port
	p_serial = p_dbg_str;

	// We haven't received good data yet
	good_data = false;
}


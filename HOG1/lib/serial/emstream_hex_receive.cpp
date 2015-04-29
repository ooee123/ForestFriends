//*************************************************************************************
/** \file emstream_hex_receive.cpp
 *    This file contains a method which receives arbitrary data items which have been
 *    converted and sent over a serial interface by \c emstream_hex_send(). 
 *
 *  Revised:
 *    \li 12-11-2012 JRR Split this file off from the main \c emstream.cpp to allow
 *                       smaller machine code if stuff in this file isn't used
 *
 *  License:
 *    This file released under the Lesser GNU Public License, version 2. This program
 *    is intended for educational use only, but it is not limited thereto. This code
 *    incorporates elements from Xmelkov's ftoa_engine.h, part of the avr-libc source,
 *    and users must accept and comply with the license of ftoa_engine.h as well. See
 *    emstream.h for a copy of the relevant license terms. */
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

#include <stdlib.h>                         // Standard C/C++ library for AVR's
#include <util/crc16.h>                     // Header for cyclic redundancy checks

#include "FreeRTOS.h"                       // Header for FreeRTOS in general
#include "task.h"                           // Header for FreeRTOS tasks in particular

#include "emstream.h"                       // Header for the emstream class
#include "base_data_receiver.h"             // Header for receivers of hex data


//-------------------------------------------------------------------------------------
/** This constructor sets up a new \c hex_receiver object. It just initializes things.
 *  If the macro \c HXR_DBG_PTR is set, a serial port pointer is given to this class
 *  so that diagnostic printouts can be produced. Usually \c HXR_DBG_PTR is set to
 *  \c void and no parameter is given to this constructor. 
 */

hex_receiver::hex_receiver (HXR_DBG_PARAM)
{
	p_first_map = NULL;
	HXR_DBG_SETPTR ();
}


//-------------------------------------------------------------------------------------
/** This method sets up a receiver which will be able to receive characters sent by
 *  some other device using \c emstream::hex_send(). If only one type of data is sent
 *  through this serial device, this method may be called just once; if more than one
 *  type of data is to be sent, this method may be called many times, once for each
 *  type of data. 
 * 
 *  An example of the use of the hex receiver is as follows assuming that \c p_serial
 *  is a pointer to an object of any descendent class of class \c emstream:
 *  \code
 *  	p_serial->hex_receiver_setup (0x01, &object_1);
 *  p_serial->hex_receiver_setup (0x20, &object_2);
 *  ...
 *  while (p_serial->hex_receiver_decode () == false) { }
 *  ...
 *  \endcode
 *  In this example, we call \c hex_receiver_setup() twice, setting up the receiver to
 *  get data for two objects. The sender can send data using either code, \c 0x01 or
 *  \c 0x20; the code determines which object will receive the data. The method
 *  \c hex_receiver_decode() is called by \c emstream::hex_receiver_loop(), documented
 *  elsewhere. 
 *  @param code The code which identifies data going to a particular object
 *  @param p_object A pointer to the object which will hold the received data
 *  @param p_receiver Pointer to the object that receives data and puts it in a buffer
 */

void emstream::hex_receiver_setup (uint8_t code, void* p_object, 
								  base_data_receiver* p_receiver)
{
	// If the hex receiver object has not been created yet, create it
	if (p_hex_receiver == NULL)
	{
		// When we call the constructor, HXR_DBG_CTRP is a pointer to this serial de-
		// vice (to be used for debugging) if debugging is enabled, or nothing if not
		p_hex_receiver = new hex_receiver HXR_DBG_CTRP;
	}

	// Create a new mapping from data type code to pointer to the object that gets data
	p_hex_receiver->map (code, p_object, p_receiver);
}


//-------------------------------------------------------------------------------------
/** This method creates a new mapping between a user-specified data type and an object
 *  in which data received by a hex receiver will be stored. The mapping is added to
 *  a linked list of mappings so that an arbitrary number of different variables can 
 *  be transmitted through the same serial device. 
 *  @param a_code A code for the type of data being received
 *  @param p_object A pointer to the object in which received data will be put
 *  @param p_receiver A pointer to the object which controls the receiving process
 */

void hex_receiver::map (uint8_t a_code, void* p_object, base_data_receiver* p_receiver)
{
	// Create a new mapping object
	hex_data_type_to_pointer* p_new_map = new hex_data_type_to_pointer;
	p_new_map->code = a_code;
	p_new_map->p_obj = p_object;
	p_new_map->p_rcvr = p_receiver;

	// If the map was successfully created, insert the mapping object into the linked 
	// list of such objects. It seems easiest to put it in as the first item to be 
	// found in the list
	if (p_new_map != NULL)
	{
		if (p_first_map == NULL)
		{
			p_new_map->p_next = NULL;
		}
		else
		{
			p_new_map->p_next = p_first_map;
		}
		p_first_map = p_new_map;

		HXR_DBG (p_serial, PMS ("HXR map: ") << p_first_map->p_obj 
				 << hex << p_first_map->code << endl);
	}
	else
	{
		// If the map wasn't successfully created, complain if debugging
		HXR_DBG (p_serial, PMS ("hex_receiver::map() alloc error ") << endl);
	}
}


//-------------------------------------------------------------------------------------
/** This method should be run periodically in a loop to receive and process characters
 *  from the stream which holds hex encoded data. It calls \c hex_receiver::decode() 
 *  as long as it finds characters that have come in through the serial port. 
 *  If the hex receiver hasn't been set up or there's no character in the receiver
 *  buffer, this method does nothing. For usage instructions, see the documentation
 *  for the method \c hex_receiver_setup(). 
 *  @return The result of calling \c hex_receiver::get(), which is true only if a
 *          character was received and that character finished a packet of good data
 */

bool emstream::hex_receiver_loop (void)
{
	// If the receiver has been set up and there are characters ready to read...
	if (p_hex_receiver != NULL)
	{
		// ...get each character and have the hex receiver decode it
		while (check_for_char ())
		{
			uint16_t char_in = getchar ();

			if (p_hex_receiver->decode (char_in))
			{
				// This call tells the receiver object for one specific data item that
				// good data has arrived and should be transferred from the receiving
				// buffer into the output buffer
				p_hex_receiver->p_map->p_rcvr->transfer ();

				return (true);
			}
		}
	}
	return (false);
}


//-------------------------------------------------------------------------------------
/** This method receives a byte from the serial interface and decodes it as a hex 
 *  digit from a package of data. Received data is placed as binary bytes into the
 *  object to which the hex receiver's data pointer points. The byte may be from a 
 *  header, from data, or from a checksum; therefore, a state machine is used to keep 
 *  track of which part of the data is being read at any time. When good data has been 
 *  received, a call to this method will return true; when only partial data has been 
 *  received or bad data has been received, this method will return false. 
 *  
 *  The receiver expects to see a data type code, followed by a size, followed by the
 *  number of data bytes specified in the size, followed by a checksum. Packets should
 *  be followed by carriage return and/or linefeed characters. A state/transition 
 *  diagram of the state machine used to decode the packets is shown below:
 *  \dot
 *  digraph states
 *  {
 *    node [ shape = ellipse, fontname = Sans, fontsize = 8 ];
 *      s0  [ label = "Code first nibble " ];
 *      s1  [ label = "Code second nibble " ];
 *      s2  [ label = "Size first nibble " ];
 *      s3  [ label = "Size second nibble " ];
 *      s4  [ label = "Data first nibble " ];
 *      s5  [ label = "Data second nibble "  ];
 *      s6  [ label = "Checksum first nibble "];
 *      s7  [ label = "Checksum second nibble "  ];
 *      s8  [ label = "Checksum third nibble "  ];
 *      s9  [ label = "Checksum fourth nibble " ];
 *    edge [ arrowhead = "normal", style = "solid", fontname = Sans, fontsize = 8 ];
 *      s0->s1  [ label = " got a char " ];
 *      s1->s2  [ label = " got a char " ];
 *      s2->s3  [ label = " got a char " ];
 *      s3->s4  [ label = " got a char " ];
 *      s4->s5  [ label = " got a char " ];
 *      s5->s4  [ label = " bytes received < size " ];
 *      s5->s6  [ label = " bytes received >= size " ];
 *      s6->s7  [ label = " got a char " ];
 *      s7->s8  [ label = " got a char " ];
 *      s8->s9  [ label = " got a char " ];
 *      s9->s0  [ label = " got last char " ];
 *      s1->s0  [ label = " got '\\r' or '\\n' " ];
 *      s2->s0  [ label = " got '\\r' or '\\n' " ];
 *      s3->s0  [ label = " got '\\r' or '\\n' " ];
 *      s4->s0  [ label = " got '\\r' or '\\n' " ];
 *      s5->s0  [ label = " got '\\r' or '\\n' " ];
 *      s6->s0  [ label = " got '\\r' or '\\n' " ];
 *      s7->s0  [ label = " got '\\r' or '\\n' " ];
 *      s8->s0  [ label = " got '\\r' or '\\n' " ];
 *  }
 *  \enddot
 *  @param char_in A character that was received by the serial interface and passed to
 *                 this method for decoding
 *  @return True if the result of this character is that the object contains good data
 *          (only when all the data for one transfer has been received and checked) 
 *          and false if the data isn't good yet or if bad data was received
 */

bool hex_receiver::decode (uint16_t char_in)
{
	// If we get a return character, expect that it will be followed by the 
	// beginning of some useful data and reset the decoder
	if ((char_in == '\n') || (char_in == '\r'))
	{
		decoder_state = 0;
	}
	else
	{
		switch (decoder_state)
		{
			// State 0: We haven't received any characters yet; the first one is 
			//          the first character of the type code. Reset everything to
			//          condition at start of reading process
			case (0):
				good_data = false;
				computed_crc = 0xFFFF;
				type_code = ascii_to_nibble (char_in) << 4;
				p_data = NULL;
				decoder_state = 1;
				break;

			// State 1: Get the second nibble of the command code
			case (1):
				type_code |= ascii_to_nibble (char_in);
				decoder_state = 2;
				HXR_DBG (p_serial, hex << PMS ("Code: ") << type_code << endl);

				// OK, we have the code; use it to get a pointer to the object into
				// which we'll be writing the data
				p_map = p_first_map;
				while (p_map != NULL)
				{
					// If debugging, show what we're looking at
					HXR_DBG (p_serial, PMS ("HXR ptr: ") << p_map << endl);

					// If we have a matching code, get the corresponding data pointer
					if (p_map->code == type_code)
					{
						p_data = (uint8_t*)(p_map->p_obj);
						HXR_DBG (p_serial, PMS (" dptr: ") << p_data << endl);
						break;
					}
					else
					{
						HXR_DBG (p_serial, PMS ("HXR ") << hex << p_map->code 
								 << PMS (" != ") << type_code << endl);
					}

					// If no match found, go to the next item in the list
					p_map = p_map->p_next;
				}
				break;

			// State 2: Get the first nibble of the package size
			case (2):
				size = ascii_to_nibble (char_in) << 4;
				decoder_state = 3;
				break;

			// State 3: Get the second nibble of the package size
			case (3):
				size |= ascii_to_nibble (char_in);
				HXR_DBG (p_serial, PMS (" size: ") << size << hex << endl);
				decoder_state = 4;
				break;

			// State 4: Get the first nibble of whatever is next
			case (4):
				num_in = ascii_to_nibble (char_in) << 4;
				decoder_state = 5;
				break;

			// State 5: Get the second nibble of whatever is next. If this isn't 
			//          the last item of data, go back to state 4 for another byte
			case (5):
				num_in |= ascii_to_nibble (char_in);
				if (p_data != NULL)
				{
					HXR_DBG (p_serial, hex << p_data << "<-" << num_in << endl);
					*p_data++ = num_in;
				}

				// Update a cyclic reduncancy check (fancy checksum) for the data
				computed_crc = _crc_ccitt_update (computed_crc, num_in);

				if (--size > 0)
				{
					decoder_state = 4;
				}
				else
				{
					decoder_state = 6;
				}
				break;

			// State 6: Get the first nibble of the checksum
			case (6):
				received_crc = (uint16_t)(ascii_to_nibble (char_in) << 4);
				decoder_state = 7;
				break;

			// State 7: Get the second nibble of the checksum
			case (7):
				received_crc |= ascii_to_nibble (char_in);
				received_crc <<= 4;
				decoder_state = 8;
				break;

			// State 8: Get the third nibble of the checksum
			case (8):
				received_crc |= ascii_to_nibble (char_in);
				received_crc <<= 4;
				decoder_state = 9;
				break;

			// State 9: Get the last nibble of the checksum and check the CRC
			case (9):
				received_crc |= ascii_to_nibble (char_in);
				decoder_state = 0;
				HXR_DBG (p_serial, PMS (":") << received_crc << endl);
				if (received_crc == computed_crc)
				{
					good_data = true;
				}
				else
				{
					HXR_DBG (p_serial, endl << PMS ("Bad CRC:") << hex << received_crc
							 << PMS (",") << computed_crc << dec << endl);
				}
				break;

			// We should never get here. Wait for a return, then reset
			default:
				// Code just before the state machine detects reset characters
				break;
		};
	}

	return (good_data);
}


//-------------------------------------------------------------------------------------
/** This method converts a single ASCII digit, which must be 0-9 or A-F, into a four 
 *  bit number which comprises part of a byte stream holding some binary data. 
 *  @param in_char A single digit which has been sent to us
 *  @return The binary number which the single digit signifies
 */ 

uint8_t hex_receiver::ascii_to_nibble (char in_char)
{
	if ((in_char <= '9') && (in_char >= '0'))
	{
		return (in_char - '0');
	}
	if ((in_char <= 'F') && (in_char >= 'A'))
	{
		return (in_char - 'A' + 10);
	}
	if ((in_char <= 'f') && (in_char >= 'a'))
	{
		return (in_char - 'a' + 10);
	}
	return (0);
}


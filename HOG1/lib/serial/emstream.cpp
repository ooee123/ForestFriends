//*************************************************************************************
/** \file emstream.cpp
 *    This file contains a base class for devices which send information in text form
 *    over serial devices in an AVR microcontroller. Example devices are serial ports
 *    (both traditional RS-232 ports and USB-serial adapters) and wireless modems. 
 *
 *  Revised:
 *    \li 04-03-2006 JRR For updated version of compiler
 *    \li 06-10-2006 JRR Ported from C++ to C for use with some C-only projects
 *    \li 08-11-2006 JRR Some bug fixes
 *    \li 03-02-2007 JRR Ported back to C++. I've had it with the limitations of C.
 *    \li 04-16-2007 JO  Added write (unsigned long)
 *    \li 07-19-2007 JRR Changed some character return values to bool, added m324p
 *    \li 01-12-2008 JRR Added code for the ATmega128 using USART number 1 only
 *    \li 02-13-2008 JRR Split into base class and device specific classes; changed
 *                       from write() to overloaded << operator in the "cout" style
 *    \li 05-12-2008 ALS Fixed bug in which signed numbers came out unsigned
 *    \li 07-05-2008 JRR Added configuration macro by which to change what "endl" is
 *    \li 07-05-2008 JRR Added 'ascii' and 'numeric' format codes
 *    \li 11-24-2009 JRR Changed operation of 'clrscr' to a function to work with LCD
 *    \li 11-26-2009 JRR Integrated floating point support into this file
 *    \li 12-16-2009 JRR Improved support for constant strings in program memory
 *    \li 10-22-2012 JRR Fixed (OK, hacked around) bug which caused spurious warning 
 *                       for all Program Memory Strings
 *    \li 11-12-2012 JRR Made puts() non-virtual; made ENDL_STYLE() a function macro
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

#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include "emstream.h"


//-------------------------------------------------------------------------------------
/** This variable, accessible to any function that includes \c emstream.h, 
 *  allows the nonmember function \c setprecision() to communicate the number of 
 *  digits after the decimal point to a specific text serial port object. The 
 *  \c setprecision() function will return a manipulator that causes a specific serial 
 *  object (the one in whose \c cout style output line it is called) to read the value 
 *  of this variable. 
 */
uint8_t bts_glob_prec = 3;


//-------------------------------------------------------------------------------------
/** This constructor sets up the base serial port object. It sets the default base for
 *  the conversion of numbers to text and the default format for converting chars. 
 */

emstream::emstream (void)
{
	base = 10;                              // Numbers are shown as decimal by default
	print_ascii = false;                    // Print 8-bit chars as numbers by default
	precision = 3;                          // Print 3 digits after a decimal point
	pgm_string = false;                     // Print strings from SRAM by default
	p_hex_receiver = NULL;                  // No hex packet receiver has been set up
}


//-------------------------------------------------------------------------------------
/** This function checks if the serial port transmitter is ready to send data.  It's 
 *  an empty base method, overridden by most serial devices. Some serial devices might
 *  always be ready to send data; those can not bother to override this method. 
 *  @return True if the serial port is ready to send, and false if not
 */

bool emstream::ready_to_send (void)
{
	return (true);                          // By default the port's always ready
}


//-------------------------------------------------------------------------------------
/** This base method just returns zero, because it shouldn't be called. There might be
 *  classes which only send characters and don't ever receive them, and this method
 *  could be left as a placeholder for those classes. 
 *  @return A zero (null character) which means nothing
 */

int16_t emstream::getchar (void)
{
	return ('\0');                          // Nothing to return, really
}


//-------------------------------------------------------------------------------------
/** This method checks if there is a character in the serial port's receiver buffer.
 *  There isn't, as this base class isn't connected to a buffer; descendent classes
 *  will override this method and check a real buffer for real characters. 
 *  @return False, because no character will ever be available
 */

bool emstream::check_for_char (void)
{
	return (false);
}


//-------------------------------------------------------------------------------------
/** This is a base method for causing immediate transmission of a buffer full of data.
 *  The base method doesn't do anything, because it will be implemented in descendent
 *  classes which have no buffers, send everything immediately by default, and don't 
 *  need to respond to calls for immediate transmission. 
 */

void emstream::transmit_now (void)
{
}


//-------------------------------------------------------------------------------------
/** This is a base method to clear a display screen, if there is one. It is called 
 *  when the format modifier 'clrscr' is inserted in an output line. Descendant
 *  classes which send things to displays should respond by clearing themselves.
 */

void emstream::clear_screen (void)
{
}


//-------------------------------------------------------------------------------------
/** This method writes a string to the serial device. It just works through the 
 *  character buffer to which its parameter points, calling \c putchar() to print each
 *  character until and end of string character (the null character, \c '\0') is 
 *  reached.
 *  @param p_string A pointer to the string which is to be printed
 */

void emstream::puts (const char* p_string)
{
	char ch;                                // Temporary storage for a character

	// If the program-string variable is set, this string is to be found in program
	// memory rather than data memory
	if (pgm_string)
	{
		pgm_string = false;
		while ((ch = pgm_read_byte_near (p_string++)))
		{
			putchar (ch);
		}
	}
	// If the program-string variable is not set, the string is in RAM and printed
	// in the normal way
	else
	{
		while (*p_string)
		{
			putchar (*p_string++);
		}
	}
}


//-------------------------------------------------------------------------------------
/** This function sets the global precision value, then returns a manipulator which 
 *  causes a serial object to change its floating point precision (the number of digits
 *  after the decimal point which will be printed). The new precision is "sticky" in
 *  that its value persists until this function is called again to change it again. 
 *  @param digits A new value for the number of digits after the decimal point;
 *		a maximum of 7 digits are allowed
 *  @return The serial manipulator called "manip_set_precision"
 */

ser_manipulator setprecision (uint8_t digits)
{
	if (digits > 7)
	{
		digits = 7;
	}
	bts_glob_prec = digits;

	return (manip_set_precision);
}


//-------------------------------------------------------------------------------------
/** This overload allows manipulators to be used to change the base of displayed 
 *  numbers to binary, octal, decimal, or hexadecimal. Also, and endline is provided
 *  with the name "endl" and the code "send_now" causes immediate transmission by
 *  devices which save stuff to be transmitted in buffers. 
 *  @param new_manip The serial manipulator which was given
 *  @return A reference to the serial device to which the data was printed. This
 *          reference is used to string printable items together with "<<" operators
 */

emstream& emstream::operator<< (ser_manipulator new_manip)
{
	switch (new_manip)
	{
		case (bin):                         // Print integers in binary
			base = 2;
			break;
		case (oct):                         // Print integers in octal
			base = 8;
			break;
		case (dec):                         // Print integers in decimal
			base = 10;
			break;
		case (hex):                         // Print integers in hexadecimal
			base = 16;
			break;
		case (ascii):                       // Print chars as ASCII letters, etc.
			print_ascii = true;
			break;
		case (numeric):                     // Print chars as numbers
			print_ascii = false;
			break;
		case (endl):                        // Send an end-of-line
			ENDL_STYLE ();
			break;
		case (clrscr):                      // Send a clear-screen code
			clear_screen ();
			break;
		case (send_now):                    // Send whatever's in the send buffer
			transmit_now ();
			break;
		case (_p_str):                      // The next string is in program memory
			pgm_string = true;
			break;
		case (manip_set_precision):         // Set precision for printing floats
			break;
	};

	return (*this);
}

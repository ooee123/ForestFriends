//*************************************************************************************
/** \file ansi_terminal.h
 *    This file contains a set of preprocessor macros which produce escape character
 *    sequences for ASCII terminals for use in class \c emstream printing lines which 
 *    use the \c << operator. These sequences cause many terminal programs to do other
 *    things than print -- for example, move the cursor around, change text style and
 *    color, and clear part or all of the screen. 
 *
 *  Revisions
 *    \li 12-31-2012 JRR Original file
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
#ifndef _ANSI_TERMINAL_H_
#define _ANSI_TERMINAL_H_

#include "emstream.h"                       // Header for serial device base class


/// This escape sequence resets characters (that is, turns all fancy attributes off).
#define ATERM_NORMAL             PMS ("\x1b[0m")

/// This escape sequence changes the font to \b bold (on terminals, it gets brighter).
#define ATERM_BOLD               PMS ("\x1b[1m")

/// This escape sequence changes the font to normal (not bold) intensity.
#define ATERM_NORM_INT           PMS ("\x1b[22m")

/** This escape sequence sets the text to inverse video. If the normal setting is green
 *  on a black background, for example, the font becomes black on a green background.
 */
#define ATERM_INVERSE            PMS ("\x1b[7m")

/// This escape sequence sets the text to \b not be inverse video anymore. 
#define ATERM_NOT_INVERSE        PMS ("\x1b[27m")

/// This escape sequence sets the font color to the terminal default color. 
#define ATERM_DEF_TXT_CLR        PMS ("\x1b[39m")

/// This escape sequence sets the background color to the default background color. 
#define ATERM_DEF_BKG_CLR        PMS ("\x1b[49m")

/// This escape sequence sets the text color to be black.
#define ATERM_TXT_BLACK          PMS ("\x1b[30m")

/// This escape sequence sets the text color to be red. 
#define ATERM_TXT_RED            PMS ("\x1b[31m")

/// This escape sequence sets the text color to be green.
#define ATERM_TXT_GREEN          PMS ("\x1b[32m")

/// This escape sequence sets the text color to be yellow.
#define ATERM_TXT_YELLOW         PMS ("\x1b[33m")

/// This escape sequence sets the text color to be blue.
#define ATERM_TXT_BLUE           PMS ("\x1b[34m")

/// This escape sequence sets the text color to be magenta.
#define ATERM_TXT_MAGENTA        PMS ("\x1b[35m")

/// This escape sequence sets the text color to be cyan.
#define ATERM_TXT_CYAN           PMS ("\x1b[36m")

/// This escape sequence sets the text color to be white.
#define ATERM_TXT_WHITE          PMS ("\x1b[37m")

/// This escape sequence sets the background color to be black.
#define ATERM_BKG_BLACK          PMS ("\x1b[40m")

/// This escape sequence sets the background color to be red.
#define ATERM_BKG_RED            PMS ("\x1b[41m")

/// This escape sequence sets the background color to be green.
#define ATERM_BKG_GREEN          PMS ("\x1b[42m")

/// This escape sequence sets the background color to be yellow.
#define ATERM_BKG_YELLOW         PMS ("\x1b[43m")

/// This escape sequence sets the background color to be blue.
#define ATERM_BKG_BLUE           PMS ("\x1b[44m")

/// This escape sequence sets the background color to be magenta.
#define ATERM_BKG_MAGENTA        PMS ("\x1b[45m")

/// This escape sequence sets the background color to be cyan.
#define ATERM_BKG_CYAN           PMS ("\x1b[46m")

/// This escape sequence sets the background color to be white.
#define ATERM_BKG_WHITE          PMS ("\x1b[47m")

/// This escape sequence moves the cursor \c n cells upward. 
#define ATERM_CURSOR_UP(x)       PMS ("\x1b[") << (uint8_t)(x) << PMS ("A")

/// This escape sequence moves the cursor \c n cells downward. 
#define ATERM_CURSOR_DOWN(x)     PMS ("\x1b[") << (uint8_t)(x) << PMS ("B")

/// This escape sequence moves the cursor \c n cells forward. 
#define ATERM_CURSOR_FORWARD(x)  PMS ("\x1b[") << (uint8_t)(x) << PMS ("C")

/// This escape sequence moves the cursor \c n cells backward. 
#define ATERM_CURSOR_BACK(x)     PMS ("\x1b[") << (uint8_t)(x) << PMS ("D")

/// This escape sequence moves the cursor to the beginning of a line, \c x lines down.
#define ATERM_CURSOR_LINEDOWN(x) PMS ("\x1b[") << (uint8_t)(x) << PMS ("E")

/// This escape sequence moves the cursor to the beginning of a line, \c x lines up.
#define ATERM_CURSOR_LINEUP(x)   PMS ("\x1b[") << (uint8_t)(x) << PMS ("F")

/// This sequence moves the cursor to the given absolute \c x (column) coordinate.
#define ATERM_CURSOR_ABS_X(x)    PMS ("\x1b[") << (uint8_t)(x) << PMS ("G")

/** This escape sequence moves the cursor to the given absolute position. The position
 *  is given as (row, column) or (y, x) which is somewhat strange. The base position
 *  is (y,x) = (1,1) which is the top left corner.
 */
#define ATERM_CURSOR_TO_YX(y,x)  PMS ("\x1b[") << (uint8_t)(y) << PMS (";") \
                                               << (uint8_t)(x) << PMS ("H")

/** This escape sequence clears part or all of the line on which the cursor sits. Its
 *  parameter \c x controls what part of the line is erased:
 *  \li \c 0 - Clear from cursor to end of line
 *  \li \c 1 - Clear from cursor back to beginning of line
 *  \li \c 2 - Clear the entire line, regardless of the cursor position
 */
#define ATERM_ERASE_IN_LINE(x)   PMS ("\x1b[") << (uint8_t)(x) << PMS ("K")

/** This escape sequence clears the screen and leaves the cursor at its current 
 *  position. If doing a full screen clear, one should probably move the cursor
 *  to position (1,1) at upper left after this, using macro \c ATERM_CURSOR_TO_YX().
 */
#define ATERM_CLEAR_SCREEN       PMS ("\x1b[2J")

/** This escape sequence clears the screen, moves the cursor to upper left, sets the
 *  default color and non-fancy text. 
 */
#define ATERM_RESET_SCREEN       PMS ("\x1b[2J\x1b[1;1H\x1b[0m")


// These aren't being used...just an idea that probably won't work out...
// enum ansi_term_commands {ATERM_NORMAL,
// 						ATERM_BOLD,
// 						ATERM_NORM_INT,
// 						ATERM_INVERSE,
// 						ATERM_NOT_INVERSE,
// 						ATERM_DEF_TXT_CLR,
// 						ATERM_DEF_BKG_CLR,
// 						ATERM_TXT_BLACK,
// 						ATERM_TXT_RED,
// 						ATERM_TXT_GREEN,
// 						ATERM_TXT_YELLOW,
// 						ATERM_TXT_BLUE,
// 						ATERM_TXT_MAGENTA,
// 						ATERM_TXT_CYAN,
// 						ATERM_TXT_WHITE,
// 						ATERM_BKG_BLACK,
// 						ATERM_BKG_RED,
// 						ATERM_BKG_GREEN,
// 						ATERM_BKG_YELLOW,
// 						ATERM_BKG_BLUE,
// 						ATERM_BKG_MAGENTA,
// 						ATERM_BKG_CYAN,
// 						ATERM_BKG_WHITE,
// 						ATERM_CURSOR_UP(x),
// 						ATERM_CURSOR_DOWN(x),
// 						ATERM_CURSOR_FORWARD(x),
// 						ATERM_CURSOR_BACK(x),
// 						ATERM_CURSOR_LINEDOWN(x),
// 						ATERM_CURSOR_LINEUP(x),
// 						ATERM_CURSOR_ABS_X(x),
// 						ATERM_CURSOR_TO_YX(y,x),
// 						ATERM_ERASE_IN_LINE(x),
// 						ATERM_CLEAR_SCREEN,
// 						ATERM_RESET_SCREEN} blah;

#endif // _ANSI_TERMINAL_H_


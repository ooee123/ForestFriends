//*************************************************************************************
/** \file frt_task.h
 *    This file contains a wrapper class which makes the use of FreeRTOS tasks a bit
 *    easier. The class incorporates a bunch of enhancements which are specifically
 *    designed to aid in the teaching and learning of embedded software programming in
 *    an RTOS environment. These enhancements include convenient diagnostic printing
 *    and links to the \c emstream class hierarchy which implements output to serial
 *    devices using an overloaded \c << operator in a style similar to that of the C++
 *    \c iostream and related classes. 
 *
 *  Revised:
 *    \li 10-21-2012 JRR Original file
 *
 *  Credits:
 *    Much of this code uses techniques learned from Amigo software, which is 
 *    copyright 2012 by Digital Aggregates Corporation of Colorado, USA. The license 
 *    used by Digital Aggregates is a modified version of the GNU Public License. 
 *    See \c http://www.diag.com/navigation/downloads/Amigo.html.
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
#ifndef _FRT_TASK_H_
#define _FRT_TASK_H_

#include <avr/wdt.h>                         // Header for watchdog timer that reboots

#include "FreeRTOS.h"                        // Main header for FreeRTOS
#include "task.h"                            // Header for FreeRTOS tasks

#include "mechutil.h"                        // Utility functions for the ME405 code
#include "emstream.h"                        // Pull in the base class header file
#include "time_stamp.h"                      // Header for timekeeping class


/* The forward declaration is needed so we can make last_created_task_pointer usable
 * from anywhere in this file. It would be nicer if this variable could be a static
 * member of class frt_task, but the C++ standard won't allow it to be initialized if
 * it's a static member. For some reason, only integer types can be initialized at
 * startup if they're static class members.
 */
class frt_task;

// This is the pointer to the last created task, created in frt_task.cpp
extern frt_task* last_created_task_pointer;

/** This macro is used to set the priority of a task. In addition to being a little 
 *  more readable than the usual priority which is calculated from the idle task's
 *  priority, this macro checks to make sure that the asked for priority doesn't 
 *  exceed the maximum priority set in \c FreeRTOSConfig.h.
 */
#define task_priority(x) ((x) < (configMAX_PRIORITIES) ? \
		((tskIDLE_PRIORITY) + (x)) : (configMAX_PRIORITIES))


/// \cond NO_DOXY 
//-------------------------------------------------------------------------------------
// If the AVR has 256KB flash, it uses a 3-byte program counter and the task function
// needs some special handling to work correctly

#if defined(__AVR_3_BYTE_PC__)
	extern "C" void _call_static_run_method (frt_task* p_task) 
		__attribute__((section(".lowtext")));
#else
	extern "C" void _call_static_run_method (frt_task* p_task);
#endif
/// \endcond


//-------------------------------------------------------------------------------------
/** \brief This class is a C++ wrapper for the FreeRTOS task functions with some extra
 *  functionality for keeping track of state transitions and for printing diagnostic 
 *  information about how tasks are configured and how they are running. 
 * 
 *  \section Usage
 *  In order to use a task, one must first create a child class of class \c frt_task.
 *  The child class must at least have a constructor and a run method: 
 *  \code
 *  class task_example : public frt_task
 *  {
 *  protected:
 *      // Task specific data goes here
 * 
 *  public:
 *      // This constructor creates an example task object
 *      	task_example (const char*, unsigned portBASE_TYPE, size_t, emstream*);
 * 
 *      // This run method is called by the RTOS and contains a loop 
 *      void run (void);
 *  };
 *  \endcode
 * 
 *  The task class's constructor can sometimes be quite simple, only calling the parent 
 *  constructor as shown in the example below:
 *  \code
 *  task_example::task_example (const char* a_name,
 *                              unsigned portBASE_TYPE a_priority,
 *                              size_t a_stack_size,
 *                              emstream* p_ser_dev
 *                             )
 *      : frt_task (a_name, a_priority, a_stack_size, p_ser_dev)
 *  {
 *  }
 *  \endcode
 *  If the task needs to initialize device drivers or other things it owns, that can
 *  be done in the constructor. 
 * 
 *  Every task must have a user-written run method named \c run(). The run method is 
 *  where most of the functionality of the task is implemented. The run method must 
 *  contain an endless loop in which the task's state machine is implemented. A 
 *  variable called \c state is kept by the parent class \c frt_task and used to 
 *  monitor the state in which the user's code is running, so one should \b not 
 *  declare another variable called \c state inside the run method:
 *  \code
 *  void task_example::run (void)
 *  {
 *      portTickType xLastWakeTime;				// For scheduling how often task runs
 *  
 *      // Initialise the xLastWakeTime variable with the current time. This happens just
 *      // once, before the infinite loop is entered
 *      	xLastWakeTime = get_tick_count ();
 * 
 *      // This is the task loop. Once the task has been initialized in the code just
 *      // above, the task loop runs, and it keeps running until the power is shut off
 *      for (;;)
 *      {
 *          // Tell the RTOS to delay until the given number of RTOS timer ticks have
 *          // elapsed. This means the code after this line runs every ticks_per_run
 *          // milliseconds if the RTOS interrupt is set to go off every 1 millisecond
 *          delay_from_to (xLastWakeTime, ticks_per_run);
 * 
 *          // Run the task's state machine here
 *          switch (state)
 *          {
 *              case 0:
 *                  // Do one thing in State 0
 *                  ...
 *                  // Check for a transition to another state
 *                  if (something)
 *                  {
 *                      transition_to (1);
 *                  }
 *                  break;
 * 
 *              case 1:
 *                  // Do another thing in State 1
 *                  ...
 *                  break;
 * 
 *              case 2:
 *                  // Do something completely different in State 2
 *                  ...
 *                  break;
 *          };
 *      }
 *  }
 *  \endcode
 * 
 *  Tasks are traditionally instantiated within \c main(). The most important and
 *  tricky part about creating a task is getting its \b stack size right. If a task's
 *  stack is too large, it will waste RAM, and small microcontrollers don't have much
 *  RAM to waste. If the task's stack is too small, it will cause disaster, usually in
 *  the form of the processor either hanging or rebooting as the stack pointer for the
 *  task goes outside the allocated range and overwrites the stack for another task. 
 *  For FreeRTOS tasks on an 8-bit AVR microcontroller, the smallest stack size that
 *  is likely to ever work is about 100 bytes. As soon as data is created within the 
 *  run method, queues are used, and other memory is used, the necessary stack size
 *  goes up; it is common to need 300 -- 500 bytes of stack space for a task that does
 *  a lot of work. Because it's very difficult to calculate the stack space needed for
 *  a task, the easiest way to set the stack size is usually to make it large enough
 *  that the program runs reliably, then decrease it by about 20 bytes at a time, 
 *  recompile and test the program, again and again until something fails. Then make
 *  the stack size a couple dozen bytes larger than the smallest size that reliably
 *  worked during testing...just in case. 
 *  
 * The code used to create a task object looks like the following:
 *  \code
 *  // Create an example task. It runs at priority 2 and has a 200 byte stack
 *  task_example* my_example_task 
 *      = new task_example ("Example", tskIDLE_PRIORITY + 2, 200, &ser_port);
 *  ...
 *  // Start up the scheduler, causing all tasks to be run
 *  vTaskStartScheduler ();
 *  \endcode
 *  In this example code, someone must have created the serial port object named 
 *  \c ser_port previously; the serial port will be used by the task to print out
 *  diagnostic information. If no diagnostic information needs to be printed, the
 *  serial port may be left out of the task constructor call or set to \c NULL. 
 * 
 */

class frt_task
{
	// These private functions can't be accessed from outside this class. The two
	// methods here can't be used to construct anything; this is intentional, as
	// it prevents an object of this class from being copied. One shouldn't copy a
	// task object; it's unnatural and can lead one to moral instability and ennui
	private:
		/** This copy constructor is poisoned by being declared private so that it 
		 *  can't be used. One should not copy a task object.
		 *  @param that_clod A reference to a task which ought not be copied
		 */
		frt_task (const frt_task& that_clod);

		/** This assignment operator is poisoned by being declared private so that it
		 *  can't be used to copy the contents of one task into another. There is no
		 *  legitimate, morally acceptable reason to copy tasks that way. 
		 *  @param that_clod A reference to a task which ought not be copied
		 */
		frt_task& operator= (const frt_task& that_clod);

	// This protected data can only be accessed from this class or its descendents
	protected:
		/// This is the handle of this RTOS task. It's typedef'd as a pointer type. 
		xTaskHandle handle;

		/** This is a pointer to the previously created task. The pointer will be used
		 *  to implement a linked list of tasks through which one can traverse.
		 */
		frt_task* prev_task_pointer;

		/** This pointer can point to a serial output device or port which will be used
		 *  for various diagnostic printouts or logging.
		 */
		emstream* p_serial;

		#if (INCLUDE_uxTaskGetStackHighWaterMark == 1)
			/// This is the size of the task's total stack space. 
			size_t total_stack;

			/** This is a pointer to the top (beginning) of the task's stack. It is
			 *  used when we want to print out the stack for debugging purposes.
			 */
			uint16_t top_of_stack;
		#endif

		/** This is the state in which the finite state machine of the task is. This
		 *  variable should be used inside run() to implement the state machine so
		 *  that state transitions can be tracked, if needed, by this parent class.
		 */
		uint8_t state;

		/** This variable keeps track of the previous (before run() runs) value of the
		 *  state so that transitions can be conveniently detected.
		 */
		uint8_t previous_state;

		/** This variable keeps track of how many times the task has run through its
		 *  loop. In order for it to work, the user must either use the \c setup() and
		 *  \c loop() methods (activated by defining \c TASK_SETUP_AND_LOOP in the
		 *  Makefile), or if using the plain \c run() method, put a line of code that
		 *  increments this variable, as in \c runs++; somewhere in the task loop.
		 */
		uint32_t runs;

		/** This method allows descendent classes to find out how many times the
			*  \c loop() method has run.
			*  @return The number of times the loop has been run
			*/
		uint32_t get_loop_runs (void)
		{
			return (runs);
		}

	// Public methods can be called from anywhere in the program where there is a 
	// pointer or reference to an object of this class
	public:
		// This constructor creates a FreeRTOS task with the given task run function, 
		// name, priority, and stack size
		explicit frt_task (const char* a_name, 
						   unsigned portBASE_TYPE a_priority = 0, 
						   size_t a_stack_size = configMINIMAL_STACK_SIZE,
						   emstream* p_ser_dev = NULL);

		/** \cond NO_DOXY 
		 *  This method is called by the task's static run method which is, in turn,
		 *  called by the FreeRTOS run function. 
		 */ 
		static void _call_users_run_method (frt_task*);
		/// \endcond 

		// If the preprocessor variable TASK_SETUP_AND_LOOP is defined, each task must
		// provide functions setup() and loop() to be called by the RTOS scheduler
		#ifdef TASK_SETUP_AND_LOOP
			/** This is a base method for the user's \c setup() method. The user's
			*  \c setup() method is called once, just after the RTOS scheduler starts
			*  up. It should set up hardware, put hello messages in the appropriate 
			*  queues, and do other things which are needed at startup. Class member 
			*  variables, which are declared in the class declaration in the \c .h 
			*  file, should usually be instantiated and initialized as need be in the 
			*  constructor. The \c setup() method belonging to the base class 
			*  \c frt_task cannot ever be called, as it is a pure virtual method with 
			*  no implementation; the user's descendent class must provide an 
			*  implementation of \c setup(). 
			*/
			virtual void setup (void) = 0;

			/** This is a base method for the user's \c loop() method. The user's 
			 *  method is called repeatedly, in an endless loop, by the RTOS scheduler,
			 *  after \c setup() has finished. Usually this method should contain a 
			 *  \c switch statement that implements a finite state machine using the 
			 *  variable \c state to keep track of which state the program is in at 
			 *  any given time.
			 */
			virtual void loop (void) = 0;

			/** This method calls the user's loop() method and records data to be used
			 *  for task profiling if needed. This method should not be called by any
			 *  user code; it's only called by _call_users_run_method(), which is 
			 *  automatically called by the RTOS scheduler. 
			 */
			void run_the_loop (void);

		// If the preprocessor variable is not defined, each task must provide a run()
		// method which is called once by the scheduler and contains an infinite loop
		#else
			/** This is the method in which the task runs the user's task code. The 
			 *  base class's \c run() method contains no code, and an object of 
			 *  class \c frt_task cannot be instantiated. The user's code for each 
			 *  task goes in the \c run() method for each of the user's task classes,
			 *  each task class being descended from \c frt_task. Often the user's 
			 *  \c run() method contains a whole finite state machine; the variable 
			 *  \c state is provided so that the state machine's operation can be 
			 *  monitored by the parent class to help with debugging. 
			 *
			 *  Because this code is expected to run in a preemptive multitasking 
			 *  environment, the run method should usually contain an infinite loop 
			 *  which never exits. An exception is when tasks are being dynamically 
			 *  created and deleted; in this case, one can program a run method which 
			 *  exits; after exit the task will be deleted. If tasks are not being 
			 *  deleted, exiting a run method just causes the task to hang in an 
			 *  indefinite loop of repeated delays. 
			 */
			virtual void run (void) = 0;
		#endif

		// This method is called within run() to cause a state transition
		void transition_to (uint8_t);

		/** This method sets the task's serial device pointer to the given address.
		 *  Changing this serial device pointer means that debugging output will be
		 *  directed to the given device. This can be helpful if task transitions or
		 *  other debugging information should be shown on a serial console as the
		 *  task is being created but logged somewhere else when the task is running.
		 *  @param p_new_dev A pointer to the serial device on which debugging
		 *                   information will be printed in the near future
		 */
		void set_serial_device (emstream* p_new_dev)
		{
			p_serial = p_new_dev;
		}

		/** This method un-sets the task's serial device pointer. Doing so prevents
		 *  serial debugging output from being sent or logged in the future unless 
		 *  \c set_serial_device() is called to set the serial device pointer again.
		 */
		void unset_serial_device (void)
		{
			p_serial = NULL;
		}

		/** This overloaded operator allows one to check if the task is valid and 
		 *  ready to run.  It looks at the task's handle, which is nonzero if the 
		 *  task has been successfully created and hasn't been stopped.
		 */
		operator bool () const
		{
			return (handle != 0);
		}

		/** This method causes the task to delay for approximately the given number
		 *  of RTOS ticks from the time it is called. This method should NOT be used
		 *  to make a task run at regular intervals, because the time between when
		 *  this task began to run and when \c delay() is called can vary. For 
		 *  periodic runs of code in a task, see \c delay_from_to() below.
		 *  @param duration The amount of time, as a number of RTOS ticks, this task
		 *                  should wait until next running
		 */
		void delay (portTickType duration)
		{
			vTaskDelay (duration);
		}

		/** This method causes the task to delay for the given number of milliseconds.
		 *  The preprocessor converts a time in milliseconds to a number of RTOS ticks
		 *  and calls the FreeRTOS function \c vTaskDelay() to do the work. The
		 *  maximum number of milliseconds the program can delay is limited by the
		 *  size of the parameter and the RTOS tick rate, but as long as the RTOS tick
		 *  type is 32 bit, it's a long time, about 49 days for a 1 ms RTOS tick rate.
		 *  This method should NOT be used to make a task run at regular intervals, 
		 *  because the time between when this task began to run and when \c delay_ms()
		 *  is called can vary. For periodic runs of code in a task, see 
		 *  \c delay_from_to_ms() below.
		 *  @param milliseconds The number of milliseconds to delay
		 */
		void delay_ms (portTickType milliseconds)
		{
			portTickType duration = configMS_TO_TICKS (milliseconds);
			vTaskDelay (duration);
		}

		/** This method causes the task to delay from a given time for a specified 
		 *  duration. The start time and duration are given in units of RTOS timer
		 *  ticks. This method can be used to implement a task that regularly wakes
		 *  up and performs some action, like a clown waking up to terrify children. 
		 *  An example of how this method is used follows:
		 *  \code
		 *  // Make a variable which will hold tick counts and initialize it
		 *  portTickType previousTicks = xTaskGetTickCount ();
		 *  ...
		 *  for (;;)                   // The task loop
		 *  {
		 *      ...                    // User code to run every 100 RTOS ticks
		 *      ...
		 *      delay_from_to (previousTicks, 100);
		 *  }
		 *  \endcode
		 *  @param from_ticks The beginning time of the duration to delay. It is
		 *                    usually set equal to the end time of the previous
		 *                    delay so as to get precise, regular timing
		 *  @param interval The duration of the delay interval in RTOS ticks
		 */
		void delay_from_to (portTickType& from_ticks, portTickType interval)
		{
			vTaskDelayUntil (&from_ticks, interval);
		}

		/** This method causes the task to delay from a given time for a specified 
		 *  duration in milliseconds. This is usually done to make a task that runs
		 *  code at a fairly precise and regular interval. The start time is given 
		 *  in units of RTOS timer ticks; although ticks might not equal milliseconds,
		 *  the user need not care, just store the value in a variable of type 
		 *  \c portTickType (see below). The delay duration is given in milliseconds.
		 *  An example of how this method is used follows:
		 *  \code
		 *  // Make a variable which will hold tick counts and initialize it
		 *  portTickType previousTicks = xTaskGetTickCount ();
		 *  ...
		 *  for (;;)                   // The task loop
		 *  {
		 *      ...                    // User code to run every 10 ms
		 *      ...
		 *      delay_from_to_ms (previousTicks, 10);
		 *  }
		 *  \endcode
		 *  @param from_ticks The beginning time of the duration to delay. It is
		 *                    usually set equal to the end time of the previous
		 *                    delay so as to get precise, regular timing
		 *  @param milliseconds The duration of the delay interval in milliseconds
		 */
		void delay_from_to_ms (portTickType& from_ticks, portTickType milliseconds)
		{
			portTickType interval = configMS_TO_TICKS (milliseconds);
			vTaskDelayUntil (&from_ticks, interval);
		}

		/** This method returns the number of RTOS ticks until the time the method
		 *  is called.
		 *  @return The number of RTOS ticks which have happened since RTOS startup
		 */
		portTickType get_tick_count (void)
		{
			return (xTaskGetTickCount ());
		}

		/** This method causes the task in which it's called to yield (give control 
		 *  back to the RTOS right away). 
		 */
		void yield (void)
		{
			taskYIELD ();
		}

		/** This method returns the task's current priority.
		 *  @return The priority at which the task is currently running
		 */
		portBASE_TYPE get_priority (void)
		{
			return (uxTaskPriorityGet(handle));
		}

		/** This method sets this task's priority to a new value. 
		 *  @param new_priority The new priority to set for this task
		 */
		void set_priority (portBASE_TYPE new_priority) 
		{
			vTaskPrioritySet (handle, new_priority);
		}

		/** This method returns the number of bytes left to be used in the heap. This
		 *  means that the number returned is how many bytes are currently unused. 
		 *  @return The approximate number of bytes left for use in the heap
		 */
		size_t heap_left (void)
		{
			return (xPortGetFreeHeapSize ());
		}

		#if (INCLUDE_uxTaskGetStackHighWaterMark == 1)
			/** This method returns an estimate of the number of bytes left in this 
			 *  task's stack. The estimate is made based on the smallest number of 
			 *  bytes seen to be left in the stack when this task was previously 
			 *  context-switched, (the 'high water mark'), so it's not necessarily an 
			 *  exact measure of the available stack space. 
			 *  @return The approximate number of bytes left in this task's stack
			 */
			size_t stack_left (void)
			{
				return (uxTaskGetStackHighWaterMark(handle));
			}
		#endif

		/** This method prints a dump of the stack's contents for debugging and
		 *  instructional purposes.
		 *  @param p_ser_d The serial device to which the stack will be printed
		 */
		void dump_stack (emstream* p_ser_d)
		{
			hex_dump_memory ((uint8_t*)(top_of_stack - total_stack), 
								(uint8_t*)(top_of_stack), p_ser_d, true, 0x11);
		}

		/** This method prints a stack dump, then asks the next task in the list 
		 *  to do the same, and so on and so on, so all tasks do it
		 */
		void print_stack_in_list (emstream*);

		/** This method returns the task's total stack size, which was set in the
		 *  constructor call.
		 *  @return The task's total stack size in bytes
		 */
		size_t get_total_stack (void)
		{
			return (total_stack);
		}

		/** This method returns the transition logic state in which the task is at the
		 *  current time. This is the value of the variable 'state' which is 
		 *  manipulated by the user within the run() method to cause state transitions.
		 *  @return The current state
		 */
		uint8_t get_state (void)
		{
			return (state);
		}

		/** This method returns a pointer to the task's name, which resides in a null 
		*  terminated character array belonging to the task. 
		*  @return A pointer to the task's name
		*/
		const char* get_name (void)
		{
			return ((const char*)(pcTaskGetTaskName (handle)));
		}

		// This method prints task status information, then asks the next task in the
		// list to do so
		void print_status_in_list (emstream*);

		/** This method returns a pointer to the most recently created task. This 
		 *  pointer is the head of a linked list of tasks; the list is maintained by
		 *  the task objects themselves. This pointer to the most recently created
		 *  task is used to begin traversing the list of all tasks. 
		 *  @return A pointer to the most recently created task
		 */
		static const frt_task* get_last_created_task_pointer (void)
		{
			return (last_created_task_pointer);
		}

		/** This method returns the handle of the FreeRTOS task which is inside this
		 *  object. Advanced users might want to use it to access task manipulation 
		 *  functions that aren't in this wrapper class or for other creative hacking.
		 *  @return The handle of the FreeRTOS task which is wrapped in this handy C++
		 *          class
		 */
		xTaskHandle get_handle (void)
		{
			return (handle);
		}

		// Print the status of this task
		virtual void print_status (emstream&);

		/*  This method prints an error message and resets the processor. It should 
		 *  only be used in cases of things going seriously to heck.
		 */
		void emergency_reset (void);

		// TODO: Add other functions we might like: suspend(), resume(), 
		// resumeFromISR(), etc. 
};

// This operator writes information about the task's status to the given serial device.
// That information can be used for debugging or perhaps reliability testing
emstream& operator << (emstream&, frt_task&);

// This function prints information about how all the tasks are doing
void print_task_list (emstream* ser_dev);

// This function has all the tasks print their stacks
void print_task_stacks (emstream* ser_dev);

#endif  // _FRT_TASK_H_

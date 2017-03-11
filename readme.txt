C++ Ethical Keylogger
Lucas Barbosa | 2017 | Open source software
Source Code @ https://github.com/lucasbrsa/MSWKeylogger

  ------------------------- // -------------------------

Overview--
Advanced C++11 keylogger built on grey hat principles
for experimental purposes ONLY. Creator and proprietor
Lucas Barbosa.
This software includes networking capabilities via
SMTP (Simple Mail Transfer Protocol), through port 25 which
is encrypted by a Base64 algorithm. Anyone who does oblige
to these encryption standards must make sure to keep they're
logged keystrokes and maximum descretion.

Privacy-
This software has been written with the intent for
experimentation, testing and modification. However it was
not written to be used as malware. Please concider other's
privacy before deploying this software somewhere where it
shouln't, penalties will apply.

Powershell SMTP Protocol:
* Powershell is optimal compared to the millions of lines
  of code required for a SSH encryption protocol to a
  encrypted SERVERCALL
* Concider the privacy of the email credentials you wish to
  use when deploying and testing this software with its
  SMTP capabilities, as if anyone does get a copy of your
  software they will have direct access to your email
  credentials via the lib\SendMail.h source file [lines 26:30]

Base64 Encryption [depricated]
* This software does encrypt its logged or outputted data
  for safety and privacy. The orignal software package does
  include a decoder and a decrypter however in future release
  these features will be deprecated along with the Base64
  encryption.

  Installation--
  * This description mainly refers on the procedure to 'execute'
    the keylogger.
  (1) Getting it on your system
      i) Copy or clone the source code from the repo (mentioned above)
     ii) Choose a reasonable remote location for the keylogger preferably on
         a USB drive, as this is how it will be deplyoyed later on.
    iii) Using powershell or the DOS prompt in the directory of the  keylogger
         type make and the file should be compiled. At this point you will see
         an .exe file in your currect directory named: `SearchSuggestionService.exe`
         This is the disguised name for the keylogger's .exe file. You may change it
         to whatever you deem fit.
     iv) Make sure to place the file in a safe location as it will do the rest on its own.
  (2) Making sure maintain the code
      As the author of this software I do highly encourage that you jump staight into the
      source and seek to not only understand how it works but to better it. There still are
      so many functionalities these keylogger can have and will have in future releases.
      If you do find any bugs or errors in the program please contact me @
      lucas.chu.barbosa@gmail.com, as I am constantly seeking to improve this project.

      Good luck! Lucas

  ------------------------- // -------------------------

IMPORTANT: To future developers
* This software is open source which does allow anyone
  with access to it to modify it how they see fit,
  however keep in mind that DEPLOYED versions of THIS
  software will be disguised as other applications for
  stealth and testing purposes.
  *NOTE: Please do not deploy this software with black
         hat principles or any intent associated with
         malware or harmful actions, as the liabilities
         and responsibilities will be represented by you
         and you ONLY.
* Be aware that deployed versions (or release versions)
  of this software will not be documented nor will it be
  guaranteed to function the same way as the original
  copy. To obtain an original copy with full documentation
  and design instructions download a copy @:
  www.themonopolist.org/technologies

  ------------------------- // -------------------------

Lucas Barbosa | 2017 | Open source software
Source Code @ https://github.com/lucasbrsa/MSWKeylogger

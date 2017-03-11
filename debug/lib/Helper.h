/*
 *  C++ lib\Helper.h
 *  Lucas Barbosa | 2017 | Open source software
 */

#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED

/*
 *  Auxilary header for keylogger support.
 *  All the functions in this header fall under the same namespace
 *  called Helper (Helper::<function>,<variable>).
 */

#include <ctime>
#include <sstream>
#include <fstream>

using namespace std;

/*
 *  HELPER.H
 *
 *  This header containes all the basic system functionality,
 *  arbitirary library includes and cast functions
 */

namespace Helper
{

template<typename T>

std::string to_string(const T &);

/*
 *  DateTime stuct will contain all the current date and time settings
 *  in which will be utilised when naming our logged files (for extra
 *  details of course).
 */

struct DateTime
{

  DateTime()
  {
    time_t ms;
    time(&ms);  // Registers local system time into the address a 'time_t' datatype variable

    struct tm *info = localtime(&ms);
    Day = info->tm_mday;
    Month = info->tm_mon + 1;   // Starts from 0 (January is actually 1)
    Year = 1900 + info->tm_year;
    Hour = info->tm_hour;
    Minute = info->tm_min;
    Second = info->tm_sec;
  }

  // If system date AND time is user defined
  DateTime(int day_param, int month_param, int year_param, int hour_param, int mins_param, int seconds_param) :
  Day(day_param), Month(month_param), Year(year_param), Hour(hour_param), Minute(mins_param), Second(seconds_param) {}

  // If time is set to 0:00:00
  DateTime(int day_param, int month_param, int year_param) :
  Day(day_param), Month(month_param), Year(year_param), Hour(0), Minute(0), Second(0) {}

  DateTime Now() const
  {
    return DateTime();
  }

  int Day, Month, Year, Hour, Minute, Second;

  std::string get_date_string() const
  {
    return std::string(Day < 10 ? "0" : "") + to_string(Day) +
           std::string(Month < 10 ? ".0" : "") + to_string(Month) +
           "." + to_string(Year);
  }

  std::string get_time_string(const std::string &separator = ":") const
  {
    return std::string(Hour < 10 ? "0" : "") + to_string(Hour) + separator +
           std::string(Minute < 10 ? "0" : "") + to_string(Minute) + separator +
           std::string(Second < 10 ? "0" : "") + to_string(Second);
  }

  std::string get_date_time_string(const std::string &separator = ":") const
  {
    return get_date_string() + " " + get_time_string(separator);
  }

};

template<typename T>

std::string to_string(const T &non_string_val)
{
  std::ostringstream output_stream;
  output_stream << non_string_val;
  return output_stream.str();
}

void write_app_log(const std::string &log_string)
{
  std::ofstream log_file("AppLog.txt", std::ios::app);
  log_file << "[" << Helper::DateTime().get_date_time_string() << "]" << std::endl
  << log_string << std::endl << std::endl;
  log_file.close();
}

}// end of namespace Helper.h

#endif // HELPER_H_INCLUDED

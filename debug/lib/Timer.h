/*
 *  C++ lib\Timer.h
 *  Lucas Barbosa | 2017 | Open source software
 */

#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

/*
 *  Asynchronous encapsulated timing processes
 *
 *  This header will source the timing functionalities of the logged key
 *  strokes. It also will handle the threaded ansynchronous operations of
 *  the keylogger in general.
 *
 *  <thread> allows us to communicate between threads much easier as
 *  opposed to communicating between processes, in turn making our
 *  application asychronous.
 *  <chrono> will provide us with a very robust timing library
 *  very unlike <ctime> which isn't as accurate and does still
 *  have many issues in which we are not interested in.
 */

 #include <thread>
 #include <chrono>      // more precise then <ctime>
 #include <functional>  // C++11 std::function<> capabilities

class Timer
{
  private:

    // thread for asynchronous communication. (won't interfere with main thread)
    std::thread Thread;
    bool alive = false;
    /*
     *  const <func_call_count> is the amount of times we wish to call a specific function
     *  ('L' for long, not int). These members nned to be of type long, to prevent
     *  overflow of data, as they could be acumilated for long periods of time,
     *  hence the 'L' in value.
     *  <repeat_count> will be the variable that holds the amount of times a function
     *  has been called or 'repeated'.
     *
     */
    long func_call_count = -1L;
    long repeat_count = -1L;
    /*
     *  <ms_interval> representas a time interval in milliseconds between fuction calls
     *  which begins at 0ms.
     */
    std::chrono::milliseconds ms_interval = std::chrono::milliseconds(0);
    /*
     *  std::function<void(void)>function_name
     *  New type to C++11, this is a type which contains other functions. Can accept a funciton
     *  which doesn't take in anything AND doesn't return anything.
     *  <nullptr>
     *  this is the same as pointing a something on the heap to 0, or nothing. Which is normally
     *  done in the initialization of pointers (without them being assigned to a value yet)
     */
    std::function<void(void)> funct = nullptr;

    void sleep_and_run()
    {
      // std::this_thread::sleep_for(<time>) blocks the current thread for a time interval
      std::this_thread::sleep_for(ms_interval);
      if (this->alive)
      {
        /*
         *  Function()()
         *  The FIRST parethesis will return a funcion for the SECOND parenthesis to catch and run
         */
        Function()();
      }
    }

    void thread_function()
    {
      if (this->func_call_count == Infinite)
      {
        while(this->alive)
        {
          sleep_and_run();
        }
      }
      else
      {
        while(this->repeat_count--)
        {
          sleep_and_run();
        }
      }
    }

  public:

    static const long Infinite = -1L;

    Timer(){}

    Timer(const std::function<void(void)> &reference_to_func) : funct(reference_to_func) {}

    Timer(const std::function<void(void)> &reference_to_func, const unsigned long &interval, const long func_repeat = Timer::Infinite) :
         funct(reference_to_func), ms_interval(std::chrono::milliseconds(interval)), func_call_count(func_repeat) {}

    // starting the timer

    void start_timer(bool async = true)
    {
      if (this->is_alive())
      {
        // if it ALREADY is running then do nothing
        return;
      }
      // if it is not running
      this->alive = true;
      this->repeat_count = this->func_call_count;
      if (async)
      {
        // asynchronous call to unblocked thread
        Thread = std::thread(thread_function, this);
      }
      else
      {
        // concurrent thread which will block the main thread
        this->thread_function();
      }
    }

    void stop_timer()
    {
      this->alive = false;
      // syncs threads together, meaning no concurrent threads
      Thread.join();
    }

    bool is_alive() const
    {
      // to figure out wether the timer is running or not
      return this->alive;
    }

    /*
     *  <set_function> will set the FUNCTION in which will be EXECUTED can
     *  be used with an empty constructor so we can set values of
     *  members ourselves (when we lack the constructor initilizer list).
     */
    void set_function(const std::function<void(void)> &custom_defined_func)
    {
      // originally <nullptr> value is now being set to the custom defined function
      funct = custom_defined_func;
    }

    void set_repeat_count(const long repeat_count_num)
    {
      if (this->alive)
      {
        return;
      }
      // sets the number of function repeatences
      this->func_call_count = repeat_count_num;
    }

    long get_remaining_count() const
    {
      // fetch how many iterations left
      return this->repeat_count;
    }

    long get_repeat_count() const
    {
      // returns total number of function calls we SPECIFIED (customed)
      return this->func_call_count;
    }

    void set_interval(const unsigned long &time_interval)
    {
      if (this->alive)
      {
        return;
      }
      // sets time interval
      this->ms_interval = std::chrono::milliseconds(time_interval);
    }

    unsigned long get_interval() const
    {
      /*
       *  .count() is there to retrieve an actual number from the ms_interval
       *  which is of type <std::chrono::milliseconds> not 'long'.
       */
      return this->ms_interval.count();
    }

    /*
     *  std::function<<return_type>(<param_type>)> template_name = original_function
     *  This concept is new to C++11. Its basicallya funny looking function template
     *  with return type and parameter type specifications.
     */
    const std::function<void(void)> &Function() const
    {
      return this->funct;
    }
};

#endif // TIMER_H_INCLUDED

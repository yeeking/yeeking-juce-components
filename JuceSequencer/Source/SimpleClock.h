#pragma once

#include <thread>
#include <chrono>
#include <functional>
#include <iostream>

class SimpleClock 
{
  public:
    SimpleClock(int sleepTimeMs = 5, 
                std::function<void()>callback = [](){
                    std::cout << "SimpleClock::default tick callback" << std::endl;
                }) : sleepTimeMs{sleepTimeMs}, running{false}, callback{callback}
     {
       // constructor body
     }

    ~SimpleClock()
    {
      stop();
      delete tickThread;
    }
    /** start with the sent interval between calls the to callback*/
    void start(int intervalMs)
    {
      stop();
      running = true;
      tickThread = new std::thread(SimpleClock::ticker, this, intervalMs, sleepTimeMs);
    }

    void stop()
    {
      if (running)
      {
       //t << "SimpleClock::stop shutting down " << std::endl;
        running = false; 
        tickThread->join(); 
        delete tickThread;
      }
    }
    /** set the function to be called when the click ticks */
    void setCallback(std::function<void()> c){
      callback = c;
    }
    void tick()
    {
      // call the callback
      //std::cout << "SimpleClock::tick" << std::endl; 
      callback();     
    }

 static void ticker(SimpleClock* clock, long intervalMs, long sleepTimeMs)
    {
      long nowMs = 0;
      long elapsedMs = 0;
      long startTimeMs = SimpleClock::getNow();
      long remainingMs;
      while(clock->running)
      {
        nowMs = SimpleClock::getNow();
        elapsedMs = nowMs - startTimeMs;
        if (elapsedMs >= intervalMs) // time to tick
        { 
          // move the start time along
          startTimeMs = nowMs;
          //std::cout << "SimpleClock::ticker elapsed time  " << elapsedMs << " of " << intervalMs << std::endl;
          clock->tick();
        }
        else {
          // sleep - but how much?
          remainingMs = intervalMs - elapsedMs;
          if (remainingMs > sleepTimeMs) // sleep as much as possible
            std::this_thread::sleep_for(std::chrono::milliseconds(remainingMs - sleepTimeMs));
          else
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeMs));
        }
      }
    }
    
  private:
    static long getNow()
    {
      return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }
    long intervalMs;
    long sleepTimeMs; // lower means more precision in the timing
    bool running;     
    std::thread* tickThread;
    std::function<void()> callback;
};

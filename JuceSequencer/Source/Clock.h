/*
  ==============================================================================

    Clock.h
    Created: 17 May 2020 4:36:02pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>

class ClockListener
{
  public:
    ClockListener()
    {

    }
    virtual void clockTicked() = 0;
};

class Clock : public HighResolutionTimer
{ 
  public:
  Clock(float bpm = 120.0f)
  {
    setBPM(bpm);
  }

  void setBPM(float bpm)
  {
    this->bpm = bpm;
  } 
  void start()
  {
    int msInterval = (60.0f / bpm) * 1000.0f;
    if (isTimerRunning()) stopTimer();
    startTimer(msInterval);
  }

  void addClockListener(ClockListener* listener)
  {
    clockListeners.push_back(listener);
  }

  void 	hiResTimerCallback () override 
  {
    for (ClockListener* listener : clockListeners )
    {
      listener->clockTicked();
    }
  }
  private:
    std::vector<ClockListener*> clockListeners;
    float bpm;
};
/*
  ==============================================================================

    YeekingVU.h
    Created: 16 May 2020 6:17:39pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <cmath>

//==============================================================================
/*
*/
class YeekingVU    : public AudioAppComponent, 
                     public Timer
{
public:
    YeekingVU() : painting{false}
    {
        startTimer(100);
    }

    ~YeekingVU()
    {
        stopTimer();
    }


    void timerCallback()
    {
        if (!painting){
            repaint();
        }
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {

    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        if (painting)
        {
            return;
        }
        else
        {
            float total = 0.0f;
            int step = 64;
            auto reader = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);
            float sample; 
            for (auto i=0; i < bufferToFill.numSamples ; i+=step)
            {
                sample = reader[i];
                total +=  fabs(sample); 
            }
            total = total / (bufferToFill.numSamples / step);
            meanDB = Decibels::gainToDecibels(total);
        }
    }
    void releaseResources() override
    {

    }


    void paint (Graphics& g) override
    {
        painting = true;

        int barWidth = getWidth() - (fabs(meanDB / 60.0f) * getWidth());
        if (barWidth <= 0) barWidth = 1;
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
        if (meanDB < -20) g.setColour(Colours::green);
        else if (meanDB < -3) g.setColour(Colours::yellow);
        else if (meanDB > -3) g.setColour(Colours::red);
        g.fillRect(0, 0, barWidth, getHeight());

        painting = false;
    }

    void resized() override
    {
    }

private:
    bool painting;
    float meanDB;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (YeekingVU)
};

/*
  ==============================================================================

    YeekingOscilloscope.h
    Created: 16 May 2020 11:40:39am
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class YeekingOscilloscope    : public AudioAppComponent, 
                               public Timer
{
public:
    YeekingOscilloscope()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        plotBuffer = new AudioBuffer<float>(1, 512);
        painting = false; 
        startTimer(100);
    }

    ~YeekingOscilloscope()
    {
        stopTimer();
        delete plotBuffer;
    }
    
     //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {

    }
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        if (painting){
            return;
        }
        auto reader = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);
        auto writer = plotBuffer->getWritePointer(0, 0);
        int min = bufferToFill.numSamples;
        
        if (min > plotBuffer->getNumSamples()) min = plotBuffer->getNumSamples();
        for (auto i=0; i < min ; ++i)
        {
            writer[i] = reader[i];
        }
    }
    void releaseResources() override
    {

    }
    /** timer interface implementation*/
    void timerCallback() override
    {
        if (!painting)
        {
            repaint();
        }
    }



    void paint (Graphics& g) override
    {
        painting = true;
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (Colours::black);

        auto reader = plotBuffer->getReadPointer(0, 0);
        g.setColour(Colours::orange);
        float yScale = getHeight() * 0.5f;
        float xScale = getWidth() / ((float) plotBuffer->getNumSamples());
        int len = plotBuffer->getNumSamples() - 1;
        float x1, y1, x2, y2;
        for (int i=0;i<len; ++i)
        {
            y1 = reader[i]; // -1 to 1
            y2 = reader[i+1];
            x1 = i;    // 0 to 1024
            x2 = i+1;
            // normalise
            y1 = (y1 + 1.0f) * yScale;
            y2 = (y2 + 1.0f) * yScale;
            x1 = x1 * xScale;
            x2 = x2 * xScale;
            //DBG(x1 << "," << y1 << " to " << x2 << "," << y2);
            g.drawLine(x1, y1, x2, y2, 10);

        }    
        painting = false; 
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

private:
    AudioBuffer< float >* plotBuffer;
    bool painting; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (YeekingOscilloscope)
};

/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    plotBuffer = new AudioBuffer<float>(1, 512);
    painting = false; 
    startTimer(100);
}

void MainComponent::timerCallback()
{
    repaint();   
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
    stopTimer();
    delete plotBuffer;

}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    //bufferToFill.
    if (painting){
//        bufferToFill.clearActiveBufferRegion();
        return;

    }
    auto reader = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);
     auto writer = plotBuffer->getWritePointer(0, 0);
    int min = bufferToFill.numSamples;
    
    if (min > plotBuffer->getNumSamples()) min = plotBuffer->getNumSamples();
    //plotBuffer->copyFrom(0, 0, reader, 0, 512);
    //int destChannel, int destStartSample, const AudioBuffer &source, int sourceChannel, int sourceStartSample, int numSamples
    for (auto i=0; i < min ; ++i)
    {
        writer[i] = reader[i];
    }
  //  now clear it to prevent pass through
//    bufferToFill.clearActiveBufferRegion();

}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    painting = true;
    // (Our component is opaque, so we must completely fill the background with a solid colour)
   // g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
   g.fillAll (Colours::black);
   
    auto reader = plotBuffer->getReadPointer(0, 0);
    g.setColour(Colours::orange);
    float yScale = getHeight() * 0.5f;
    float xScale = getWidth() / ((float) plotBuffer->getNumSamples());
    int len = plotBuffer->getNumSamples() - 1;
    float x1, y1, x2, y2;
    for (int i=0;i<len; ++i)
    
 //  for (int i=0;i<510; ++i)
   
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
    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

/**
 * Sequencer stores a set of sequences each of which stores a step
 * 
 */

#pragma once 

#include <vector>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <functional>

class SimpleClock 
{

  public:
    SimpleClock() : sleepTimeMs{5}, running{false}, callback{
      [](){
          std::cout << "SimpleClock::default tick callback" << std::endl;
      }
     } 
     {}
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
        std::cout << "SimpleClock::stop shutting down " << std::endl;
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
      std::cout << "SimpleClock::tick" << std::endl; 
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
          std::cout << "SimpleClock::ticker elapsed time  " << elapsedMs << " of " << intervalMs << std::endl;
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


class Step{
  public:
    Step()
    {
      // for (auto i=0;i<16;i++)
      // {
      //   data.push_back(0.0f);
      // }
      data.push_back(0.0f);
    }
    std::vector<float> getData() const
    {
      return data; 
    }
    void setData(std::vector<float> data)
    {
      this->data = data; 
    }
  private: 
    std::vector<float> data;
};


class Sequence{
  public:
    Sequence() : currentStep{0}, currentLength{16}
    {
      for (auto i=0;i<128;i++)
      {
        steps.push_back(Step{});
      }
      
    }
    /** go to the next step */
    void tick()
    {
      currentStep = (++currentStep) % currentLength;
    }
    bool assertStep(unsigned int step)
    {
      if (step >= steps.size() || step < 0) return false;
      return true; 
    }

    std::vector<float> getStepData(int step)
    {
      return steps[step].getData();
    }
    std::vector<float> getCurrentStepData()
    {
      return steps[currentStep].getData();
    }

    void setStepData(unsigned int step, std::vector<float> data)
    {
      steps[step].setData(data);
    }

    std::string stepToString(int step)
    {
      std::vector<float> data = getStepData(step);
      return std::to_string(data[0]);
    }

    unsigned int howManySteps() const 
    {
      return steps.size();
    }

  private:
    int currentLength;
    int currentStep;
    std::vector<Step> steps;

};

/** represents a sequencer which is used to store a grid of data and to step through it */
class Sequencer  {
    public:
      Sequencer()
      {
        for (auto i=0;i<4;i++)
        {
          sequences.push_back(Sequence{});
        }
      }


      unsigned int howManySequences() const 
      {
        return sequences.size();
      }
      unsigned int howManySteps(unsigned int sequence) const 
      {
        if (assertSequence(sequence)) return sequences[sequence].howManySteps();
        else return 0;
      }

      /** move the sequencer along by one tick */
      void tick()
      {
        std::cout << "Sequencer.h: tick" << std::endl;
        for (Sequence& seq : sequences)
        {
            seq.tick();
        }
      }
      /** update the data stored at a step in the sequencer */
      void setStepData(unsigned int sequence, unsigned int step, std::vector<float> data)
      {
        if (!assertSeqAndStep(sequence, step)) return;
        sequences[sequence].setStepData(step, data);
      }
      /** retrieve the data for the current step */
      std::vector<float> getCurrentStepData(int sequence)
      {
        if (sequence >= sequences.size() || sequence < 0) return std::vector<float>{};
        return sequences[sequence].getCurrentStepData();
      }
      /** retrieve the data for a specific step */
      std::vector<float> getStepData(int sequence, int step)
      {
        if (!assertSeqAndStep(sequence, step)) return std::vector<float>{};
        return sequences[sequence].getStepData(step);
      }
      
      void addStepListener();

  /** print out a tracker style view of the sequence */
    std::string toString()
    {
      std::string s{""};
      for (int step = 0; step < 32; ++step)
      {
        s += std::to_string(step) + "\t: ";
        for (Sequence& seq : sequences)
        {
          s += seq.stepToString(step) + "\t";
        }
        s += "\n";
      }
      return s;
    }

    private:

    bool assertSeqAndStep(unsigned int sequence, unsigned int step)
      {
        if (!assertSequence(sequence)) return false;
        if (!sequences[sequence].assertStep(step)) return false; 
        return true;
      }

    bool assertSequence(unsigned int sequence) const
    {
      if (sequence >= sequences.size() || sequence < 0)
        {
          std::cout << "Sequence " << sequence << " out of range. use 0 - " << (sequences.size()-1) << std::endl;
          return false;
        } 
      return true;
    }

    /// class data members  
      std::vector<Sequence> sequences;
};

/** Represents an editor for a sequencer, which allows stateful edit operations to be applied 
 * to sequences. For example, select sequemce, select step, enter data
 * Used to build editing interfaces for a sequencer 
*/

enum class SequencerEditorMode {selectingStep, editingStep};

class SequencerEditor {
  public:
    SequencerEditor(Sequencer* sequencer) : sequencer{sequencer}, currentSequence{0}, currentStep{0}, currentStepIndex{0}, editMode{SequencerEditorMode::selectingStep}, stepIncrement{0.5f}
    {

    }

  void setEditMode(SequencerEditorMode mode)
  {
    this->editMode = mode;
  }

  /** moves the editor cursor up. 
   * If in selectingStep mode, steps through the sequenbces, wrapping at the top
   * if in editingStep mode, edits the 
   * */

  void moveCursorUp()
  {
    if (editMode == SequencerEditorMode::selectingStep)
    {
      currentSequence -= 1;
      if (currentSequence < 0) currentSequence = 0;
    }
  }

  void moveCursorDown()
  {
    if (editMode == SequencerEditorMode::selectingStep)
    {
      currentSequence += 1;
      if (currentSequence >= sequencer->howManySequences()) currentSequence = sequencer->howManySequences() - 1;
    }
  }

  void moveCursorLeft()
  {
    if (editMode == SequencerEditorMode::selectingStep)
      {
        currentStep -= 1;
        if (currentStep < 0) currentStep = 0;
      }
  }

  void moveCursorRight()
  {
    if (editMode == SequencerEditorMode::selectingStep)
      {
        currentStep += 1;
        if (currentStep >= sequencer->howManySteps(currentSequence)) currentStep = sequencer->howManySteps(currentSequence) - 1;
      }
  }

  int getCurrentSequence() const 
  {
    return currentSequence;
  }
  /**  */
  int getCurrentStep() const 
  {
    return currentStep;
  }
  /** which data point in a step are we editing */
  int getCurrentStepIndex() const
  {
    return currentStepIndex;
  }

  private:
    Sequencer* sequencer; 
    /** which sequence*/
    int currentSequence;
    /** which step */
    int currentStep;
    /** which data point inside a step*/
    int currentStepIndex;
    
    SequencerEditorMode editMode;
    float stepIncrement;    
};







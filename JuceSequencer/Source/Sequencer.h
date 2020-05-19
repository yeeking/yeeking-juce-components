/**
 * Sequencer stores a set of sequences each of which stores a step
 * 
 */

#pragma once 

#include <vector>
#include <iostream>
#include <string>

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

  private:
    int totalLength;
    int currentLength;
    int currentStep;
    std::vector<Step> steps;

};

class Sequencer  {
    public:
      Sequencer()
      {
        for (auto i=0;i<4;i++)
        {
          sequences.push_back(Sequence{});
        }
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
        if (sequence >= sequences.size() || sequence < 0)
        {
          std::cout << "Sequence " << sequence << " out of range. use 0 - " << (sequences.size()-1) << std::endl;
          return false;
        } 
        if (!sequences[sequence].assertStep(step))
        {
          //std::cout << "Step " << step << " out of range. Use 0 - " << (sequences[sequence].size() - 1) << std::endl;
          return false;
        }
        return true;
      }

    /// class data members  
      std::vector<Sequence> sequences;
};

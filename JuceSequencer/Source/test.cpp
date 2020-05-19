#include "Sequencer.h"
#include <iostream>

bool testTick()
{
  Sequencer seq{};
  seq.tick();
  seq.tick();
  return true;
}

bool testUpdate()
{
  Sequence seq{};
  seq.setStepData(0, std::vector<float>{0.1f});
  std::vector<float> data = seq.getStepData(0);
  if (data[0] == 0.1f) return true;
  return false;
}

bool testUpdate2()
{
  Sequence seq{};
  seq.setStepData(1, std::vector<float>{0.5f});
  std::vector<float> data = seq.getStepData(1);
  if (data[0] == 0.5f) return true;
  return false;
}

bool testToString()
{
  Sequencer seq;
  std::string s = seq.toString();
  //std::cout << s << std::endl;
  return true;
}

bool testTick2()
{
  // set a value and check we get it back after a tick
  Sequencer seqr;
  seqr.setStepData(0, 0, std::vector<float>{0.1f});
  seqr.setStepData(0, 1, std::vector<float>{0.2f});
  // pull data for track 0, current step
  std::vector<float> data = seqr.getCurrentStepData(0);
  if (data[0] != 0.1f) return false;
  seqr.tick();
  data = seqr.getCurrentStepData(0);
  if (data[0] != 0.2f) return false;
  return true;
}

bool testCursorStart()
{
  Sequencer seqr{};
  SequencerEditor cursor{&seqr};
  // should be 0
  if (cursor.getCurrentSequence() == 0 && cursor.getCurrentStep() == 0)
  return true;
  else return false; 
}
bool testCursorRight()
{
  Sequencer seqr{};
  SequencerEditor cursor{&seqr};
  cursor.moveCursorRight();
  // should be at step 1
  if (cursor.getCurrentSequence() == 0 && cursor.getCurrentStep() == 1)
  return true;
  else return false; 
}
bool testCursorRightLimit()
{
  Sequencer seqr{};
  SequencerEditor cursor{&seqr};
  for (int i=0;i<1000;++i) cursor.moveCursorRight();
  // should be at step seqr.numbersteps
  if (cursor.getCurrentSequence() == 0 && cursor.getCurrentStep() == seqr.howManySteps(0)-1)
  return true;
  else return false; 
}

bool testCursorLeftLimit()
{
  Sequencer seqr{};
  SequencerEditor cursor{&seqr};
  cursor.moveCursorLeft();
  cursor.moveCursorLeft();
  cursor.moveCursorLeft();
  
  // should be at step 0
  if (cursor.getCurrentSequence() == 0 && cursor.getCurrentStep() == 0)
  return true;
  else return false; 
}

bool testCursorLeft()
{
  Sequencer seqr{};
  SequencerEditor cursor{&seqr};
  // 10 to the right then 1 to the left
  // should be at 9
  for (int i=0;i<10;i++) cursor.moveCursorRight();
  cursor.moveCursorLeft();
  // should be at step 1
  if (cursor.getCurrentSequence() == 0 && cursor.getCurrentStep() == 9)
  return true;
  else return false; 
}


void log(std::string test, bool res)
{
  std::string msg;
  if (res) msg = " passed.";
  if (!res) msg = " failed.";
  std::cout << test << msg << std::endl;
}

int main()
{
  // log("testTick", testTick());
  // log("testTick2", testTick2());
  // log("testUpdate", testUpdate());
  // log("testUpdate2", testUpdate2());
  // log("testToString", testToString());

  // log("testCursorStart", testCursorStart());
  // log("testCursorLeft", testCursorLeft());
  // log("testCursorLeftLimit", testCursorLeftLimit());
  // log("testCursorRight", testCursorRight());
  // log("testCursorRightLimit", testCursorRightLimit());
  
  SimpleClock clock;
  clock.start(1000);
  std::string s;
  std::cout << "Enter something " << std::endl;
  std::cin >> s;
  clock.stop();

}

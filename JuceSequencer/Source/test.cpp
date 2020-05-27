#include "Sequencer.h"
#include <iostream>
#include "RtMidiUtils.h"

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


bool testTwoLinesInit()
{
  Sequencer seqr{};
  SequencerEditor cursor{&seqr};
  std::string want = "1 -Ioooo\n2 -Ooooo";
  std::string got = SequencerViewer::toTextDisplay(2, 8, &seqr, &cursor);
  if (want != got) 
  {
    std::cout << "testTwoLinesInit:: Wanted \n" << want << " \n got \n" << got << std::endl;
    return false;
  }
  else return true;
}

bool testTwoLines16()
{
  Sequencer seqr{};
  SequencerEditor cursor{&seqr};
  std::string want = "1 -Ioooooooooooo\n2 -Ooooooooooooo";
  std::string got = SequencerViewer::toTextDisplay(2, 16, &seqr, &cursor);
  if (want != got) 
  {
    std::cout << "testTwoLinesInit:: Wanted \n" << want << " \n got \n" << got << std::endl;
    return false;
  }
  else return true;
}

bool testTwoLines16Tick()
{
  Sequencer seqr{};
  seqr.tick();
  SequencerEditor cursor{&seqr};
  std::string want = "1 -IOooooooooooo\n2 -oOooooooooooo";
  std::string got = SequencerViewer::toTextDisplay(2, 16, &seqr, &cursor);
  if (want != got) 
  {
    std::cout << "testTwoLinesInit:: Wanted \n" << want << " \n got \n" << got << std::endl;
    return false;
  }
  else return true;
}

bool testTwoLinesWrapSeq()
{
  Sequencer seqr{};
  // set seq lengths to 8
  // 
  // 9 ticks takes it to step 1 (index 2)
  SequencerEditor cursor{&seqr};
  seqr.setSequenceLength(0, 8);
  seqr.setSequenceLength(1, 8);
  
  for (int i=0;i<10;++i) seqr.tick();

  std::string want = "1 -IoOoo\n2 -ooOoo";

  std::string got = SequencerViewer::toTextDisplay(2, 8, &seqr, &cursor);
  if (want != got) 
  {
    std::cout << "testTwoLinesInit:: Wanted \n" << want << " \n got \n" << got << std::endl;
    return false;
  }
  else return true;
}

bool testFollowEditCursorRight()
{
  Sequencer seqr{};
  SequencerEditor cursor{&seqr};
  cursor.moveCursorRight();
  std::string want = "1  Ioooo\n2  ooooo";

  std::string got = SequencerViewer::toTextDisplay(2, 8, &seqr, &cursor);
  if (want != got) 
  {
    std::cout << "testTwoLinesInit:: Wanted \n" << want << " \n got \n" << got << std::endl;
    return false;
  }
  else return true;
}

bool testFollowEditCursorDown()
{
  Sequencer seqr{};
  SequencerEditor cursor{&seqr};
  cursor.moveCursorDown();
  std::string want = "2 -Ioooo\n3 -Ooooo";

  std::string got = SequencerViewer::toTextDisplay(2, 8, &seqr, &cursor);
  if (want != got) 
  {
    std::cout << "testTwoLinesInit:: Wanted \n" << want << " \n got \n" << got << std::endl;
    return false;
  }
  else return true;
}

bool testFollowEditCursorLeftLimit()
{
  Sequencer seqr{};
  SequencerEditor cursor{&seqr};
  cursor.moveCursorLeft();
  std::string want = "1 -Ioooo\n2 -Ooooo";

  std::string got = SequencerViewer::toTextDisplay(2, 8, &seqr, &cursor);
  if (want != got) 
  {
    std::cout << "testTwoLinesInit:: Wanted \n" << want << " \n got \n" << got << std::endl;
    return false;
  }
  else return true;
}

bool testFollowEditCursorRightLimit()
{
  Sequencer seqr{};
  SequencerEditor cursor{&seqr};
  for (int i=0; i < 20; ++i) cursor.moveCursorRight();
  std::string want = "1  I    \n2  o    ";

  std::string got = SequencerViewer::toTextDisplay(2, 8, &seqr, &cursor);
  if (want != got) 
  {
    std::cout << "testTwoLinesInit:: Wanted \n" << want << " \n got \n" << got << std::endl;
    return false;
  }
  else return true;
}

bool testFollowEditCursorRightLimitNearly()
{
  Sequencer seqr{4, 4};
  SequencerEditor cursor{&seqr};
  for (int i=0; i < 2; ++i) cursor.moveCursorRight();
  std::string want = "1  Io   \n2  oo   ";

  std::string got = SequencerViewer::toTextDisplay(2, 8, &seqr, &cursor);
  if (want != got) 
  {
    std::cout << "testTwoLinesInit:: Wanted \n" << want << " \n got \n" << got << std::endl;
    return false;
  }
  else return true;
}

bool testFollowEditCursorDownLimit()
{
  Sequencer seqr{4}; // 4 seqs
  SequencerEditor cursor{&seqr};
  for (int i=0; i<10; ++i) cursor.moveCursorDown();

  std::string want = "2 -Ioooo\n3 -Ooooo";

  std::string got = SequencerViewer::toTextDisplay(2, 8, &seqr, &cursor);
  if (want != got) 
  {
    std::cout << "testTwoLinesInit:: Wanted \n" << want << " \n got \n" << got << std::endl;
    return false;
  }
  else return true;
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
  // log("testTwoLinesInit", testTwoLinesInit());
  // log("testTwoLines16", testTwoLines16());
  // log("testTwoLines16Tick", testTwoLines16());
  // log("testTwoLinesWrapSeq", testTwoLinesWrapSeq());
//  log("testFollowEditCursorRight", testFollowEditCursorRight());
//  log("testFollowEditCursorDown", testFollowEditCursorDown());
  //log("testFollowEditCursorLeftLimit", testFollowEditCursorLeftLimit());
//log("testFollowEditCursorRightLimit", testFollowEditCursorRightLimit());
//log("testFollowEditCursorRightLimitNearly", testFollowEditCursorRightLimitNearly());

PiRtMidi rtmidi;
rtmidi.chooseMidiPort();
rtmidi.playNote(0, 64, 64, 500);
//rtmidi.playNote(0, 64, 64, 500);

//log("testFollowEditCursorDownLimit", testFollowEditCursorDownLimit());
  

}

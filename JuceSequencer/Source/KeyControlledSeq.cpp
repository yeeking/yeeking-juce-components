#include <functional>
#include <iostream>

#include "Sequencer.h"
#include "SimpleClock.h"
#include "KeyInput.h"
#include "PiRtMidi.h"

void redraw(Sequencer& seqr, SequencerEditor& seqEditor)
{
    std::string disp = SequencerViewer::toTextDisplay(2, 16, &seqr, &seqEditor);
    std::cout << disp << std::endl;
}
 
int main()
{
    Sequencer seqr{1}; // 1 channel
    SequencerEditor seqEditor{&seqr};
    SimpleClock clock{};
    
    PiRtMidi rtmidi;
    rtmidi.chooseMidiPort();
    

    // set up step callbacks
    for (int seq = 0; seq < seqr.howManySequences(); ++seq)
    {
        seqr.setSequenceCallback(seq, [&seq, &rtmidi](std::vector<float> data){
            rtmidi.playNote(0, 64, 64, 50);
            //std::cout << seq << " step " << std::endl;
        });
    }

    clock.setCallback([&seqr, &seqEditor](){
        seqr.tick();
        redraw(seqr, seqEditor);     
    });
    
    clock.start(100);    

    return KeyReader::startKeyListener([&seqr, &seqEditor](char key){
        //std::cout << key << std::endl;
        switch (key){
            case 'a':
                seqEditor.moveCursorLeft();
                break;
            case 'd':
                seqEditor.moveCursorRight();
                break;
            case 'w':
                seqEditor.moveCursorUp();
                break;
            case 's':
                seqEditor.moveCursorDown();
                break;
        }
        redraw(seqr, seqEditor);     
    });
}
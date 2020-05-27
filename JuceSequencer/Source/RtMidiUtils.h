#pragma once 
#include <iostream>
#include <cstdlib>
#include "/usr/include/rtmidi/RtMidi.h"

// Platform-dependent sleep routines.
#if defined(WIN32)
  #include <windows.h>
  #define SLEEP( milliseconds ) Sleep( (DWORD) milliseconds ) 
#else // Unix variants
  #include <unistd.h>
  #define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif

class PiRtMidi
{
    public:
        PiRtMidi()
        {

        }
        ~PiRtMidi()
        {
            delete midiout;
        }

        bool chooseMidiPort()
        {
            try {
                midiout = new RtMidiOut();
            }
            catch ( RtMidiError &error ) {
                error.printMessage();
                exit( EXIT_FAILURE );
            }

            std::string portName;
            unsigned int i = 0, nPorts = midiout->getPortCount();
            if ( nPorts == 0 ) {
                std::cout << "No output ports available!" << std::endl;
                return false;
            }
            if ( nPorts == 1 ) {
                std::cout << "\nOpening " << midiout->getPortName() << std::endl;
            }
            else {
                for ( i=0; i<nPorts; i++ ) {
                    portName = midiout->getPortName(i);
                    std::cout << "  Output port #" << i << ": " << portName << '\n';
                }
                do {
                    std::cout << "\nChoose a port number: ";
                    std::cin >> i;
                } while ( i >= nPorts );
            }
            std::cout << "\n";
            midiout->openPort( i );
            return true;
        }

        void playNote(char channel, char note, char vel, int lengthMs)
        {
            //https://www.wikiaudio.org/midi-channel-messages-tutorial/
            std::vector<unsigned char> message;
            message.push_back(144 + channel); // 1001cccc
            message.push_back(note); 
            message.push_back(vel);
            midiout->sendMessage( &message );
            SLEEP(lengthMs);
            // Note Off: 128, 64, 40
            message[0] = 128 + channel;  // 1000cccc
            message[1] = note;
            message[2] = 0;
            midiout->sendMessage( &message );
        }

    private:
        RtMidiOut *midiout = 0;
};


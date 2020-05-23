#include <functional>
#include <iostream>

#include "SimpleClock.h"
#include "Sequencer.h"
#include "grovepi.h"
#include "grove_rgb_lcd.h"
#include "GroveUtils.h"

/** utilities to make use of the Grove LCD RGB widget */
// class GroveLCD{
//     public:

// };



int main()
{
    Sequencer seqr;
    SequencerEditor seqEditor{&seqr};
    SimpleClock clock{};
    GrovePi::LCD lcd{};

    clock.setCallback([&seqr, &lcd, &seqEditor](){
        seqr.tick();
            std::string disp = SequencerViewer::toTextDisplay(2, 16, &seqr, &seqEditor);
            std::cout << disp << std::endl;
            lcd.setText(disp.c_str());
        });
    clock.start(1000);
    
    try
	{
		// connect to the i2c-line
		lcd.connect();
		// set text and RGB color on the LCD
		lcd.setText("Loading sequencer....");
		lcd.setRGB(255, 0, 0);
    }
    catch(GrovePi::I2CError &error)
	{
		printf(error.detail());
		return -1;
	}
    
    GroveJoystick joy([&seqEditor, &seqr, &lcd](JoystickEvent event){
        switch (event)
        {
            case JoystickEvent::up:
                //std::cout << "Received jpystick event up" << std::endl;
                seqEditor.moveCursorUp();
                break;
            case JoystickEvent::down:
                //std::cout << "Received jpystick event down" << std::endl;
                seqEditor.moveCursorDown();
                break;
            case JoystickEvent::left:
                //std::cout << "Received jpystick event left" << std::endl;
                seqEditor.moveCursorLeft();              
                break;
            case JoystickEvent::right:
                //std::cout << "Received jpystick event right " << std::endl;
                seqEditor.moveCursorRight();
                break;
            case JoystickEvent::click:
                std::cout << "Received jpystick event click " << std::endl;
                break;   
        }
        // now update the display
        std::string disp = SequencerViewer::toTextDisplay(2, 16, &seqr, &seqEditor);
        std::cout << disp << std::endl;
        lcd.setText(disp.c_str());		
    });

    int x;
    std::cin >> x;
}
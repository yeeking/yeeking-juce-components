#include <functional>
#include <iostream>

#include "Sequencer.h"
#include "grovepi.h"
#include "grove_rgb_lcd.h"
/** utilities to make use of the Grove LCD RGB widget */
// class GroveLCD{
//     public:

// };

/** used to transmit joystick states*/
enum class JoystickEvent{up, down, left, right, click};
/**
 *  Talks to the Grove Joystick widget 
 * https://wiki.seeedstudio.com/Grove-Thumb_Joystick/
 * Based on https://dexterind.github.io/GrovePi
 */
class GroveJoystick
{
    public:
    GroveJoystick(std::function<void(JoystickEvent)> callback = [](JoystickEvent e){printf("%d", e);}, int _pinX=0, int _pinY=1) : callback{callback}, pinX{_pinX}, pinY{_pinY}   
    {
        try
        {
            GrovePi::initGrovePi(); 
            GrovePi::pinMode(pinX, GrovePi::INPUT);
            GrovePi::pinMode(pinY, GrovePi::INPUT);
            poller.start(10);
        }catch (GrovePi::I2CError &error)
        {
            std::cout << "GroveJoystick::GroveJoystick grovepi issue" << std::endl;
        }
    }
    /** call this to read an event from the joystick*/
    void pollJoystick()
    {
        inX = GrovePi::analogRead(pinX);
        inY = GrovePi::analogRead(pinY);
        dX = abs(inX - lastX);
        dY = abs(inY - lastY);
        lastX = inX;
        lastY = inY;

        if (inX == 1023)
        {            
            callback(JoystickEvent::click);
            return;
        }
        if (dX > 10)
        {
            // send an x change message
            if (inX < 400) 
            {
                ///std::cout << "left  d: " << dX << " in: " << inX << " last " << lastX << std::endl;
                callback(JoystickEvent::left);
            }
            else if (inX > 520) 
            {
                //std::cout << "right  d: " << dX << " in: " << inX << " last " << lastX << std::endl;
                callback(JoystickEvent::right);
            }
            return;
        }
        if (dY > 10)
        {
            // send an x change message
            if (inY < 400) 
            {
                //std::cout << "down  d: " << dY << " in: " << inY << " last " << lastY << std::endl;
                callback(JoystickEvent::down);
            }
            else if (inY > 520) 
            {
                //std::cout << "up  d: " << dY << " in: " << inY << " last " << lastY << std::endl;
                callback(JoystickEvent::up);
            }
            return;
        }
    }
    
    private:
        std::function<void(JoystickEvent)> callback;
        SimpleClock poller{10, [this](){
            this->pollJoystick();
        }}; // poll every 10 ms
        int pinX; 
        int pinY;
        int inX;
        int inY;
        int lastX;
        int lastY;
        int dX;
        int dY;
};

class PiSeq 
{
    public:
    PiSeq()
    {

    }
    ~PiSeq()
    {
        clock.stop();
    }
    private:
        Sequencer seqr;
        SimpleClock clock;
};


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
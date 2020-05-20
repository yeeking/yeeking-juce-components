#include <functional>
#include <iostream>

#include "Sequencer.h"
#include "grovepi.h"

// class for converting getting shit from the joystick on A0

enum class JoystickEvent{up, down, left, right, click};



class GroveJoystick
{
    public:
    GroveJoystick(std::function<void(JoystickEvent)> callback) : callback{callback}, pinX{0}, pinY{1}   
    {
        try
        {
            GrovePi::initGrovePi(); 
            GrovePi::pinMode(0, GrovePi::INPUT);
            GrovePi::pinMode(1, GrovePi::INPUT);
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
    GroveJoystick joy([](JoystickEvent event){
        switch (event)
        {
            case JoystickEvent::up:
                std::cout << "Received jpystick event up" << std::endl;
                break;
            case JoystickEvent::down:
                std::cout << "Received jpystick event down" << std::endl;
                break;
            case JoystickEvent::left:
                std::cout << "Received jpystick event left" << std::endl;
                break;
            case JoystickEvent::right:
                std::cout << "Received jpystick event right " << std::endl;
                break;
            case JoystickEvent::click:
                std::cout << "Received jpystick event click " << std::endl;
                break;
            
        }
    });
    int x;
    std::cin >> x;
}
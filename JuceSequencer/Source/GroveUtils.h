#pragma once

/** used to transmit joystick states*/
enum class JoystickEvent{up, down, left, right, click};
/**
 *  Talks to the Grove Joystick widget 
 * https://wiki.seeedstudio.com/Grove-Thumb_Joystick/
 * Based on https://dexterind.github.io/GrovePi
 */

#include "Sequencer.h"
#include "grovepi.h
#include "grove_rgb_lcd.h"

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


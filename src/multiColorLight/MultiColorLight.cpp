#include "MultiColorLight.h"

#include <logger/Logger.h>

MultiColorLight::MultiColorLight():rgbLeds(ledAmount,ledPin){

};

void MultiColorLight::begin(void){
    rgbLeds.begin();
    this->turnOffLed();

    Logger::getInstance().logTrace("Successfully started MultiColorLight module");
};

void MultiColorLight::setLed(uint8_t index , uint32_t color){
    if (index > ledAmount-1){
        Logger::getInstance().logError("MultiColorLight index out of range");
    }
    rgbLeds.setPixelColor(index, normalizeColor(color));
    rgbLeds.show();
};


void MultiColorLight::setLed(leds leds, uint32_t color){
    Logger::getInstance().logInfo(
        "Setting LED "
        + std::to_string(leds)
        + " to color value: "
        + std::to_string(color)
    );
    switch (leds){
        case TOP_LEFT:
            MultiColorLight::setLed(1,color);break;
        case TOP_RIGHT:
            MultiColorLight::setLed(0,color);break;
        case BOTTOM:
            MultiColorLight::setLed(2,color);break;
        case TOP:
            for (int index = 0; index<2; index++){
                MultiColorLight::setLed(index,color);
            }break;
        case ALL:
            for (int index = 0; index<ledAmount; index++){
                MultiColorLight::setLed(index,color);
            }break;
        default:
            Logger::getInstance().logWarning(
                "MultiColorLight.setLed() could not set color to LED as the provided LED is unknown"
            );
            break;
    }

};

void MultiColorLight::setLed(leds leds, uint8_t red, uint8_t green, uint8_t blue){
    Logger::getInstance().logInfo(
        "Setting LED "
        + std::to_string(leds)
        + "to RGB value: ("
        + std::to_string(red)
        + ", "
        + std::to_string(green)
        + ", "
        + std::to_string(blue)
    );
    MultiColorLight::setLed(leds, MultiColorLight::color(red,green,blue));
};


void MultiColorLight::setTopLeds(uint32_t color){
    Logger::getInstance().logInfo(
        "Setting Top LED to color value: "
        + std::to_string(color)
    );
    MultiColorLight::setLed(TOP,color);
}; 

void MultiColorLight::setTopLeds(uint8_t red, uint8_t green, uint8_t blue){
    Logger::getInstance().logInfo(
        "Setting Top LED to RGB value: ("
        + std::to_string(red)
        + ", "
        + std::to_string(green)
        + ", "
        + std::to_string(blue)
    );
    MultiColorLight::setTopLeds(MultiColorLight::color(red,green,blue));
}; 

void MultiColorLight::blink(uint16_t amount,uint32_t color, leds leds, uint32_t interval){
    Logger::getInstance().logInfo(
        "Set blinking with values: Amount: "
        + std::to_string(amount)
        + " Color: "
        + std::to_string(color)
        + " LED: "
        + std::to_string(leds)
        + " Interval: "
        + std::to_string(interval)
    );
    for(uint16_t index = 0; index < amount;index++){
        MultiColorLight::setLed(leds, color);
        vTaskDelay(interval);
        MultiColorLight::turnOffLed(leds);
        vTaskDelay(interval);
    }
};

void MultiColorLight::turnOffLed(leds leds){
    switch (leds){
        case TOP_LEFT:
            MultiColorLight::setLed(1,0);break;
        case TOP_RIGHT:
            MultiColorLight::setLed(0,0);break;
        case BOTTOM:
            MultiColorLight::setLed(2,0);break;
        case TOP:
            for (int index = 0; index<2; index++){
                MultiColorLight::setLed(index,0);
            }break;
        case ALL:
            for (int index = 0; index<3; index++){
                MultiColorLight::setLed(index,0);
            }break;
        default:
            Logger::getInstance().logError("MultiColorLight.turnOffLed() invalid leds");
            break;
    }
};

uint32_t MultiColorLight::color(uint8_t r, uint8_t g, uint8_t b){
    return rgbLeds.Color(r,g,b);
};

//PRIVATE
uint32_t MultiColorLight::normalizeColor(uint32_t color,uint8_t maxBrightness){
    uint8_t red = (color&0x00FF0000)>>16;
    uint8_t green = (color&0x0000FF00)>>8;
    uint8_t blue = (color&0x000000FF);
    if (red > maxBrightness){
        red = maxBrightness;
    }
    if(green > maxBrightness-70){
        green = maxBrightness-70;
    }
    if(blue > maxBrightness-50){
        blue = maxBrightness-50;
    }
    return MultiColorLight::color(red,green,blue);
}
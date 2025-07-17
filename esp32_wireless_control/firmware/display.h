#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <Arduino.h>

class Display
{
  public:
    Display();

    virtual void init() = 0;

    void begin();

    virtual void updateDisplay() = 0;

    const char* getCurrentStatusMessage();

  private:
    static void displayTask(void* pvParameters);
};

extern Display* display;

#endif /* _DISPLAY_H_ */

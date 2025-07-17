#ifndef _SIMPLE_DISPLAY_H_
#define _SIMPLE_DISPLAY_H_

#include "display.h"

class SimpleDisplay : public Display
{
  public:
    SimpleDisplay(int sda, int scl);
    ~SimpleDisplay()
    {
    }
    void init();

    void updateDisplay() override;

  private:
    int _sda;
    int _scl;
};

#endif /* _SIMPLE_DISPLAY_H_ */

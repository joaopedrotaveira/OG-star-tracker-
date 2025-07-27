#ifndef _SIMPLE_DISPLAY_H_
#define _SIMPLE_DISPLAY_H_

#include "display.h"
#include <ItemValue.h>

class MenuDisplay : public Display
{
  public:
    MenuDisplay(uint8_t columns, uint8_t rows, uint8_t sda, uint8_t scl, uint8_t rot_a,
                uint8_t rot_b, uint8_t rot_sw);
    ~MenuDisplay()
    {
    }
    void init();

    void updateDisplay() override;

    //  private:
    uint8_t _columns;
    uint8_t _rows;
    uint8_t _sda;
    uint8_t _scl;
    uint8_t _rot_a;
    uint8_t _rot_b;
    uint8_t _rot_sw;
    LiquidCrystal_I2C _lcd;
    CharacterDisplayRenderer _renderer;
    LcdMenu _menu;
    SimpleRotary _encoder;
    SimpleRotaryAdapter _rotaryInput;

    static void encoderTask(void* pvParameters);
};

template <typename T> class ItemValueLabel : public ItemValue<T>
{
  public:
    ItemValueLabel(const char* text, T& value, const char* format)
        : ItemValue<T>(text, value, format)
    {
    }

    bool isSelectable() const override
    {
        return false;
    }
};

template <typename T>
inline MenuItem* ITEM_VALUE_LABEL(const char* text, T& value, const char* format = "%s")
{
    return new ItemValueLabel<T>(text, value, format);
}
#endif /* _SIMPLE_DISPLAY_H_ */

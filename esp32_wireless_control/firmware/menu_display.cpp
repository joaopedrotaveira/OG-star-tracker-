#include "config.h"

#if HAVE_DISPLAY
#include <ESP32Time.h>
#include <ItemBack.h>
#include <ItemCommand.h>
#include <ItemLabel.h>
#include <ItemValue.h>
#include <ItemBool.h>
#include <LcdMenu.h>
#include <LiquidCrystal_I2C.h>
#include <MenuScreen.h>
#include <SimpleRotary.h>
#include <display/LiquidCrystal_I2CAdapter.h>
#include <input/SimpleRotaryAdapter.h>
#include <inttypes.h>
#include <renderer/CharacterDisplayRenderer.h>

#include "axis.h"
#include "menu_display.h"
#include "uart.h"
#include "utils/sideral_functions.h"

//Display* display = new MenuDisplay(LCD_COLUMNS, LCD_ROWS, SDA_PIN, SCL_PIN, ENCODER_B_PIN,
//                                   ENCODER_A_PIN, BUTTON_PIN);

MENU_SCREEN(mainScreen, mainItems,
        ITEM_COMMAND("Status", []() { ((MenuDisplay*) display)->_menu.hide(); }),
        ITEM_BOOL("Tracking", ra_axis.trackingActive, "On", "Off", [](const bool value) {
             Serial.println(value ? "Tracking On" : "Tracking Off");
        }),
        ITEM_BASIC("Mode"),
        ITEM_BASIC("Capture"));

char line2[LCD_COLUMNS + 1];

MenuDisplay::MenuDisplay(uint8_t columns, uint8_t rows, uint8_t sda, uint8_t scl, uint8_t rot_a,
                         uint8_t rot_b, uint8_t rot_sw)
    : _columns(columns), _rows(rows), _sda(sda), _scl(scl), _rot_a(rot_a), _rot_b(rot_b),
      _rot_sw(rot_sw), _lcd(0x27, _columns, _rows),
      _renderer(new LiquidCrystal_I2CAdapter(&_lcd), _columns, _rows), _menu(_renderer),
      _encoder(_rot_a, _rot_b, _rot_sw), _rotaryInput(&_menu, &_encoder)
{
}

void MenuDisplay::init()
{
//    std::vector<MenuItem*> mainItems = {
//        (new MenuItem("Option 1")),
//        (new MenuItem("Placeholder 2")),
//        (new MenuItem("Placeholder 3")),
//        (new MenuItem("Placeholder 4")),
//        ITEM_COMMAND("Item 1", []() { ((MenuDisplay*) display)->_menu.hide(); })};
//    MenuScreen* mainScreen = new MenuScreen(mainItems);
    _renderer.begin();
    _menu.setScreen(mainScreen);
    _menu.hide();

    if (xTaskCreate(encoderTask, "encoder", 4096, this, 1, NULL))
        print_out("started encoderTask");
}

void MenuDisplay::updateDisplay()
{
    if (!_menu.isEnabled())
    {
        ESP32Time esp32_time;
        struct timeval tv;
        gettimeofday(&tv, NULL);
        tm time = esp32_time.getTimeStruct();

        static Moving_Average<double, double, 5> ma;

        _lcd.setCursor(0, 0);
        for (int i = 0; i < LCD_COLUMNS; i++)
        {
            line2[i] = ' ';
        }
        line2[LCD_COLUMNS] = '\0';
        _lcd.print(line2);
        _lcd.setCursor(0, 0);
        snprintf(line2, LCD_COLUMNS + 1, "%s", getCurrentStatusMessage());
        _lcd.print(line2);

        int64_t position = ra_axis.getPosition();
        _lcd.setCursor(0, 1);

        int64_t position2 = position % ((int64_t) (24 * 60 * 60 * STEPS_PER_SECOND_256MICROSTEP));
        position2 = (position2 > 0) ? position2
                                    : position2 + (24 * 60 * 60 * STEPS_PER_SECOND_256MICROSTEP);

        position = ra_axis.getPosition();
        position2 = position % ((int64_t) (24 * 60 * 60 * STEPS_PER_SECOND_256MICROSTEP));
        position2 = (position2 > 0) ? position2
                                    : position2 + (24 * 60 * 60 * STEPS_PER_SECOND_256MICROSTEP);

        double ut = universal_time(time.tm_hour, time.tm_min, time.tm_sec + tv.tv_usec / 1000000.0);
        double jd = julian_day(time.tm_year + 1900, time.tm_mon, time.tm_mday, time.tm_hour,
                               time.tm_min, time.tm_sec + tv.tv_usec / 1000000.0);
        double lmst = local_mean_sidereal_time(jd, -9.07564194);
        double ha = position2 / (60.0 * 60.0 * STEPS_PER_SECOND_256MICROSTEP);
        double ra = haToRa(ha, lmst);

        double ma_ra = ma(ra);

        snprintf(line2, LCD_COLUMNS + 1, "RA: %s",
                 Hours(ma_ra).toHourMinuteSecond().toString().c_str());
        _lcd.print(line2);

#if LCD_ROWS > 2
        //    static bool line2Cleared = false;
        //    if (intervalometer.intervalometerActive)
        //    {
        //        lcd.setCursor(0, 2);
        //        uint16_t exposures = intervalometer.currentSettings.exposures;
        //        uint16_t currentExposure = intervalometer.getCurrentExposure();
        //        uint16_t exposuresTaken = intervalometer.getExposuresTaken();
        //        snprintf(line, LCD_COLUMNS + 1, "Exposures: %" PRIu16 "/%" PRIu16 "/%" PRIu16,
        //                 exposuresTaken, currentExposure, exposures);
        //        lcd.print(line);
        //
        //        line2Cleared = false;
        //    }
        //    else if (!line2Cleared)
        //    {
        //        lcd.setCursor(0, 2);
        //        lcd.print("                    ");
        //        line2Cleared = true;
        //    }
        _lcd.setCursor(0, 2);
        //    snprintf(line, LCD_COLUMNS + 1, "%s", time.getTime("%Y-%m-%dT%H:%M:%S").c_str());
        //    lcd.print(line);
        snprintf(line2, LCD_COLUMNS + 1, "HA: %s",
                 Hours(ha).toHourMinuteSecond().toString().c_str());
        _lcd.print(line2);

#endif
#if LCD_ROWS > 3
        //  static bool line3Cleared = false;
        //  if(intervalometer.intervalometerActive)
        //  {
        //      lcd.setCursor(0, 3);
        //      TickType_t currentTicks = xTaskGetTickCount();
        //      TickType_t startCaptureTickCount = intervalometer.getStartCaptureTickCount();
        //      TickType_t captureDurationTickCount =
        //            intervalometer.getCaptureDurationTickCount();
        //
        //      snprintf(line, LCD_COLUMNS+1, "Time: %.1f/%.1f",
        //              pdTICKS_TO_MS(currentTicks - startCaptureTickCount)/1000.0,
        //              pdTICKS_TO_MS(captureDurationTickCount)/1000.0);
        //      lcd.print(line);
        //
        //      line3Cleared = false;
        //  } else if(!line3Cleared){
        //      lcd.setCursor(0, 3);
        //      lcd.print("                    ");
        //      line3Cleared = true;
        //  }
        //    lcd.setCursor(0, 3);
        //    snprintf(line, LCD_COLUMNS + 1, "%10lld", ra_axis.rate.tracking);
        //    lcd.print(line);

        _lcd.setCursor(0, 3);
        snprintf(
            line2, LCD_COLUMNS + 1, "UT: %s",
            Hours(universal_time(time.tm_hour, time.tm_min, time.tm_sec + tv.tv_usec / 1000000.0))
                .toHourMinuteSecond()
                .toString()
                .c_str());
        _lcd.print(line2);

        print_out_nonl("ut: %f jd: %f lmst: %f ha: %f ra: %f %s ma_ra: %f %s\n", ut, jd, lmst, ha,
                       ra, Hours(ra).toHourMinuteSecond().toString().c_str(), ma_ra,
                       Hours(ma_ra).toHourMinuteSecond().toString().c_str());

#endif
    }
}

void MenuDisplay::encoderTask(void* pvParameters)
{
    MenuDisplay* display = (MenuDisplay*) pvParameters;
    for (;;)
    {
        if (display->_menu.isEnabled())
        {
            display->_rotaryInput.observe();
        }
        else
        {
            uint8_t pressType = display->_encoder.pushType(LONG_PRESS_DURATION);
            if (pressType == 1)
            {
                display->_menu.show();
                display->_renderer.setEditMode(false);
            }
        }
        vTaskDelay(1);
    }
}

#endif /* HAVE_DISPLAY */

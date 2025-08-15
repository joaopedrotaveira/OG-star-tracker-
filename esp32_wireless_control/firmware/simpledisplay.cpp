#include "config.h"

#if HAVE_DISPLAY
#include <ESP32Time.h>
#include <LiquidCrystal_I2C.h>
#include <inttypes.h>

#include "axis.h"
#include "simpledisplay.h"
#include "uart.h"
#include "utils/sideral_functions.h"

 Display* display = new SimpleDisplay(SDA_PIN, SCL_PIN);

LiquidCrystal_I2C lcd(0x27, LCD_COLUMNS, LCD_ROWS);
char line[LCD_COLUMNS + 1];

SimpleDisplay::SimpleDisplay(int sda, int scl) : _sda(sda), _scl(scl)
{
}

void SimpleDisplay::init()
{
    Wire.end();
    Wire.begin(_sda, _scl); // define SDA and SCL pins
    Wire.setClock(400000UL);
    lcd.init();
    lcd.clear();
    lcd.setBacklight(1); // Switch backlight LED on
}

void SimpleDisplay::updateDisplay()
{
    ESP32Time esp32_time;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    tm time = esp32_time.getTimeStruct();

    static Moving_Average<double, double, 5> ma;

    lcd.setCursor(0, 0);
    for (int i = 0; i < LCD_COLUMNS; i++)
    {
        line[i] = ' ';
    }
    line[LCD_COLUMNS] = '\0';
    lcd.print(line);
    lcd.setCursor(0, 0);
    snprintf(line, LCD_COLUMNS + 1, "%s", getCurrentStatusMessage());
    lcd.print(line);

    int64_t position = ra_axis.getPosition();
    lcd.setCursor(0, 1);

    int64_t position2 = position % ((int64_t) (24 * 60 * 60 * STEPS_PER_SECOND_256MICROSTEP));
    position2 =
        (position2 > 0) ? position2 : position2 + (24 * 60 * 60 * STEPS_PER_SECOND_256MICROSTEP);

    //    int seconds = position2 / STEPS_PER_SECOND_256MICROSTEP;
    //    int milisec = (1000 / STEPS_PER_SECOND_256MICROSTEP) *
    //                  (position2 - STEPS_PER_SECOND_256MICROSTEP * seconds);
    //    int sec = seconds % 60;
    //    int min = (seconds / 60) % 60;
    //    int hour = seconds / 3600;
    //    //    snprintf(line, LCD_COLUMNS+1, "RA:%s%02d %02d' %02d.%03d\"", seconds < 0 ? "-":" " ,
    //    // abs(hour), abs(min), abs(sec), abs(milisec));
    //    snprintf(line, LCD_COLUMNS + 1, "%02d %02d' %02d.%03d\"", abs(hour), abs(min), abs(sec),
    //             abs(milisec));
    //    lcd.print(line);

    position = ra_axis.getPosition();
    position2 = position % ((int64_t) (24 * 60 * 60 * STEPS_PER_SECOND_256MICROSTEP));
    position2 =
        (position2 > 0) ? position2 : position2 + (24 * 60 * 60 * STEPS_PER_SECOND_256MICROSTEP);

    double ut = universal_time(time.tm_hour, time.tm_min, time.tm_sec + tv.tv_usec / 1000000.0);
    double jd = julian_day(time.tm_year + 1900, time.tm_mon, time.tm_mday, time.tm_hour,
                           time.tm_min, time.tm_sec + tv.tv_usec / 1000000.0);
    double lmst = local_mean_sidereal_time(jd, -9.07564194);
    double ha = position2 / (60.0 * 60.0 * STEPS_PER_SECOND_256MICROSTEP);
    double ra = haToRa(ha, lmst);

    double ma_ra = ma(ra);

    snprintf(line, LCD_COLUMNS + 1, "RA: %s", Hours(ma_ra).toHourMinuteSecond().toString().c_str());
    lcd.print(line);

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
    lcd.setCursor(0, 2);
    //    snprintf(line, LCD_COLUMNS + 1, "%s", time.getTime("%Y-%m-%dT%H:%M:%S").c_str());
    //    lcd.print(line);
    snprintf(line, LCD_COLUMNS + 1, "HA: %s", Hours(ha).toHourMinuteSecond().toString().c_str());
    lcd.print(line);

#endif
#if LCD_ROWS > 3
    //  static bool line3Cleared = false;
    //  if(intervalometer.intervalometerActive)
    //  {
    //      lcd.setCursor(0, 3);
    //      TickType_t currentTicks = xTaskGetTickCount();
    //      TickType_t startCaptureTickCount = intervalometer.getStartCaptureTickCount();
    //      TickType_t captureDurationTickCount = intervalometer.getCaptureDurationTickCount();
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

    lcd.setCursor(0, 3);
    snprintf(line, LCD_COLUMNS + 1, "UT: %s",
             Hours(universal_time(time.tm_hour, time.tm_min, time.tm_sec + tv.tv_usec / 1000000.0))
                 .toHourMinuteSecond()
                 .toString()
                 .c_str());
    lcd.print(line);

    print_out_nonl("ut: %f jd: %f lmst: %f ha: %f ra: %f %s ma_ra: %f %s\n", ut, jd, lmst, ha, ra,
                   Hours(ra).toHourMinuteSecond().toString().c_str(), ma_ra,
                   Hours(ma_ra).toHourMinuteSecond().toString().c_str());

#endif
}

#endif /* HAVE_DISPLAY */

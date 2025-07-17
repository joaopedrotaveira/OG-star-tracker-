#include "config.h"

#if HAVE_DISPLAY
#include <ESP32Time.h>
#include <inttypes.h>

#include "axis.h"
#include "display.h"
#include "intervalometer.h"
#include "uart.h"
#include "utils/sideral_functions.h"
#include "web_languages.h"

extern Languages language;

Display::Display()
{
}

void Display::begin()
{
    init();
    if (xTaskCreate(displayTask, "display", 4096, this, 1, NULL))
        print_out("started displayTask");
}

const char* Display::getCurrentStatusMessage()
{
    if (intervalometer.intervalometerActive)
    {
        switch (intervalometer.currentState)
        {
            case Intervalometer::PRE_DELAY:
                return languageMessageStrings[language][MSG_CAP_PREDELAY];
                break;
            case Intervalometer::CAPTURE:
                return languageMessageStrings[language][MSG_CAP_EXPOSING];
                break;
            case Intervalometer::DITHER:
                return languageMessageStrings[language][MSG_CAP_DITHER];
                break;
            case Intervalometer::PAN:
                return languageMessageStrings[language][MSG_CAP_PANNING];
                break;
            case Intervalometer::DELAY:
                return languageMessageStrings[language][MSG_CAP_DELAY];
                break;
            case Intervalometer::REWIND:
                return languageMessageStrings[language][MSG_CAP_REWIND];
                break;
            case Intervalometer::INACTIVE:
            default:
                break;
        }
    }
    else if (ra_axis.slewActive && !ra_axis.goToTarget)
    {
        return languageMessageStrings[language][MSG_SLEWING];
    }
    else if (ra_axis.slewActive && ra_axis.goToTarget)
    {
        return languageMessageStrings[language][MSG_GOTO_RA_PANNING_ON];
    }
    else if (ra_axis.trackingActive)
    {
        return languageMessageStrings[language][MSG_TRACKING_ON];
    }
    else
    {
        if (intervalometer.currentErrorMessage == ErrorMessage::ERR_MSG_NONE)
            return languageMessageStrings[language][MSG_IDLE];
        else
            return languageErrorMessageStrings[language][intervalometer.currentErrorMessage];
    }
    return languageMessageStrings[language][MSG_IDLE];
}

void Display::displayTask(void* pvParameters)
{
    Display* disp = (Display*) pvParameters;
    for (;;)
    {
        vTaskDelay(500 * portTICK_PERIOD_MS);
        disp->updateDisplay();
    }
}

#endif /* HAVE_DISPLAY */

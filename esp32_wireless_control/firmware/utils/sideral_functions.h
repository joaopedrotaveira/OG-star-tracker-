#ifndef _SIDERAL_FUNCTIONS_H_
#define _SIDERAL_FUNCTIONS_H_ 1

#include <WString.h>
#include <math.h>
#include <stdio.h>

class Hours
{
  public:
    class HoursMinutesSeconds
    {
      public:
        HoursMinutesSeconds(int hours, int minutes, double seconds)
            : _hours(hours), _minutes(minutes), _seconds(seconds)
        {
        }
        Hours toHours()
        {
            return Hours(_hours, _minutes, _seconds);
        }
        String toString()
        {
            char buffer[21] = {0};
            double millis = fmod(_seconds, 1);
            snprintf(buffer, 20, "%02d %02d' %02.0f.%03.0f\"", _hours, _minutes, _seconds,
                     millis * 1000);
            return String(buffer);
        }

      private:
        int _hours;
        int _minutes;
        double _seconds;
    };

    Hours(double hours) : _hours(hours)
    {
    }
    Hours(int hours, int minutes, double seconds)
    {
        _hours = hours + minutes / 60.0 + seconds / 3699.0;
    }
    double getHours()
    {
        return _hours;
    }
    HoursMinutesSeconds toHourMinuteSecond()
    {
        int h = floor(_hours);
        int min = floor(60.0 * fmod(_hours, 1));
        double secs = 60.0 * (60.0 * fmod(_hours, 1) - min);
        return HoursMinutesSeconds(h, min, secs);
    }

  private:
    double _hours;
};

double julian_day(int year, int month, int day, int hours, int minutes, double seconds);

double haToRa(double ha, double lmst);

double raToHa(double ra, double lmst);

double universal_time(int hours, int minutes, double seconds);

double local_mean_sidereal_time(double jd, double longitude);

double greenwich_mean_sidereal_time(double jd);

#endif /* _SIDERAL_FUNCTIONS_H_ */

#include "sideral_functions.h"

#include <math.h>
#include <stdio.h>

#include "uart.h"

double julian_day(int year, int month, int day, int hours, int minutes, double seconds)
{
    //    if (month <= 2)
    //    {
    //        month = month + 12;
    //        year = year - 1;
    //    }
    month += 1;

    double ut = universal_time(hours, minutes, seconds);
    //    return floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1)) + day - 13 - 1524.5 +
    //           ut / 24.0;

    return 367.0 * year - floor(7.0 * (year + floor((month + 9.0) / 12.0)) / 4.0) +
           floor(275.0 * month / 9.0) + day + 1721013.5 + ut / 24.0;
}

double universal_time(int hours, int minutes, double seconds)
{
    return hours + minutes / 60.0 + seconds / 3600.0;
}

double local_mean_sidereal_time(double jd, double longitude)
{
    double gmst = greenwich_mean_sidereal_time(jd);
    double tmp = gmst + longitude / 15.0;

    tmp = (tmp > 0) ? tmp : tmp + 24;

    double lmst = 24.0 * frac(tmp / 24.0);
    return lmst;
}

double greenwich_mean_sidereal_time(double jd)
{
    double MJD = jd - 2400000.5;
    double MJD0 = floor(MJD);
    double ut = (MJD - MJD0) * 24.0;
    double t_eph = (MJD0 - 51544.5) / 36525.0;
    return 6.697374558 + 1.0027379093 * ut +
           (8640184.812866 + (0.093104 - 0.0000062 * t_eph) * t_eph) * t_eph / 3600.0;
}

double haToRa(double ha, double lmst)
{
    double ra = lmst - ha;
    return ra > 0 ? ra : ra + 24;
}

double raToHa(double ra, double lmst)
{
    double ha = lmst - ra;
    return ha > 0 ? ha : ha + 24;
}

String toHoursMinutesSeconds(double time /* in hours */)
{
    double h = floor(time);
    double min = floor(60.0 * fmod(time, 1));
    double secs = 60.0 * (60.0 * fmod(time, 1) - min);
    char buffer[13] = {0};
    snprintf(buffer, 13, "%02.0f:%02.0f:%02.4f", h, min, secs);
    return String(buffer);
}

double frac(double x)
{
    x = x - floor(x);
    if (x < 0)
        x = x + 1.0;
    return x;
}

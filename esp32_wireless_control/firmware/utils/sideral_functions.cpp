#include "sideral_functions.h"

#include <math.h>
#include <stdio.h>

#include "uart.h"

double julian_day(int year, int month, int day, int hours, int minutes, double seconds)
{
    month += 1;

    double ut = universal_time(hours, minutes, seconds);
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

    double lmst = 24.0 * fmod(tmp / 24.0, 1);

    return lmst > 0 ? lmst : lmst + 24.0;
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

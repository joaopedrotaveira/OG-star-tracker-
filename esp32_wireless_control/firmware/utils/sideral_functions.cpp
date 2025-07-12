#include "sideral_functions.h"

#include <math.h>

double julian_day(int year, int month, int day, double universal_time)
{
    if (month <= 2)
    {
        month = month + 12;
        year = year - 1;
    }
    return floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1)) + day - 13 - 1524.5 +
           universal_time / 24.0;
}

double local_mean_sidereal_time(double jd, double longitude)
{
    double gmst = greenwich_mean_sidereal_time(jd);
    double lmst = 24.0 * frac((gmst + longitude / 15.0) / 24.0);
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

double frac(double x)
{
    x = x - floor(x);
    if (x < 0)
        x = x + 1.0;
    return x;
}

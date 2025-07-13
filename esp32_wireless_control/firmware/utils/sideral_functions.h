#ifndef _SIDERAL_FUNCTIONS_H_
#define _SIDERAL_FUNCTIONS_H_ 1

#include <WString.h>

double julian_day(int year, int month, int day, int hours, int minutes, double seconds);

double haToRa(double ha, double lmst);

double raToHa(double ra, double lmst);

double universal_time(int hours, int minutes, double seconds);

double local_mean_sidereal_time(double jd, double longitude);

double greenwich_mean_sidereal_time(double jd);

String toHoursMinutesSeconds(double time /* in hours */);

double frac(double x);

#endif /* _SIDERAL_FUNCTIONS_H_ */

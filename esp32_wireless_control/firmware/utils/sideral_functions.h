#ifndef _SIDERAL_FUNCTIONS_H_
#define _SIDERAL_FUNCTIONS_H_ 1

double julian_day(int year, int month, int day, double universal_time);

double local_mean_sidereal_time(double jd, double longitude);

double greenwich_mean_sidereal_time(double jd);

double frac(double x);

#endif /* _SIDERAL_FUNCTIONS_H_ */

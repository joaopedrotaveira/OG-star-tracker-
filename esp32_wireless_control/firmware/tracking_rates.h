#ifndef _TRACKING_RATES_H_
#define _TRACKING_RATES_H_ 1

#include "config.h"

#if MOTOR_TRACKING_RATE == TRACKING_RATE_BOARD_V2
#if STEPPER_TYPE == STEPPER_0_9
// gear ratio 101.25, 0.9deg motor, 64 msteps, f_cpu@240MHz, f_isr@40MHz
#define TRACKER_MOTOR_MICROSTEPPING 64
#define STEPS_PER_SECOND_256MICROSTEP 120
enum trackingRateS
{
    TRACKING_SIDEREAL = 664846, // SIDEREAL (23h,56 min)
    TRACKING_SOLAR = 666667,    // SOLAR (24h)
    TRACKING_LUNAR = 680967,    // LUNAR (24h, 31 min)
};
#else // stepper 1.8 deg
// gear ratio 101.25, 1.8deg motor, 64 msteps, f_cpu@240MHz, f_isr@40MHz
#define TRACKER_MOTOR_MICROSTEPPING 64
#define STEPS_PER_SECOND_256MICROSTEP 60

enum trackingRateS
{
    TRACKING_SIDEREAL = 1329691, // SIDEREAL (23h,56 min)
    TRACKING_SOLAR = 1333333,    // SOLAR (24h)
    TRACKING_LUNAR = 1361934,    // LUNAR (24h, 31 min)
};
#endif
#elif MOTOR_TRACKING_RATE == TRACKING_RATE_D1_R32
#define STEPS_PER_SECOND_256MICROSTEP 60

// gear ratio 101.25, 1.8deg motor, 128 msteps, f_cpu@240MHz
//#define TRACKER_MOTOR_MICROSTEPPING 128
// enum trackingRateS
//{
//    TRACKING_SIDEREAL = 664846, // SIDEREAL (23h,56 min)
//    TRACKING_SOLAR = 666667,    // SOLAR (24h)
//    TRACKING_LUNAR = 680967,    // LUNAR (24h, 31 min)
//};

// gear ratio 101.25, 1.8deg motor, 256 msteps, f_cpu@240MHz
//#define TRACKER_MOTOR_MICROSTEPPING 256
//enum trackingRateS
//{
//    TRACKING_SIDEREAL = 332423, // SIDEREAL (23h,56 min)
//    TRACKING_SOLAR = 333333,    // SOLAR (24h)
//    TRACKING_LUNAR = 340483,    // LUNAR (24h, 31 min)
//};

// gear ratio 101.25, 1.8deg motor, 64 msteps, f_cpu@240MHz
#define TRACKER_MOTOR_MICROSTEPPING 64
enum trackingRateS
{
    TRACKING_SIDEREAL = 1329691, // SIDEREAL (23h,56 min)
    TRACKING_SOLAR = 1333334,    // SOLAR (24h)
    TRACKING_LUNAR = 1361934,    // LUNAR (24h, 31 min)
};

// gear ratio 101.25, 1.8deg motor, 16 msteps, f_cpu@240MHz
//#define TRACKER_MOTOR_MICROSTEPPING 16
// enum trackingRateS
//{
//    TRACKING_SIDEREAL = 5318765, // SIDEREAL (23h,56 min)
//    TRACKING_SOLAR = 5333333,    // SOLAR (24h)
//    TRACKING_LUNAR = 5447735,    // LUNAR (24h, 31 min)
//};

#elif MOTOR_TRACKING_RATE == TRACKING_RATE_NATGEO
// gear ratio 475.2, 1.8deg motor, 16 msteps, alarm freq@80MHz
#define STEPS_PER_SECOND_256MICROSTEP 281.6
//#define TRACKER_MOTOR_MICROSTEPPING 16
//enum trackingRateS
//{
//    TRACKING_SIDEREAL = 1133234, // SIDEREAL (23h,56 min)
//    TRACKING_SOLAR = 1136364,    // SOLAR (24h)
//    TRACKING_LUNAR = 1160739,    // LUNAR (24h, 31 min)
//};

// gear ratio 475.2, 1.8deg motor, 32 msteps, alarm freq@80MHz
//#define TRACKER_MOTOR_MICROSTEPPING 32
//enum trackingRateS
//{
//    TRACKING_SIDEREAL = 566617, // SIDEREAL (23h,56 min)
//    TRACKING_SOLAR = 568182,    // SOLAR (24h)
//    TRACKING_LUNAR = 580370,    // LUNAR (24h, 31 min)
//};

// gear ratio 475.2, 1.8deg motor, 16 msteps, alarm freq@80MHz
#define TRACKER_MOTOR_MICROSTEPPING 64
enum trackingRateS
{
    TRACKING_SIDEREAL = 283309, // SIDEREAL (23h,56 min)
    TRACKING_SOLAR = 284091,    // SOLAR (24h)
    TRACKING_LUNAR = 290185,    // LUNAR (24h, 31 min)
};

// gear ratio 475.2, 1.8deg motor, 16 msteps, f_cpu@240MHz
//#define STEPS_PER_SECOND_256MICROSTEP	281.6
//#define TRACKER_MOTOR_MICROSTEPPING 16
// enum trackingRateS
//{
//    TRACKING_SIDEREAL = 3399779, // SIDEREAL (23h,56 min)
//    TRACKING_SOLAR = 3409091,    // SOLAR (24h)
//    TRACKING_LUNAR = 3482217,    // LUNAR (24h, 31 min)
//};
// gear ratio 590,4, 1.8deg motor, 64 msteps, f_cpu@240MHz
//#define STEPS_PER_SECOND_256MICROSTEP	350
//#define TRACKER_MOTOR_MICROSTEPPING 64
// enum trackingRateS
//{
//    TRACKING_SIDEREAL = 283315, // SIDEREAL (23h,56 min)
//    TRACKING_SOLAR = 284091,    // SOLAR (24h)
//    TRACKING_LUNAR = 290185,    // LUNAR (24h, 31 min)
//};
#else
#error Unknown tracking rate setting
#endif

#endif /* _TRACKING_RATES_H_ */

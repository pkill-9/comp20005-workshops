/**
 *  Program to simulate a rocket's flight by solving the system of ODE's
 *  for position and velocity from acceleration. Note that for this 
 *  problem I did quite a bit of work on paper to derive the ODE's based
 *  on the force diagram. Also, because the rocket's mass is changing due
 *  to the decreasing fuel mass, we need to use the alternative form of
 *  Newton's equation, \sum{F} = m.dv/dt + v.dm/dt.
 *
 *  The problem is simplified in that we are only considering motion in
 *  one dimension, vertically.
 *
 *  Acceleration is given by the ODE:
 *
 *  a = 500/m - g - k.A.abs(v)^2/m - v/m.dm/dt
 *
 *  m and dm/dt are time varying; mass decreases by 0.8 kg per second
 *  until t=10 seconds, when the rocket runs out of fuel. Then, dm/dt is
 *  zero, and mass constant at 10kg.
 */

#include <stdio.h>
#include <math.h>

#define G                       9.81L       /** m/s^2 */
#define DRAG_COEFFICIENT        0.6L        /** kg/m^3 */
#define FRONTAL_AREA            0.1L        /** m^2 */
#define STEP_SIZE               0.001L      /** s */

/**********************************************************/

double thrust (double time);
double mass (double time);
double dmdt (double time);
double drag (double velocity);
double acceleration (double velocity, double time);

/**********************************************************/

    int
main (int argc, char **argv)
{
    double time = 0;
    double velocity = 0, height = 0;
    double max_height = 0;
    double a;

    while (height >= 0)
    {
        /** get the acceleration. */
        a = acceleration (velocity, time);

        /** new velocity = old velocity + acceleration * time */
        velocity += a * STEP_SIZE;
        height += velocity * STEP_SIZE;

        if (height > max_height)
            max_height = height;

        time += STEP_SIZE;
    }

    printf ("Maximum altitude: %10.3f m\n", max_height);

    return 0;
}

/**********************************************************/

/**
 *  Calculate the acceleration at the given time, and given the velocity.
 */
    double
acceleration (double velocity, double time)
{
    double m = mass (time);
    return thrust (time) / m - G - drag (velocity) / m - (velocity / m) *
        dmdt (time);
}

/**********************************************************/

/**
 *  Returns the thrust acting on the rocket at a given time.
 */
    double
thrust (double time)
{
    if (time <= 10.0)
        return 500.0;

    return 0.0;
}

/**********************************************************/

/**
 *  Returns the drag force acting on the rocket, with the sign in the
 *  opposite direction to velocity.
 */
    double
drag (double velocity)
{
    double sign = (velocity >= 0)? 1 : -1;
    return sign * DRAG_COEFFICIENT * FRONTAL_AREA * fabs (velocity) *
        fabs (velocity);
}

/**********************************************************/

/**
 *  Returns the mass of the rocket at the given time. For the first 10
 *  seconds of flight, the mass decreases at 0.8 kg per second as fuel is
 *  consumed, then the mass remains constant once the rocket runs out of
 *  fuel.
 */
    double
mass (double time)
{
    if (time < 10.0)
        return 18 - 0.8 * time;

    return 10.0;
}

/**********************************************************/

/**
 *  Returns the rate of change of the rocket's mass with respect to time
 *  at the given time.
 */
    double
dmdt (double time)
{
    if (time < 10.0)
        return -0.8;

    return 0.0;
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */

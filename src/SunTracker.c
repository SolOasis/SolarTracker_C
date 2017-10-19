/**
 * SunTracker.cpp
 * Purpose: This program calculate the sun position with Solar Position Algorithm
 * by Ibrahim Reda and Afshin Andreas.
 * 
 * @author   Ya-Liang Chang (Allen)
 * @version  0.1 2017-10-18
 *
 */

// #define DEBUG
#ifdef DEBUG
#define DEBUG_MSG(str) do { cout << str; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../lib/spa.h"  //include the SPA header file

void print_help_message() {
    printf("***********************************************\n");
    printf("*  Welcome to SolOasis Solar Tracking System  *\n");
    printf("*                                             *\n");
    printf("*  The SPA library is from 'Solar Position    *\n");
    printf("*  Algorithm for Solar Radiation Applications,*\n");
    printf("*  2008' by Ibrahim Reda and Afshin Andreas   *\n");
    printf("*                                             *\n");
    printf("***********************************************\n\n");
}
int main (int argc, char *argv[])
{
    print_help_message();

    spa_data spa;  //declare the SPA structure
    int result;
    float min, sec;

    //enter required input values into SPA structure

    int yy, mm, dd, hr, mmin, ssec;
    char ans;
    printf("Using the current time?(y/n) ");
    scanf("%c", &ans);
    if (ans == 'y') {
        //Using current time
        time_t t = time(0);   // get time now
        struct tm * timePtr = localtime( & t );
        yy = timePtr->tm_year + 1900;
        mm = timePtr->tm_mon + 1;
        dd = timePtr->tm_mday;
        hr = timePtr->tm_hour;
        mmin = timePtr->tm_min;
        ssec = timePtr->tm_sec;
        printf("Using %4d-%02d-%02d %02d:%02d:%02d \n", 
                yy, mm, dd, hr, mmin, ssec);
    } else {
        printf("Please input 'Year Month Day Hour Minute Second':");
        int input_num = scanf("%d %d %d %d %d %d", 
                &yy, &mm, &dd, &hr, &mmin, &ssec);

        if (input_num != 6) {
            fprintf(stderr, "Invalid input.\n");
            fprintf(stderr, "Valid example: 2003 10 17 12 30 30 .\n");
            exit(EXIT_FAILURE);
        }
    }
    spa.year          = yy;
    spa.month         = mm;
    spa.day           = dd;
    spa.hour          = hr;
    spa.minute        = mmin;
    spa.second        = ssec;
    spa.timezone      = -4; // Pittsburgh
    spa.delta_ut1     = 0.3; // constant for 2017
    spa.delta_t       = 69.1; // constant for 2017
    spa.longitude     = -79.958767; //Pittsburgh
    spa.latitude      = 40.443651; //Pittsburgh
    spa.elevation     = 40; // Height of building
    spa.pressure      = 1013.25; 
    spa.temperature   = 15;
    spa.slope         = 0;
    spa.azm_rotation  = -10;
    spa.atmos_refract = 0.5667;
    spa.function      = SPA_ALL;

    //call the SPA calculate function and pass the SPA structure

    result = spa_calculate(&spa);

    if (result == 0)  //check for SPA errors
    {
        //display the results inside the SPA structure

        printf("Julian Day:    %.6f\n",spa.jd);
        printf("L:             %.6e degrees\n",spa.l);
        printf("B:             %.6e degrees\n",spa.b);
        printf("R:             %.6f AU\n",spa.r);
        printf("H:             %.6f degrees\n",spa.h);
        printf("Delta Psi:     %.6e degrees\n",spa.del_psi);
        printf("Delta Epsilon: %.6e degrees\n",spa.del_epsilon);
        printf("Epsilon:       %.6f degrees\n",spa.epsilon);
        printf("Zenith:        %.6f degrees\n",spa.zenith);
        printf("Azimuth:       %.6f degrees\n",spa.azimuth);
        printf("Incidence:     %.6f degrees\n",spa.incidence);

        min = 60.0*(spa.sunrise - (int)(spa.sunrise));
        sec = 60.0*(min - (int)min);
        printf("Sunrise:       %02d:%02d:%02d Local Time\n", (int)(spa.sunrise), (int)min, (int)sec);

        min = 60.0*(spa.sunset - (int)(spa.sunset));
        sec = 60.0*(min - (int)min);
        printf("Sunset:        %02d:%02d:%02d Local Time\n", (int)(spa.sunset), (int)min, (int)sec);

    } else printf("SPA Error Code: %d\n", result);

    return 0;
}

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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../lib/spa.h"  //include the SPA header file

//Update period of the solar tracker
#define UPDATE_PEIORD 1800

//2017 constants
#define DELTA_UT1 0.3 
#define DELTA_T 69.1

//Pitt Benedum Hall constant 
#define TIMEZONE -4 // Pittsburgh
#define LONGITUDE -79.958767 // Pitt Benedum Hall
#define LATITUDE 40.443651 // Pitt Benedum Hall
#define HEIGHT 40 // Pitt Benedum Hall

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
    int result; // SPA result
    float min, sec; // For sunrise/set
    int yy, mm, dd, hr, mmin, ssec; // Desired time to calculate
	float lo = LONGITUDE;
	float la = LATITUDE;
	float hei = HEIGHT;
    char ans[100]; // Answer of yes/no
	char buffer[100]; //for input
    FILE *filePtr; // For recording sun location of the day
    char filename[sizeof "output/20171024_lookup_table.txt"];
    char location_result[100];
    time_t t;
    struct tm * timePtr;

	// Create output directory if not exists
	struct stat st = {0};
	if (stat("./output", &st) == -1) {
    	mkdir("./output", 0700);
	}

    // Enter required input values into SPA structure

    printf("Using the current time? [y]");
	fgets(ans, 100, stdin);
	
    //scanf("%c", &ans);
    if (ans[0] == 'y' || ans[0] == '\n') {
        t = time(0);   // get time now
        timePtr = localtime( & t );
        //Using current time
        yy = timePtr->tm_year + 1900;
        mm = timePtr->tm_mon + 1;
        dd = timePtr->tm_mday;
        hr = timePtr->tm_hour;
        mmin = timePtr->tm_min;
        ssec = timePtr->tm_sec;
    } else {
        printf("Please input 'Year Month Day Hour Minute Second':");
        int input_num = scanf("%d %d %d %d %d %d", 
                &yy, &mm, &dd, &hr, &mmin, &ssec);
        timePtr->tm_year = yy - 1900;
        timePtr->tm_mon = mm -1;
        timePtr->tm_mday = dd;
        timePtr->tm_hour = hr;
        timePtr->tm_min = mmin;
        timePtr->tm_sec = ssec;
        mktime(timePtr);

        if (input_num != 6) {
            fprintf(stderr, "Invalid input.\n");
            fprintf(stderr, "Valid example: 2003 10 17 12 30 30 .\n");
            exit(EXIT_FAILURE);
        }
    }
    printf("Time: %4d-%02d-%02d %02d:%02d:%02d \n", 
                yy, mm, dd, hr, mmin, ssec);
    //f = fopen(("%4d-%02d-%02d %02d:%02d:%02d.txt", yy, mm, dd, hr, mmin, ssec), "ab+");
    sprintf(filename, "output/%04d%02d%02d_lookup_table.txt", yy, mm, dd);
    filePtr = fopen(filename, "w");
    printf("Created %s \n\n", filename);

	printf("Using current GPS location? [y]");
	fgets(ans, 100, stdin);
    //scanf("%c", &ans);
    if (ans[0] != 'y' && ans[0] != '\n') {
		printf("Input longitude: ");
		scanf("%f", &lo);
		printf("Input latitude: ");
		scanf("%f", &la);
	}
	printf("Using GPS location (%6f, %6f)\n\n", lo, la);


    spa.year          = yy;
    spa.month         = mm;
    spa.day           = dd;
    spa.hour          = hr;
    spa.minute        = mmin;
    spa.second        = ssec;
    spa.timezone      = TIMEZONE;
    spa.delta_ut1     = DELTA_UT1;
    spa.delta_t       = DELTA_T; 
    spa.longitude     = lo; //Pittsburgh
    spa.latitude      = la; //Pittsburgh
    spa.elevation     = hei;
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

	printf("\nPress ENTER to continue writing lookup table of the day .. ");
	getchar();
	getchar();

	// Write lookup table for the day into file and print
	printf("UPDATE_PEIORD = %d\n", UPDATE_PEIORD);
	fprintf(filePtr, "UPDATE_PEIORD = %ds\n", UPDATE_PEIORD);
    for (int i = 0; i < (60*60*24/UPDATE_PEIORD); i++) {
        yy = timePtr->tm_year + 1900;
        mm = timePtr->tm_mon + 1;
        dd = timePtr->tm_mday;
        hr = timePtr->tm_hour;
        mmin = timePtr->tm_min;
        ssec = timePtr->tm_sec;

        spa.year          = yy;
        spa.month         = mm;
        spa.day           = dd;
        spa.hour          = hr;
        spa.minute        = mmin;
        spa.second        = ssec;
        result = spa_calculate(&spa);
        if (result != 0) {
            printf("SPA Error Code: %d\n", result);
            break;
        }

        sprintf(location_result, "%4d-%02d-%02d %02d:%02d:%02d Evaluation: % 4.3f Azimuth: %.3f\n", 
                yy, mm, dd, hr, mmin, ssec, 90-spa.zenith, spa.azimuth);
        printf("%s", location_result);
        fprintf(filePtr, "%s", location_result);

        timePtr->tm_sec += UPDATE_PEIORD;
        mktime(timePtr);
    }

    return 0;
}

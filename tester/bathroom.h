// Nicholas Delli Carpini

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <sched.h>
#include <math.h>
#include <sys/time.h>

#ifndef BATHROOM_H_
#define BATHROOM_H_

typedef enum genderENum {none, male, female} genderNum; // gender enum -> useful for empty bathroom


typedef struct bathroom { // bath struct -> only way to get locks into enterBath / leaveBath
	genderNum curGen;
	int usedNum;
	int curMen;
	int curWMen;
	double vacTot;
	double usedTot;
	struct timeval svtime; // start vacant time
	struct timeval evtime; // end vacant time
	struct timeval sutime; // start used time
	struct timeval eutime; // end used time
	pthread_mutex_t lock;
	pthread_cond_t noWMen;
	pthread_cond_t noMen;

} bathStruct;

typedef struct individual { // user struct -> keeps track of id and only way to get printLock into thread
	genderNum gender;
	int userID;
	int arrive;
	int needed;
	int times;

} userStruct;

void initialBath();
void printStat(genderNum gender, int threadNum, int times, double minTime, double maxTime, double avgTime);
void printBath();

double enterBath(genderNum gender);
void leaveBath();

#endif /* BATHROOM_H_ */

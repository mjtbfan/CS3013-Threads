// Nicholas Delli Carpini

#include "bathroom.h"

bathStruct *globalBath;

genderNum curGenHelper() { // couldnt get accurate numbers without helper, no clue why
	if (globalBath->curMen > 0) {
		globalBath->curGen = male;
		return male;
	} else if (globalBath->curWMen > 0) {
		globalBath->curGen = female;
		return female;
	} else {
		globalBath->curGen = none;
		return none;
	}
}

void initialBath() { // initializes the bathroom -> ready for users
	globalBath = malloc(sizeof(bathStruct));
	globalBath->curGen = none;
	globalBath->curMen = 0;
	globalBath->curWMen = 0;
	globalBath->usedNum = 0;
	globalBath->vacTot = 0;
	globalBath->usedTot = 0;

	pthread_mutex_init(&globalBath->lock, NULL);
	pthread_cond_init(&globalBath->noWMen, NULL);
	pthread_cond_init(&globalBath->noWMen, NULL);

	gettimeofday(&globalBath->svtime, NULL);
}

void printStat(genderNum gender, int userID, int times, double minTime, double maxTime, double avgTime) { // prints info for each user
	printf("\nUser %d has Finished All Uses of the Bathroom\n", userID);
	printf("   Thread ID: %ld\n", (long) pthread_self()); // suggested use on forum -> dont really know if useful but didnt hurt to add
	if (gender == male) {
		printf("   Gender: Male\n");
	} else {
		printf("   Gender: Female\n");
	}
	printf("   Uses: %d\n", times);
	printf("   Min Wait Time: %.2f ms\n", minTime);
	printf("   Max Wait Time: %.2f ms\n", maxTime);
	printf("   Avg Wait Time: %.2f ms\n", avgTime);
}

void printBath() { // prints the final output
	printf("\nAll Users Have Finished\n");
	printf("   Total Uses: %d\n", globalBath->usedNum);
	printf("   Total Used Time: %.2f ms\n", globalBath->usedTot);
	printf("   Total Vacant Time: %.2f ms\n", globalBath->vacTot);
}

double enterBath(genderNum gender) { // actual enter function -> returns the time a user was spent waiting for the bathroom (could've done in main but would've been inaccurate)
	struct timeval swtime, ewtime;
	pthread_mutex_lock(&globalBath->lock); // LOCK
	swtime.tv_sec = 0; // have to clear these values before each enterBath, or else output values completely borked
	swtime.tv_usec = 0;
	ewtime.tv_sec = 0;
	ewtime.tv_usec = 0;
	if (curGenHelper() == male) { // checks gender of bath
		if (gender == male) { // checks gender of user
			globalBath->curMen++;
			globalBath->usedNum++;
		} else { // if user gender is female
			gettimeofday(&swtime, NULL);
			while (curGenHelper() == male) { // while bathroom is male -> wait for noMen broadcast
				pthread_cond_wait(&globalBath->noMen, &globalBath->lock);
			}
			gettimeofday(&ewtime, NULL);
			globalBath->curWMen++;
			globalBath->usedNum++;
		}
	} else if (curGenHelper() == female) { // checks gender of bath
		if (gender == female) { // checks gender of user
			globalBath->curWMen++;
			globalBath->usedNum++;
		} else { // if user is male
			gettimeofday(&swtime, NULL);
			while (curGenHelper() == female) { // while bathroom is female -> wait for noWMen broadcast
				pthread_cond_wait(&globalBath->noWMen, &globalBath->lock);
			}
			gettimeofday(&ewtime, NULL);
			globalBath->curMen++;
			globalBath->usedNum++;
		}
	} else { // if bathroom is empty
		gettimeofday(&globalBath->evtime, NULL);
		if (gender == male) {
			globalBath->curMen++;
			globalBath->curGen = male;
		} else if (gender == female) {
			globalBath->curWMen++;
			globalBath->curGen = female;
		}
		globalBath->usedNum++;
		globalBath->vacTot += (double)(((globalBath->evtime.tv_sec - globalBath->svtime.tv_sec) * 1000) + ((globalBath->evtime.tv_usec - globalBath->svtime.tv_usec) / 1000));
		gettimeofday(&globalBath->sutime, NULL);
	}
	pthread_mutex_unlock(&globalBath->lock); // UNLOCK
	return ((double)((ewtime.tv_sec - swtime.tv_sec) * 1000) + ((ewtime.tv_usec - swtime.tv_usec) / 1000));
}

void leaveBath() { // general leave function - weird issue that usedTot needed to be in the if statement to work and is not always 100% accurate
	pthread_mutex_lock(&globalBath->lock); // LOCK
	gettimeofday(&globalBath->eutime, NULL);
	if (curGenHelper() == 1) { // checks bathroom gender (non curGen user wouldn't be leaving -> checking bath is fine)
		globalBath->curMen--;
		if(globalBath->curMen == 0) { // checks if last man leaving
			globalBath->curGen = none;
			globalBath->usedTot += (double)(((globalBath->eutime.tv_sec - globalBath->sutime.tv_sec) * 1000) + ((globalBath->eutime.tv_usec - globalBath->sutime.tv_usec) / 1000));
			pthread_cond_broadcast(&globalBath->noMen); // broadcasts that no men are left in bathroom -> women free to go in (could lead to starving)
		}
	} else if (curGenHelper() == 2) { // checks bathroom gender (non curGen user wouldn't be leaving -> checking bath is fine)
		globalBath->curWMen--;
		if(globalBath->curWMen == 0) { // checks if last woman leaving
			globalBath->curGen = none;
			globalBath->usedTot += (double)(((globalBath->eutime.tv_sec - globalBath->sutime.tv_sec) * 1000) + ((globalBath->eutime.tv_usec - globalBath->sutime.tv_usec) / 1000));
			pthread_cond_broadcast(&globalBath->noWMen); // broadcasts that no women are left in bathroom -> men free to go in (could lead to starving)
		}
	}
	gettimeofday(&globalBath->svtime, NULL);
	pthread_mutex_unlock(&globalBath->lock); // UNLOCK
}



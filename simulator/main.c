// Nicholas Delli Carpini

#include "bathroom.h"
pthread_mutex_t printLock; // global printLock

int stdDeviation(int value) { // helper function to get rand std deviation +/- argv value
	int tempRand = (rand() % value / 2);
	int signRand = rand() % 2;
	if (value == 0) {
		return 0;
	} else {
		if (signRand == 0) {
			return value + tempRand;
		} else {
			return value - tempRand;
		}
	}
}


void *runUser(void* userVoid) { // runs the thread and all functions needed from module
	userStruct *user = malloc(sizeof(userStruct));
	user = (userStruct*) userVoid;
	double waitTime;
	double avgTime;
	double totTime = 0;
	double maxTime = 0; // comparison value for maxTime
	double minTime = 999 * 1000; // comparison value for minTime
	int i = 0;

	user->times = stdDeviation(user->times); // makes random time value
	if (user->times < 1) { // checks that rand time is > 0
		user->times = 1;
	}

	for (i = 0; i < user->times; i++) { // for each time
		user->arrive = stdDeviation(user->arrive); // makes random arrive
		user->needed = stdDeviation(user->needed); // makes random needed
		usleep(user->arrive * 1000); // sleeps for arrive (milliseconds)
		waitTime = enterBath(user->gender);
		usleep(user->needed * 1000); // sleeps for needed (milliseconds)
		leaveBath();
		if (waitTime < minTime) { // gets minTime
			minTime = waitTime;
		}
		if (waitTime > maxTime) { // gets maxTime
			maxTime = waitTime;
		}
		totTime += waitTime; // to get avgTime
	}
	avgTime = totTime / user->times; // avgTime / # of times

	pthread_mutex_lock(&printLock); // LOCK
	printStat(user->gender, (user->userID + 1), user->times, minTime, maxTime, avgTime);
	pthread_mutex_unlock(&printLock); // UNLOCK
	return 0;
}


int main(int argc, char* argv[]) {
	// checks for valid input args
	if (argc != 5) {
		printf("\nError: Incorrect Number of Arguments\n\n");
		return -1;
	}
	for (int i = 1; i < argc; i++) {
		for (int j = 0; j < (int) strlen(argv[i]); j++) {
			if (!isdigit(argv[i][j])) {
				printf("\nError: Argument %d is Invalid\n\n", i);
				return -1;
			}
		}
	}
	srand(time(NULL));
	int numUsers = atoi(argv[1]);
	if (numUsers == 0) {
		printf("\nError: Must Have at Least 1 User\n\n");
		return -1;
	}
	int avgTimes = atoi(argv[2]);
	if(avgTimes == 0) {
		printf("\nError: Each User Must Go to the Bathroom at Least Once\n\n");
		return -1;
	}
	int avgArrive = atoi(argv[3]);
	int avgNeed = atoi(argv[4]);

	srand(time(NULL));
	initialBath(); // from module
	pthread_mutex_t lock;

	pthread_mutex_init(&lock, NULL);
	pthread_mutex_init(&printLock, NULL);

	pthread_mutex_lock(&lock); // LOCK
	pthread_t id[numUsers];
	userStruct *userList = malloc(sizeof(userStruct) * numUsers);
	for (int i = 0; i < numUsers; i++) { // loop for each user
		printf("User %d has Arrived to the Bathroom\n", i + 1);
		userList[i].gender = (rand() % 2) + 1; // needed to make values in for loop -> breaks using an external function (no clue why)
		userList[i].userID = i;
		userList[i].arrive = avgArrive;
		userList[i].needed = avgNeed;
		userList[i].times = avgTimes;
		pthread_create(&id[i], NULL, &runUser, (void*) &userList[i]);
	}
	pthread_mutex_unlock(&lock); // UNLOCK

	for (int i = 0; i < numUsers; i++) {
		pthread_join(id[i], NULL);
	}

	printBath(); // from module
	free(userList);
	return 0;
}

Nicholas Delli Carpini

----- Functions -----
genderNum curGenHelper() { // couldn't get accurate numbers without helper, no clue why
void initialBath() { // initializes the bathroom -> ready for users
void printStat(genderNum gender, int userID, int times, double minTime, double maxTime, double avgTime) { // prints info for each user
void printBath() { // prints the final output
double enterBath(genderNum gender) { // actual enter function -> returns the time a user was spent waiting for the bathroom 
	(could've done in main but would've been inaccurate)
void leaveBath() { // general leave function - weird issue that usedTot needed to be in the if statement to work and is not always 100% accurate

----- Write-Up -----
Module basically works as a clean way to keep functions that run in the thread separate from functions that run in the main,
with the exception of printBath and initialBath, but putting them in the module led to a lot less headaches when it came to 
global variable values. 
I know that there are never two users of differing genders in the bathroom at the same time thanks to the curGenHelper. Without this
function it was impossible to stop two users of differing genders to enter the bathroom at once (as stated in the function header, I
originally made a genderNum enum in the bathStruct, but for some reason this wasn't enough). This function checks the current gender
in the bathroom, and enterBath would keep any other users of the opposite gender in a pthread cond loop until it got the broadcast
that no users of the opposite gender were still in the bathroom. Therefore, anybody of the wrong gender would be looped until they
got the broadcast, and only then would they be allowed to edit the values corresponding to the bathroom.

----- Issues -----
Most of the module works as expected, with the exception of vacant and used times. For some reason the vacant time always seems too
low used time always seems to high. These values also became progressive less accurate proportionally on the length of time 
before users arrived and the length of time said users were in the bathroom. Whether this is because of race conditions or some other
multithreading issue was beyond me.
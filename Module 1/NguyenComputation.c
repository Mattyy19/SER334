/**
* This program takes the number of cylinders, the height and radius of each cylinder,
* from the user to sum up the volume and display it back to the user
*
* Completion time: 30 minutes
*
* @author Matthew Nguyen
* @version 1.0
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <math.h>

int main(void)
{
	//Add a prompt for the number of cylinders to sum. Make sure to use the control
	//symbol for integers. [2 points]
	int i, numOfCylinders;
	float sumVolume = 0.00;

	printf("How many cylinders would you like to add:");
	scanf("%d", &numOfCylinders);
	printf("\n");

	//Create a loop based on the number of cylinders the user enters. [2 points]

	//    Within the loop, prompt for height and radius parameters (allow floating
	//    point numbers), and calculate the volume for that particular cylinder.
	//    [4 points]
	for(i = 0; i < numOfCylinders; i++)
	{
		float height, radius, volume = 0.00;

		printf("What is the height of cylinder %d:", i + 1);
		scanf("%f", &height);

		printf("What is the radius of cylinder %d:", i + 1);
		scanf("%f", &radius);
		printf("\n");

		volume = M_PI * (radius * radius) * height;

		sumVolume += volume;
	}

	//Display the total volume sum back to the user. Make sure to use the right control
	//symbol. [2 points]
	printf("The total volume of all the cylinders is %.2f.\n", sumVolume);

	return 0;
}

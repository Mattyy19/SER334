/**
* This program manages a set of courses a student is taking.
*
* Completion time: 9 hours
*
* @author Matthew Nguyen
* @version 1.0
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
//MACROS: CONSTANTS

#define MAX_LEN 1024

////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES

typedef enum {CSE = 0, EEE = 1, EGR = 2, SER = 3} Subjects;
struct courseNode
{
	Subjects subject;
	int number;
	char teachers[MAX_LEN];
	int credits;

	struct courseNode* next;
	struct courseNode* prev;
};

////////////////////////////////////////////////////////////////////////////////
//GLOBAL VARIABLES

//place to store course information
struct courseNode* course_collection = NULL;
//Counts total credits
int totalCredits = 0;

////////////////////////////////////////////////////////////////////////////////
//FORWARD DECLARATIONS
void branching(char option);
void course_insert(int subjectNum, int courseNum, int courseCreds, char* teacher);
void schedule_print();
void course_drop(int subjectRem, int courseNumRem);
void schedule_load();
void schedule_save();

int main() {
	char input_buffer;

	printf("\n\nWelcome to ASU Class Schedule\n");

	schedule_load();

	//menu and input loop
	do {
		printf("\nMenu Options\n");
		printf("------------------------------------------------------\n");
		printf("a: Add a class\n");
		printf("d: Drop a class\n");
		printf("s: Show your classes\n");
		printf("q: Quit\n");
		printf("\nTotal Credits: %d\n\n", totalCredits);
		printf("Please enter a choice ---> ");

		scanf(" %c", &input_buffer);

		branching(input_buffer);
	} while (input_buffer != 'q');

	schedule_save();

	return 0;
}

//takes a character representing an inputs menu choice and calls the appropriate
//function to fulfill that choice. display an error message if the character is
//not recognized.
void branching(char option) {
	switch (option) {
	case 'a':
		int subjectNum;
		int courseNum;
		int courseCreds;
		char teacher[MAX_LEN];

		//Gets course information from user
		printf("\nWhat is the subject? (CSE = 0, EEE = 1, EGR = 2, SER = 3):");
		scanf("%d", &subjectNum);
		printf("\nWhat is the course number? (e.g. 334):");
		scanf("%d", &courseNum);
		printf("\nHow many credits is the class? (e.g. 3):");
		scanf("%d", &courseCreds);
		printf("\nWhat is the name of the professor?:");
		scanf("%s", teacher);
		course_insert(subjectNum, courseNum, courseCreds, teacher);
		break;

	case 'd':
		int subjectRem;
		int courseRem;

		//Gets course information from user
		printf("\nWhat is the subject? (CSE = 0, EEE = 1, EGR = 2, SER = 3):");
		scanf("%d", &subjectRem);
		printf("\nWhat is the course number? (e.g. 334):");
		scanf("%d", &courseRem);
		course_drop(subjectRem, courseRem);
		break;

	case 's':
		schedule_print();
		break;

	case 'q':
		// main loop will take care of this.
		break;

	default:
		printf("\nError: Invalid Input.  Please try again...");
		break;
	}

}

//Inserts course into course_collection based on input
void course_insert(int subjectNum, int courseNum, int courseCreds, char* teacher) {
	//check if course is already in list
	struct courseNode* tempList = course_collection;
	while(tempList != NULL)
	{
		if(tempList->subject == subjectNum && tempList->number == courseNum)
		{
			printf("Cannot add a duplicate course");
			return;
		}
		tempList = tempList->next;
	}

	//creates new courseNode to add to list
	struct courseNode* newNode = (struct courseNode*)malloc(sizeof(struct courseNode));
	newNode->subject = subjectNum;
	newNode->number = courseNum;
	newNode->credits = courseCreds;
	totalCredits = totalCredits + courseCreds;
	strcpy(newNode->teachers, teacher);
	newNode->next = NULL;
	newNode->prev = NULL;

	//If course_collection is empty
	if(course_collection == NULL)
	{
		course_collection = newNode;
	}
	else
	{
		struct courseNode* iter = course_collection;

		//If insertion is at the head
		if(newNode->subject < iter->subject || (newNode->subject == iter->subject && newNode->number < iter->number))
		{
			newNode->next = course_collection;
			course_collection->prev = newNode;
			course_collection = newNode;
			return;
		}

		//Insert newNode in sorted order
		while(iter->next != NULL)
		{
			if(newNode->subject < iter->subject ||
				(newNode->subject == iter->subject && newNode->number < iter->number))
			{
				newNode->prev = iter->prev;
				newNode->next = iter;
				iter->prev->next = newNode;
				iter->prev = newNode;
				return;
			}
			iter = iter->next;
		}

		//If insert is before the tail
		if(newNode->subject < iter->subject ||
				(newNode->subject == iter->subject && newNode->number < iter->number))
		{
			newNode->prev = iter->prev;
			newNode->next = iter;
			iter->prev->next = newNode;
			iter->prev = newNode;
			return;
		}

		//If insert is at the tail
		iter->next = newNode;
		newNode->prev = iter;
	}
}

//Displays all courses in course_collection
void schedule_print()
{
	printf("Class Schedule:\n");
	struct courseNode* iter = course_collection;

	while(iter != NULL)
	{
		switch(iter->subject)
		{
		case 0:
			printf("CSE%d %d %s\n", iter->number, iter->credits, iter->teachers);
			break;
		case 1:
			printf("EEE%d %d %s\n", iter->number, iter->credits, iter->teachers);
			break;
		case 2:
			printf("EGR%d %d %s\n", iter->number, iter->credits, iter->teachers);
			break;
		case 3:
			printf("SER%d %d %s\n", iter->number, iter->credits, iter->teachers);
			break;
		}
		iter = iter->next;
	}
}

//Removes specified course from course_collection
void course_drop(int subjectNum, int courseNumRem)
{
	//Traverses through list to find course to remove
	struct courseNode* iter = course_collection;
	while(iter != NULL)
	{
		if(iter->subject == subjectNum && iter->number == courseNumRem)
		{
			//Only node in list
			if(iter->next == NULL && iter->prev == NULL)
			{
				course_collection = NULL;
				totalCredits = totalCredits - iter->credits;
				free(iter);
				iter = NULL;
				return;
			}
			//If course is the head
			if(iter->prev == NULL)
			{
				course_collection = course_collection->next;
				course_collection->prev = NULL;
				totalCredits = totalCredits - iter->credits;
				free(iter);
				iter = NULL;
				return;
			}
			//If course is the tail
			if(iter->next == NULL)
			{
				iter->prev->next = NULL;
				totalCredits = totalCredits - iter->credits;
				free(iter);
				iter = NULL;
				return;
			}

			iter->prev->next = iter->next;
			iter->next->prev = iter->prev;
			totalCredits = totalCredits - iter->credits;
			free(iter);
			iter = NULL;
			return;
		}

		iter = iter->next;
	}

	printf("This course does not exist in the schedule");

}

//Loads schedule from data.txt file
void schedule_load()
{
	//Tries to open data.txt
	FILE* file = fopen("data.txt", "rb");

	//If file does not exist
	if(file == NULL)
	{
		return;
	}

	char data[MAX_LEN];

	while(fgets(data, MAX_LEN, file) != NULL)
	{
		int subject, courseNum, credits;
		char teacher[MAX_LEN];

		//Removes \n at at of string
		int length = strlen(data);
		if(length > 0 && data[length-1] == '\n')
		{
			data[length-1] = '\0';
		}
		//Gets line of course
		sscanf(data, "%d,%d,%d,%s", &subject, &courseNum, &credits, teacher);
		//Adds to course_collection
		course_insert(subject, courseNum, credits, teacher);
	}

	fclose(file);
}

//Saves schedule to data.txt file
void schedule_save()
{
	//Tries to open data.txt
	FILE* file = fopen("data.txt", "wb");

	//If file does not exist
	if(file == NULL)
	{
		return;
	}

	//Writes to data.txt
	struct courseNode* iter = course_collection;
	while(iter != NULL)
	{
		fprintf(file, "%d,%d,%d,%s\n", iter->subject, iter->number, iter->credits, iter->teachers);
		//Deallocates nodes
		course_drop(iter->subject, iter->number);

		iter = iter->next;
	}

	fclose(file);
}
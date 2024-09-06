/**
* This program manages a set of courses a student is taking.
*
* Completion time: (estimation of hours spent on this program)
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
struct CourseNode
{
	Subjects subject;
	int number;
	char teachers[MAX_LEN];
	int credits;

	struct CourseNode* next;
	struct CourseNode* prev;
};

////////////////////////////////////////////////////////////////////////////////
//GLOBAL VARIABLES

//place to store course information
struct CourseNode* course_collection = NULL;

////////////////////////////////////////////////////////////////////////////////
//FORWARD DECLARATIONS
void branching(char option);
void course_insert(int subjectNum, int courseNum, int courseCreds, char* teacher);
void schedule_print();
void course_drop();
void schedule_load();
void schedule_save();

//main entry point. Starts the program by displaying a welcome and beginning an 
//input loop that displays a menu and processes user input. Pressing q quits.      
int main() {
	char input_buffer;

	printf("\n\nWelcome to ASU Class Schedule\n");

	//TODO: stuff goes here...
	schedule_load();

	//menu and input loop
	do {
		printf("\nMenu Options\n");
		printf("------------------------------------------------------\n");
		printf("a: Add a class\n");
		printf("d: Drop a class\n");
		printf("s: Show your classes\n");
		printf("q: Quit\n");
		//printf("\nTotal Credits: %d\n\n", TODO);
		printf("Please enter a choice ---> ");

		scanf(" %c", &input_buffer);

		branching(input_buffer);
	} while (input_buffer != 'q');

	//TODO: stuff goes here...
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
		//TODO
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
	struct CourseNode* tempList = course_collection;
	while(tempList != NULL)
	{
		if(tempList->subject == subjectNum && tempList->number == courseNum)
		{
			printf("Cannot add a duplicate course");
			return;
		}
		tempList = tempList->next;
	}

	//creates new CourseNode to add to list
	struct CourseNode* newNode = (struct CourseNode*)malloc(sizeof(struct CourseNode));
	newNode->subject = subjectNum;
	newNode->number = courseNum;
	newNode->credits = courseCreds;
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
		struct CourseNode* iter = course_collection;
		//Adds newNode in sorted order
		while(iter != NULL)
		{
			if(newNode->subject <= iter->subject && newNode->number < iter->number)
			{
				//If insert is at the head
				if(iter->prev == NULL)
				{
					newNode->next = iter;
					iter->prev = newNode;
					course_collection = newNode;
					free(newNode);
					return;
				}

				newNode->prev = iter;
				newNode->next = iter->next;
				iter->next = newNode;

				//If newNode is not the tail
				if(newNode->next != NULL)
				{
					newNode->next->prev = newNode;
					free(newNode);
					return;
				}
			}
			iter = iter->next;
		}

		iter->next = newNode;
		newNode->prev = iter;
		free(newNode);
	}
}

//Displays all courses in course_collection
void schedule_print()
{
	printf("Class Schedule:\n");
	struct CourseNode* iter = course_collection;

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
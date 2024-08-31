/**
* This program sorts chunks of strings, decrypts them, and then displays them.
*
* Completion time: 40 minutes
*
* @author Matthew Nguyen
* @version 1.0
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <string.h>

//macros: constants
#define CHUNK_LENGTH (20+1)  //each chunk has twenty characters, we add 1 so
                           //there is space for the null terminator.
#define NUMBER_OF_CHUNKS 4 //the message is spread across 4 chunks.
#define DECRYPTION_SHIFT 5 //this is the ASCII table shift used for decryption.

//forward declarations
void sort_chunks();
void decrypt_chunks();
void display_chunks();

char chunks[NUMBER_OF_CHUNKS][CHUNK_LENGTH];

int main() {
	//copy message into memory.
	strcpy(chunks[0], "2i1%fsi%fs%jstwrtzx%");
	strcpy(chunks[1], "1'H%nx%vznwp~1%kqf|j");
	strcpy(chunks[2], "4R3%Wnyhmnj%%%%%%%%%");
	strcpy(chunks[3], "3xzhhjxx3'%2%Ijssnx%");

	//the format of a chunk is a single number indicating its order in overall
	//message followed by twenty encrypted characters.


	//reorder chunks in message by sorting them based on the first digital
	//they contain. looking above, one can see they are currently in the order
	//2, 1, 4, 3 but should be ordered 1, 2, 3, 4.
	sort_chunks();

	//shift the characters in the message to produce the original characters.
	decrypt_chunks();

	//display the decrypted message.
	display_chunks();


	return 0; 
}

//given two strings, swaps their contents in memory.
void swap_strings(char* x, char* y) {
	//create a temporary holding place for the data so we don't lose it.
	char temp[CHUNK_LENGTH];

	strcpy(temp, x);
	strcpy(x, y);
	strcpy(y, temp);
}

//sorts the strings the global chunks variable by the first character they contain.
void sort_chunks() {
	int i, j;
	for(i = 0; i < NUMBER_OF_CHUNKS - 1; i++)
	{
		int index = i;
		for(j = i + 1; j < NUMBER_OF_CHUNKS; j++)
		{
			if(chunks[j][0] < chunks[i][0])
			{
				index = j;
			}
		}

		swap_strings(chunks[i], chunks[index]);
	}
}

//for each string in the global chunks variable, shifts the characters in it by
//DECRYPTION_SHIFT.
void decrypt_chunks() {
    int i;
	for(i = 0; i < NUMBER_OF_CHUNKS; i++)
	{
		char* p = chunks[i];

		while(*p)
		{
			*p = *p - DECRYPTION_SHIFT;
			p++;
		}
	}
}


//displays the strings in the global chunks variable
void display_chunks() {
	int i;
	for(i = 0; i < NUMBER_OF_CHUNKS; i++)
	{
		char* p = chunks[i];
		p++;

		while(*p)
		{
			printf("%c", *p);
			p++;
		}
	}
}
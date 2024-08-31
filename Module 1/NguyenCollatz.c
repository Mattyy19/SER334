/**
* This program checks whether a positive integer satisfies a collatz conjecture
*
* Completion time: 25 minutes
*
* @author Matthew Nguyen
* @version 1.0
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>

void termination(int input);
void output(int iter, int origInput);

int main()
{
    int input;

    printf("Please input a positive integer to check if it satisfies a collatz conjecture:");
    scanf("%d", &input);

    termination(input);
}

void termination(int input)
{
    int iter= 0;
    int origInput = input;

    while(input != 1)
    {
        if(input % 2 == 0)
        {
            input = input / 2;
        }
        else
        {
            input = (input * 3) + 1;
        }

        iter++;
    }

    output(iter, origInput);
}

void output(int iter, int origInput)
{
    printf("\nIt took %d iterations for input %d to satisfy the collatz conjecture.", iter, origInput);
}
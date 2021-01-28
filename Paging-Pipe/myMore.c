#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"

#define MAX_LINE_SIZE 500
#define MAX_LINE_NUMBER 100
#define LINE_TURN 24

int main(int argc, char *argv[])
{
    int readx = atoi(argv[1]);

    char read_msg[MAX_LINE_NUMBER][MAX_LINE_SIZE];

    read(readx, &read_msg, LINE_TURN * MAX_LINE_SIZE);


    for (int i = 0; i < LINE_TURN; ++i)
    {
        printf("%s", read_msg[i]);
    }


    close(readx);

    return 0;
}

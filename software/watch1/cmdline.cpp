/*
2014 Jasper ter Weeme
*/

#include <unistd.h>
#include <stdio.h>

int main()
{
    puts("\r10:01");
    usleep(1000000);
    puts("\r10:02\n");
    return 0;
}


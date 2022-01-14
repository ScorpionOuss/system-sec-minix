//
// Created by heisenberg on 02/12/2021.
//

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    printf("csc test starting");
    int x = 0;
    for (int i = 0; i < 1000000; i++){
        x += (i - (10-i));
    }
    printf("%d /n", x);

    return 0;
}
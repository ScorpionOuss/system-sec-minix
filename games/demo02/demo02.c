//
// Created by heisenberg on 02/12/2021.
//

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int x = 0;
    for (int i = 0; i < 100000; i++) {
        x = (i * i + 1) - 1 - (i * i); // always 0
    }

    printf("Demo 02 is out.\n");

    return x;
}
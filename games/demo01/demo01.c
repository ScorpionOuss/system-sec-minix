//
// Created by heisenberg on 02/12/2021.
//

#include "stdlib.h"
#include "stdio.h"

int main(int argc, char *argv[])
{
    int i = 0;
    FILE* fp;
    fp= fopen("/usr/share/signatures.txt", "r");

    fclose(fp);



    return i;
}
/******************************************************************************/
/*The structure of the elf file is as follow:   */
/* First we have the elf file header*/
/*Then we have the program header*/
/*Then the sections headers and then  the actual sections*/
/*From the elf header we can retreive the address
where the program header starts and where the sections headers table starts
There is a section where the names of all sections are stored
We can have the address of the header of this section from
the elf header. Then we can iterate through the sections headers
to find the section with ".text" as name.
we we find such section header we can access the section offset
and size. Then we have all the necessary informations to access and sign
the text segments.

The vaddr field references the virtual address, where the section should be loaded
not the actual address in the image file.
*/ 

#include <elf.h>
#include<stdio.h>
#include <stdlib.h>
#include <string.h>

long int findsize(char* filename){
    FILE* fp ;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("File Not Found!\n");
        return -1;
    }
    fseek(fp, 0L, SEEK_END);

    // calculating the size of the file
    long int res = ftell(fp);

    fclose(fp);
    return res;
}
int main(int argc, char** argv){
    Elf32_Ehdr elfHdr;
    Elf32_Shdr sectHdr;
    Elf32_Shdr textSec;
    Elf32_Phdr progHeader;

    long int file_size;
    FILE* elfFile;
    char* sectNames = NULL;

    if(argc < 2){
        perror("Parameters error");
        return 1;
    }
    int n= argc-1;
    char *programs[n];
    int32_t *adresses[n];
    int32_t *signs[n];
    int32_t sizes[n];
    for(int i=1;i<argc;i++ ) {
        programs[i-1]=argv[i];
        elfFile = fopen(argv[i], "rb");
        if (elfFile == NULL) {
            printf("Error opening file");
            return 1;
        }

        /******** The elfHdr contains the elf file header******/
        fread(&elfHdr, 1, sizeof(Elf32_Ehdr), elfFile);
        // printf("The entry point is : %8x\n", elfHdr.e_entry);
        int64_t phoff = elfHdr.e_phoff;
        fseek(elfFile, elfHdr.e_phoff, SEEK_SET);
        fread(&progHeader, 1, sizeof(progHeader), elfFile);
        int64_t offset = progHeader.p_offset;
        int64_t size = progHeader.p_filesz;



        int32_t *signatures = malloc(sizeof(int32_t) * size / 4096);
        int32_t *adrs = malloc(sizeof(int32_t) * size / 4096);
        char *page = malloc(4096 * sizeof(char));

        for (int c = 0; c < size / 4096; c++) {
            fseek(elfFile, offset + c * 4096, SEEK_SET);
            fread(page, 1, 4096 * sizeof(char), elfFile);
            //sign here and append signature
            int32_t sign = 0;
            for (int i = 0; i < 4096; i += 32) {
                sign ^= *((int32_t * )(page + i));
            }
            signatures[c] = sign;
            adrs[c]= progHeader.p_vaddr + offset + c * 4096;
        }
        signs[i-1] = signatures;
        adresses[i-1]=adrs;
        sizes[i-1]=size/4096;

        fclose(elfFile);
        // /****We generate signatures for the different blocks****/
        // int numBlocks = ((int) size/4096) + 1;
        // char *buffer = malloc(4096* sizeof(char));
        // int * signatures = malloc(numBlocks*sizeof(int));
        // for (int i = 0; i < numBlocks; i++){
        //     if (i == numBlocks -1){
        //         memset(buffer, 0, sizeof(buffer));
        //     }
        //     fread(buffer, 1, sizeof(buffer), elfFile);
        //     *(signatures + i) = sign(buffer);
        // }

        // for (int i = 0; i < numBlocks; i++){
        //     printf("The signature number %d is %d", i, *(signatures + i));
        // }
    }
    int32_t ss = 0;
    for(int i=0 ;i<n;i++) {
        if(ss<sizes[i]) {
        ss = sizes[i];
        }
    }
    FILE *sigh;

    sigh=fopen("minix/servers/csc/signatures.h","w");
    fprintf(sigh,"#ifndef SYSTEM_SEC_MINIX_GROUP_1_SIGNATURES_H\n"
                 "#define SYSTEM_SEC_MINIX_GROUP_1_SIGNATURES_H\n");
    fprintf(sigh,"static int n = %d;\n",n);
    fprintf(sigh,"static char *white_list[%d]={\"",n);
    for(int i=0; i<n-1 ;i++) {
        char * token= strtok(argv[i+1],"/");
        char* lasttoken;
        while (token!=NULL){
            lasttoken =token;
            token= strtok(NULL,"/");
        }
        fprintf(sigh,"%s\",\"",lasttoken);
    }
    char * token= strtok(argv[n],"/");
    char* lasttoken;
    while (token!=NULL){
        lasttoken =token;
        token= strtok(NULL,"/");
    }
    fprintf(sigh,"%s\"};\n",lasttoken);
    fprintf(sigh,"int32_t sizes[%d]={",n);
    for(int i=0; i<n-1 ;i++) {
        fprintf(sigh,"%d,",sizes[i]);
    }
    fprintf(sigh,"%d};\n",sizes[n-1]);
    fprintf(sigh,"static int32_t adresses[%d][%d]={",n,ss);//Todo
    for(int i=0; i<n-1 ;i++) {
        fprintf(sigh,"{");
        for(int k=0; k<sizes[i]-1 ;k++) {
            fprintf(sigh,"%d,",adresses[i][k]);
        }
        for(int k=sizes[i]; k<ss-1 ;k++) {
            fprintf(sigh,"0,");
        }
        if(sizes[i]==ss) {
            fprintf(sigh, "%d},", adresses[i][ss-1]);
        }else {
            fprintf(sigh,"0},");
        }
    }
    fprintf(sigh,"{");
    for(int k=0; k<sizes[n-1]-1 ;k++) {
        fprintf(sigh,"%d,",adresses[n-1][k]);
    }
    for(int k=sizes[n-1]; k<ss-1 ;k++) {
        fprintf(sigh,"0,");
    }
    if(sizes[n-1]==ss) {
        fprintf(sigh, "%d}};\n", adresses[n-1][ss-1]);
    }else {
        fprintf(sigh,"0}};\n");
    }



    fprintf(sigh,"static int32_t signatures[%d][%d]={",n,ss);//Todo
    for(int i=0; i<n-1 ;i++) {
        fprintf(sigh,"{");
        for(int k=0; k<sizes[i]-1 ;k++) {
            fprintf(sigh,"%d,",signs[i][k]);
        }
        for(int k=sizes[i]; k<ss-1 ;k++) {
            fprintf(sigh,"0,");
        }
        if(sizes[i]==ss) {
            fprintf(sigh, "%d},", signs[i][ss-1]);
        }else {
            fprintf(sigh,"0},");
        }
    }
    fprintf(sigh,"{");
    for(int k=0; k<sizes[n-1]-1 ;k++) {
        fprintf(sigh,"%d,",signs[n-1][k]);
    }
    for(int k=sizes[n-1]; k<ss-1 ;k++) {
        fprintf(sigh,"0,");
    }
    if(sizes[n-1]==ss) {
        fprintf(sigh, "%d}};\n", signs[n-1][ss-1]);
    }else {
        fprintf(sigh,"0}};\n");
    }
    fprintf(sigh,"#endif //SYSTEM_SEC_MINIX_GROUP_1_SIGNATURES_H");
    fclose(sigh);

    
    return 0;
}
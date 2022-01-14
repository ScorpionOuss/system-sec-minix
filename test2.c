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

int main(int argc, char** argv){
    Elf32_Ehdr elfHdr;
    Elf32_Shdr sectHdr;
    Elf32_Shdr textSec;
    Elf32_Phdr progHeader;

    FILE* elfFile;
    char* sectNames = NULL;

    if(argc != 2){
        perror("Parameters error");
        return 1;
    }

    elfFile = fopen(argv[1], "r");
    if(elfFile == NULL){
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
    printf("vadrr:%d",progHeader.p_vaddr);









    printf("The offset is %ld and the size is %ld \n", offset, size);
    int32_t * signatures= malloc(sizeof(int32_t)*size/4096);
    char *page = malloc(4096*sizeof(char));
    for (int c = 0; c < size/4096; c++){
        fseek(elfFile, offset + c*4096, SEEK_SET);
        fread(page, 1, 4096*sizeof(char), elfFile);
        //sign here and append signature
        int32_t sign = 0;
        for (int i=0; i<4096; i+=32){
            sign^= *((int32_t *) (page+i));
        }
        signatures[c]=sign;
        printf("%ld : %d\n",progHeader.p_vaddr+offset + c*4096,sign);
    }
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
    return 0;
}
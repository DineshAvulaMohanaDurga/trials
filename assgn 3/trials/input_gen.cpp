#include<stdio.h>
#include<stdlib.h>
#include<random>
#include "basic.h"

int main()
{
    FILE* ptr[NUM_FILES];
    char filename[20];
    char foldername[20];
    srand(time(NULL));
    int proc_time=rand()%50;
    for(unsigned k=2;k<11;k++)
    {
        for(unsigned i=0;i<NUM_FILES;i++)
        {
            sprintf(filename,"%dprocesses/trial%d",k,i);
            ptr[i]=fopen(filename,"w");
        }

        for(unsigned i=0;i<NUM_FILES;i++)
        {
            fprintf(ptr[i],"%d\n",NUM_PROC);
            for(unsigned j=0;j<NUM_PROC;j++)
            {
                proc_time=rand()%50;
                fprintf(ptr[i],"%d %d %d %d \n",i,proc_time,proc_time+(rand()%20),NUM_TIMES);
            }
        }
    }
    return EXIT_SUCCESS;
}
#include "stdio.h"
#include "stdlib.h"
#include <iostream>
#include <string>
#include <string.h>
#include <stdbool.h>

using namespace std;

#define NUM_PROC 8
#define NUM_TIMES 10
#define NUM_FILES 100
#define CONTEXT_TIME 10
//This class is used to store the data from the input file
class Input_data_t
{
    public:
    int period;
    int num_times;
    int processing_time;
    float waiting_time=0;
    float prev_end_time=0;
    int num_deadlines_missed=0;
    char process_name[20];
};
//This class is used to store the order in thich the processes are to be executed
class Scheduling_set_t
{
public:
    int deadline_time;
    int processing_time;
    char process_name[20];
    int process_num;
};

void run_stimulation(Input_data_t input_data[],Scheduling_set_t sched_set[],int num_processes,int max_deadline,int total_num_times,FILE *ptr)
{
    for(unsigned i=0;i<num_processes;i++)
    {
        //cout<<"Process"<<input_data[i].process_name<<":process time "<<input_data[i].processing_time<<";Deadline "<<input_data[i].period<<";period "<<input_data[i].period<<";joined the system at time 0"<<endl;
        fprintf(ptr,"Process %s:process time %d, deadline %d, period %d;joined the system at time 0\n",input_data[i].process_name,input_data[i].processing_time,input_data[i].period,input_data[i].period);
    }
    int processed_time=0;
    int context_switch_time=0;
    int set_entry_num=0;
    //bool switch_proc=false;
    //cout<<"Process"<<sched_set[set_entry_num].process_name<<" starts execution at time 0"<<endl;
    fprintf(ptr,"Process %s starts execution at time 0\n",input_data[0].process_name);
    for(int i=0;i<max_deadline;i++)
    {
        if(processed_time==sched_set[set_entry_num].processing_time)
        {
            //if(!switch_proc)
            
            //cout<<"Process"<<sched_set[set_entry_num].process_name<<" finishes execution at time "<<i<<endl;
            fprintf(ptr,"Process %s finishes execution at time %d\n",sched_set[set_entry_num].process_name,i);
            input_data[sched_set[set_entry_num].process_num].prev_end_time=i+(context_switch_time*CONTEXT_TIME*0.001);
            //switch_proc=false;
            set_entry_num++;
            processed_time=0;
            if(set_entry_num!=total_num_times)
            {
                //cout<<"Process"<<sched_set[set_entry_num].process_name<<" starts execution at time "<<i<<endl;
                fprintf(ptr,"Process %s starts execution at time %d\n",sched_set[set_entry_num].process_name,i);
                input_data[sched_set[set_entry_num].process_num].waiting_time+=(i+(context_switch_time*CONTEXT_TIME*0.001)-input_data[sched_set[set_entry_num].process_num].prev_end_time);

                context_switch_time+=CONTEXT_TIME;
            }
            else
            {
                break ;
            }
        }
        processed_time++;
        if(i>=sched_set[set_entry_num].deadline_time&&processed_time<sched_set[set_entry_num].processing_time)
        {
            //cout<<"Process"<<sched_set[set_entry_num].process_name<<" deadline time "<<sched_set[set_entry_num].deadline_time<<" missed"<<endl;
            fprintf(ptr,"Process %s missed deadline at time %d\n",sched_set[set_entry_num].process_name,sched_set[set_entry_num].deadline_time);
            input_data[sched_set[set_entry_num].process_num].num_deadlines_missed++;
            //processed_time=0;
            //set_entry_num++;
            //switch_proc=true;
            //i--;
        }
    }
}

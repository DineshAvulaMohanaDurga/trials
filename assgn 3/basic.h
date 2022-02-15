#include "stdio.h"
#include "stdlib.h"
#include <iostream>
#include <string>
#include <string.h>
#include <stdbool.h>

using namespace std;

//This class is used to store the data from the input file
class Input_data_t
{
    public:
    int period;
    int num_times;
    int processing_time;
    char process_name[20];
    
    int waiting_time=0;
    int prev_end_time=0;
    int next_period_time=0;
    int num_deadlines_missed=0;

};
//This class is used to store the order in thich the processes are to be executed
class Scheduling_set_t
{
public:
    int deadline_time;
    int processing_time;
    int period;
    char process_name[20];
    int process_num;
    bool deadline_missed=false;
};

//This function initialises a element in scheduling_set
void initialise_sched_set(Scheduling_set_t *sched)
{
    sched->deadline_time=-1;
    sched->period=-1;
    strcpy(sched->process_name," ");
    sched->process_num=-1;
    sched->processing_time=-1;
    sched->deadline_missed=false;
    return ;
}

//This function adds data of a process to scheduling set 
void add_process(Input_data_t input_data[],Scheduling_set_t scheduling_set[],int time,int num_processes,int* num_processes_in_queue,bool *added)
{
    for(unsigned i=0;i<num_processes;i++)
    {
        if(time==input_data[i].next_period_time&&(time<input_data[i].period*input_data[i].num_times))
        {
            input_data[i].next_period_time=input_data[i].next_period_time+input_data[i].period;
            scheduling_set[*num_processes_in_queue].deadline_time=input_data[i].next_period_time;
            scheduling_set[*num_processes_in_queue].process_num=i;
            scheduling_set[*num_processes_in_queue].processing_time=input_data[i].processing_time;
            scheduling_set[*num_processes_in_queue].period=input_data[i].period;
            strcpy(scheduling_set[*num_processes_in_queue].process_name,input_data[i].process_name);
            *num_processes_in_queue=*num_processes_in_queue+1;
            *added=true;
        }
    }
}

//This function removes data of a process in the given location from the sceduling set
void remove_process(Scheduling_set_t sched_set[],int num_processes_in_queue,int num_in_queue)
{
    for(unsigned i=num_in_queue;i<num_processes_in_queue-1;i++)
    {
        sched_set[i]=sched_set[i+1];
    }
    initialise_sched_set(&sched_set[num_processes_in_queue-1]);
}

int find_max_deadline(Input_data_t input_data[],int num_processes)
{
    int max_deadline=0;
    for(unsigned i=0;i<num_processes;i++)
    {
        if(max_deadline<input_data[i].next_period_time)
        {
            max_deadline=input_data[i].next_period_time;
        }
    }
    return max_deadline;
}

//this function takes input from the respective order(RMS or EDF) and schedules them
void run_stimulation(Input_data_t input_data[],Scheduling_set_t sched_set[],int num_processes,int tot_num_process_in_queue,int total_num_times,FILE *ptr,void (*sort_algo)(Input_data_t* ,Scheduling_set_t* ,int ,int ))
{
    int current_process_num=0;
    bool added=false;
    int max_deadline=find_max_deadline(input_data,num_processes);
    int num_processes_in_queue=tot_num_process_in_queue;
    int processed_time=0;
    int num_processes_completed=0;
    bool busy=false;
    bool empty=false;
 
    for(unsigned i=0;i<num_processes;i++)
    {
        fprintf(ptr,"Process %s:process time %d, deadline %d, period %d;joined the system at time 0\n",input_data[i].process_name,input_data[i].processing_time,input_data[i].period,input_data[i].period);
    }
    for(int i=0;(i<max_deadline)||(num_processes_in_queue>0);i++)
    {
        if(i==50)
        {
            int stop=1;
        }
       
        if(num_processes_in_queue==0)
        {
            empty=true;
        }
        add_process(input_data,sched_set,i,num_processes,&num_processes_in_queue,&added);
        if(added)
        {
            sort_algo(input_data,sched_set,num_processes,num_processes_in_queue);
            max_deadline=find_max_deadline(input_data,num_processes);
            if(!empty&&current_process_num!=sched_set[0].process_num)
            {
                fprintf(ptr,"Process %s preempted by process %s at time %d; Remaining time of Process %s is %d\n",input_data[current_process_num].process_name,sched_set[0].process_name,i,input_data[current_process_num].process_name,input_data[current_process_num].next_period_time-i);
                input_data[current_process_num].prev_end_time=i;
                current_process_num=sched_set[0].process_num;
                processed_time=0;
            }
            if(empty)
            {
                processed_time=0;
            }
            added=false;
            empty=false;
        }
        if(!busy&&num_processes_in_queue>0)
        {
            if(num_processes_completed!=total_num_times)
            {
                fprintf(ptr,"Process %s starts execution at time %d\n",sched_set[0].process_name,(i==0)?i:i-1);
                input_data[sched_set[0].process_num].waiting_time+=(i-input_data[sched_set[0].process_num].prev_end_time);
                busy=true;
                current_process_num=sched_set[0].process_num;
            }
            else
            {
//If the scheduling is completed the stimulation exits here                
                return ;
            }
        }
        if(busy&&processed_time==sched_set[0].processing_time)
        {
            
            fprintf(ptr,"Process %s finishes execution at time %d\n",sched_set[0].process_name,i);
            input_data[sched_set[0].process_num].prev_end_time=i;
            num_processes_completed++;
            processed_time=0;
            remove_process(sched_set,num_processes_in_queue,0);
            num_processes_in_queue--;
            busy=false;
        }
        
        for(unsigned j=0;j<num_processes_in_queue;j++)
        {
            if(i>sched_set[j].deadline_time&&(j!=0||processed_time<sched_set[j].processing_time)&&!sched_set[j].deadline_missed)
            {
                fprintf(ptr,"Process %s missed deadline at time %d\n",sched_set[j].process_name,sched_set[j].deadline_time);
                input_data[sched_set[j].process_num].num_deadlines_missed++;
                sched_set[j].deadline_missed=true;
            }
        }
 
        processed_time++;
    }
}

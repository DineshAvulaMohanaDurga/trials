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
};
//This class is used to store the order in thich the processes are to be executed
class Scheduling_set_t
{
public:
    int deadline_time;
    int processing_time;
    char process_name[20];
};
//This function reorders the scheduling set according to earliest deadline first order
void sort_sched_set_edf(Scheduling_set_t sched_set[],int num_sets,int* max_deadline)
{
    for(unsigned i=0;i<num_sets;i++)
    {
        for(unsigned j=i+1;j<num_sets;j++)
        {
            if(sched_set[j].deadline_time<sched_set[i].deadline_time)
            {
                Scheduling_set_t temp_set;
                temp_set=sched_set[i];
                sched_set[i]=sched_set[j];
                sched_set[j]=temp_set;
            }
        }
    }

    *max_deadline=sched_set[num_sets-1].deadline_time;
    return ;
}

//This function reorders the scheduling set according to rate monotonic scheduling order
void sort_sched_set_rms(Input_data_t input_data[],Scheduling_set_t sched_set[],int num_processes,int max_times,int *max_deadline)
{
    *max_deadline=0;
    Input_data_t temp_data[num_processes];
    for(unsigned i=0;i<num_processes;i++)
    {
        temp_data[i]=input_data[i];
    }
    for(unsigned i=0;i<num_processes;i++)
    {
        for(unsigned j=i+1;j<num_processes;j++)
        {
            if(temp_data[j].period<temp_data[i].period)
            {
                Input_data_t temp=temp_data[i];
                temp_data[i]=temp_data[j];
                temp_data[j]=temp;
            }
        }
    }
    int entry_num=0;
    for(unsigned i=0;i<max_times;i++)
    {
        for(unsigned j=0;j<num_processes;j++)
        {
            if(i<temp_data[j].num_times)
            {
                strcpy(sched_set[entry_num].process_name,temp_data[j].process_name);
                sched_set[entry_num].deadline_time=(i+1)*input_data[j].period;
                if(*max_deadline<sched_set[entry_num].deadline_time)
                {
                    *max_deadline=sched_set[entry_num].deadline_time;
                }
                sched_set[entry_num].processing_time=input_data[j].processing_time;
            }
        }
    }
    return ;
}

void run_stimulation(Input_data_t input_data[],Scheduling_set_t sched_set[],int num_processes,int max_deadline,int total_num_times,FILE *ptr)
{
    for(unsigned i=0;i<num_processes;i++)
    {
        //cout<<"Process"<<input_data[i].process_name<<":process time "<<input_data[i].processing_time<<";Deadline "<<input_data[i].period<<";period "<<input_data[i].period<<";joined the system at time 0"<<endl;
        fprintf(ptr,"Process %s:process time %d, deadline %d, period %d;joined the system at time 0\n",input_data[i].process_name,input_data[i].processing_time,input_data[i].period,input_data[i].period);
    }
    int temp=0;
    int set_entry_num=0;
    //bool switch_proc=false;
    //cout<<"Process"<<sched_set[set_entry_num].process_name<<" starts execution at time 0"<<endl;
    fprintf(ptr,"Process %s starts execution at time 0\n",input_data[0].process_name);
    for(int i=0;i<max_deadline;i++)
    {
        if(temp==sched_set[set_entry_num].processing_time)
        {
            //if(!switch_proc)
            
            //cout<<"Process"<<sched_set[set_entry_num].process_name<<" finishes execution at time "<<i<<endl;
            fprintf(ptr,"Process %s finishes execution at time %d\n",sched_set[set_entry_num].process_name,i);
            
            //switch_proc=false;
            set_entry_num++;
            temp=0;
            if(set_entry_num!=total_num_times)
            {
                //cout<<"Process"<<sched_set[set_entry_num].process_name<<" starts execution at time "<<i<<endl;
                fprintf(ptr,"Process %s starts execution at time %d\n",sched_set[set_entry_num].process_name,i);
            }
            else
            {
                break ;
            }
        }
        temp++;
        if(i>=sched_set[set_entry_num].deadline_time&&temp<sched_set[set_entry_num].processing_time)
        {
            //cout<<"Process"<<sched_set[set_entry_num].process_name<<" deadline time "<<sched_set[set_entry_num].deadline_time<<" missed"<<endl;
            fprintf(ptr,"Process %s missed deadline at time %d\n",sched_set[set_entry_num].process_name,sched_set[set_entry_num].deadline_time);
            //temp=0;
            //set_entry_num++;
            //switch_proc=true;
            //i--;
        }
    }
}

int main()
{
    int num_processes = 5;
    int total_num_times=0;
    int max_num_times=0;
    int max_deadline=0;
    //cout<<"Enter the number of processes"<<endl;
    //cin>>num_processes;
    FILE *ptr;
    FILE *ptr1;
    FILE *ptr2;
    ptr=fopen("inp-params1.txt","r");
    ptr1=fopen("RM-Log.txt","w");
    ptr2=fopen("EDF-Log.txt","w");
    if(ptr==NULL)
    {
        cout<<"Error in opening the file"<<endl;
    }
    fscanf(ptr,"%d\n",&num_processes);
    
    Input_data_t input_data[num_processes];
    for(unsigned i=0;i<num_processes;i++)
    {
        fscanf(ptr,"%s , %d , %d , %d\n",input_data[i].process_name,&input_data[i].processing_time,&input_data[i].period,&input_data[i].num_times);
    }
    for(unsigned i=0;i<num_processes;i++)
    {
        total_num_times=total_num_times+input_data[i].num_times;
        if(max_num_times<input_data[i].num_times)
        {
            max_num_times=input_data[i].num_times;
        }
    }
    Scheduling_set_t sched_set_edf[total_num_times];
    Scheduling_set_t sched_set_rms[total_num_times];
    int temp=0;
    for(unsigned i=0;i<num_processes;i++)
    {
        for(unsigned j=0;j<input_data[i].num_times;j++)
        {
            strcpy(sched_set_edf[temp].process_name,input_data[i].process_name);
            sched_set_edf[temp].processing_time=input_data[i].processing_time;
            sched_set_edf[temp].deadline_time=(j+1)*input_data[i].period;
            temp++;
        }
    }
    sort_sched_set_edf(sched_set_edf,total_num_times,&max_deadline);
    //max_deadline=sched_set_edf[total_num_times-1].deadline_time;
    sort_sched_set_rms(input_data,sched_set_rms,num_processes,max_num_times,&max_deadline);
    /* for(unsigned i=0;i<total_num_times;i++)
    {
        cout<<"PID is"<<sched_set_edf[i].process_name<<",deadline is "<<sched_set_edf[i].deadline_time<<endl;
    }*/
    int set_entry_num=0;
    temp=0;
    /*for(unsigned i=0;i<num_processes;i++)
    {
        cout<<"Process"<<input_data[i].process_name<<":process time "<<input_data[i].processing_time<<";Deadline "<<input_data[i].period<<";period "<<input_data[i].period<<";joined the system at time 0"<<endl;
    }
    for(unsigned i=0;i<max_deadline;i++)
    {
        if(temp==sched_set_edf[set_entry_num].processing_time)
        {
            cout<<"Process"<<sched_set_edf[set_entry_num].process_name<<" finishes execution at time "<<i<<endl;
            set_entry_num++;
            temp=0;
            if(set_entry_num!=total_num_times)
            {
                cout<<"Process"<<sched_set_edf[set_entry_num].process_name<<" starts execution at time "<<i<<endl;
            }
            else
            {
                break ;
            }
        }
        temp++;
        if(i>sched_set_edf[set_entry_num].deadline_time)
        {
            cout<<"Process"<<sched_set_edf[set_entry_num].process_name<<" deadline time "<<sched_set_edf[set_entry_num].deadline_time<<" missed"<<endl;
            temp=0;
            set_entry_num++;
        }
    }*/
    run_stimulation(input_data,sched_set_rms,num_processes,max_deadline,total_num_times,ptr1);
    run_stimulation(input_data,sched_set_edf,num_processes,max_deadline,total_num_times,ptr2);
    return EXIT_SUCCESS;
}
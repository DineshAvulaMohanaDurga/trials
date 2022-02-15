#include "basic.h"

//This function reorders the scheduling set according to rate monotonic scheduling order
void sort_sched_set_rms(Input_data_t input_data[],Scheduling_set_t sched_set[],int num_processes,int num_processes_in_queue)
{   
   
    for(unsigned i=0;i<num_processes_in_queue;i++)
    {
        for(unsigned j=i+1;j<num_processes_in_queue;j++)
        {
            if(sched_set[j].period<sched_set[i].period)
            {
                Scheduling_set_t temp_set;
                temp_set=sched_set[i];
                sched_set[i]=sched_set[j];
                sched_set[j]=temp_set;
            }
        }
    }

    return ;
}

void create_sched_set_rms(Input_data_t input_data[],Scheduling_set_t sched_set[],int num_processes,int *num_processes_in_queue)
{
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
    for(unsigned j=0;j<num_processes;j++)
    {
        strcpy(sched_set[j].process_name,temp_data[j].process_name);
        sched_set[j].deadline_time=input_data[j].period;
        sched_set[j].processing_time=input_data[j].processing_time;
        sched_set[j].process_num=j;
        sched_set[j].period=input_data[j].period;

        *num_processes_in_queue=*num_processes_in_queue+1;
        input_data[j].next_period_time=input_data[j].period;
    }
    return ;
}

int main()
{
    int num_processes = 5;
    int total_num_times=0;
    int max_num_times=0;
    int context_switch_time=0;
    int total_missed_deadlines=0;
    int num_processes_in_queue=0;


    FILE *ptr;
    FILE *ptr1;
    FILE *ptr2;
    ptr=fopen("inp-params.txt","r");
    ptr1=fopen("RMS-Log.txt","w");
    ptr2=fopen("RMS-stats.txt","w");
    if(ptr==NULL||ptr1==NULL||ptr2==NULL)
    {
        cout<<"Error in opening the file"<<endl;
        return EXIT_FAILURE;
    }
    fscanf(ptr,"%d\n",&num_processes);
    
    Input_data_t input_data[num_processes];
    for(unsigned i=0;i<num_processes;i++)
    {
        fscanf(ptr,"%s %d %d %d\n",input_data[i].process_name,&input_data[i].processing_time,&input_data[i].period,&input_data[i].num_times);
    }
    for(unsigned i=0;i<num_processes;i++)
    {
        total_num_times=total_num_times+input_data[i].num_times;
        if(max_num_times<input_data[i].num_times)
        {
            max_num_times=input_data[i].num_times;
        }
    }
    
    Scheduling_set_t sched_set_rms[4*num_processes];
    create_sched_set_rms(input_data,sched_set_rms,num_processes,&num_processes_in_queue);
    sort_sched_set_rms(input_data,sched_set_rms,num_processes,/*max_num_times*/num_processes_in_queue);
    
    run_stimulation(input_data,sched_set_rms,num_processes,/*total_num_times*/num_processes_in_queue,total_num_times,&context_switch_time,ptr1,&sort_sched_set_rms);

    for(unsigned i=0;i<num_processes;i++)
    {
        total_missed_deadlines+=input_data[i].num_deadlines_missed;
    }
    fprintf(ptr2,"The number of processes entered is %d\n",total_num_times);
    fprintf(ptr2,"The number of processes that were successfully completed is %d\n",total_num_times-total_missed_deadlines);
    fprintf(ptr2,"The number of processes whose deadlines were missed is %d\n",total_missed_deadlines);
    for(unsigned i=0;i<num_processes;i++)
    {
        fprintf(ptr2,"Process %s: Average waiting time is %d ms\n",input_data[i].process_name,input_data[i].waiting_time/input_data[i].num_times);
    }
    fprintf(ptr2,"\n The context switch time taken is %d micro seconds\n",context_switch_time);
    return EXIT_SUCCESS;
}
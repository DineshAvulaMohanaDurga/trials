#include "basic.h"

//This function reorders the scheduling set according to earliest deadline first order
void sort_sched_set_edf(Input_data_t input_data[],Scheduling_set_t sched_set[],int num_processes,int num_processes_in_queue)
{   
   
    for(unsigned i=0;i<num_processes_in_queue;i++)
    {
        for(unsigned j=i+1;j<num_processes_in_queue;j++)
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

    return ;
}



int main()
{
    int num_processes = 5;
    int total_num_times=0;
    int max_num_times=0;
    int max_deadline=0;
    int context_switch_time=0;
    int total_missed_deadlines=0;
    int num_processes_in_queue=0;

    FILE *ptr;
    FILE *ptr1;
    FILE *ptr2;

    ptr=fopen("inp-params.txt","r");
    ptr1=fopen("EDF-Log.txt","w");
    ptr2=fopen("EDF-stats.txt","w");
    if(ptr==NULL)
    {
        cout<<"Error in opening the file"<<endl;
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
    Scheduling_set_t sched_set_edf[2*num_processes];
    for(unsigned i=0;i<num_processes;i++)
    {
        strcpy(sched_set_edf[i].process_name,input_data[i].process_name);
        sched_set_edf[i].processing_time=input_data[i].processing_time;
        sched_set_edf[i].process_num=i;
        sched_set_edf[i].deadline_time=input_data[i].period;
        num_processes_in_queue++;
        input_data[i].next_period_time=input_data[i].period;
    }
    sort_sched_set_edf(input_data,sched_set_edf,num_processes,num_processes_in_queue);
   
    run_stimulation(input_data,sched_set_edf,num_processes,num_processes_in_queue,total_num_times,&context_switch_time,ptr1,&sort_sched_set_edf);
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
#include "basic.h"

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
                sched_set[entry_num].process_num=j;
                entry_num++;
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
    int total_missed_deadlines=0;


    //cout<<"Enter the number of processes"<<endl;
    //cin>>num_processes;
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
    
    Scheduling_set_t sched_set_rms[total_num_times];
    //max_deadline=sched_set_edf[total_num_times-1].deadline_time;
    sort_sched_set_rms(input_data,sched_set_rms,num_processes,max_num_times,&max_deadline);
    /* for(unsigned i=0;i<total_num_times;i++)
    {
        cout<<"PID is"<<sched_set_edf[i].process_name<<",deadline is "<<sched_set_edf[i].deadline_time<<endl;
    }
    int set_entry_num=0;
    int temp=0;
    for(unsigned i=0;i<num_processes;i++)
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
    for(unsigned i=0;i<num_processes;i++)
    {
        total_missed_deadlines+=input_data[i].num_deadlines_missed;
    }
    fprintf(ptr2,"The number of processes entered is %d\n",total_num_times);
    fprintf(ptr2,"The number of processes that were successfully completed is %d\n",total_num_times-total_missed_deadlines);
    fprintf(ptr2,"The number of processes whose deadlines were missed is %d\n",total_missed_deadlines);
    for(unsigned i=0;i<num_processes;i++)
    {
        fprintf(ptr2,"Process %s: Average waiting time is %f ms\n",input_data[i].process_name,input_data[i].waiting_time/input_data[i].num_times);
    }
    cout<<total_missed_deadlines<<"+";
    return EXIT_SUCCESS;
}
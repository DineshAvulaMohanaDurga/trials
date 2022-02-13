#include "basic.h"

//This function reorders the scheduling set according to earliest deadline first order
void sort_sched_set_edf(Input_data_t input_data[],Scheduling_set_t sched_set[],int num_processes,int total_num_times,int* max_deadline)
{   
    int temp=0;
    for(unsigned i=0;i<num_processes;i++)
    {
        for(unsigned j=0;j<input_data[i].num_times;j++)
        {
            strcpy(sched_set[temp].process_name,input_data[i].process_name);
            sched_set[temp].processing_time=input_data[i].processing_time;
            sched_set[temp].process_num=i;
            sched_set[temp].deadline_time=(j+1)*input_data[i].period;
            temp++;
        }
    }
    for(unsigned i=0;i<total_num_times;i++)
    {
        for(unsigned j=i+1;j<total_num_times;j++)
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

    *max_deadline=sched_set[total_num_times-1].deadline_time;
    return ;
}



int main()
{
    int num_processes = 5;
    int total_num_times=0;
    int max_num_times=0;
    int max_deadline=0;
    int total_missed_deadlines=0;
    float total_waiting_time=0;
    char filename[20];
    //cout<<"Enter the number of processes"<<endl;
    //cin>>num_processes;
    FILE* ptr[NUM_FILES];
    FILE *ptr1;
    FILE *ptr2;
    for(unsigned k=2;k<11;k++)
    {
        cout<<k<<endl<<endl;
        for(unsigned i=0;i<NUM_FILES;i++)
        {
            sprintf(filename,"%dprocesses/trial%d",k,i);
            ptr[i]=fopen(filename,"r");
        }
    //   ptr=fopen("inp-params1.txt","r");
        ptr1=fopen("EDF-Log.txt","w");
        ptr2=fopen("EDF-stats.txt","w");
        if(ptr==NULL)
        {
            cout<<"Error in opening the file"<<endl;
        }
        for(unsigned times=0;times<NUM_FILES;times++)
        { 
            total_num_times=0;
            max_num_times=0;
            max_deadline=0;
            total_missed_deadlines=0;
            total_waiting_time=0;
            fscanf(ptr[times],"%d\n",&num_processes);
            
            Input_data_t input_data[num_processes];
            for(unsigned i=0;i<num_processes;i++)
            {
                fscanf(ptr[times],"%s %d %d %d\n",input_data[i].process_name,&input_data[i].processing_time,&input_data[i].period,&input_data[i].num_times);
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
            
            sort_sched_set_edf(input_data,sched_set_edf,num_processes,total_num_times,&max_deadline);
            //max_deadline=sched_set_edf[total_num_times-1].deadline_time;
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
            run_stimulation(input_data,sched_set_edf,num_processes,max_deadline,total_num_times,ptr1);
            for(unsigned i=0;i<num_processes;i++)
            {
                total_missed_deadlines+=input_data[i].num_deadlines_missed;
                total_waiting_time+=input_data[i].waiting_time;
            }
            fprintf(ptr2,"The number of processes entered is %d\n",total_num_times);
            fprintf(ptr2,"The number of processes that were successfully completed is %d\n",total_num_times-total_missed_deadlines);
            fprintf(ptr2,"The number of processes whose deadlines were missed is %d\n",total_missed_deadlines);
            for(unsigned i=0;i<num_processes;i++)
            {
                fprintf(ptr2,"Process %s: Average waiting time is %f ms\n",input_data[i].process_name,input_data[i].waiting_time/input_data[i].num_times);
            }
            cout<<total_waiting_time<<"+";
        }
        cout<<"0"<<endl;
    }
    return EXIT_SUCCESS;
}
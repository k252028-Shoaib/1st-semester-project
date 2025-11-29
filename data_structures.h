#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H
#include <time.h>

typedef long long ll;
 
typedef struct subtask subtask;
typedef struct categories categories;
typedef struct task task;
//1. Task
typedef struct task
{
    int task_id;
    char task_name[100];
    char task_description[1000];
    int priority_level;
    time_t start_time; // start time of the task
    time_t due_time; // end time of the task
    time_t reminder_time; 
    int recurring_id;
    char recurring_name[100]; //name of the recurring task series
    int no_of_recurrences; //number of times this task will recur
    int current_recurrence; //current recurrence number
    int interval; //interval in seconds till the next occurance of this task, the start time of the next occurance will be 
                  //calculated by adding this interval to the previous occurance's "base" start time.
    time_t edited_start_time; //this is only for recurring tasks, this will store the edited start time of the current 
                              //occurance, this will not affect other occurances' start time         
    int xth_occurance_start_time; //the xth occurance is the occurance whose start time has been edited.
    time_t recurrence_end_time; //time when the recurrance will stop
    int completed; //is the task completed or not, 0 means no and 1 means yes (will auto complete if all subtasks are comkpleted)
    int auto_complete_enabled;//will complete the task automatically when all subtasks are completed
    int category_id; 
    subtask *subtask_ptr;
    task *prev_task_ptr;
    task *next_task_ptr;
}task;
 
//2. Subtask
typedef struct subtask
{
    int parent_task_id;
    int subtask_id;
    char subtask_name[100];
    char subtask_description[1000];
    int completed;
    subtask *next_subtask_ptr;
}subtask;

//3. show these features or not (using bit flags)
enum display_flags{
    show_description= (1 << 0),
    show_priority= (1 << 1), 
    show_start_date=  (1 << 2),
    show_due_date=  (1 << 3),
    show_completed=  (1 << 4), 
    show_category=  (1 << 5),
    show_subtasks = (1 << 6),
    show_interval = (1 << 7), 
    show_auto_complete_status = (1 << 8), // auto complete when all the subtasks are completed //both calendar and list views
    flag_auto_sort = (1 << 9) // automatically sort the task ids when a task is deleted
};

//4. categories
#define colour_red "\x1b[31m"
#define colour_orange "\x1b[38;5;208m"
#define colour_yellow "\x1b[33m"
#define colour_green "\x1b[32m"
#define colour_blue "\x1b[34m"
#define colour_cyan "\x1b[36m"
#define colour_magenta "\x1b[35m"
#define colour_brown "\x1b[38;5;94m"
#define colour_reset "\x1b[0m"
#define colour_bg_white "\x1b[30;47m"

typedef struct categories
{
    int category_id;
    char name[20];
    char colour[14];
}categories;

//variables:
extern task *head_ptr;
extern task *tail_ptr;
extern int next_available_id;
extern int next_available_recurring_id;
extern categories category[20];
extern int category_amount;
extern int setting_flags;

#endif
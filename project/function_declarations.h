#ifndef FUNCTION_DECLARATIONS_H
#define FUNCTION_DECLARATIONS_H
#include "data_structures.h"
 
char* get_month_name(int month_no);
char* get_day_name(int day_no);
void get_normal_time_format(int date_array[], time_t time);
void get_time_in_seconds(int date_array[], time_t *time);
void calendar_view();
void list_view();

//linked list for tasks
void add_task();
void complete_task(int task_id);
void edit_task(int task_id);
void delete_task(int task_id);
void uncomplete_task(int task_id);
void display_some_task_details(task* task_ptr);
void display_complete_task_details(task* task_ptr);
void get_current_recurrence_number(task *ptr);
task* find_task_by_id(int task_id);
task* find_task_by_date(task *ptr, int date_array1[]);

//linked list for subtasks
void add_subtask(task *parent_task);
void complete_subtask (int task_id, int subtask_id);
void edit_subtask (int task_id, int subtask_id);
void delete_subtask (int task_id, int subtask_id);
void uncomplete_subtask(int task_id, int subtask_id);
subtask* find_subtask_by_id(task *task_ptr, int subtask_id);

//categories (array)
void add_category ();
void display_categories();
void edit_category();
void delete_category();

//settings (bit flags)
void change_settings();
void display_current_settings();

//extras
void auto_complete(task *ptr);
void auto_sort_f();
//will be added later:
//void reminders(){}
//void manual_sort(){}
//void view_missing_tasks(){} and also display them
//find recurring tasks by recurring series name
//display subtasks separately
//display today's tasks

//saving and loading(reading) data
void load_from_file();//done
void save_to_file();//done
 
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function_declarations.h"
 
task *tail_ptr = NULL;
task *head_ptr = NULL;
int next_available_id = 1;
int next_available_recurring_id = 1;
categories category[20];
int category_amount = 0;
int setting_flags = 1023;

//display information
char* get_month_name(int month_no){
    switch (month_no)
    {
        case 0:
            return "January";
            break;
        case 1:
            return "February";
            break;
        case 2:
            return "March";
            break;
        case 3:
            return "April";
            break;
        case 4:
            return "May";
            break;
        case 5:
            return "June";
            break;
        case 6:
            return "July";
            break;
        case 7:
            return "August";
            break;
        case 8:
            return "September";
            break;
        case 9:
            return "October";
            break;
        case 10:
            return "November";
            break;
        case 11:
            return "December";
            break;        
        default:
            return "Invalid month";
            break;
    }
}
char* get_day_name(int day_no){
    switch (day_no)
    {
        case 0:
            return "Sunday";
            break;
        case 1:
            return "Monday";
            break;
        case 2:
            return "Tuesday";
            break;
        case 3:
            return "Wednesday";
            break;
        case 4:
            return "Thursday";
            break;
        case 5:
            return "Friday";
            break;
        case 6:
            return "Saturday";
            break;      
        default:
            return "Invalid day";
            break;
    }
}
void get_normal_time_format(int date_array[], time_t time){
    if (time == -1)
    {
        return;
    }
    
    struct tm *normal_time_format = localtime(&time);
    if (normal_time_format == NULL)
    {
        printf("Error converting time to normal format.\n");
        for (int i=0;i<6;i++) date_array[i]=0;
        return;
    }
    date_array[0] = (normal_time_format->tm_year) + 1900; //tm stores years from 1900 so need to + 1900
    date_array[1] = (normal_time_format->tm_mon); //tm stores month since jan (0-11)
    date_array[2] = (normal_time_format->tm_mday); //day of the month(1-31)
    date_array[3] = (normal_time_format->tm_wday); //day of the week (0-6, sun-sat)
    date_array[4] = (normal_time_format->tm_hour); //hour of the day(0-23)
    date_array[5] = (normal_time_format->tm_min); //min of the day (0-59)
}
void get_time_in_seconds(int date_array[], time_t *time){
    struct tm normal_time_format = {0};
    if (date_array[0] == 0 && date_array[1] == 0 && date_array[2] == 0 && date_array[3] == 0 && date_array[4] == 0){
        *time = -1;
        return;
    }    

    normal_time_format.tm_year = date_array[0] - 1900; //tm stores years from 1900 so need to - 1900
    normal_time_format.tm_mon = date_array[1] - 1; //tm stores month since jan (0-11)
    normal_time_format.tm_mday = date_array[2]; //day of the month(1-31)
    normal_time_format.tm_hour = date_array[3]; //hour of the day(0-23)
    normal_time_format.tm_min = date_array[4]; //min of the day (0-59)
    normal_time_format.tm_sec = 0; //seconds
    normal_time_format.tm_isdst = -1; //not considering daylight saving time

    *time = mktime(&normal_time_format);
    if (*time == -1)
    {
        printf("Error converting normal format to time in seconds.\n");
    }
}
void calendar_view(){
    int current_date_array[6]; //stores year, month, day of the month(1-31), day of the week (0-6, sun-sat), hour of the day(0-23), min of the day (0-59)
    int date_array[6]; // same, but for other dates
    char choice = ' ';//initialising choice variable so that the do-while loop works

    printf("---------------------- Calendar view ----------------------\n");
    //today's date:
    time_t current_time = time(NULL);
    get_normal_time_format(current_date_array, current_time);
    printf("Today's Date is : %s %d, %d \n", get_month_name(current_date_array[1]), current_date_array[2], current_date_array[0]);
    
    //calculating the date of sunday of the current week:
    int week_no = 0; //0 = current week
    time_t current_sunday = current_time - (time_t)((current_date_array[3])*24*60*60);//to get the time of the current sunday
    
    do
    {
        //changing week number:
        printf("-----------------------------------------------------------\n");
        if (choice == 'N' || choice == 'n')
        {
            week_no++;
        }
        else if (choice == 'P' || choice == 'p')
        {
            week_no--;
        }

        //displaying the time frame of the week:
        time_t week_start = current_sunday + (time_t)(week_no * 7LL * 24 * 60 * 60); // compute from base each time
        get_normal_time_format(date_array, week_start);
        printf("Displaying %s %d, %d to ", get_month_name(date_array[1]), date_array[2], date_array[0]);

        time_t date_on_saturday = week_start + (time_t)(6*24*60*60); //end of the week
        get_normal_time_format(date_array, date_on_saturday);
        printf("%s %d, %d\n", get_month_name(date_array[1]), date_array[2], date_array[0]);
        printf("-----------------------------------------------------------\n");

        //displaying the tasks (only name & id):
        for (int i = 0; i < 7; i++)
        {
            printf("%s:\n", get_day_name(i));
            task *next_ptr = head_ptr;
            get_normal_time_format(date_array, (week_start+(i*24*60*60)));
            //change background colour to gray/white if the day being displayed is today:
            if (current_date_array[0] == date_array[0] && current_date_array[1] == date_array[1] && current_date_array[2] == date_array[2])
            {
                printf("%s", colour_bg_white);
            }
            int count = 1;
            while ((next_ptr = find_task_by_date(next_ptr, date_array)) != NULL)
            {
                printf("\t%d.\n",count);
                count++;
                display_some_task_details(next_ptr);
                next_ptr = next_ptr->next_task_ptr;
            }
            printf("%s\n", colour_reset);
            printf("-------------------------------------------\n");
        }

        //menu options
        printf("Do you want to: \n1. see the next week[N] \n2. see the previous week[P] \n3. View task details[D] \n4. exit back to the main menu[E]\nEnter your choice: ");
        scanf(" %c", &choice);

        //viewing task details
        while (choice == 'D' || choice == 'd')
        {
            int task_id;
            printf("Enter the task id to view details: ");
            scanf(" %d", &task_id);
            task *task_ptr = find_task_by_id(task_id);
            if (task_ptr == NULL) 
            {
                printf("Task with id %d not found.\n", task_id);
            } 
            else 
            {
                display_complete_task_details(task_ptr);
            }

            printf("Do you want to: \n1. Go back to the calendar view [C] \n2. View task details[D] \n3. exit back to the main menu[E]: ");
            scanf(" %c", &choice);
        }
        
    } while (choice != 'E' && choice != 'e');
}
void list_view(){
    printf("------------------------- List view -------------------------\n");
    task *ptr = head_ptr;
    int page = 0;// 0 = first page
    char choice = ' ';//initialising choice variable so that the do-while loop works
    do
    {
        //changing week number:
        if (choice == 'N' || choice == 'n')
        {
            page++;
        }
        else if (choice == 'P' || choice == 'p')
        {
            page = (page > 0) ? page - 1 : 0;
        }
        int count = 1 + (page*10);
        printf("Displaying tasks %d - %d:\n", count, count+9);

        //Going to the correct page:
        ptr = head_ptr;
        int i;
        for (i = 0; i < (page*10) && ptr != NULL; i++)
        {
            ptr = ptr->next_task_ptr;
        }

        //checking if there are tasks to display on this page
        if (ptr == NULL) // not enough tasks to display on this page (i.e. user went to a page number greater than available)
        {
            printf("No more tasks to display. Please either exit to main menu or go to a previous page.\n");
        }

        else
        {
            printf("-----------------------------------------------------------\n");
            //displaying the tasks (only name & id):
            for (count; count <= (page*10)+10 && (ptr != NULL); count++)
            {
                printf("\t%d. \n", count);
                display_some_task_details(ptr);
                ptr = ptr->next_task_ptr;
            }
        }
        
        //menu options
        int has_prev = (page > 0);//to check if there is a previous page
        int has_next = (ptr != NULL);//to check if there is a next page
        printf("Do you want to: \n");
        if (has_next && has_prev) 
        {
            printf("1. see the next page[N] \n2. see the previous page[P] \n3. View task details[D] \n4. exit back to the main menu[E]: \n");
        } 
        else if (has_next) 
        {
            printf("1. see the next page[N] \n2. View task details[D] \n3. exit back to the main menu[E]: \n");
        } 
        else if (has_prev) 
        {
            printf("1. see the previous page[P] \n2. View task details[D] \n3. exit back to the main menu[E]: \n");
        } 
        else 
        {
            printf("1. View task details[D] \n2. exit back to the main menu[E]: \n");
        }
        scanf(" %c", &choice);

        //viewing task details
        while (choice == 'D' || choice == 'd')
        {
            int task_id;
            printf("Enter the task id to view details: ");
            scanf(" %d", &task_id);
            task *task_ptr = find_task_by_id(task_id);
            if (task_ptr == NULL) 
            {
                printf("Task with id %d not found.\n", task_id);
            } 
            else 
            {
                display_complete_task_details(task_ptr);
            }

            printf("Do you want to: \n1. Go back to the list view [C] \n2. View task details[D] \n3. exit back to the main menu[E]: ");
            scanf(" %c", &choice);
        }
    } while (choice != 'E' && choice != 'e');
}
 
//tasks (linked list)
void add_task(){
    task *new_task;
    if (head_ptr == NULL){
        head_ptr = (task *)malloc(sizeof(task));
        new_task = head_ptr;
    }
    else {
        new_task = (task *)malloc(sizeof(task));
        tail_ptr->next_task_ptr = new_task;
        new_task->prev_task_ptr = tail_ptr;
    }
    tail_ptr = new_task;
    tail_ptr->task_id = next_available_id;
    next_available_id++;
    printf("Enter task name: ");
    scanf(" %99[^\n]", tail_ptr->task_name);

    printf("Enter task description: ");
    scanf(" %999[^\n]", tail_ptr->task_description);

    printf("Enter task priority level (1-100, 1 being URGENT): ");
    scanf(" %d", &tail_ptr->priority_level);

    printf("Enter task start time (YYYY MM DD HH MM): ");
    int date_array[5];
    for (int i = 0; i < 5; i++) {
        scanf(" %d", &date_array[i]);
    }
    get_time_in_seconds(date_array, &tail_ptr->start_time);

    printf("Enter task due time (YYYY MM DD HH MM) (enter 0 0 0 0 0 if no due time): ");
    for (int i = 0; i < 5; i++) {
        scanf(" %d", &date_array[i]);
    }
    get_time_in_seconds(date_array, &tail_ptr->due_time);
    
    printf("Enter task reminder time (YYYY MM DD HH MM) (enter 0 0 0 0 0 if no reminder): ");
    for (int i = 0; i < 5; i++) {
        scanf(" %d", &date_array[i]);
    }
    get_time_in_seconds(date_array, &tail_ptr->reminder_time);

    int rchoice;
    printf("Is the task recurring? (1 for yes, 0 for no): ");
    scanf(" %d", &rchoice);
    if (rchoice == 1)
    {
        tail_ptr->recurring_id = next_available_recurring_id;
        next_available_recurring_id++;
        printf("Enter recurring task series name: ");
        scanf(" %99[^\n]", tail_ptr->recurring_name);
        printf("Enter number of recurrences (enter 0 for infinite recurrences): ");
        scanf(" %d", &tail_ptr->no_of_recurrences);
        printf("Enter recurrence end time (YYYY MM DD HH MM) (enter 0 0 0 0 0 if no end time): ");
        int date_array[5];
        for (int i = 0; i < 5; i++) {
            scanf(" %d", &date_array[i]);
        }
        get_time_in_seconds(date_array, &tail_ptr->recurrence_end_time);
        printf("Enter recurrence interval (yearly[Y], monthly[M], weekly[W], daily[D], hourly[H], custom[C]) NOTE:The recurrence interval is the time from the start date: ");
        int ichoice;
        scanf(" %c", &ichoice);
        if (ichoice == 'y' || ichoice == 'Y')
        {
            tail_ptr->interval = 31536000; //seconds in a year
        }
        else if (ichoice == 'm' || ichoice == 'M')
        {
            tail_ptr->interval = 2592000; //seconds in a month
        }
        else if (ichoice == 'w' || ichoice == 'W')
        {
            tail_ptr->interval = 604800; //seconds in a week
        }
        else if (ichoice == 'd' || ichoice == 'D')
        {
            tail_ptr->interval = 86400; //seconds in a day
        }
        else if (ichoice == 'h' || ichoice == 'H')
        {
            tail_ptr->interval = 3600; //seconds in an hour
        }
        else if (ichoice == 'c' || ichoice == 'C')
        {
            printf("Enter custom interval in seconds (The number of seconds from the start date): ");
            scanf(" %d", &tail_ptr->interval);
        }
        else
        {
            printf("Invalid choice. Setting interval to 0.\n");
            tail_ptr->interval = 0;
        }

    }
    else
    {
        tail_ptr->recurring_id = 0;
        tail_ptr->interval = 0;
        tail_ptr->no_of_recurrences = 0;
        tail_ptr->recurrence_end_time = 0;
        tail_ptr->edited_start_time = 0;
        tail_ptr->current_recurrence = 0;
        tail_ptr->recurring_name[0] = '\0';
    }
    tail_ptr->completed = 0; //not completed
    tail_ptr->auto_complete_enabled = 1; //auto complete enabled by default
    printf("Enter task category id (0 if none): ");
    scanf(" %d", &tail_ptr->category_id);
    int subchoice;
    printf("Does the task have subtasks? (1 for yes, 0 for no): ");
    scanf(" %d", &subchoice);
    if (subchoice == 1)
    {
        add_subtask(tail_ptr);
    }
    else
    {
        tail_ptr->subtask_ptr = NULL;
    }
    tail_ptr->next_task_ptr = NULL;
    printf("New task successfully added with id %d\n", tail_ptr->task_id);
}
void complete_task(int task_id){
    task *task_ptr = find_task_by_id(task_id);
    if (task_ptr == NULL) 
    {
        printf("Task with id %d not found.\n", task_id);
        return;
    } 
    //checking if all subtasks are completed
    subtask *subtask_ptr = task_ptr->subtask_ptr;
    while (subtask_ptr != NULL)
    {
        if (subtask_ptr->completed == 0)
        {
            printf("Cannot complete task with id %d as not all subtasks are completed.\n", task_id);
            return;
        }
        subtask_ptr = subtask_ptr->next_subtask_ptr;
    }
    task_ptr->completed = 1;
    printf("Task with id %d marked as completed.\n", task_id);
}
void edit_task (int task_id){
    task *task_ptr = find_task_by_id(task_id);
    
    if (task_ptr == NULL) 
    {
        printf("Task with id %d not found.\n", task_id);
        return;
    } 

    task temp_task = *task_ptr; // Create a copy of the original task
    printf("Editing task with id %d:\n", task_id);
    char choice;
    int date_array[6]; //stores year, month, day of the month(1-31), day of the week (0-6, sun-sat), hour of the day(0-23), min of the day (0-59)
    char changes[500] = ""; //to store the changes made
    
    printf("Do you want to edit task name? (Y/N): ");
    scanf(" %c", &choice);
    if (choice == 'Y' || choice == 'y')
    {
        printf("Enter new task name (current: %s): ", temp_task.task_name);
        scanf(" %99[^\n]", temp_task.task_name);
        strcat(changes, "Name edited. \n");
    }
    
    printf("Do you want to edit task priority level? (Y/N): ");
    scanf(" %c", &choice);
    if (choice == 'Y' || choice == 'y')
    {
        printf("Enter new task priority level (1-100, 1 being URGENT) (current: %d): ", temp_task.priority_level);
        scanf(" %d", &temp_task.priority_level);
        strcat(changes, "Priority level edited. \n");
    }
        
    printf("Do you want to edit task start time? (Y/N): ");
    scanf(" %c", &choice);
    if (choice == 'Y' || choice == 'y')
    {
        printf("Enter new task start time (YYYY MM DD HH MM) (current: ");
        get_normal_time_format(date_array, temp_task.start_time);
        printf("%s %02d, %d. At %02d:%02d): ", get_month_name(date_array[1]), date_array[2], date_array[0], date_array[4], date_array[5]);
        for (int i = 0; i < 5; i++) {
            scanf(" %d", &date_array[i]);
        }
        get_time_in_seconds(date_array, &temp_task.start_time);
        strcat(changes, "Start time edited. \n");
    }

    printf("Do you want to edit task due time? (Y/N): ");
    scanf(" %c", &choice);
    if (choice == 'Y' || choice == 'y')
    {
        printf("Enter new task due time (YYYY MM DD HH MM) (current: ");
        get_normal_time_format(date_array, temp_task.due_time);
        printf("%s %02d, %d. At %02d:%02d) (enter 0 0 0 0 0 if no due time): ", get_month_name(date_array[1]), date_array[2], date_array[0], date_array[4], date_array[5]);
        for (int i = 0; i < 5; i++) {
            scanf(" %d", &date_array[i]);
        }
        get_time_in_seconds(date_array, &temp_task.due_time);
        strcat(changes, "Due time edited. \n");
    }

    printf("Do you want to edit task reminder time? (Y/N): ");
    scanf(" %c", &choice);
    if (choice == 'Y' || choice == 'y')
    {
        printf("Enter new task reminder time (YYYY MM DD HH MM) (current: ");
        get_normal_time_format(date_array, temp_task.reminder_time);
        printf("%s %02d, %d. At %02d:%02d) (enter 0 0 0 0 0 if no reminder): ", get_month_name(date_array[1]), date_array[2], date_array[0], date_array[4], date_array[5]);
        for (int i = 0; i < 5; i++) {
            scanf(" %d", &date_array[i]);
        }
        get_time_in_seconds(date_array, &temp_task.reminder_time);
        strcat(changes, "Reminder time edited. \n");
    }

    printf("Do you want to edit task recurrence related details? (Y/N): ");
    scanf(" %c", &choice);
    if (choice == 'Y' || choice == 'y')
    {
        //recurrence related edits
        printf("Do you want to edit recurring task series name? (Y/N): ");
        scanf(" %c", &choice);
        if (choice == 'Y' || choice == 'y')
        {
            printf("Enter new recurring task series name (current: %s): ", temp_task.recurring_name);
            scanf(" %99[^\n]", temp_task.recurring_name);
            strcat(changes, "Recurring series name edited. \n");
        }

        printf("Do you want to edit number of recurrences? (Y/N): ");
        scanf(" %c", &choice);
        if (choice == 'Y' || choice == 'y')
        {
            printf("Enter new number of recurrences (enter 0 for infinite recurrences) (current: %d): ", temp_task.no_of_recurrences);
            scanf(" %d", &temp_task.no_of_recurrences);
            strcat(changes, "Number of recurrences edited. \n");
        }

        printf("Do you want to edit recurrence end time? (Y/N): ");
        scanf(" %c", &choice);
        if (choice == 'Y' || choice == 'y')
        {
            printf("Enter new recurrence end time (YYYY MM DD HH MM) (current: ");
            get_normal_time_format(date_array, temp_task.recurrence_end_time);
            printf("%s %02d, %d. At %02d:%02d) (enter 0 0 0 0 0 if no end time): ", get_month_name(date_array[1]), date_array[2], date_array[0], date_array[4], date_array[5]);
            for (int i = 0; i < 5; i++) {
                scanf(" %d", &date_array[i]);
            }
            get_time_in_seconds(date_array, &temp_task.recurrence_end_time);
            strcat(changes, "Recurrence end time edited. \n");
        }

        printf("Do you want to edit recurrence interval? (Y/N): ");
        scanf(" %c", &choice);
        if (choice == 'Y' || choice == 'y')
        {
            printf("Enter recurrence interval (yearly[Y], monthly[M], weekly[W], daily[D], hourly[H], custom[C]) NOTE:The recurrence interval is the time from the start date: ");
            int ichoice;
            scanf(" %c", &ichoice);
            if (ichoice == 'y' || ichoice == 'Y')
            {
                tail_ptr->interval = 31536000; //seconds in a year
                strcat(changes, "Edited the interval of the task to yearly. \n");
            }
            else if (ichoice == 'm' || ichoice == 'M')
            {
                tail_ptr->interval = 2592000; //seconds in a month
                strcat(changes, "Edited the interval of the task to monthly. \n");
            }
            else if (ichoice == 'w' || ichoice == 'W')
            {
                tail_ptr->interval = 604800; //seconds in a week
                strcat(changes, "Edited the interval of the task to weekly. \n");
            }
            else if (ichoice == 'd' || ichoice == 'D')
            {
                tail_ptr->interval = 86400; //seconds in a day
                strcat(changes, "Edited the interval of the task to daily. \n");
            }
            else if (ichoice == 'h' || ichoice == 'H')
            {
                tail_ptr->interval = 3600; //seconds in an hour
                strcat(changes, "Edited the interval of the task to hourly. \n");
            }
            else if (ichoice == 'c' || ichoice == 'C')
            {
                printf("Enter custom interval in seconds (The number of seconds from the start date): ");
                scanf(" %d", &tail_ptr->interval);
                strcat(changes, "Edited the interval of the task to custom. \n");
            }
            else
            {
                printf("Invalid choice. Setting interval to 0.\n");
                tail_ptr->interval = 0;
            }
        }
        printf("Do you want to edit the start time of a specific occurrence in the series? (Y/N): ");
        scanf(" %c", &choice);
        if (choice == 'Y' || choice == 'y')
        {
            printf("Enter occurrence number to edit start time for (1 for first occurrence, 2 for second occurrence, etc.) (current: %d): ", temp_task.current_recurrence);
            int occurrence_no;
            scanf(" %d", &occurrence_no);
            printf("Enter new start time for occurrence %d (YYYY MM DD HH MM): ", occurrence_no);
            for (int i = 0; i < 5; i++) {
                scanf(" %d", &date_array[i]);
            }
            time_t edited_start_time;
            get_time_in_seconds(date_array, &edited_start_time);
            temp_task.edited_start_time = edited_start_time;
            strcat(changes, "Edited start time for specific occurrence. \n");
        }
    }

    printf("Do you want to edit task category? (Y/N): ");
    scanf(" %c", &choice);
    if (choice == 'Y' || choice == 'y')
    {
        printf("Enter new task category id (current: %d) (0 if none): ", temp_task.category_id);
        scanf(" %d", &temp_task.category_id);
        strcat(changes, "Category edited. \n");
    }

    printf("Do you want to edit task auto-complete setting? (Y/N): ");
    scanf(" %c", &choice);
    if (choice == 'Y' || choice == 'y')
    {
        printf("Enter new auto-complete setting (1 for enabled, 0 for disabled) (current: %d): ", temp_task.auto_complete_enabled);
        scanf(" %d", &temp_task.auto_complete_enabled);
        strcat(changes, "Auto-complete setting edited. \n");
    }

    printf("Do you want to edit task's subtasks? (Y/N): ");
    scanf(" %c", &choice);
    if (choice == 'Y' || choice == 'y')
    {
        subtask *sub_ptr = temp_task.subtask_ptr;
        while (sub_ptr != NULL)
        {
            edit_subtask(task_ptr->task_id, sub_ptr->subtask_id);
            sub_ptr = sub_ptr->next_subtask_ptr;
        }
        strcat(changes, "Subtasks edited. \n");
    }

    //applying the changes to the original task
    printf("The following changes were made to task with id %d:\n%s. Do you want to confirm the changes?(Y/N): ", task_id, changes);
    scanf(" %c", &choice);
    if (choice != 'Y' && choice != 'y')
    {
        printf("Changes discarded. Task with id %d remains unchanged.\n", task_id);
        return;
    }
    *task_ptr = temp_task;
    printf("Task with id %d edited successfully.\n", task_id);

}
void delete_task(int task_id){
    task *task_ptr = find_task_by_id(task_id);
    if (task_ptr == NULL) 
    {
        printf("Task with id %d not found.\n", task_id);
        return;
    } 
    printf("Deleting task with id:%d\n", task_id);
    //if the task to be deleted is the head
    if (task_ptr == head_ptr)
    {
        head_ptr = head_ptr->next_task_ptr;
        if (head_ptr != NULL)
        {
            head_ptr->prev_task_ptr = NULL;
        }
        else
        {
            tail_ptr = NULL; //list is now empty
            next_available_id = 1; //resetting task id counter
        }
    }
    //if the task to be deleted is the tail
    else if (tail_ptr == task_ptr)
    {
        (task_ptr->prev_task_ptr)->next_task_ptr = NULL;
        tail_ptr = task_ptr->prev_task_ptr;
    }
    //if the task to be deleted is not the head or tail
    else
    {
        (task_ptr->prev_task_ptr)->next_task_ptr = task_ptr->next_task_ptr;//now prev task points to the next task, skipping the task in between
        (task_ptr->next_task_ptr)->prev_task_ptr = task_ptr->prev_task_ptr;//now next task points to the prev task, skipping the task in between
    }
    
    free(task_ptr);
    if (setting_flags & flag_auto_sort && head_ptr != NULL)
    {
        auto_sort_f();
    }
    printf("Task with id %d deleted successfully.\n", task_id);
}
void uncomplete_task(int task_id){
    task *task_ptr = find_task_by_id(task_id);
    if (task_ptr == NULL) 
    {
        printf("Task with id %d not found.\n", task_id);
        return;
    } 
    task_ptr->completed = 0;
    printf("Task with id %d marked as uncompleted.\n", task_id);

}
void display_some_task_details(task* task_ptr){
    char colour[14];
    strcpy(colour, category[task_ptr->category_id].colour);
    printf("%s", colour);
    printf("\tTask Id: %d\n", (task_ptr->task_id));
    printf("\tTask Name: %s\n", (task_ptr->task_name));
    printf("%s", colour_reset);
}
void display_complete_task_details(task* task_ptr){
    char colour[14];
    strcpy(colour, category[task_ptr->category_id].colour);
    printf("%s", colour);
    printf("\tTask Id: %d\n", (task_ptr->task_id));
    printf("\tTask Name: %s\n", (task_ptr->task_name));
    if (setting_flags & show_description)
    {
        printf("\tDescription: %s\n", (task_ptr->task_description));
    }
    if (setting_flags & show_priority)
    {
        printf("\tPriority Level: %d\n", (task_ptr->priority_level));
    }
    int task_date_array[6];
    get_current_recurrence_number(task_ptr);
    if (setting_flags & show_start_date)
    {
        if (task_ptr->xth_occurance_start_time == task_ptr->current_recurrence && task_ptr->edited_start_time != 0)
        {
            get_normal_time_format(task_date_array, task_ptr->edited_start_time);
            printf("\tStart time of this task (edited for this occurrence): %s %02d, %d. At %02d:%02d\n", get_month_name(task_date_array[1]), task_date_array[2], task_date_array[0], task_date_array[4], task_date_array[5]);
        }
        else if (task_ptr->recurring_id != 0)
        {
            time_t occurrence_time = task_ptr->start_time + (time_t)((task_ptr->current_recurrence - 1LL) * task_ptr->interval);
            get_normal_time_format(task_date_array, occurrence_time);
            printf("\tStart time of this task: %s %02d, %d. At %02d:%02d\n", get_month_name(task_date_array[1]), task_date_array[2], task_date_array[0], task_date_array[4], task_date_array[5]);
        }
        else
        {
            get_normal_time_format(task_date_array, task_ptr->start_time);
            printf("\tStart time of this task: %s %02d, %d. At %02d:%02d\n", get_month_name(task_date_array[1]), task_date_array[2], task_date_array[0], task_date_array[4], task_date_array[5]);
        }
    }
    if (setting_flags & show_due_date)
    {
        if (task_ptr->due_time == -1)
        {
            printf("\tNo due date\n");
        }
        else
        {
            get_normal_time_format(task_date_array, task_ptr->due_time);
            printf("\tDue time of this task: %s %02d, %d. At %02d:%02d\n", get_month_name(task_date_array[1]), task_date_array[2], task_date_array[0], task_date_array[4], task_date_array[5]);
        }
        }
    if (setting_flags & show_completed)
    {
        if (task_ptr->completed)
        {
            printf("\tCompleted : Yes \n");
        }
        else
        {
            printf("\tCompleted : No \n");
        }
    }
    if (setting_flags & show_category)
    {
        printf("\tCategory Id: %d \n", task_ptr->category_id);
    }
    if (task_ptr->recurring_id != 0)
    {
        printf("\tRecurring id: %d\n", task_ptr->recurring_id);
        printf("\tRecurring series name: %s\n", task_ptr->recurring_name);
        if (task_ptr->no_of_recurrences == 0)
        {
            printf("\tNumber of recurrences: Infinite\n");
        }
        else
        {
            printf("\tNumber of recurrences: %d\n", task_ptr->no_of_recurrences);
        }
        if (task_ptr->recurrence_end_time == -1)
        {
            printf("\tNo recurrence end time\n");
        }
        else if (task_ptr->recurrence_end_time != 0)
        {
            get_normal_time_format(task_date_array, task_ptr->recurrence_end_time);
            printf("\tRecurrence end time: %s %02d, %d. At %02d:%02d\n", get_month_name(task_date_array[1]), task_date_array[2], task_date_array[0], task_date_array[4], task_date_array[5]);
        }
        else
        {
            printf("\tRecurrence end time: None\n");
        }
        printf("\tCurrent occurrence number: %d\n", task_ptr->current_recurrence);

        if (setting_flags & show_interval)
        {
            long secs = (long) task_ptr->interval;
            long years = secs / 31536000;
            secs %= 31536000;
            long months = secs / 2592000;
            secs %= 2592000;
            long days = secs / 86400;
            secs %= 86400;
            long hours = secs / 3600;
            long minutes = (secs % 3600) / 60;
            printf("\tThe task recurs every ");
            if (years) printf("%ld year(s), ", years);
            if (months) printf("%ld month(s), ", months);
            if (days) printf("%ld day(s), ", days);
            if (hours) printf("%ld hour(s), ", hours);
            printf("%ld minute(s)\n", minutes);
        }
    }
    
    if (setting_flags & show_auto_complete_status)
    {
        if (task_ptr->auto_complete_enabled)
        {
            printf("\tAuto Complete Status: On \n");
        }
        else
        {
            printf("\tAuto Complete Status: Off \n");
        }
    }

    //displaying subtasks
    if (task_ptr->subtask_ptr != NULL)
    {
        printf("\tSubtasks:\n");
        subtask *s = task_ptr->subtask_ptr;
        int subcount = 1;
        while (s != NULL)
        {
            printf("\t\t%d.\n", subcount);
            printf("\t\tSubtask Id: %d\n", (s->subtask_id));
            printf("\t\tSubtask Name: %s\n", (s->subtask_name)); 
            printf("\t\tDescription: %s\n", (s->subtask_description));
            if (s->completed)
            {
                printf("\tCompleted : Yes \n");
            }
            else
            {
                printf("\tCompleted : No \n");
            }
            s = s->next_subtask_ptr;
            subcount++;
        }
    }
    printf("%s", colour_reset);
}
void get_current_recurrence_number(task *ptr){
    time_t current_time = time(NULL);
    if (ptr->recurring_id == 0)
    {
        ptr->current_recurrence = 0;
        return;
    }
    time_t occurrence_time = ptr->start_time;
    int occurrence_no = 0;
    while (occurrence_time <= current_time)
    {
        occurrence_no++;
        occurrence_time += ptr->interval;
        if (ptr->no_of_recurrences != 0 && occurrence_no >= ptr->no_of_recurrences)
        {
            break;
        }
        if (ptr->recurrence_end_time != 0 && occurrence_time > ptr->recurrence_end_time)
        {
            break;
        }
    }
    ptr->current_recurrence = occurrence_no;
}
task* find_task_by_id(int task_id){
    task *ptr = head_ptr;
    while (ptr != NULL)
    {
        if (ptr->task_id == task_id)
        {
            return ptr;
        }
        else
        {
            ptr = ptr->next_task_ptr;
        }
    }
    return NULL;
}
task* find_task_by_date(task *ptr, int date_array1[]){
    while (ptr != NULL)
    {
        int date_array2[6] = {0};
        get_normal_time_format(date_array2, (ptr->start_time));
        if (date_array1[0] == date_array2[0] && date_array1[1] == date_array2[1] && date_array1[2] == date_array2[2])
        {
            return ptr;
        }
        get_normal_time_format(date_array2, (ptr->due_time));
        if (date_array1[0] == date_array2[0] && date_array1[1] == date_array2[1] && date_array1[2] == date_array2[2])
        {
            return ptr;
        }
        if (ptr->recurring_id != 0)
        {
            time_t occurrence_time = ptr->start_time;
            int occurrence_no = 0;
            while (1)
            {
                occurrence_no++;
                occurrence_time += ptr->interval;
                if (ptr->no_of_recurrences != 0 && occurrence_no >= ptr->no_of_recurrences)
                {
                    break;
                }
                if (ptr->recurrence_end_time != 0 && occurrence_time > ptr->recurrence_end_time)
                {
                    break;
                }
                get_normal_time_format(date_array2, occurrence_time);
                if (date_array1[0] == date_array2[0] && date_array1[1] == date_array2[1] && date_array1[2] == date_array2[2])
                {
                    return ptr;
                }
            }
        }
        
        ptr = ptr->next_task_ptr;
    }
    return NULL;
}

//subtasks (linked list)
void add_subtask(task *parent_task){
    subtask *new_subtask = (subtask*)malloc(sizeof(subtask));
    int subtask_id = 1;
    if (parent_task->subtask_ptr == NULL)
    {
        parent_task->subtask_ptr = new_subtask;
    }
    //find the last subtask:
    else
    {
        subtask *tail_subtask_ptr = parent_task->subtask_ptr;
        while (tail_subtask_ptr->next_subtask_ptr != NULL)
        {
            tail_subtask_ptr = tail_subtask_ptr->next_subtask_ptr;
            subtask_id = tail_subtask_ptr->subtask_id +1;
            tail_subtask_ptr->next_subtask_ptr = new_subtask;
        }
    }
    new_subtask->parent_task_id = parent_task->task_id;
    new_subtask->subtask_id = subtask_id;
    printf("Enter subtask name: ");
    scanf(" %99[^\n]", new_subtask->subtask_name);
    printf("Enter subtask description: ");  
    scanf(" %999[^\n]", new_subtask->subtask_description);
    new_subtask->completed = 0; //not completed 
    new_subtask->next_subtask_ptr = NULL;
    printf("New subtask successfully added with id %d to task with id %d\n", new_subtask->subtask_id, parent_task->task_id);
    //adding details

}
void complete_subtask (int task_id, int subtask_id){
    task *parent_task = find_task_by_id(task_id);
    if (parent_task == NULL)
    {
        printf("Parent task with id %d not found.\n", task_id);
        return;
    }
    subtask *subtask_ptr = find_subtask_by_id(parent_task, subtask_id)  ;
    if (subtask_ptr != NULL)
    {
        subtask_ptr->completed = 1;
        printf("Subtask with id %d of task with id %d marked as completed.\n", subtask_id, task_id);
        if (parent_task->auto_complete_enabled)
        {
            auto_complete(parent_task);
        }
        return;
    }
    printf("Subtask with id %d not found in task with id %d.\n", subtask_id, task_id);
}
void edit_subtask (int task_id, int subtask_id){
    task *parent_task = find_task_by_id(task_id);
    if (parent_task == NULL)
    {
        printf("Parent task with id %d not found.\n", task_id);
        return;
    }
    
    subtask *subtask_ptr = find_subtask_by_id(parent_task, subtask_id)  ;
    if (subtask_ptr != NULL)
    {
        printf("Editing subtask with id %d of task with id %d:\n", subtask_id, task_id);
        char choice;
        printf("Do you want to edit subtask name? (Y/N): ");
        scanf(" %c", &choice);
        if (choice == 'Y' || choice == 'y')
        {
            printf("Enter new subtask name (current: %s): ", subtask_ptr->subtask_name);
            scanf(" %99[^\n]", subtask_ptr->subtask_name);
        }
        
        printf("Do you want to edit subtask description? (Y/N): ");
        scanf(" %c", &choice);
        if (choice == 'Y' || choice == 'y')
        {
            printf("Enter new subtask description (current: %s): ", subtask_ptr->subtask_description);
            scanf(" %999[^\n]", subtask_ptr->subtask_description);
        }
        printf("Subtask with id %d of task with id %d edited successfully.\n", subtask_id, task_id);
        return;
    }
    printf("Subtask with id %d not found in task with id %d.\n", subtask_id, task_id);
}
void delete_subtask (int task_id, int subtask_id){
    task *parent_task = find_task_by_id(task_id);
    if (parent_task == NULL)
    {
        printf("Parent task with id %d not found.\n", task_id);
        return;
    }
    
    subtask *subtask_ptr = find_subtask_by_id(parent_task, subtask_id)  ;
    if (subtask_ptr != NULL)
    {
        //if the subtask to be deleted is the head
        if (subtask_ptr == parent_task->subtask_ptr)
        {
            parent_task->subtask_ptr = parent_task->subtask_ptr->next_subtask_ptr;
        }
        else
        {
            //finding the previous subtask
            subtask *prev_subtask_ptr = parent_task->subtask_ptr;
            while (prev_subtask_ptr->next_subtask_ptr != subtask_ptr)
            {
                prev_subtask_ptr = prev_subtask_ptr->next_subtask_ptr;
            }
            if (subtask_ptr->next_subtask_ptr != NULL)
            {
                prev_subtask_ptr->next_subtask_ptr = subtask_ptr->next_subtask_ptr;
            }
            //if the subtask to be deleted is the tail
            else
            {
                prev_subtask_ptr->next_subtask_ptr = NULL;
            }
        }
        //remove all the gaps in subtask ids
        subtask *temp_ptr = parent_task->subtask_ptr;
        int current_id = 1;
        while (temp_ptr != NULL)
        {
            if (temp_ptr->subtask_id != current_id)
            {
                temp_ptr->subtask_id = current_id;
            }
            current_id++;
            temp_ptr = temp_ptr->next_subtask_ptr;
        }
        free(subtask_ptr);
        return;
    }
    printf("Subtask with id %d not found in task with id %d.\n", subtask_id, task_id);
}
void uncomplete_subtask(int task_id, int subtask_id){
    task *parent_task = find_task_by_id(task_id);
    if (parent_task == NULL)
    {
        printf("Parent task with id %d not found.\n", task_id);
        return;
    }
    
    subtask *subtask_ptr = find_subtask_by_id(parent_task, subtask_id)  ;
    if (subtask_ptr != NULL)
    {
        subtask_ptr->completed = 0;
        printf("Subtask with id %d of task with id %d marked as uncomplete.\n", subtask_id, task_id);
        uncomplete_task(task_id);
        return;
    }
    printf("Subtask with id %d not found in task with id %d.\n", subtask_id, task_id);
}
subtask* find_subtask_by_id(task *task_ptr, int subtask_id){
    subtask *subtask_ptr = task_ptr->subtask_ptr;
    while (subtask_ptr != NULL)
    {
        if (subtask_ptr->subtask_id == subtask_id)
        {
            return subtask_ptr;
        }
        subtask_ptr = subtask_ptr->next_subtask_ptr;
    }
    return NULL;
}

//categories (array)
void add_category (){
    if (category_amount == 20)
    {
        printf("No more categories can be added.\n");
        return;
    }
    else
    {
        printf("Enter category name: ");
        scanf(" %s", category[category_amount].name);
        category[category_amount].category_id = category_amount+1;
        printf("Enter the following colour choices: \n1. Red[R] \n2. Orange[O] \n3. Yellow[Y] \n4. Green[G] \n5. Blue[B] \n6. Cyan[C] \n7. Magenta[M] \n8. Brown[W] \n9. None[N]\n");
        char Cchoice;
        scanf(" %c", &Cchoice);
        switch (Cchoice)
        {
            case 'R':
                strcpy(category[category_amount].colour, colour_red);
                break;
            case 'O':
                strcpy(category[category_amount].colour, colour_orange);
                break;
            case 'Y':
                strcpy(category[category_amount].colour, colour_yellow);
                break;
            case 'G':
                strcpy(category[category_amount].colour, colour_green);
                break;
            case 'B':
                strcpy(category[category_amount].colour, colour_blue);
                break;
            case 'C':
                strcpy(category[category_amount].colour, colour_cyan);
                break;
            case 'M':
                strcpy(category[category_amount].colour, colour_magenta);
                break;
            case 'W':
                strcpy(category[category_amount].colour, colour_brown);
                break;
            default:
                strcpy(category[category_amount].colour, "");
                break;
        }
        category_amount++;
    }
}
void display_categories(){
    printf("Category Details:\n");
    for (int i = 0; i < category_amount; i++)
    {
        printf("%d.\n", i+1);
        printf("Category id: %d\n", category[i].category_id);
        printf("Category Name: %s\n", category[i].name);
        printf("Category colour: %s\n", category[i].colour);
    }
}
void edit_category(){
    int id;
    char choice;
    printf("Enter the id of the category you want to edit: ");
    scanf(" %d", &id);
    if (id > category_amount)
    {
        printf("Category not found\n");
        return;
    }

    printf("Editing category with id:%d", id);
    char changes[100] = ""; //to store the changes made
    categories temp = category[id];
    printf("Do you want to change the name of the category? (Y/N): ");
    scanf(" %c", &choice);
    if (choice == 'Y' || choice == 'y')
    {
        printf("Enter the new name: ");
        scanf(" %s", temp.name);
        strcat(changes, "Edited the name \n");
    }

    printf("Do you want to change the colour of the category? (Y/N): ");
    scanf(" %c", &choice);
    if (choice == 'Y' || choice == 'y')
    {
        printf("Enter the following colour choices: \n1. Red[R] \n2. Orange[O] \n3. Yellow[Y] \n4. Green[G] \n5. Blue[B] \n6. Cyan[C] \n7. Magenta[M] \n8. Brown[W] \n9. None[N]\n");
        char Cchoice;
        scanf(" %c", &Cchoice);
        switch (Cchoice)
        {
            case 'R':
                strcpy(temp.colour, colour_red);
                break;
            case 'O':
                strcpy(temp.colour, colour_orange);
                break;
            case 'Y':
                strcpy(temp.colour, colour_yellow);
                break;
            case 'G':
                strcpy(temp.colour, colour_green);
                break;
            case 'B':
                strcpy(temp.colour, colour_blue);
                break;
            case 'C':
                strcpy(temp.colour, colour_cyan);
                break;
            case 'M':
                strcpy(temp.colour, colour_magenta);
                break;
            case 'W':
                strcpy(temp.colour, colour_brown);
                break;
            default:
                strcpy(temp.colour, "");
                break;
        }
        
        strcat(changes, "Edited the colour \n");
    }
    printf("The following changes were made:\n%s", changes);
    printf("Do you want to confirm the changes? (Y/N): ");
    scanf(" %c", &choice);
    if (choice == 'Y' || choice == 'y')
    {
        category[id] = temp;
    }
}
void delete_category(){
    int id;
    printf("Enter the category id to delete: ");
    scanf(" %d", &id);
    if (id > category_amount)
    {
        printf("Category not found\n");
        return;
    }
}

//settings (bit flags)
void change_settings(){
    printf("Current Settings:\n");
    display_current_settings();
    printf("Enter the number corresponding to the setting you want to toggle (1-9): ");
    int choice;
    scanf(" %d", &choice);
    switch (choice)
    {
        case 1:
            setting_flags ^= show_description;
            break;
        case 2:
            setting_flags ^= show_priority;
            break;
        case 3:
            setting_flags ^= show_start_date;
            break;
        case 4:
            setting_flags ^= show_due_date;
            break;
        case 5:
            setting_flags ^= show_completed;
            break;
        case 6:
            setting_flags ^= show_category;
            break;
        case 7:
            setting_flags ^= show_interval;
            break;
        case 8:
            setting_flags ^= show_auto_complete_status;
            break;
        case 9:
            setting_flags ^= flag_auto_sort;
            if (setting_flags & flag_auto_sort)
            {
                auto_sort_f();
            }
            break;
        default:
            printf("Invalid choice.\n");
            return;
    }
    printf("Setting updated successfully.\n");
}
void display_current_settings(){
    printf("1. Show Description: %s\n", (setting_flags & show_description) ? "On" : "Off");
    printf("2. Show Priority Level: %s\n", (setting_flags & show_priority) ? "On" : "Off");
    printf("3. Show Start Date: %s\n", (setting_flags & show_start_date) ? "On" : "Off");
    printf("4. Show Due Date: %s\n", (setting_flags & show_due_date) ? "On" : "Off");
    printf("5. Show Completed Status: %s\n", (setting_flags & show_completed) ? "On" : "Off");
    printf("6. Show Category: %s\n", (setting_flags & show_category) ? "On" : "Off");
    printf("7. Show Interval: %s\n", (setting_flags & show_interval) ? "On" : "Off");
    printf("8. Show Auto-Complete Status: %s\n", (setting_flags & show_auto_complete_status) ? "On" : "Off");
    printf("9. Auto-Sort Tasks: %s\n", (setting_flags & flag_auto_sort) ? "On" : "Off");
    //remove this
    printf("Current setting flags value: %d\n", setting_flags);
    //more settings will be added later
}

//extras
void auto_complete(task *ptr){
    subtask *subtask_ptr = ptr->subtask_ptr;
    while (subtask_ptr != NULL)
    {
        if (subtask_ptr->completed == 0)
        {
            return;
        }
    }
    ptr->completed = 1;
    printf("Task with id %d automatically marked as completed as all its subtasks are completed.\nYou can turn off the setting by editting the task.\n", ptr->task_id);
}
void auto_sort_f(){
    int id = 1;
    task *ptr = head_ptr;
    while (ptr != NULL)
    {
        ptr->task_id = id;
        ptr = ptr->next_task_ptr;
        id++;
    }
    next_available_id = id;
}
//will be added later:
//void reminders(){}
//void manual_sort(){}
//void view_missing_tasks(){} and also display them
//find recurring tasks by recurring series name
//display subtasks separately
//display today's tasks

//saving and loading(reading) data
void load_from_file()
{
    //loading tasks
    FILE *task_file;
    task_file = fopen("tasks.dat", "rb");
    if (task_file == NULL) 
    {
        printf("Error opening tasks.dat\n");
    }
    else
    {
        printf("Loading tasks from file...\n");
        task temp_task;
        task *tail_ptr = NULL;
        while (fread(&temp_task, sizeof(task), 1, task_file))
        {
            task *next_task = (task *)malloc(sizeof(task));
            *next_task = temp_task;
            next_task->next_task_ptr = NULL;
            next_task->subtask_ptr = NULL;

            if (head_ptr == NULL)
            {
                head_ptr = next_task;
                head_ptr->prev_task_ptr = NULL;
                tail_ptr = head_ptr;
            }
            else
            {
                next_task->prev_task_ptr = tail_ptr;
                tail_ptr->next_task_ptr = next_task; //store the new address in the last task 
                tail_ptr = next_task; // move the tail (last) pointer to the new address
            }
        }
        fclose(task_file);
    }
    

    //loading subtasks
    FILE *subtask_file;
    subtask_file = fopen("subtasks.dat", "rb");
    if (subtask_file == NULL) 
    {
        printf("Error opening subtasks.dat\n");
    }

    else
    {
        printf("Loading subtasks from file...\n");
        subtask temp_subtask;
        while (fread(&temp_subtask, sizeof(subtask), 1, subtask_file))
        {
            task *parent = find_task_by_id(temp_subtask.parent_task_id);
            if (parent != NULL)
            {
                subtask *next_subtask = (subtask *)malloc(sizeof(subtask));
                *next_subtask = temp_subtask;
                next_subtask->next_subtask_ptr = NULL;

                if (parent->subtask_ptr == NULL)
                {
                    parent->subtask_ptr = next_subtask;
                }
                else
                {
                    subtask *sub_tail_ptr = parent->subtask_ptr;
                    while (sub_tail_ptr->next_subtask_ptr != NULL)
                    {
                        sub_tail_ptr = sub_tail_ptr->next_subtask_ptr;
                    }
                    sub_tail_ptr->next_subtask_ptr = next_subtask;
                }
            }
            else
            {
                printf("Parent task not found for subtask with parent-task id: %d\n",temp_subtask.parent_task_id);
            }
        }
        fclose(subtask_file);
    }
    

    //loading categories
    FILE *category_file;
    category_file = fopen("categories.dat", "rb");
    if (category_file == NULL) 
    {
        printf("Error opening categories.dat\n");
    }
    else
    {
        printf("Loading categories from file...\n");
        fread(&category, sizeof(categories), 20, category_file);
        fclose(category_file);
    }
    
    //loading settings
    FILE *settings_file;
    settings_file = fopen("settings.dat", "rb");
    if (settings_file == NULL) 
    {
        printf("Error opening settings.dat\n");
    }
    else
    {
        printf("Loading settings from file...\n");
        fread(&setting_flags, sizeof(int), 1, settings_file);
        fclose(settings_file);
    }
    
}
void save_to_file(){

    //saving tasks AND subtasks
    FILE *task_file;
    task_file = fopen("tasks.dat", "wb");
    if (task_file == NULL) 
    {
        printf("Error opening tasks.dat\n");
    }
    else
    {
        FILE *subtask_file;
        subtask_file = fopen("subtasks.dat", "wb");
        if (subtask_file == NULL) 
        {
            printf("Error opening subtasks.dat\n");
            fclose(task_file);
        }
        else
        {
            printf("Saving tasks and subtasks to file...\n");
            task *ptr = head_ptr;
            while (ptr != NULL)
            {
                fwrite(ptr, sizeof(task), 1, task_file);
                subtask *sub_ptr = ptr->subtask_ptr;
                while (sub_ptr != NULL)
                {
                    fwrite(sub_ptr, sizeof(subtask), 1, subtask_file);
                    sub_ptr = sub_ptr->next_subtask_ptr;
                }
                ptr = ptr->next_task_ptr;
            }
            fclose(task_file);
            fclose(subtask_file);
        }
    }

    //saving categories
    FILE *category_file;
    category_file = fopen("categories.dat", "wb");
    if (category_file == NULL) 
    {
        printf("Error opening categories.dat\n");
    }
    else
    {
        printf("Saving categories to file...\n");
        fwrite(&category, sizeof(categories), 20, category_file);
        fclose(category_file);
    }
    
    //saving settings
    FILE *settings_file;
    settings_file = fopen("settings.dat", "wb");
    if (settings_file == NULL) 
    {
        printf("Error opening settings.dat\n");
    }
    else
    {
        printf("Saving settings to file...\n");
        fwrite(&setting_flags, sizeof(int), 1, settings_file);
        fclose(settings_file);
    }
}
//END:) 
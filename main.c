#include <stdio.h>
#include "function_declarations.h"

int main() {
    load_from_file();
    int choice1 ,choice2 ,task_id, subtask_id;
    do {
        printf("=============== Task Management System ===============\n");
        printf("Enter the corresponding number to choose an option:\n");
        printf("1. Calendar View \n");
        printf("2. List View \n");
        printf("3. Task actions \n");
        printf("4. Subtask actions \n");
        printf("5. Category actions\n");
        printf("6. Change Settings \n");
        printf("7. Display Current Settings \n");
        printf("8. Save and Exit \n");
        printf("Enter your choice: ");
        scanf(" %d", &choice1);

        switch (choice1) {
            case 1:
                calendar_view();
                break;
            case 2:
                list_view();
                break;
            case 3:
                printf("Task Actions Menu:\n");
                printf("1. Add Task\n");
                printf("2. Edit Task\n");
                printf("3. Complete Task\n");
                printf("4. Uncomplete Task\n");
                printf("5. Delete Task\n");
                printf("6. See a task's details\n");
                printf("Enter your choice: ");
                scanf(" %d", &choice2);
                switch (choice2)
                {
                case 1:
                    add_task();
                    break;
                case 2:
                    printf("Enter task id: ");
                    scanf(" %d", &task_id);
                    edit_task(task_id);
                    break;
                case 3:
                    printf("Enter task id: ");
                    scanf(" %d", &task_id);
                    complete_task(task_id);
                    break;
                case 4:
                    printf("Enter task id: ");
                    scanf(" %d", &task_id);
                    uncomplete_task(task_id);
                    break;
                case 5:
                    printf("Enter task id: ");
                    scanf(" %d", &task_id);
                    delete_task(task_id);
                    break;
                case 6:
                    printf("Enter task id: ");
                    scanf(" %d", &task_id);
                    display_complete_task_details(find_task_by_id(task_id));
                    break;
                default:
                    printf("Invalid choice.\n");
                    break;
                }
                break;
            case 4:
                printf("Subtask Actions Menu:\n");
                printf("1. Add subtask\n");
                printf("2. Edit subtask\n");
                printf("3. Complete subtask\n");
                printf("4. Uncomplete subtask\n");
                printf("5. Delete subtask\n");
                printf("Enter your choice: ");
                scanf(" %d", &choice2);
                switch (choice2)
                {
                case 1:
                    printf("Enter parent task id: ");
                    scanf(" %d", &task_id);
                    add_subtask(find_task_by_id(task_id));
                    break;
                case 2:
                    printf("Enter parent task id: ");
                    scanf(" %d", &task_id);
                    printf("Enter subtask id: ");
                    scanf(" %d", &subtask_id);
                    edit_subtask(find_task_by_id(task_id), subtask_id);
                    break;
                case 3:
                printf("Enter parent task id: ");
                    scanf(" %d", &task_id);
                    printf("Enter subtask id: ");
                    scanf(" %d", &subtask_id);
                    complete_subtask(task_id, subtask_id);
                    break;
                case 4:
                    printf("Enter parent task id: ");
                    scanf(" %d", &task_id);
                    printf("Enter subtask id: ");
                    scanf(" %d", &subtask_id);
                    uncomplete_subtask(task_id, subtask_id);
                    break;
                case 5:
                    printf("Enter parent task id: ");
                    scanf(" %d", &task_id);
                    printf("Enter subtask id: ");
                    scanf(" %d", &subtask_id);
                    delete_subtask(task_id, subtask_id);
                    break;
                default:
                    break;
                }
                break;
            case 5:
                printf("Category Actions Menu:\n");
                printf("1. Add Category\n");
                printf("2. Display Categories\n");
                printf("3. Edit Category\n");
                printf("4. Delete Category\n");
                printf("Enter your choice: ");
                scanf(" %d", &choice2);
                switch (choice2)    
                {
                case 1:
                    add_category();
                    break;
                case 2:
                    display_categories();
                    break;
                case 3:
                    edit_category();
                    break;
                case 4:
                    delete_category();
                    break;
                default:
                    break;
                }
                break;
            case 6:
                change_settings();
                break;
            case 7:
                display_current_settings();
                break;
            case 8:
                save_to_file();
                printf("Exiting the program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice1 != 8);
    return 0;
}
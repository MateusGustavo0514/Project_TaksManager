#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DESCRIPTION_SIZE 100

typedef struct Task {
    int id;
    char description[MAX_DESCRIPTION_SIZE];
    struct Task* prox;
} TASK;

TASK* pendingQueueFront = NULL;
TASK* pendingQueueRear = NULL;
TASK* completedList = NULL;
TASK* draftStack = NULL;

// CONSTRUCTOR
TASK* createTask(int id, char* description);
TASK* createTaskByScanf();

// LIST
void addToCompletedList(TASK* newTask);
TASK* removeFromCompletedListByItsId(int id);
void seeAllCompletedList();

// QUEUE
void putToPendingQueue(TASK* newTask);
TASK* getFromPendingQueue();
void seeAllPendingQueue();

// STACK
void pushToDraftStack(TASK* newTask);
TASK* popFromDraftStack();
void seeAllDraftStack();

// MENU
void displayMenu();

int main() {
    int choice;
    printf("################# TASK MANAGER SYSTEM #################\n");

    do {
        displayMenu();
        printf("Choose an option: ");
        scanf("%d", &choice);

        TASK* newTask;
        TASK* task;
        int id;

        switch (choice) {
            case 1:
                // CREATE A TASK, THEN ADD TO PENDING QUEUE
                newTask = createTaskByScanf();
                putToPendingQueue(newTask);
                break;

            case 2:
                // SEE ALL TASKS FROM PENDING QUEUE
                seeAllPendingQueue();
                break;

            case 3:
                // COMPLETE FIRST PENDING TASK
                newTask = getFromPendingQueue();
                if (newTask != NULL) {
                    addToCompletedList(newTask);
                } else {
                    printf("No pending tasks to complete.\n");
                }
                break;

            case 4:
                // SEE ALL TASKS FROM COMPLETED LIST
                seeAllCompletedList();
                break;

            case 5:
                // SET COMPLETED TASK TO DRAFT
                printf("Enter Task ID (number): ");
                scanf("%d", &id);

                task = removeFromCompletedListByItsId(id);
                if (task != NULL) {
                    pushToDraftStack(task);
                } else {
                    printf("Task ID %d not found in completed tasks.\n", id);
                }
                break;

            case 6:
                // SEE ALL TASKS FROM DRAFT STACK
                seeAllDraftStack();
                break;

            case 7:
                // SET LAST DRAFT AS PENDING TASK
                task = popFromDraftStack();
                if (task != NULL) {
                    putToPendingQueue(task);
                } else {
                    printf("No draft tasks to set as pending.\n");
                }
                break;

            case 0:
                printf("Exiting program\n");
                break;
            default:
                printf("Invalid choice\n");
                break;
        }
    } while (choice != 0);

    printf("################# SYSTEM SHUT DOWN #################\n");
    return 0;
}

// CONSTRUCTOR
TASK* createTask(int id, char* description) {
    TASK* newTask = (TASK*) malloc(sizeof(TASK));

    newTask->id = id;
    strcpy(newTask->description, description);
    newTask->prox = NULL;

    return newTask;
}

TASK* createTaskByScanf() {
    int id;
    char description[MAX_DESCRIPTION_SIZE];

    printf("Enter Task ID (number): ");
    scanf("%d", &id);

    getchar();
    printf("Enter Task Description (string): ");

    fgets(description, MAX_DESCRIPTION_SIZE, stdin);
    description[strcspn(description, "\n")] = '\0';

    return createTask(id, description);
}

// LIST
void addToCompletedList(TASK* newTask) {
    newTask->prox = completedList;
    completedList = newTask;
    printf("Task %d added to Completed List.\n", newTask->id);
}

TASK* removeFromCompletedListByItsId(int id) {
    TASK* prev = NULL;
    TASK* curr = completedList;

    while (curr != NULL && curr->id != id) {
        prev = curr;
        curr = curr->prox;
    }

    if (curr == NULL) {
        return NULL; // Task not found
    }

    if (prev == NULL) {
        completedList = curr->prox;
    } else {
        prev->prox = curr->prox;
    }

    curr->prox = NULL;
    printf("Task %d removed from Completed List.\n", id);
    return curr;
}

void seeAllCompletedList() {
    TASK* temp = completedList;

    if (temp == NULL) {
        printf("Completed List is empty.\n");
        return;
    }

    printf("Completed List:\n");
    while (temp != NULL) {
        printf("ID: %d, Description: %s\n", temp->id, temp->description);
        temp = temp->prox;
    }
}

// QUEUE
void putToPendingQueue(TASK* newTask) {
    if (pendingQueueRear == NULL) {
        pendingQueueFront = pendingQueueRear = newTask;
    } else {
        pendingQueueRear->prox = newTask;
        pendingQueueRear = newTask;
    }
    printf("Task %d added to Pending Queue.\n", newTask->id);
}

TASK* getFromPendingQueue() {
    if (pendingQueueFront == NULL) {
        return NULL;
    }

    TASK* temp = pendingQueueFront;
    pendingQueueFront = pendingQueueFront->prox;

    if (pendingQueueFront == NULL) {
        pendingQueueRear = NULL;
    }

    temp->prox = NULL;
    printf("Task %d removed from Pending Queue.\n", temp->id);
    return temp;
}

void seeAllPendingQueue() {
    TASK* temp = pendingQueueFront;

    if (temp == NULL) {
        printf("Pending Queue is empty.\n");
        return;
    }

    printf("Pending Queue:\n");
    while (temp != NULL) {
        printf("ID: %d, Description: %s\n", temp->id, temp->description);
        temp = temp->prox;
    }
}

// STACK
void pushToDraftStack(TASK* newTask) {
    newTask->prox = draftStack;
    draftStack = newTask;
    printf("Task %d pushed to Draft Stack.\n", newTask->id);
}

TASK* popFromDraftStack() {
    if (draftStack == NULL) {
        return NULL;
    }

    TASK* temp = draftStack;
    draftStack = draftStack->prox;
    temp->prox = NULL;
    printf("Task %d popped from Draft Stack.\n", temp->id);
    return temp;
}

void seeAllDraftStack() {
    TASK* temp = draftStack;

    if (temp == NULL) {
        printf("Draft Stack is empty.\n");
        return;
    }

    printf("Draft Stack:\n");
    while (temp != NULL) {
        printf("ID: %d, Description: %s\n", temp->id, temp->description);
        temp = temp->prox;
    }
}

// MENU
void displayMenu() {
    printf("\nMenu:\n");
    printf("1 - Create New Pending Task\n");
    printf("2 - See All Pending Tasks\n");
    printf("3 - Complete First Pending Task\n");
    printf("4 - See All Completed Tasks\n");
    printf("5 - Set Completed Task as Draft by its ID\n");
    printf("6 - See All Draft Tasks\n");
    printf("7 - Set Last Draft Task as Pending Task\n");
    printf("0 - Exit\n");
}
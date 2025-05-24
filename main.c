#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TASKS 100
#define MAX_DIARY_ENTRIES 100
#define MAX_TEXT 1024

typedef struct {
    char description[MAX_TEXT];
    char timestamp[64];
} Task;

typedef struct {
    char timestamp[64];
    char entry[MAX_TEXT];
} DiaryEntry;

Task tasks[MAX_TASKS];
int task_count = 0;

DiaryEntry diary_entries[MAX_DIARY_ENTRIES];
int diary_count = 0;

// --- Utility Functions ---
void get_current_time(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", t);
}

void greet_user() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    int hour = t->tm_hour;

    if (hour < 12)
        printf("Good morning!\n");
    else if (hour < 18)
        printf("Good afternoon!\n");
    else
        printf("Good evening!\n");

    printf("How was your day?\n");
    char buffer[MAX_TEXT];
    fgets(buffer, MAX_TEXT, stdin); // Just for interaction
}

// --- Task Functions ---
void save_tasks() {
    FILE *file = fopen("tasks.txt", "w");
    for (int i = 0; i < task_count; i++) {
        fprintf(file, "%s|%s\n", tasks[i].description, tasks[i].timestamp);
    }
    fclose(file);
}

void load_tasks() {
    FILE *file = fopen("tasks.txt", "r");
    if (!file) return;

    while (fscanf(file, "%[^|]|%[^\n]\n", tasks[task_count].description, tasks[task_count].timestamp) == 2) {
        task_count++;
        if (task_count >= MAX_TASKS) break;
    }

    fclose(file);
}

void add_task() {
    if (task_count >= MAX_TASKS) {
        printf("Task limit reached.\n");
        return;
    }

    printf("Enter task: ");
    fgets(tasks[task_count].description, MAX_TEXT, stdin);
    tasks[task_count].description[strcspn(tasks[task_count].description, "\n")] = 0;

    get_current_time(tasks[task_count].timestamp, sizeof(tasks[task_count].timestamp));
    printf("Task added: %s (Added on %s)\n", tasks[task_count].description, tasks[task_count].timestamp);

    task_count++;
    save_tasks();
}

void view_tasks() {
    if (task_count == 0) {
        printf("No tasks available.\n");
        return;
    }

    printf("\n📌 To-Do List:\n");
    for (int i = 0; i < task_count; i++) {
        printf("%d. %s (Added on %s)\n", i + 1, tasks[i].description, tasks[i].timestamp);
    }
}

void delete_task() {
    view_tasks();
    printf("Enter task number to delete: ");
    int index;
    scanf("%d", &index);
    getchar(); // consume newline

    if (index <= 0 || index > task_count) {
        printf("Invalid task number.\n");
        return;
    }

    printf("Task removed: %s\n", tasks[index - 1].description);

    for (int i = index - 1; i < task_count - 1; i++) {
        tasks[i] = tasks[i + 1];
    }
    task_count--;
    save_tasks();
}

// --- Diary Functions ---
void save_diary() {
    FILE *file = fopen("diary.txt", "w");
    for (int i = 0; i < diary_count; i++) {
        fprintf(file, "%s|%s\n", diary_entries[i].timestamp, diary_entries[i].entry);
    }
    fclose(file);
}

void load_diary() {
    FILE *file = fopen("diary.txt", "r");
    if (!file) return;

    while (fscanf(file, "%[^|]|%[^\n]\n", diary_entries[diary_count].timestamp, diary_entries[diary_count].entry) == 2) {
        diary_count++;
        if (diary_count >= MAX_DIARY_ENTRIES) break;
    }

    fclose(file);
}

void write_diary() {
    if (diary_count >= MAX_DIARY_ENTRIES) {
        printf("Diary entry limit reached.\n");
        return;
    }

    printf("Write your diary entry: ");
    fgets(diary_entries[diary_count].entry, MAX_TEXT, stdin);
    diary_entries[diary_count].entry[strcspn(diary_entries[diary_count].entry, "\n")] = 0;

    get_current_time(diary_entries[diary_count].timestamp, sizeof(diary_entries[diary_count].timestamp));
    printf("Diary entry saved for %s.\n", diary_entries[diary_count].timestamp);

    diary_count++;
    save_diary();
}

void view_diary() {
    if (diary_count == 0) {
        printf("No diary entries found.\n");
        return;
    }

    printf("\n📖 Diary Entries:\n");
    for (int i = 0; i < diary_count; i++) {
        printf("\nDate: %s\n%s\n------------------------------\n", diary_entries[i].timestamp, diary_entries[i].entry);
    }
}

// --- Main Menu ---
void show_menu() {
    printf("\nOptions: add, view, delete, diary, view_diary, exit\n");
    printf("Enter choice: ");
}

int main() {
    load_tasks();
    load_diary();
    greet_user();

    char choice[64];

    while (1) {
        show_menu();
        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = 0; // Remove newline

        if (strcmp(choice, "add") == 0) {
            add_task();
        } else if (strcmp(choice, "view") == 0) {
            view_tasks();
        } else if (strcmp(choice, "delete") == 0) {
            delete_task();
        } else if (strcmp(choice, "diary") == 0) {
            write_diary();
        } else if (strcmp(choice, "view_diary") == 0) {
            view_diary();
        } else if (strcmp(choice, "exit") == 0) {
            printf("Goodbye! Have a great day! 😊\n");
            break;
        } else {
            printf("Invalid option.\n");
        }
    }

    return 0;
}

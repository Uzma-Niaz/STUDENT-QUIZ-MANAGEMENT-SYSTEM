
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100

typedef struct
{
    char name[50];
    char id[20];
    char password[20];
} Student;

typedef struct
{
    char question[200];
    char option1[50];
    char option2[50];
    char option3[50];
    char option4[50];
    int correctOption;
} Question;

void registerStudent();
int loginStudent();
void createQuiz();
void takeQuiz();
void viewResults();
void timerDelay(int seconds);

//student registration function
void registerStudent() {
    FILE *fp;
    Student s;

    fp = fopen("students.txt", "a");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Name: ");
    scanf(" %[^\n]", s.name);
    printf("Enter ID: ");
    scanf("%s", s.id);
    printf("Enter Password: ");
    scanf("%s", s.password);

    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);

    printf("Registration Successful!\n");
}

int loginStudent() {
    FILE *fp;
    Student s;
    char id[20], password[20];
    int found = 0;

    fp = fopen("students.txt", "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return 0;
    }

    printf("Enter ID: ");
    scanf("%s", id);
    printf("Enter Password: ");
    scanf("%s", password);

    while (fread(&s, sizeof(Student), 1, fp)) {
        if (strcmp(s.id, id) == 0 && strcmp(s.password, password) == 0) {
            printf("Login Successful!\n");
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found) {
        printf("Login Failed. Try again.\n");
    }
    return found;
}

//admin login Function
int adminLogin() {
    char adminPass[20];
    printf("Enter Admin Password: ");
    scanf("%s", adminPass);

    if (strcmp(adminPass, "admin123") == 0) {
        printf("Admin Login Successful!\n");
        return 1; // success
    } else {
        printf("Invalid Admin Password!\n");
        return 0; // failed
    }
}

//quiz creation function
void createQuiz() {
    FILE *fp;
    Question q;
    int n, i;

    fp = fopen("quiz.txt", "w");
    if (fp == NULL) {
        printf("Error opening quiz file!\n");
        return;
    }

    printf("How many questions to add? ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        printf("\nEnter Question %d:\n", i + 1);
        printf("Question: ");
        scanf(" %[^\n]", q.question);
        printf("Option 1: ");
        scanf(" %[^\n]", q.option1);
        printf("Option 2: ");
        scanf(" %[^\n]", q.option2);
        printf("Option 3: ");
        scanf(" %[^\n]", q.option3);
        printf("Option 4: ");
        scanf(" %[^\n]", q.option4);
        printf("Correct Option (1-4): ");
        scanf("%d", &q.correctOption);

        fwrite(&q, sizeof(Question), 1, fp);
    }

    fclose(fp);
    printf("Quiz Created Successfully!\n");
}

//timer enabled quiz attempts function
void takeQuiz() {
    FILE *fp;
    Question q;
    int score = 0, total = 0, answer;
    time_t start, end;
    double elapsed;

    fp = fopen("quiz.txt", "r");
    if (fp == NULL) {
        printf("No Quiz Available.\n");
        return;
    }

    while (fread(&q, sizeof(Question), 1, fp)) {
        printf("\n%s\n", q.question);
        printf("1. %s\n2. %s\n3. %s\n4. %s\n", q.option1, q.option2, q.option3, q.option4);
        printf("You have 30 seconds to answer...\n");

        time(&start);
        printf("Your Answer (1-4): ");
        scanf("%d", &answer);
        time(&end);

        elapsed = difftime(end, start);

        if (elapsed > 30) {
            printf("Time's up!\n");
        } else if (answer == q.correctOption) {
            printf("Correct!\n");
            score++;
        } else {
            printf("Wrong! Correct answer was %d\n", q.correctOption);
        }

        total++;
    }

    fclose(fp);

    printf("\nQuiz Completed! Your Score: %d/%d\n", score, total);

    FILE *rfp = fopen("results.txt", "a");
    if (rfp != NULL) {
        fprintf(rfp, "Score: %d/%d\n", score, total);
        fclose(rfp);
    }
}

void viewResults() {
    FILE *fp;
    char ch;

    fp = fopen("results.txt", "r");
    if (fp == NULL) {
        printf("No Results Available.\n");
        return;
    }

    printf("\n===== Previous Results =====\n");
    while ((ch = fgetc(fp)) != EOF) {
        putchar(ch);
    }
    fclose(fp);
}
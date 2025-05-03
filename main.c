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

//Student registration function
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


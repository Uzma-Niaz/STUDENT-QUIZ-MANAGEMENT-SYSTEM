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
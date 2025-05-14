//Libraries....
#include <stdio.h>
#include <conio.h>  // Required for _getch() function to hide password input
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h> 

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
int loginStudent(char *studentName);
void createQuiz();
void takeQuiz(const char *studentName);
void viewResults();
void timerDelay(int seconds);
void getHiddenPassword() ;
int loginStudent(char *studentName);
char studentName[50];
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

int loginStudent(char *studentName){
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
 printf("Enter Password: ");
getHiddenPassword(password);  // mask input


    while (fread(&s, sizeof(Student), 1, fp)) {
        if (strcmp(s.id, id) == 0 && strcmp(s.password, password) == 0) {
            printf("Login Successful!\n");
            strcpy(studentName, s.name);  // save the name for later
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found) {
        printf( "Login Failed. Try again.\n");
    }
    return found;
}

// HEAD
// Function to take password input with asterisks
void getHiddenPassword(char *password) {
    int i = 0;
    char ch;
    while ((ch = _getch()) != '\r' && i < 19) {  // Stop on Enter key
        if (ch == '\b') {  // Handle backspace
            if (i > 0) {
                printf("\b \b");
                i--;
            }
        } else {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");
}


// Admin login function
// Admin login function with password masking

int adminLogin() {
    char adminPass[20];
    printf("Enter Admin Password: ");
    getHiddenPassword(adminPass);  // mask input

    if (strcmp(adminPass, "12345") == 0) {
        printf("Admin Login Successful!\n");
        return 1;
    } else {
        printf("Invalid Admin Password!\n");
        return 0;
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
void takeQuiz(const char *studentName) {
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

int timeLimit = 30;
int inputGiven = 0;
int ch;

while (timeLimit--) {
    if (_kbhit()) {
        ch = _getch();
        if (ch >= '1' && ch <= '4') {
            answer = ch - '0';
            inputGiven = 1;
            break;
        }
    }
    Sleep(1000); // wait 1 second
    printf("%d seconds remaining...\r", timeLimit);
}

if (!inputGiven) {
    printf("\nTime's up! Moving to next question.\n");
} else if (answer == q.correctOption) {
    printf("\nCorrect!\n");
    score++;
} else {
    printf("\nWrong! Correct answer was %d\n", q.correctOption);
}


        total++;
    }

    fclose(fp);

    printf("\nQuiz Completed! Your Score: %d/%d\n", score, total);

    FILE *rfp = fopen("results.txt", "a");
    if (rfp != NULL) {
     fprintf(rfp, "Name: %s | Score: %d/%d\n", studentName, score, total);
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
//Main menu
int main() {
    int choice, loginSuccess = 0;

    while (1) {
        printf("\n===== Student Quiz Management System =====\n");
        printf("1. Register\n");
        printf("2. Student Login\n");
        printf("3. Admin Login (Create Quiz)\n");
        printf("4. View Results\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerStudent();
                break;
            case 2:
               
             loginSuccess = loginStudent(studentName);
             if (loginSuccess) takeQuiz(studentName);

                break;
            case 3:
    if (adminLogin()) {
        createQuiz();
    }
    break;
            case 4:
                viewResults();
                break;
            case 5:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
 

// Libraries
#include <stdio.h>
#include <conio.h>     // For _getch() and _kbhit() to hide password input and detect key press
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>   // For colors and Sleep

#define MAX 100

// Handle for console color output
HANDLE hConsole;

// Set console text color
void setColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

// Structure to store student data
typedef struct {
    char name[50];
    char id[20];
    char password[20];
} Student;

// Structure to store a quiz question and options
typedef struct {
    char question[200];
    char option1[50], option2[50], option3[50], option4[50];
    int correctOption;
} Question;

// Function declarations
void registerStudent();
int loginStudent(char *studentName);
void createQuiz();
void takeQuiz(const char *studentName);
void viewResults();
void getHiddenPassword(char *password);
int adminLogin();

// Global student name (used after login)
char studentName[50];

// Function to register a student
void registerStudent() {
    FILE *fp;
    Student s;

    system("cls");  // Clear screen

    fp = fopen("students.txt", "a");  // Open student file to append
    if (fp == NULL) {
        setColor(4);
        printf("Error opening file!\n");
        setColor(7);
        return;
    }

    // Take student details
    printf("Enter Name: ");
    scanf(" %[^\n]", s.name);
    printf("Enter ID: ");
    scanf("%s", s.id);
    printf("Enter Password: ");
    scanf("%s", s.password);

    // Save to file
    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);

    setColor(2);
    printf("Registration Successful!\n");
    setColor(7);
}

// Function to login a student
int loginStudent(char *studentName) {
    FILE *fp;
    Student s;
    char id[20], password[20];
    int found = 0;

    system("cls");
    fp = fopen("students.txt", "r");
    if (fp == NULL) {
        setColor(4);
        printf("Error opening file!\n");
        setColor(7);
        return 0;
    }

    // Ask for login credentials
    printf("Enter ID: ");
    scanf("%s", id);
    printf("Enter Password: ");
    getHiddenPassword(password);

    // Check each record in the file
    while (fread(&s, sizeof(Student), 1, fp)) {
        if (strcmp(s.id, id) == 0 && strcmp(s.password, password) == 0) {
            system("cls");
            setColor(2);
            printf("Login Successful!\n");
            setColor(7);
            strcpy(studentName, s.name);  // Save student name
            found = 1;
            break;
        }
    }
    fclose(fp);

    // Show login result
    if (!found) {
        setColor(4);
        printf("Login Failed. Try again.\n");
        setColor(7);
    }
    return found;
}

// Function to get password input with * masking
void getHiddenPassword(char *password) {
    int i = 0;
    char ch;
    while ((ch = _getch()) != '\r' && i < 19) {  // Until Enter is pressed
        if (ch == '\b') {  // Backspace
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

// Function for admin login
int adminLogin() {
    char adminPass[20];
    system("cls");
    printf("Enter Admin Password: ");
    getHiddenPassword(adminPass);

    if (strcmp(adminPass, "12345") == 0) {
        system("cls");
        setColor(3);
        printf("Admin Login Successful!\n");
        setColor(7);
        return 1;
    } else {
        setColor(4);
        printf("Invalid Admin Password!\n");
        setColor(7);
        return 0;
    }
}

// Function to create quiz questions (admin only)
void createQuiz() {
    FILE *fp;
    Question q;
    int n, i;

    system("cls");
    fp = fopen("quiz.txt", "w");
    if (fp == NULL) {
        setColor(4);
        printf("Error opening quiz file!\n");
        setColor(7);
        return;
    }

    // Input number of questions
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

        fwrite(&q, sizeof(Question), 1, fp);  // Save question to file
    }

    fclose(fp);
    setColor(2);
    printf("Quiz Created Successfully!\n");
    setColor(7);
}

// Function to take quiz (for students)
void takeQuiz(const char *studentName) {
    FILE *fp;
    Question q;
    int score = 0, total = 0, answer;
    int ch, inputGiven = 0;
    int timeLimit;

    fp = fopen("quiz.txt", "r");
    if (fp == NULL) {
        setColor(4);
        printf("No Quiz Available.\n");
        setColor(7);
        return;
    }

    system("cls");
    // Read each question from file
    while (fread(&q, sizeof(Question), 1, fp)) {
        printf("\n%s\n", q.question);
        printf("1. %s\n2. %s\n3. %s\n4. %s\n", q.option1, q.option2, q.option3, q.option4);

        setColor(6);
        printf("You have 30 seconds to answer...\n");
        setColor(7);

        timeLimit = 30;
        inputGiven = 0;

        // Wait up to 30 seconds for input
        while (timeLimit--) {
            if (_kbhit()) {
                ch = _getch();
                if (ch >= '1' && ch <= '4') {
                    answer = ch - '0';
                    inputGiven = 1;
                    break;
                }
            }
            Sleep(1000);  // Wait 1 second
            setColor(6);
            printf("%d seconds remaining...\r", timeLimit);
            setColor(7);
        }

        // Evaluate answer
        if (!inputGiven) {
            setColor(4);
            printf("\nTime's up! Moving to next question.\n");
            setColor(7);
        } else if (answer == q.correctOption) {
            setColor(2);
            printf("\nCorrect!\n");
            setColor(7);
            score++;
        } else {
            setColor(4);
            printf("\nWrong! Correct answer was %d\n", q.correctOption);
            setColor(7);
        }
        total++;
    }

    fclose(fp);

    // Show final score
    setColor(3);
    printf("\nQuiz Completed! Your Score: %d/%d\n", score, total);
    setColor(7);

    // Save result to results.txt
    FILE *rfp = fopen("results.txt", "a");
    if (rfp != NULL) {
        fprintf(rfp, "Name: %s | Score: %d/%d\n", studentName, score, total);
        fclose(rfp);
    }
}

// Function to view past results (admin or anyone)
void viewResults() {
    FILE *fp;
    char ch;

    system("cls");
    fp = fopen("results.txt", "r");
    if (fp == NULL) {
        setColor(4);
        printf("No Results Available.\n");
        setColor(7);
        return;
    }

    setColor(3);
    printf("\n===== Previous Results =====\n");
    setColor(7);
    while ((ch = fgetc(fp)) != EOF) {
        putchar(ch);
    }
    fclose(fp);
}

// Main menu loop
int main() {
    int choice, loginSuccess = 0;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // For colors

    while (1) {
        system("cls");  // Clear screen each time menu shows
        setColor(1);
        printf("\n===== Student Quiz Management System =====\n");
        setColor(7);
        printf("1. Register\n");
        printf("2. Student Login\n");
        printf("3. Admin Login (Create Quiz)\n");
        printf("4. View Results\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Menu options
        switch (choice) {
            case 1:
                registerStudent();
                break;
            case 2:
                loginSuccess = loginStudent(studentName);
                if (loginSuccess)
                    takeQuiz(studentName);
                break;
            case 3:
                if (adminLogin())
                    createQuiz();
                break;
            case 4:
                viewResults();
                break;
            case 5:
                setColor(3);
                printf("Exiting...\n");
                setColor(7);
                exit(0);
            default:
                setColor(4);
                printf("Invalid choice. Try again.\n");
                setColor(7);
        }

        printf("\nPress any key to continue...");
        _getch();  // Pause before returning to menu
    }

    return 0;
}


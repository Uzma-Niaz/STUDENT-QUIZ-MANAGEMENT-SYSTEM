#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define MAX 100

HANDLE hConsole;

// Set console text color
void setColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

typedef struct {
    char name[50];
    char id[20];
    char password[20];
} Student;

typedef struct {
    char question[200];
    char option1[50], option2[50], option3[50], option4[50];
    int correctOption;
} Question;

void registerStudent();
int loginStudent(char *studentName);
void createQuiz();
void takeQuiz(const char *studentName);
void viewResults();
void getHiddenPassword(char *password);
int adminLogin();
void displayFeedback(int score, int total);

char studentName[50];

// Register student
void registerStudent() {
    FILE *fp;
    Student s;

    system("cls");
    fp = fopen("students.txt", "a");
    if (fp == NULL) {
        setColor(4);
        printf("Error opening file!\n");
        setColor(7);
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

    setColor(2);
    printf("Registration Successful!\n");
    setColor(7);
}

// Student login
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

    printf("Enter ID: ");
    scanf("%s", id);
    printf("Enter Password: ");
    getHiddenPassword(password);

    while (fread(&s, sizeof(Student), 1, fp)) {
        if (strcmp(s.id, id) == 0 && strcmp(s.password, password) == 0) {
            system("cls");
            setColor(2);
            printf("Login Successful!\n");
            setColor(7);
            strcpy(studentName, s.name);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found) {
        setColor(4);
        printf("Login Failed. Try again.\n");
        setColor(7);
    }
    return found;
}

// Hidden password input
void getHiddenPassword(char *password) {
    int i = 0;
    char ch;
    while ((ch = _getch()) != '\r' && i < 19) {
        if (ch == '\b') {
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

// Admin login
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

// Create quiz questions
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
    setColor(2);
    printf("Quiz Created Successfully!\n");
    setColor(7);
}

// Display detailed feedback with color coding
void displayFeedback(int score, int total) {
    float percentage = ((float)score / total) * 100;
    
    setColor(3);
    printf("\n===== Quiz Results =====\n");
    printf("Score: %d/%d\n", score, total);
    printf("Percentage: %.1f%%\n", percentage);
    
    setColor(14); // Yellow for feedback title
    printf("\nFeedback: ");
    
    if (percentage >= 90) {
        setColor(10); // Bright green
        printf("Outstanding!\n");
        printf("You have demonstrated excellent understanding of the material.\n");
        printf("Keep up the great work!\n");
    }
    else if (percentage >= 75) {
        setColor(2); // Green
        printf("Very Good!\n");
        printf("You have a strong grasp of most concepts with room for minor improvements.\n");
        printf("Review the questions you missed to perfect your knowledge.\n");
    }
    else if (percentage >= 60) {
        setColor(6); // Yellow
        printf("Good Attempt\n");
        printf("You understand the basics but need more practice in some areas.\n");
        printf("Focus on the topics related to the questions you missed.\n");
    }
    else if (percentage >= 40) {
        setColor(12); // Light red
        printf("Needs Improvement\n");
        printf("You're on the right track but need to review the material more thoroughly.\n");
        printf("Consider studying the topics again and retaking the quiz.\n");
    }
    else {
        setColor(4); // Red
        printf("Requires Significant Improvement\n");
        printf("You should revisit the study materials and concepts.\n");
        printf("Don't get discouraged - use this as a learning opportunity!\n");
    }
    
    setColor(7); // Reset to default color
}

// Take quiz (with shuffled questions)
void takeQuiz(const char *studentName) {
    FILE *fp;
    Question questions[MAX];
    int total = 0, i, score = 0;
    int answer, ch, inputGiven, timeLimit;

    fp = fopen("quiz.txt", "r");
    if (fp == NULL) {
        setColor(4);
        printf("No Quiz Available.\n");
        setColor(7);
        return;
    }

    while (fread(&questions[total], sizeof(Question), 1, fp) == 1 && total < MAX)
        total++;
    fclose(fp);

    // Shuffle questions
    srand(time(NULL));
    for (i = total - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Question temp = questions[i];
        questions[i] = questions[j];
        questions[j] = temp;
    }

    system("cls");
    for (i = 0; i < total; i++) {
        printf("\n%s\n", questions[i].question);
        printf("1. %s\n2. %s\n3. %s\n4. %s\n", questions[i].option1, questions[i].option2, questions[i].option3, questions[i].option4);

        setColor(6);
        printf("You have 30 seconds to answer...\n");
        setColor(7);

        timeLimit = 30;
        inputGiven = 0;

        while (timeLimit--) {
            if (_kbhit()) {
                ch = _getch();
                if (ch >= '1' && ch <= '4') {
                    answer = ch - '0';
                    inputGiven = 1;
                    break;
                }
            }
            Sleep(1000);
            setColor(6);
            printf("%d seconds remaining...\r", timeLimit);
            setColor(7);
        }

        if (!inputGiven) {
            setColor(4);
            printf("\nTime's up! Moving to next question.\n");
            setColor(7);
        } else if (answer == questions[i].correctOption) {
            setColor(2);
            printf("\nCorrect!\n");
            setColor(7);
            score++;
        } else {
            setColor(4);
            printf("\nWrong! Correct answer was %d\n", questions[i].correctOption);
            setColor(7);
        }
    }

    // Display detailed feedback
    displayFeedback(score, total);

    // Save results to file
    FILE *rfp = fopen("results.txt", "a");
    if (rfp != NULL) {
        float percentage = ((float)score / total) * 100;
        char feedback[50];
        
        if (percentage >= 90) strcpy(feedback, "Outstanding");
        else if (percentage >= 75) strcpy(feedback, "Very Good");
        else if (percentage >= 60) strcpy(feedback, "Good Attempt");
        else if (percentage >= 40) strcpy(feedback, "Needs Improvement");
        else strcpy(feedback, "Requires Significant Improvement");
        
        fprintf(rfp, "Name: %s | Score: %d/%d (%.1f%%) | Feedback: %s\n", 
                studentName, score, total, percentage, feedback);
        fclose(rfp);
    }
}

// View results
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
    while ((ch = fgetc(fp)) != EOF)
        putchar(ch);
    fclose(fp);
}

// Main menu
int main() {
    int choice, loginSuccess = 0;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    while (1) {
        system("cls");
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

        switch (choice) {
            case 1:
                registerStudent();
                break;
            case 2:
                loginSuccess = loginStudent(studentName);
                if (loginSuccess) {
                    printf("\nPress any key to start the quiz...");
                    _getch();
                    takeQuiz(studentName);
                }
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
        _getch();
    }

    return 0;
}

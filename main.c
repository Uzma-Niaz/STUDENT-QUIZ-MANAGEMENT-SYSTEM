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

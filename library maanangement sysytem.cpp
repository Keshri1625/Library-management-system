#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Book {
    int id;
    char title[50];
    char author[50];
    int available; // 1 = available, 0 = issued
};

void addBook();
void displayBooks();
void searchBook();
void issueBook();
void returnBook();
void deleteBook();
void updateBook();

int main() {
    int choice;
    while(1) {
        printf("\n===== Library Management System =====\n");
        printf("1. Add Book\n2. Display Books\n3. Search Book\n");
        printf("4. Issue Book\n5. Return Book\n6. Delete Book\n7. Update Book\n8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: issueBook(); break;
            case 5: returnBook(); break;
            case 6: deleteBook(); break;
            case 7: updateBook(); break;
            case 8: exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

void addBook() {
    struct Book b;
    FILE *fp = fopen("library.dat", "ab");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Book ID: ");
    scanf("%d", &b.id);
    printf("Enter Title: ");
    scanf(" %[^\n]", b.title);
    printf("Enter Author: ");
    scanf(" %[^\n]", b.author);
    b.available = 1; // Initially available

    fwrite(&b, sizeof(b), 1, fp);
    fclose(fp);
    printf("Book added successfully!\n");
}

void displayBooks() {
    struct Book b;
    FILE *fp = fopen("library.dat", "rb");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    printf("\n--- Books in Library ---\n");
    while(fread(&b, sizeof(b), 1, fp)) {
        printf("ID: %d | Title: %s | Author: %s | Available: %s\n",
               b.id, b.title, b.author, b.available ? "Yes" : "No");
    }
    fclose(fp);
}

void searchBook() {
    int id;
    int found = 0;
    struct Book b;
    FILE *fp = fopen("library.dat", "rb");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    printf("Enter Book ID to search: ");
    scanf("%d", &id);

    while(fread(&b, sizeof(b), 1, fp)) {
        if(b.id == id) {
            printf("Book Found: ID: %d | Title: %s | Author: %s | Available: %s\n",
                   b.id, b.title, b.author, b.available ? "Yes" : "No");
            found = 1;
            break;
        }
    }

    if(!found) {
        printf("Book with ID %d not found!\n", id);
    }

    fclose(fp);
}

void issueBook() {
    int id;
    int found = 0;
    struct Book b;
    FILE *fp = fopen("library.dat", "rb+");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    printf("Enter Book ID to issue: ");
    scanf("%d", &id);

    while(fread(&b, sizeof(b), 1, fp)) {
        if(b.id == id) {
            if(b.available == 1) {
                b.available = 0;
                fseek(fp, -sizeof(b), SEEK_CUR);
                fwrite(&b, sizeof(b), 1, fp);
                printf("Book issued successfully!\n");
            } else {
                printf("Book is already issued.\n");
            }
            found = 1;
            break;
        }
    }

    if(!found) {
        printf("Book with ID %d not found!\n", id);
    }

    fclose(fp);
}

void returnBook() {
    int id;
    int found = 0;
    struct Book b;
    FILE *fp = fopen("library.dat", "rb+");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    printf("Enter Book ID to return: ");
    scanf("%d", &id);

    while(fread(&b, sizeof(b), 1, fp)) {
        if(b.id == id) {
            if(b.available == 0) {
                b.available = 1;
                fseek(fp, -sizeof(b), SEEK_CUR);
                fwrite(&b, sizeof(b), 1, fp);
                printf("Book returned successfully!\n");
            } else {
                printf("Book was not issued.\n");
            }
            found = 1;
            break;
        }
    }

    if(!found) {
        printf("Book with ID %d not found!\n", id);
    }

    fclose(fp);
}

void deleteBook() {
    int id;
    int found = 0;
    struct Book b;
    FILE *fp = fopen("library.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (fp == NULL || temp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Book ID to delete: ");
    scanf("%d", &id);

    while(fread(&b, sizeof(b), 1, fp)) {
        if(b.id == id) {
            found = 1;
            continue; // skip writing this book to temp file
        }
        fwrite(&b, sizeof(b), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("library.dat");
    rename("temp.dat", "library.dat");

    if(found) {
        printf("Book deleted successfully!\n");
    } else {
        printf("Book with ID %d not found!\n", id);
    }
}

void updateBook() {
    int id;
    int found = 0;
    struct Book b;
    FILE *fp = fopen("library.dat", "rb+");
    if (fp == NULL) {
        printf("No records found!\n");
        return;
    }

    printf("Enter Book ID to update: ");
    scanf("%d", &id);

    while(fread(&b, sizeof(b), 1, fp)) {
        if(b.id == id) {
            printf("Enter new Title: ");
            scanf(" %[^\n]", b.title);
            printf("Enter new Author: ");
            scanf(" %[^\n]", b.author);
            fseek(fp, -sizeof(b), SEEK_CUR);
            fwrite(&b, sizeof(b), 1, fp);
            printf("Book updated successfully!\n");
            found = 1;
            break;
        }
    }

    if(!found) {
        printf("Book with ID %d not found!\n", id);
    }

    fclose(fp);
}


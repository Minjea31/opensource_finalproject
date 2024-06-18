#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ResearchPaper ����ü ����
typedef struct {
    char* title;
    char* author;
    int year;
    char* url;
} ResearchPaper;

// ��� ����ü ����
typedef struct Node {
    ResearchPaper paper;
    struct Node* next;
} Node;

// ���ڿ��� �����ϴ� �Լ�
char* my_strdup(const char* str) {
    char* copy = (char*)malloc(strlen(str) + 1);
    if (copy) {
        strcpy(copy, str);
    }
    return copy;
}

// ���Ḯ��Ʈ�� ���� ���
Node* head = NULL;

// ���ο� ��带 �����ϰ� ���Ḯ��Ʈ�� �߰��ϴ� �Լ�
void addPaper(char* title, char* author, int year, char* url) {
    // ���ο� ��� ����
    Node* newNode = (Node*)malloc(sizeof(Node));

    // ResearchPaper �� ����
    newNode->paper.title = my_strdup(title);
    newNode->paper.author = my_strdup(author);
    newNode->paper.year = year;
    newNode->paper.url = my_strdup(url);
    newNode->next = NULL;

    // ����Ʈ�� ��������� ���ο� ��带 ���� ���� ����
    if (head == NULL) {
        head = newNode;
    }
    else {
        // ����Ʈ�� ���� ���ο� ��� �߰�
        Node* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// ���Ḯ��Ʈ�� ��� ��带 ����ϴ� �Լ�
void printPapers() {
    Node* temp = head;
    while (temp != NULL) {
        printf("����: %s\n", temp->paper.title);
        printf("����: %s\n", temp->paper.author);
        printf("����: %d\n", temp->paper.year);
        printf("�ּ�: %s\n\n", temp->paper.url);
        temp = temp->next;
    }
}

// ���� �Լ�
int main() {
    char title[100];
    char author[100];
    int year;
    char url[100];

    while (1) {
        printf("����(exit �Է½� ����) : ");
        scanf("%99s", title);
        if (strcmp(title, "exit") == 0) {
            break;
        }

        printf("���� : ");
        scanf("%99s", author);

        printf("���� : ");
        scanf("%d", &year);

        printf("URL �ּ�: ");
        scanf("%99s", url);

        addPaper(title, author, year, url);
    }

    printf("\n�� ����Ʈ:\n");
    printPapers();

    return 0;
}
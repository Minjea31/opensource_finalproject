#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ResearchPaper 구조체 정의
typedef struct {
    char* title;
    char* author;
    int year;
    char* url;
} ResearchPaper;

// 노드 구조체 정의
typedef struct Node {
    ResearchPaper paper;
    struct Node* next;
} Node;

// 문자열을 복사하는 함수
char* my_strdup(const char* str) {
    char* copy = (char*)malloc(strlen(str) + 1);
    if (copy) {
        strcpy(copy, str);
    }
    return copy;
}

// 연결리스트의 시작 노드
Node* head = NULL;

// 새로운 노드를 생성하고 연결리스트에 추가하는 함수
void addPaper(char* title, char* author, int year, char* url) {
    // 새로운 노드 생성
    Node* newNode = (Node*)malloc(sizeof(Node));

    // ResearchPaper 값 설정
    newNode->paper.title = my_strdup(title);
    newNode->paper.author = my_strdup(author);
    newNode->paper.year = year;
    newNode->paper.url = my_strdup(url);
    newNode->next = NULL;

    // 리스트가 비어있으면 새로운 노드를 시작 노드로 설정
    if (head == NULL) {
        head = newNode;
    }
    else {
        // 리스트의 끝에 새로운 노드 추가
        Node* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// 연결리스트의 모든 노드를 출력하는 함수
void printPapers() {
    Node* temp = head;
    while (temp != NULL) {
        printf("제목: %s\n", temp->paper.title);
        printf("저자: %s\n", temp->paper.author);
        printf("연도: %d\n", temp->paper.year);
        printf("주소: %s\n\n", temp->paper.url);
        temp = temp->next;
    }
}

// 메인 함수
int main() {
    char title[100];
    char author[100];
    int year;
    char url[100];

    while (1) {
        printf("제목(exit 입력시 종료) : ");
        scanf("%99s", title);
        if (strcmp(title, "exit") == 0) {
            break;
        }

        printf("저자 : ");
        scanf("%99s", author);

        printf("연도 : ");
        scanf("%d", &year);

        printf("URL 주소: ");
        scanf("%99s", url);

        addPaper(title, author, year, url);
    }

    printf("\n논문 리스트:\n");
    printPapers();

    return 0;
}
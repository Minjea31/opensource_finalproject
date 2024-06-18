#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ResearchPaper_contents 구조체 정의
typedef struct {
    int page;
    char* contents;
} ResearchPaper_contents;

// 노드 구조체 정의
typedef struct Node {
    ResearchPaper_contents paperContent;
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

// 새로운 노드를 생성하고 연결리스트에 추가하는 함수 (오름차순으로 정렬)
void addPaperContent(int page, char* contents) {
    // 새로운 노드 생성
    Node* newNode = (Node*)malloc(sizeof(Node));

    // ResearchPaper_contents 값 설정
    newNode->paperContent.page = page;
    newNode->paperContent.contents = my_strdup(contents);
    newNode->next = NULL;

    // 리스트가 비어있으면 새로운 노드를 시작 노드로 설정
    if (head == NULL || head->paperContent.page > page) {
        newNode->next = head;
        head = newNode;
    }
    else {
        // 리스트의 적절한 위치에 새로운 노드 삽입
        Node* temp = head;
        while (temp->next != NULL && temp->next->paperContent.page < page) {
            temp = temp->next;
        }
        newNode->next = temp->next;
        temp->next = newNode;
    }
}

// 연결리스트의 모든 노드를 출력하는 함수
void printPaperContents() {
    Node* temp = head;
    while (temp != NULL) {
        printf("페이지 : %d\n", temp->paperContent.page);
        printf("중요 내용 : %s\n\n", temp->paperContent.contents);
        temp = temp->next;
    }
}

// 메인 함수
int main()
{
    int page;
    char contents[500]; // 내용을 저장할 배열 크기를 충분히 크게 설정

    while (1) {
        printf("페이지를 입력하시오 (종료 : -1) : ");
        scanf("%d", &page);
        if (page == -1) {
            break;
        }

        printf("내용을 입력하시오 : ");
        getchar(); // 버퍼 비우기
        fgets(contents, sizeof(contents), stdin);
        contents[strcspn(contents, "\n")] = 0; // fgets로 입력받은 문자열에서 개행문자를 제거

        addPaperContent(page, contents);
    }

    printf("\n논문 내용 리스트 :\n");
    printPaperContents();

    return 0;
}

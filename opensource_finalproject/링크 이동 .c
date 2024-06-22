#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <shellapi.h>

#pragma warning(disable : 4996)

// ResearchPaper_contents 구조체 정의
typedef struct {
    int page;
    char* keywords; // 키워드 추가
} ResearchPaper_contents;

// ResearchPaper 구조체 정의
typedef struct ResearchPaper {
    char* title;
    char* author;
    int year;
    char* url;
    struct ContentNode* contentsHead; // 논문 내용의 연결리스트의 시작 노드
    struct ResearchPaper* next;
} ResearchPaper;

// 논문 내용의 노드 구조체 정의
typedef struct ContentNode {
    ResearchPaper_contents paperContent;
    struct ContentNode* next;
} ContentNode;

// 문자열을 복사하는 함수
char* my_strdup(const char* str) {
    char* copy = (char*)malloc(strlen(str) + 1);
    if (copy) {
        strcpy(copy, str);
    }
    return copy;
}

// 논문 연결리스트의 시작 노드
ResearchPaper* head = NULL;

// 링크를 열기 위한 함수
void openLink(const char* url) {
    ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

// 특정 제목의 논문을 출력하고 링크를 클릭하여 열 수 있도록 설정
void printPaperByTitle(const char* title) {
    ResearchPaper* paperTemp = head;
    while (paperTemp != NULL) {
        if (strcmp(paperTemp->title, title) == 0) {
            printf("제목: %s\n", paperTemp->title);
            printf("저자: %s\n", paperTemp->author);
            printf("연도: %d\n", paperTemp->year);
            printf("주소: %s\n", paperTemp->url);

            // 링크를 열기 위해 함수 호출
            openLink(paperTemp->url);

            ContentNode* contentTemp = paperTemp->contentsHead;
            while (contentTemp != NULL) {
                printf("페이지: %d\n", contentTemp->paperContent.page);
                printf("키워드: %s\n", contentTemp->paperContent.keywords); // 키워드 출력 추가
                contentTemp = contentTemp->next;
            }
            return;
        }
        paperTemp = paperTemp->next;
    }

    printf("제목에 해당하는 논문을 찾을 수 없습니다.\n");
}

int main() {
    // 예제를 위해 논문 데이터 초기화
    ResearchPaper paper1 = { "Sample Paper 1", "John Doe", 2023, "https://www.example.com/paper1", NULL, NULL };
    head = &paper1;

    // 특정 제목의 논문 출력 및 링크 열기 테스트
    printPaperByTitle("Sample Paper 1");

    return 0;
}

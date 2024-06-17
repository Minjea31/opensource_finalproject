#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ResearchPaper_contents 구조체 정의
typedef struct {
    int page;
    char* contents;
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

// 새로운 논문 노드를 생성하고 연결리스트에 추가하는 함수
void addPaper(char* title, char* author, int year, char* url) {
    // 새로운 논문 노드 생성
    ResearchPaper* newPaper = (ResearchPaper*)malloc(sizeof(ResearchPaper));

    // ResearchPaper 값 설정
    newPaper->title = my_strdup(title);
    newPaper->author = my_strdup(author);
    newPaper->year = year;
    newPaper->url = my_strdup(url);
    newPaper->contentsHead = NULL; // 내용의 시작 노드는 NULL로 초기화
    newPaper->next = NULL;

    // 리스트가 비어있으면 새로운 논문 노드를 시작 노드로 설정
    if (head == NULL) {
        head = newPaper;
    }
    else {
        // 리스트의 끝에 새로운 논문 노드 추가
        ResearchPaper* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newPaper;
    }
}

// 논문 내용 노드를 추가하는 함수
void addPaperContent(ResearchPaper* paper, int page, char* contents) {
    // 새로운 논문 내용 노드 생성
    ContentNode* newNode = (ContentNode*)malloc(sizeof(ContentNode));

    // ResearchPaper_contents 값 설정
    newNode->paperContent.page = page;
    newNode->paperContent.contents = my_strdup(contents);
    newNode->next = NULL;

    // 내용 리스트가 비어있으면 새로운 내용을 시작 노드로 설정
    if (paper->contentsHead == NULL) {
        paper->contentsHead = newNode;
    }
    else {
        // 내용 리스트의 끝에 새로운 내용 노드 추가
        ContentNode* temp = paper->contentsHead;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// 연결리스트의 모든 논문과 그 내용을 출력하는 함수
void printPapers() {
    ResearchPaper* paperTemp = head;
    while (paperTemp != NULL) {
        printf("제목: %s\n", paperTemp->title);
        printf("저자: %s\n", paperTemp->author);
        printf("연도: %d\n", paperTemp->year);
        printf("주소: %s\n", paperTemp->url);

        ContentNode* contentTemp = paperTemp->contentsHead;
        while (contentTemp != NULL) {
            printf("  페이지: %d\n", contentTemp->paperContent.page);
            printf("  내용: %s\n", contentTemp->paperContent.contents);
            contentTemp = contentTemp->next;
        }
        printf("\n");
        paperTemp = paperTemp->next;
    }
}

// 논문 리스트를 파일에 저장하는 함수 (각 논문 제목으로 파일 생성)
void savePapersToFile(const char* folderPath) {
    ResearchPaper* paperTemp = head;
    while (paperTemp != NULL) {
        // 파일명을 논문 제목으로 지정
        char filename[256]; // 적절한 길이로 설정
        snprintf(filename, sizeof(filename), "%s/%s.txt", folderPath, paperTemp->title);

        FILE* file = fopen(filename, "w");
        if (file == NULL) {
            printf("파일을 열 수 없습니다: %s\n", filename);
        }
        else {
            // 파일에 논문 정보 작성
            fprintf(file, "제목: %s\n", paperTemp->title);
            fprintf(file, "저자: %s\n", paperTemp->author);
            fprintf(file, "연도: %d\n", paperTemp->year);
            fprintf(file, "주소: %s\n", paperTemp->url);

            ContentNode* contentTemp = paperTemp->contentsHead;
            while (contentTemp != NULL) {
                fprintf(file, "  페이지: %d\n", contentTemp->paperContent.page);
                fprintf(file, "  내용: %s\n", contentTemp->paperContent.contents);
                contentTemp = contentTemp->next;
            }

            fclose(file);
            printf("파일 저장 완료: %s\n", filename);
        }

        paperTemp = paperTemp->next;
    }
}

// 메인 함수
int main() {
    char title[100];
    char author[100];
    int year;
    char url[100];
    int page;
    char contents[500];
    const char* folderPath = "C:\\Users\\kimmi\\Desktop\\논문정리"; // 저장할 폴더 경로

    while (1) {
        printf("논문 제목 (exit 입력시 종료): ");
        scanf("%99s", title);
        if (strcmp(title, "exit") == 0) {
            break;
        }

        printf("저자: ");
        scanf("%99s", author);

        printf("연도: ");
        scanf("%d", &year);

        printf("URL 주소: ");
        scanf("%99s", url);

        addPaper(title, author, year, url);

        ResearchPaper* currentPaper = head;
        while (currentPaper->next != NULL) {
            currentPaper = currentPaper->next;
        }

        while (1) {
            printf("페이지를 입력하시오 (종료: -1): ");
            scanf("%d", &page);
            if (page == -1) {
                break;
            }

            getchar(); // 버퍼 비우기
            printf("내용을 입력하시오: ");
            fgets(contents, sizeof(contents), stdin);
            contents[strcspn(contents, "\n")] = 0; // fgets로 입력받은 문자열에서 개행문자를 제거

            addPaperContent(currentPaper, page, contents);
        }
    }

    printf("\n논문 리스트:\n");
    printPapers();

    // 각 논문을 파일로 저장
    savePapersToFile(folderPath);

    return 0;
}
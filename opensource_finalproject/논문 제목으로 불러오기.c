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

// 함수 프로토타입 선언
void addPaper(char* title, char* author, int year, char* url);
void addPaperContent(ResearchPaper* paper, int page, char* contents);
void readPaperFromFile(const char* filepath);
void editPaper(ResearchPaper* paper);
void addContentToPaper(ResearchPaper* paper);
void readAndEditPaper();
void printPapers();
char* my_strdup(const char* str);

// ResearchPaper 연결리스트의 시작 노드
ResearchPaper* head = NULL;

// 문자열을 복사하는 함수
char* my_strdup(const char* str) {
    char* copy = (char*)malloc(strlen(str) + 1);
    if (copy) {
        strcpy(copy, str);
    }
    return copy;
}

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
        // 내용 리스트의 적절한 위치를 찾아서 삽입
        ContentNode* temp = paper->contentsHead;
        ContentNode* prev = NULL;

        while (temp != NULL && temp->paperContent.page < page) {
            prev = temp;
            temp = temp->next;
        }

        if (prev == NULL) {
            newNode->next = paper->contentsHead;
            paper->contentsHead = newNode;
        }
        else {
            newNode->next = prev->next;
            prev->next = newNode;
        }
    }
}

// 파일에서 논문 정보를 읽어와 연결리스트에 추가하는 함수
void readPaperFromFile(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        printf("파일을 열 수 없습니다: %s\n", filepath);
        return;
    }

    char title[100];
    char author[100];
    int year;
    char url[100];
    int page;
    char contents[500];

    // 제목, 저자, 연도, 주소 읽기
    fscanf(file, "제목: %99[^\n]\n", title);
    fscanf(file, "저자: %99[^\n]\n", author);
    fscanf(file, "연도: %d\n", &year);
    fscanf(file, "주소: %99[^\n]\n", url);

    // 새로운 논문 추가
    addPaper(title, author, year, url);
    ResearchPaper* currentPaper = head;
    while (currentPaper->next != NULL) {
        currentPaper = currentPaper->next;
    }

    // 페이지와 내용 읽어서 논문 내용에 추가
    while (fscanf(file, " 페이지: %d\n 내용: %499[^\n]\n", &page, contents) == 2) {
        addPaperContent(currentPaper, page, contents);
    }

    fclose(file);
}

// 특정 논문을 수정하는 함수
void editPaper(ResearchPaper* paper) {
    char input[100];
    int year;

    // 제목 수정
    printf("제목(%s): ", paper->title);
    fgets(input, sizeof(input), stdin);
    if (strcmp(input, "\n") != 0) {
        input[strcspn(input, "\n")] = '\0'; // 개행 문자 제거
        free(paper->title);
        paper->title = my_strdup(input);
    }

    // 저자 수정
    printf("저자(%s): ", paper->author);
    fgets(input, sizeof(input), stdin);
    if (strcmp(input, "\n") != 0) {
        input[strcspn(input, "\n")] = '\0'; // 개행 문자 제거
        free(paper->author);
        paper->author = my_strdup(input);
    }

    // 연도 수정
    printf("연도(%d): ", paper->year);
    fgets(input, sizeof(input), stdin);
    if (strcmp(input, "\n") != 0) {
        sscanf(input, "%d", &year);
        paper->year = year;
    }

    // 주소 수정
    printf("주소(%s): ", paper->url);
    fgets(input, sizeof(input), stdin);
    if (strcmp(input, "\n") != 0) {
        input[strcspn(input, "\n")] = '\0'; // 개행 문자 제거
        free(paper->url);
        paper->url = my_strdup(input);
    }

    // 내용 추가
    printf("내용을 추가합니다.\n");
    addContentToPaper(paper);
}

// 특정 논문 내용을 추가하는 함수
void addContentToPaper(ResearchPaper* paper) {
    int page;
    char contents[500];

    while (1) {
        // 페이지 번호 입력
        printf("추가할 페이지 번호(종료: -1): ");
        scanf("%d", &page);
        getchar(); // scanf 후 개행 문자 처리
        if (page == -1) {
            break;
        }

        // 추가할 내용 입력
        printf("추가할 내용: ");
        fgets(contents, sizeof(contents), stdin);
        contents[strcspn(contents, "\n")] = '\0'; // 개행 문자 제거

        // 논문에 내용 추가
        addPaperContent(paper, page, contents);
    }
}

// 파일에서 논문 정보를 읽고 수정할 수 있게 하는 함수
void readAndEditPaper() {
    while (1) {
        char title[100];
        printf("수정할 논문의 제목을 입력하세요 (종료를 입력하면 프로그램이 종료됩니다): ");
        fgets(title, sizeof(title), stdin);
        title[strcspn(title, "\n")] = '\0'; // 개행 문자 제거

        // 종료 조건 확인
        if (strcmp(title, "종료") == 0) {
            break;
        }

        // 파일 경로 생성
        char filepath[200];
        sprintf(filepath, "C:\\Users\\kimmi\\Desktop\\논문정리\\%s.txt", title);

        // 파일 열기 시도
        FILE * file = fopen(filepath, "r");
        if (file == NULL) {
            printf("파일을 열 수 없습니다: %s\n", filepath);
            printf("다시 입력하거나 '종료'를 입력하세요.\n");
            continue;
        }
        fclose(file);

        // 파일에서 논문 정보를 읽어서 연결리스트에 추가
        readPaperFromFile(filepath);

        // 리스트에서 해당 논문을 찾아 수정
        ResearchPaper* paperTemp = head;
        while (paperTemp != NULL) {
            if (strcmp(paperTemp->title, title) == 0) {
                editPaper(paperTemp); // 논문 수정
                break;
            }
            paperTemp = paperTemp->next;
        }
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

// 메인 함수
int main() {
    // 논문 수정 기능
    readAndEditPaper();

    // 수정 후 논문 리스트 출력
    printf("\n수정된 논문 리스트:\n");
    printPapers();

    return 0;
}

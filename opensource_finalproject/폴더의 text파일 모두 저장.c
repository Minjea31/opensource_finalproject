//파일주소를 입력하면 안에 있는 text파일을 순회하면서 
//저장할려고 했는데 일단 안됨.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_PATH_LEN 260

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

    fscanf(file, "제목: %99[^\n]\n", title);
    fscanf(file, "저자: %99[^\n]\n", author);
    fscanf(file, "연도: %d\n", &year);
    fscanf(file, "주소: %99[^\n]\n", url);

    addPaper(title, author, year, url);
    ResearchPaper* currentPaper = head;
    while (currentPaper->next != NULL) {
        currentPaper = currentPaper->next;
    }

    while (fscanf(file, " 페이지: %d\n 내용: %499[^\n]\n", &page, contents) == 2) {
        addPaperContent(currentPaper, page, contents);
    }

    fclose(file);
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

// 특정 폴더 내의 모든 텍스트 파일을 순회하면서 논문 정보를 읽어오는 함수
void readAllPapersFromFolder(const char* folderPath) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    char searchPath[MAX_PATH_LEN];

    snprintf(searchPath, sizeof(searchPath), "%s\\*.txt", folderPath);
    hFind = FindFirstFile(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("폴더를 열 수 없습니다: %s\n", folderPath);
        return;
    }

    do {
        char filePath[MAX_PATH_LEN];
        snprintf(filePath, sizeof(filePath), "%s\\%s", folderPath, findFileData.cFileName);
        FILE* file = fopen(filePath, "r");
        if (file == NULL) {
            printf("파일을 열 수 없습니다: %s\n", filePath);
        }
        else {
            fclose(file);
            readPaperFromFile(filePath);
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

// 메인 함수
int main() {
    const char* folderPath = "C:\\Users\\kimmi\\Desktop\\논문정리\\"; // 순회할 폴더 경로
    printf("검색 경로: %s\n", folderPath);

    // 폴더 내의 모든 텍스트 파일을 읽어와서 논문 정보를 저장
    readAllPapersFromFolder(folderPath);

    // 논문 리스트 출력
    printf("\n논문 리스트:\n");
    printPapers();

    return 0;
}

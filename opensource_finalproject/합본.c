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

// 파일에서 논문 정보를 읽어와 연결리스트에 추가하는 함수
void readPaperFromFile(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    while (file == NULL) {
        printf("파일을 열 수 없습니다: %s\n", filepath);
        printf("다시 입력하세요: ");
        char new_filepath[200];
        fgets(new_filepath, sizeof(new_filepath), stdin);
        new_filepath[strcspn(new_filepath, "\n")] = '\0'; // 개행 문자 제거
        file = fopen(new_filepath, "r");
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

// 특정 제목의 논문 파일에서 정보를 읽어와 연결리스트에 추가하는 함수
void readPaperByTitle(const char* title) {
    // 파일 경로 생성
    char filepath[200];
    sprintf(filepath, "C:\\Users\\kimmi\\Desktop\\논문정리\\%s.txt", title);

    // 파일 열기 시도
    FILE* file = fopen(filepath, "r");
    while (file == NULL) {
        printf("파일을 열 수 없습니다: %s\n", filepath);
        printf("다시 입력하세요: ");
        char new_title[100];
        fgets(new_title, sizeof(new_title), stdin);
        new_title[strcspn(new_title, "\n")] = '\0'; // 개행 문자 제거
        sprintf(filepath, "C:\\Users\\kimmi\\Desktop\\논문정리\\%s.txt", new_title);
        file = fopen(filepath, "r");
    }

    char file_title[100];
    char author[100];
    int year;
    char url[100];
    int page;
    char contents[500];

    // 제목, 저자, 연도, 주소 읽기
    fscanf(file, "제목: %99[^\n]\n", file_title);
    fscanf(file, "저자: %99[^\n]\n", author);
    fscanf(file, "연도: %d\n", &year);
    fscanf(file, "주소: %99[^\n]\n", url);

    // 새로운 논문 추가
    addPaper(file_title, author, year, url);
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
}

// 특정 논문 내용을 추가하는 함수
void addContentToPaper(ResearchPaper* paper) {
    int page;
    char contents[500];

    while (1) {
        // 페이지 번호 입력
        printf("추가할 페이지 번호(종료: -1): ");
        scanf("%d", &page);
        getchar(); // scanf 이후 남은 개행 문자를 제거
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

// 논문 내용의 페이지 순서대로 정렬하는 함수
void sortPaperContents(ResearchPaper* paper) {
    if (paper->contentsHead == NULL) {
        return;
    }

    ContentNode* sorted = NULL;

    ContentNode* current = paper->contentsHead;
    while (current != NULL) {
        ContentNode* next = current->next;
        if (sorted == NULL || sorted->paperContent.page > current->paperContent.page) {
            current->next = sorted;
            sorted = current;
        }
        else {
            ContentNode* temp = sorted;
            while (temp->next != NULL && temp->next->paperContent.page <= current->paperContent.page) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }

    paper->contentsHead = sorted;
}

// 특정 논문을 검색하는 함수
ResearchPaper* searchPaper(char* title) {
    ResearchPaper* current = head;
    while (current != NULL) {
        if (strcmp(current->title, title) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// 모든 논문 정보를 출력하는 함수
void printPapers() {
    ResearchPaper* current = head;
    while (current != NULL) {
        printf("제목: %s\n저자: %s\n연도: %d\n주소: %s\n", current->title, current->author, current->year, current->url);

        ContentNode* contentNode = current->contentsHead;
        while (contentNode != NULL) {
            printf(" 페이지 %d: %s\n", contentNode->paperContent.page, contentNode->paperContent.contents);
            contentNode = contentNode->next;
        }

        printf("\n");
        current = current->next;
    }
}

int main() {
    int choice;
    char title[100];
    ResearchPaper* paper;

    while (1) {
        printf("\n메뉴:\n");
        printf("1. 논문 추가\n");
        printf("2. 논문 내용 추가\n");
        printf("3. 논문 수정\n");
        printf("4. 논문 내용 정렬\n");
        printf("5. 논문 검색\n");
        printf("6. 논문 파일에서 불러오기\n");
        printf("7. 특정 제목의 논문 파일에서 불러오기\n");
        printf("8. 모든 논문 출력\n");
        printf("9. 종료\n");
        printf("선택: ");
        scanf("%d", &choice);
        getchar(); // 개행 문자 제거

        switch (choice) {
        case 1:
            printf("제목: ");
            fgets(title, sizeof(title), stdin);
            title[strcspn(title, "\n")] = '\0'; // 개행 문자 제거
            char author[100];
            int year;
            char url[100];
            printf("저자: ");
            fgets(author, sizeof(author), stdin);
            author[strcspn(author, "\n")] = '\0'; // 개행 문자 제거
            printf("연도: ");
            scanf("%d", &year);
            getchar(); // 개행 문자 제거
            printf("주소: ");
            fgets(url, sizeof(url), stdin);
            url[strcspn(url, "\n")] = '\0'; // 개행 문자 제거

            addPaper(title, author, year, url);
            break;
        case 2:
            printf("논문 제목: ");
            fgets(title, sizeof(title), stdin);
            title[strcspn(title, "\n")] = '\0'; // 개행 문자 제거
            paper = searchPaper(title);
            if (paper) {
                addContentToPaper(paper);
            }
            else {
                printf("논문을 찾을 수 없습니다: %s\n", title);
            }
            break;
        case 3:
            printf("논문 제목: ");
            fgets(title, sizeof(title), stdin);
            title[strcspn(title, "\n")] = '\0'; // 개행 문자 제거
            paper = searchPaper(title);
            if (paper) {
                editPaper(paper);
            }
            else {
                printf("논문을 찾을 수 없습니다: %s\n", title);
            }
            break;
        case 4:
            printf("논문 제목: ");
            fgets(title, sizeof(title), stdin);
            title[strcspn(title, "\n")] = '\0'; // 개행 문자 제거
            paper = searchPaper(title);
            if (paper) {
                sortPaperContents(paper);
                printf("논문 내용이 페이지 순서대로 정렬되었습니다.\n");
            }
            else {
                printf("논문을 찾을 수 없습니다: %s\n", title);
            }
            break;
        case 5:
            printf("논문 제목: ");
            fgets(title, sizeof(title), stdin);
            title[strcspn(title, "\n")] = '\0'; // 개행 문자 제거
            paper = searchPaper(title);
            if (paper) {
                printf("제목: %s\n저자: %s\n연도: %d\n주소: %s\n", paper->title, paper->author, paper->year, paper->url);
                ContentNode* contentNode = paper->contentsHead;
                while (contentNode != NULL) {
                    printf(" 페이지 %d: %s\n", contentNode->paperContent.page, contentNode->paperContent.contents);
                    contentNode = contentNode->next;
                }
            }
            else {
                printf("논문을 찾을 수 없습니다: %s\n", title);
            }
            break;
        case 6:
            printf("불러올 파일 경로: ");
            char filepath[200];
            fgets(filepath, sizeof(filepath), stdin);
            filepath[strcspn(filepath, "\n")] = '\0'; // 개행 문자 제거
            readPaperFromFile(filepath);
            break;
        case 7:
            printf("논문 제목: ");
            fgets(title, sizeof(title), stdin);
            title[strcspn(title, "\n")] = '\0'; // 개행 문자 제거
            readPaperByTitle(title);
            break;
        case 8:
            printPapers();
            break;
        case 9:
            return 0;
        default:
            printf("잘못된 선택입니다. 다시 시도하세요.\n");
        }
    }

    return 0;
}

//text파일을 불러와서 수정할수 있음 + 논문내용 오름차순 정리

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

// 특정 논문을 수정하는 함수
void editPaper(ResearchPaper* paper) {
    char input[100];
    int year;

    printf("제목(%s): ", paper->title);
    fgets(input, sizeof(input), stdin);
    if (strcmp(input, "\n") != 0) {
        input[strcspn(input, "\n")] = '\0'; // Remove newline character
        free(paper->title);
        paper->title = my_strdup(input);
    }

    printf("저자(%s): ", paper->author);
    fgets(input, sizeof(input), stdin);
    if (strcmp(input, "\n") != 0) {
        input[strcspn(input, "\n")] = '\0'; // Remove newline character
        free(paper->author);
        paper->author = my_strdup(input);
    }

    printf("연도(%d): ", paper->year);
    fgets(input, sizeof(input), stdin);
    if (strcmp(input, "\n") != 0) {
        sscanf(input, "%d", &year);
        paper->year = year;
    }

    printf("주소(%s): ", paper->url);
    fgets(input, sizeof(input), stdin);
    if (strcmp(input, "\n") != 0) {
        input[strcspn(input, "\n")] = '\0'; // Remove newline character
        free(paper->url);
        paper->url = my_strdup(input);
    }
}

// 특정 논문 내용을 추가하는 함수
void addContentToPaper(ResearchPaper* paper) {
    int page;
    char contents[500];

    while (1) {
        printf("추가할 페이지 번호(종료: -1): ");
        scanf("%d", &page);
        getchar(); // consume the newline character left by scanf
        if (page == -1) {
            break;
        }
        printf("추가할 내용: ");
        fgets(contents, sizeof(contents), stdin);
        contents[strcspn(contents, "\n")] = '\0'; // Remove newline character

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

// 특정 논문을 선택하여 수정 및 내용을 추가할 수 있도록 하는 함수
void selectAndEditPaper() {
    char title[100];
    printf("수정할 논문의 제목을 입력하세요: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0'; // Remove newline character

    ResearchPaper* paperTemp = head;
    while (paperTemp != NULL) {
        if (strcmp(paperTemp->title, title) == 0) {
            editPaper(paperTemp);

            char choice;
            printf("내용을 추가하시겠습니까? (y/n): ");
            scanf(" %c", &choice);
            getchar(); // consume the newline character left by scanf
            if (choice == 'y' || choice == 'Y') {
                addContentToPaper(paperTemp);
                sortPaperContents(paperTemp); // 내용 추가 후 정렬
            }
            return;
        }
        paperTemp = paperTemp->next;
    }

    printf("제목에 해당하는 논문을 찾을 수 없습니다.\n");
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
    const char* filePath1 = "C:\\Users\\kimmi\\Desktop\\논문정리\\김민재.txt"; // 첫 번째 텍스트 파일 경로

    // 각 텍스트 파일에서 논문 정보를 읽어와서 연결리스트에 저장
    readPaperFromFile(filePath1);

    // 논문 리스트 출력
    printf("\n논문 리스트:\n");
    printPapers();

    // 논문 수정 및 내용 추가 기능
    printf("\n논문 수정 및 내용 추가:\n");
    selectAndEditPaper();

    // 수정 후 논문 리스트 출력
    printf("\n수정된 논문 리스트:\n");
    printPapers();

    return 0;
}
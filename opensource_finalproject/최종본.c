#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <windows.h>
#include <shellapi.h>

#pragma warning(disable : 4996)

#define DIRECTORY 1
#define FILE_TYPE 0

// ResearchPaper_contents 구조체 정의
typedef struct 
{
    int page;
    char* keywords; // 키워드 추가
} ResearchPaper_contents;

// ResearchPaper 구조체 정의
typedef struct ResearchPaper 
{
    char* title;
    char* author;
    int year;
    char* url;
    struct ContentNode* contentsHead; // 논문 내용의 연결리스트의 시작 노드
    struct ResearchPaper* next;
} ResearchPaper;

// 논문 내용의 노드 구조체 정의
typedef struct ContentNode 
{
    ResearchPaper_contents paperContent;
    struct ContentNode* next;
} ContentNode;

// 문자열을 복사하는 함수
char* my_strdup(const char* str) 
{
    char* copy = (char*)malloc(strlen(str) + 1);
    if (copy) 
    {
        strcpy(copy, str);
    }
    return copy;
}

void openLink(const char* url) 
{
    ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

// ANSI escape codes를 사용하여 콘솔 텍스트 색상을 설정하는 함수
void setConsoleTextColor(const char* colorCode) {
    printf("%s", colorCode);
}

// 논문 연결리스트의 시작 노드
ResearchPaper* head = NULL;



// 새로운 논문 노드를 생성하고 연결리스트에 추가하는 함수
void addPaper(char* title, char* author, int year, char* url) 
{
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
    if (head == NULL) 
    {
        head = newPaper;
    }
    else {
        // 리스트의 끝에 새로운 논문 노드 추가
        ResearchPaper* temp = head;
        while (temp->next != NULL) 
        {
            temp = temp->next;
        }
        temp->next = newPaper;
    }
}

// 논문 내용 노드를 추가하는 함수
void addPaperContent(ResearchPaper* paper, int page, char* keywords) 
{
    // 새로운 논문 내용 노드 생성
    ContentNode* newNode = (ContentNode*)malloc(sizeof(ContentNode));

    // ResearchPaper_contents 값 설정
    newNode->paperContent.page = page;
    newNode->paperContent.keywords = my_strdup(keywords); // 키워드 추가
    newNode->next = NULL;

    // 내용 리스트가 비어있으면 새로운 내용을 시작 노드로 설정
    if (paper->contentsHead == NULL) 
    {
        paper->contentsHead = newNode;
    }
    else {
        // 내용 리스트의 끝에 새로운 내용 노드 추가
        ContentNode* temp = paper->contentsHead;
        while (temp->next != NULL) 
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// 파일에서 논문 정보를 읽어와 연결리스트에 추가하는 함수
void readPaperFromFile(const char* filepath) 
{
    FILE* file = fopen(filepath, "r");
    if (file == NULL) 
    {
        setConsoleTextColor("\033[31m"); // 빨간색
        printf("파일을 열 수 없습니다: %s\n", filepath);
        setConsoleTextColor("\033[0m"); // 기본 색상(흰색)
        return;
    }

    char title[100];
    char author[100];
    int year;
    char url[300];
    int page;
    char keywords[200]; // 키워드 추가

    fscanf(file, "제목: %99[^\n]\n", title);
    fscanf(file, "저자: %99[^\n]\n", author);
    fscanf(file, "연도: %d\n", &year);
    fscanf(file, "주소: %99[^\n]\n", url);

    addPaper(title, author, year, url);
    ResearchPaper* currentPaper = head;
    while (currentPaper->next != NULL) 
    {
        currentPaper = currentPaper->next;
    }

    while (fscanf(file, " 페이지: %d\n 키워드: %199[^\n]\n", &page, keywords) == 2) 
    {
        addPaperContent(currentPaper, page, keywords);
    }

    fclose(file);
}

// 논문 데이터를 파일에서 삭제하는 함수
void deletePaperFile(const char* title, const char* folderPath) 
{
    char filePath[200];
    snprintf(filePath, sizeof(filePath), "%s\\%s.txt", folderPath, title);

    // 파일이 존재하는지 확인 후 삭제
    if (_access(filePath, 0) == 0) 
    {
        if (remove(filePath) != 0) 
        {
            setConsoleTextColor("\033[31m"); // 빨간색
            printf("파일을 삭제할 수 없습니다: %s\n", filePath);
            setConsoleTextColor("\033[0m"); // 기본 색상(흰색)
        }
        else 
        {
            setConsoleTextColor("\033[31m"); // 빨간색
            printf("파일 삭제 완료: %s\n", filePath);
            setConsoleTextColor("\033[0m"); // 기본 색상(흰색)
        }
    }
    else 
    {
        setConsoleTextColor("\033[31m"); // 빨간색
        printf("파일이 존재하지 않습니다: %s\n", filePath);
        setConsoleTextColor("\033[0m"); // 기본 색상(흰색)
    }
}

void editPaper(ResearchPaper* paper) {
    char input[100];
    int year;
    char temp[300]; // 임시 배열

    printf("제목(%s): ", paper->title);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove newline character
    if (strcmp(input, "") != 0) {
        // 제목을 수정하면 기존 파일 삭제 후 새로운 파일 생성
        deletePaperFile(paper->title, "C:\\Users\\kimmi\\Desktop\\논문정리\\");
        free(paper->title);
        paper->title = my_strdup(input);
    }

    printf("저자(%s): ", paper->author);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove newline character
    if (strcmp(input, "") != 0) {
        free(paper->author);
        paper->author = my_strdup(input);
    }

    printf("연도(%d): ", paper->year);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0'; // Remove newline character
    if (strcmp(temp, "") != 0) {
        sscanf(temp, "%d", &year);
        paper->year = year;
    }

    printf("주소(%s): ", paper->url);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove newline character
    if (strcmp(input, "") != 0) {
        free(paper->url);
        paper->url = my_strdup(input);
    }

    // 현재 페이지 및 키워드 목록 출력
    printf("현재 저장된 페이지 및 키워드:\n");
    ContentNode* contentTemp = paper->contentsHead;
    while (contentTemp != NULL) {
        printf("페이지 %d: %s\n", contentTemp->paperContent.page, contentTemp->paperContent.keywords);
        contentTemp = contentTemp->next;
    }

    // 페이지 및 키워드 수정 또는 추가
    while (1) {
        setConsoleTextColor("\033[31m"); // 빨간색
        printf("기존 내용을 수정하시겠습니까? (y/n): ");
        setConsoleTextColor("\033[0m"); // 기본 색상(흰색)
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // Remove newline character
        if (strcmp(input, "y") != 0 && strcmp(input, "Y") != 0) {
            break;
        }

        printf("수정할 페이지 번호(종료: -1): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // Remove newline character
        if (strcmp(input, "-1") == 0) {
            break;
        }
        int editPage = atoi(input);

        // 입력받은 페이지 번호에 해당하는 노드 찾기
        contentTemp = paper->contentsHead;
        ContentNode* prevNode = NULL;
        while (contentTemp != NULL) {
            if (contentTemp->paperContent.page == editPage) {
                // 해당 페이지의 키워드 수정
                printf("페이지 %d의 키워드(%s): ", contentTemp->paperContent.page, contentTemp->paperContent.keywords);
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = '\0'; // Remove newline character
                if (strcmp(input, "") != 0) {
                    free(contentTemp->paperContent.keywords);
                    contentTemp->paperContent.keywords = my_strdup(input);
                }
                break;
            }
            prevNode = contentTemp;
            contentTemp = contentTemp->next;
        }

        // 입력받은 페이지 번호에 해당하는 노드가 없는 경우, 새로운 페이지 및 키워드 추가
        if (contentTemp == NULL) {
            setConsoleTextColor("\033[31m"); // 빨간색
            printf("페이지 %d를 찾을 수 없습니다. 새로운 페이지 및 키워드를 추가합니다.\n", editPage);
            setConsoleTextColor("\033[0m"); // 기본 색상(흰색)
            printf("추가할 키워드: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0'; // Remove newline character

            addPaperContent(paper, editPage, input);
        }
    }
}


// 특정 논문 내용을 추가하는 함수
void addContentToPaper(ResearchPaper* paper) 
{
    int page;
    char keywords[200]; // 키워드 추가

    while (1) {
        printf("추가할 페이지 번호(-1 입력시 종료): ");
        scanf("%d", &page);
        getchar(); // 입력 버퍼에서 '\n' 제거

        if (page == -1) 
        {
            break;
        }

        printf("페이지 %d의 키워드: ", page);
        fgets(keywords, sizeof(keywords), stdin);
        keywords[strcspn(keywords, "\n")] = '\0'; // Remove newline character

        addPaperContent(paper, page, keywords);
        setConsoleTextColor("\033[31m"); // 빨간색
        printf("페이지 %d에 키워드 '%s' 추가 완료.\n", page, keywords);
        setConsoleTextColor("\033[0m"); // 기본 색상(흰색)
    }
}

// 논문 내용의 페이지 순서대로 정렬하는 함수
void sortPaperContents(ResearchPaper* paper) 
{
    if (paper->contentsHead == NULL) 
    {
        return;
    }

    ContentNode* sorted = NULL;

    ContentNode* current = paper->contentsHead;
    while (current != NULL) 
    {
        ContentNode* next = current->next;
        if (sorted == NULL || sorted->paperContent.page > current->paperContent.page) {
            current->next = sorted;
            sorted = current;
        }
        else 
        {
            ContentNode* temp = sorted;
            while (temp->next != NULL && temp->next->paperContent.page <= current->paperContent.page) 
            {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    paper->contentsHead = sorted;
}

void removeContentFromPaper(ResearchPaper* paper, int page) {
    ContentNode* current = paper->contentsHead;
    ContentNode* prev = NULL;

    while (current != NULL) {
        if (current->paperContent.page == page) {
            if (prev == NULL) {
                // 첫 번째 노드를 삭제
                paper->contentsHead = current->next;
            }
            else {
                prev->next = current->next;
            }
            free(current->paperContent.keywords);
            free(current);
            setConsoleTextColor("\033[31m"); // 빨간색
            printf("페이지 %d가 삭제되었습니다.\n", page);
            setConsoleTextColor("\033[0m"); // 기본 색상(흰색)
            return;
        }
        prev = current;
        current = current->next;
    }
    setConsoleTextColor("\033[31m"); // 빨간색
    printf("페이지 %d를 찾을 수 없습니다.\n", page);
    setConsoleTextColor("\033[0m"); // 기본 색상(흰색)
}



void selectAndEditPaper() {
    char title[100];
    printf("수정할 논문의 제목을 입력하세요: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0'; // Remove newline character

    ResearchPaper* paperTemp = head;
    while (paperTemp != NULL) {
        if (strcmp(paperTemp->title, title) == 0) {
            // 페이지 삭제 여부 물어보기
            setConsoleTextColor("\033[31m"); // 빨간색
            printf("삭제할 페이지가 있습니까? (y/n): ");
            setConsoleTextColor("\033[0m"); // 기본 색상(흰색)
            char deleteChoice;
            fgets(&deleteChoice, sizeof(deleteChoice), stdin);
            deleteChoice = deleteChoice != '\0' ? deleteChoice : 'n';

            if (deleteChoice == 'y' || deleteChoice == 'Y') {
                while (1) {
                    printf("삭제할 페이지 번호(종료: -1): ");
                    fgets(title, sizeof(title), stdin);
                    int pageToDelete = atoi(title);
                    if (pageToDelete == -1) {
                        break;
                    }
                    removeContentFromPaper(paperTemp, pageToDelete);
                }
            }

            // 논문 수정
            editPaper(paperTemp);


            // 추가할 내용 입력
            setConsoleTextColor("\033[31m"); // 빨간색
            printf("추가할 내용을 입력하시겠습니까? (y/n): ");
            setConsoleTextColor("\033[0m"); // 기본 색상(흰색)
            char addMore;
            fgets(&addMore, sizeof(addMore), stdin);
            addMore = addMore != '\0' ? addMore : 'n';

            if (addMore == 'y' || addMore == 'Y') {
                addContentToPaper(paperTemp);
                sortPaperContents(paperTemp); // 추가된 내용 정렬
            }
            return;
        }
        paperTemp = paperTemp->next;
    }
    setConsoleTextColor("\033[31m"); // 빨간색
    printf("제목에 해당하는 논문을 찾을 수 없습니다.\n");
    setConsoleTextColor("\033[0m"); // 기본 색상(흰색)
}

// 특정 논문을 제목으로 찾아 출력하는 함수
void printPaperByTitle(const char* title) 
{
    ResearchPaper* paperTemp = head;
    while (paperTemp != NULL) 
    {
        if (strcmp(paperTemp->title, title) == 0) 
        {
            printf("제목: %s\n", paperTemp->title);
            printf("저자: %s\n", paperTemp->author);
            printf("연도: %d\n", paperTemp->year);
            printf("주소: %s\n", paperTemp->url);
            openLink(paperTemp->url);
            ContentNode* contentTemp = paperTemp->contentsHead;
            while (contentTemp != NULL) 
            {
                printf("페이지: %d\n", contentTemp->paperContent.page);
                printf("키워드: %s\n", contentTemp->paperContent.keywords); // 키워드 출력 추가
                contentTemp = contentTemp->next;
            }
            return;
        }
        paperTemp = paperTemp->next;
    }
    setConsoleTextColor("\033[31m"); // 빨간색
    printf("제목에 해당하는 논문을 찾을 수 없습니다.\n");
    setConsoleTextColor("\033[0m"); // 기본 색상(흰색)
}

void printAllPapers() 
{
    if (head == NULL) 
    {
        setConsoleTextColor("\033[31m"); // 빨간색
        printf("저장된 논문이 없습니다.\n");
        setConsoleTextColor("\033[0m"); // 기본 색상(흰색)
        return;
    }

    ResearchPaper* paperTemp = head;
    while (paperTemp != NULL) 
    {
        printf("제목: %s\n", paperTemp->title);
        printf("저자: %s\n", paperTemp->author);
        printf("연도: %d\n", paperTemp->year);
        printf("주소: %s\n", paperTemp->url);

        ContentNode* contentTemp = paperTemp->contentsHead;
        while (contentTemp != NULL) 
        {
            printf("페이지: %d\n", contentTemp->paperContent.page);
            printf("키워드: %s\n", contentTemp->paperContent.keywords); // 키워드 출력 추가
            contentTemp = contentTemp->next;
        }
        printf("\n");
        paperTemp = paperTemp->next;
    }
}

// 논문 데이터를 파일에 쓰는 함수
void writePaperToFile(const ResearchPaper* paper, const char* folderPath) 
{
    char filePath[200];
    snprintf(filePath, sizeof(filePath), "%s\\%s.txt", folderPath, paper->title);

    FILE* file = fopen(filePath, "w");
    if (file == NULL) 
    {
        setConsoleTextColor("\033[31m"); // 빨간색
        printf("파일을 열 수 없습니다: %s\n", filePath);
        setConsoleTextColor("\033[0m"); // 기본 색상(흰색)
        return;
    }

    fprintf(file, "제목: %s\n", paper->title);
    fprintf(file, "저자: %s\n", paper->author);
    fprintf(file, "연도: %d\n", paper->year);
    fprintf(file, "주소: %s\n", paper->url);

    ContentNode* contentTemp = paper->contentsHead;
    while (contentTemp != NULL) 
    {
        fprintf(file, " 페이지: %d\n 키워드: %s\n", contentTemp->paperContent.page, contentTemp->paperContent.keywords);
        contentTemp = contentTemp->next;
    }

    fclose(file);
}

// 모든 논문 데이터를 파일에 쓰는 함수
void writePapersToFolder(const char* folderPath) 
{
    ResearchPaper* paperTemp = head;
    while (paperTemp != NULL) 
    {
        writePaperToFile(paperTemp, folderPath);
        paperTemp = paperTemp->next;
    }
}

// 폴더에서 모든 텍스트 파일을 읽어오는 함수
void readPapersFromFolder(const char* folderPath) 
{
    char searchPath[300];
    snprintf(searchPath, sizeof(searchPath), "%s\\*.txt", folderPath);
    struct _finddata_t file;
    intptr_t handle;

    handle = _findfirst(searchPath, &file);
    if (handle == -1) 
    {
        return;
    }

    do 
    {
        char filePath[300];
        snprintf(filePath, sizeof(filePath), "%s\\%s", folderPath, file.name);
        readPaperFromFile(filePath);
    } while (_findnext(handle, &file) == 0);

    _findclose(handle);
}

// 메뉴를 표시하고 사용자의 선택을 처리하는 함수
void displayMenu() 
{
    while (1) 
    {
        int choice;
        printf("===== 논문 관리 시스템 =====\n");
        printf("1. 논문 추가\n");
        printf("2. 논문 수정\n");
        printf("3. 특정 제목의 논문 출력\n");
        printf("4. 모든 논문 출력\n");
        printf("5. 종료\n");
        printf("==========================\n");
        printf("선택: ");
        scanf("%d", &choice);
        getchar(); // consume the newline character left by scanf
        switch (choice) 
        {
        case 1: 
        {
            char title[100];
            char author[100];
            int year;
            char url[300];
            int page;
            char keywords[200];

            printf("제목: ");
            fgets(title, sizeof(title), stdin);
            title[strcspn(title, "\n")] = '\0'; // Remove newline character

            printf("저자: ");
            fgets(author, sizeof(author), stdin);
            author[strcspn(author, "\n")] = '\0'; // Remove newline character

            printf("연도: ");
            scanf("%d", &year);
            getchar(); // consume the newline character left by scanf

            printf("주소: ");
            fgets(url, sizeof(url), stdin);
            url[strcspn(url, "\n")] = '\0'; // Remove newline character

            addPaper(title, author, year, url);

            ResearchPaper* currentPaper = head;
            while (currentPaper->next != NULL) 
            {
                currentPaper = currentPaper->next;
            }

            while (1) 
            {
                printf("추가할 페이지 번호(종료: -1): ");
                scanf("%d", &page);
                getchar(); // consume the newline character left by scanf
                if (page == -1) 
                {
                    break;
                }

                printf("추가할 키워드: ");
                fgets(keywords, sizeof(keywords), stdin);
                keywords[strcspn(keywords, "\n")] = '\0'; // Remove newline character

                addPaperContent(currentPaper, page, keywords);
            }
            sortPaperContents(currentPaper);
            break;
        }
        case 2:
            selectAndEditPaper();
            break;
        case 3: 
        {
            char title[100];
            printf("출력할 논문의 제목을 입력하세요: ");
            fgets(title, sizeof(title), stdin);
            title[strcspn(title, "\n")] = '\0'; // Remove newline character

            printPaperByTitle(title);
            break;
        }
        case 4:
            printAllPapers();
            break;
        case 5:
            return;
        default:
            setConsoleTextColor("\033[31m"); // 빨간색
            printf("올바른 번호를 선택하세요.\n");
            setConsoleTextColor("\033[0m"); // 기본 색상(흰색)
        }
    }
}

int main() 
{
    const char* folderPath = "C:\\Users\\kimmi\\Desktop\\논문정리\\";
    // 프로그램 시작 시 모든 텍스트 파일 읽어오기
    readPapersFromFolder(folderPath);

    /*clearConsole();*/

    // 사용자 메뉴 표시
    displayMenu();

    // 프로그램 종료 시 모든 논문 데이터를 파일로 저장
    writePapersToFolder(folderPath);

    return 0;
}
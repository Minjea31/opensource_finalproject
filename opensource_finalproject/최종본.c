#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <windows.h>
#include <shellapi.h>

#pragma warning(disable : 4996)

#define DIRECTORY 1
#define FILE_TYPE 0

// ResearchPaper_contents ����ü ����
typedef struct 
{
    int page;
    char* keywords; // Ű����
} ResearchPaper_contents;

// ResearchPaper ����ü ����
typedef struct ResearchPaper 
{
    char* title; //����
    char* author; //����
    int year; //����
    char* url; //�ּ�
    // �� ������ ���Ḯ��Ʈ�� ���� ���
    struct ContentNode* contentsHead;
    struct ResearchPaper* next;
} ResearchPaper;

// �� ������ ��� ����ü ����
typedef struct ContentNode 
{
    ResearchPaper_contents paperContent;
    struct ContentNode* next;
} ContentNode;

void clearConsole() 
{
    system("cls");
}

// ���ڿ��� �����ϴ� �Լ�
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

// ANSI escape codes�� ����Ͽ� �ܼ� �ؽ�Ʈ ������ �����ϴ� �Լ�
void setConsoleTextColor(const char* colorCode) 
{
    printf("%s", colorCode);
}

// �� ���Ḯ��Ʈ�� ���� ���
ResearchPaper* head = NULL;

// ���ο� �� ��带 �����ϰ� ���Ḯ��Ʈ�� �߰��ϴ� �Լ�
void addPaper(char* title, char* author, int year, char* url) 
{
    // ���ο� �� ��� ����
    ResearchPaper* newPaper = (ResearchPaper*)malloc(sizeof(ResearchPaper));

    newPaper->title = my_strdup(title);
    newPaper->author = my_strdup(author);
    newPaper->year = year;
    newPaper->url = my_strdup(url);
    newPaper->contentsHead = NULL; // ������ ���� ���� NULL�� �ʱ�ȭ
    newPaper->next = NULL;

    // ����Ʈ�� ��������� ���ο� �� ��带 ���� ���� ����
    if (head == NULL) 
    {
        head = newPaper;
    }
    else 
    {
        // ����Ʈ�� ���� ���ο� �� ��� �߰�
        ResearchPaper* temp = head;
        while (temp->next != NULL) 
        {
            temp = temp->next;
        }
        temp->next = newPaper;
    }
}

// �� ���� ��带 �߰��ϴ� �Լ�
void addPaperContent(ResearchPaper* paper, int page, char* keywords) 
{
    // ���ο� �� ���� ��� ����
    ContentNode* newNode = (ContentNode*)malloc(sizeof(ContentNode));

    newNode->paperContent.page = page;
    newNode->paperContent.keywords = my_strdup(keywords);
    newNode->next = NULL;

    // ���� ����Ʈ�� ��������� ���ο� ������ ���� ���� ����
    if (paper->contentsHead == NULL) 
    {
        paper->contentsHead = newNode;
    }
    else 
    {
        // ���� ����Ʈ�� ���� ���ο� ���� ��� �߰�
        ContentNode* temp = paper->contentsHead;
        while (temp->next != NULL) 
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// ���Ͽ��� �� ������ �о�� ���Ḯ��Ʈ�� �߰��ϴ� �Լ�
void readPaperFromFile(const char* filepath) 
{
    FILE* file = fopen(filepath, "r");
    if (file == NULL) 
    {
        clearConsole();
        setConsoleTextColor("\033[31m"); 
        printf("������ �� �� �����ϴ�: %s\n", filepath);
        setConsoleTextColor("\033[0m"); 
        return;
    }

    char title[100];
    char author[100];
    int year;
    char url[300];
    int page;
    char keywords[200];

    fscanf(file, "����: %99[^\n]\n", title);
    fscanf(file, "����: %99[^\n]\n", author);
    fscanf(file, "����: %d\n", &year);
    fscanf(file, "�ּ�: %99[^\n]\n", url);

    addPaper(title, author, year, url);
    ResearchPaper* currentPaper = head;
    while (currentPaper->next != NULL) 
    {
        currentPaper = currentPaper->next;
    }

    while (fscanf(file, " ������: %d\n Ű����: %199[^\n]\n", &page, keywords) == 2) 
    {
        addPaperContent(currentPaper, page, keywords);
    }

    fclose(file);
}

// �� �����͸� ���Ͽ��� �����ϴ� �Լ�
void deletePaperFile(const char* title, const char* folderPath) 
{
    char filePath[200];
    snprintf(filePath, sizeof(filePath), "%s\\%s.txt", folderPath, title);

    // ������ �����ϴ��� Ȯ�� �� ����
    if (_access(filePath, 0) == 0) 
    {
        if (remove(filePath) != 0) 
        {
            clearConsole();
            setConsoleTextColor("\033[31m"); 
            printf("������ ������ �� �����ϴ�: %s\n", filePath);
            setConsoleTextColor("\033[0m"); 
        }
        else 
        {
            clearConsole();
            setConsoleTextColor("\033[31m"); 
            printf("���� ���� �Ϸ�: %s\n", filePath);
            setConsoleTextColor("\033[0m"); 
        }
    }
    else 
    {
        clearConsole();
        setConsoleTextColor("\033[31m"); 
        printf("������ �������� �ʽ��ϴ�: %s\n", filePath);
        setConsoleTextColor("\033[0m"); 
    }
}

void editPaper(ResearchPaper* paper) 
{
    char input[300];
    int year;
    char temp[300]; 

    printf("����(%s): ", paper->title);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; 
    if (strcmp(input, "") != 0) 
    {
        // ������ �����ϸ� ���� ���� ���� �� ���ο� ���� ����
        deletePaperFile(paper->title, "C:\\Users\\kimmi\\Desktop\\������\\");
        free(paper->title);
        paper->title = my_strdup(input);
    }

    printf("����(%s): ", paper->author);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; 
    if (strcmp(input, "") != 0) 
    {
        free(paper->author);
        paper->author = my_strdup(input);
    }

    printf("����(%d): ", paper->year);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0'; 
    if (strcmp(temp, "") != 0) 
    {
        sscanf(temp, "%d", &year);
        paper->year = year;
    }

    printf("�ּ�(%s): ", paper->url);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; 
    if (strcmp(input, "") != 0) 
    {
        free(paper->url);
        paper->url = my_strdup(input);
    }

    // ���� ������ �� Ű���� ��� ���
    printf("���� ����� ������ �� Ű����:\n");
    ContentNode* contentTemp = paper->contentsHead;
    while (contentTemp != NULL) 
    {
        printf("������ %d: %s\n", contentTemp->paperContent.page, contentTemp->paperContent.keywords);
        contentTemp = contentTemp->next;
    }

    // ������ �� Ű���� ����
    while (1) 
    {
        setConsoleTextColor("\033[31m"); 
        printf("����� ������ �� Ű���带 �����Ͻðڽ��ϱ�? (y/n): ");
        setConsoleTextColor("\033[0m"); 
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; 
        if (strcmp(input, "y") != 0 && strcmp(input, "Y") != 0) 
        {
            break;
        }

        printf("������ ������ ��ȣ(����: -1): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; 
        if (strcmp(input, "-1") == 0) 
        {
            break;
        }
        int deletePage = atoi(input);

        // �Է¹��� ������ ��ȣ�� �ش��ϴ� ��� ã�� �� ����
        contentTemp = paper->contentsHead;
        ContentNode* prevNode = NULL;
        while (contentTemp != NULL) 
        {
            if (contentTemp->paperContent.page == deletePage) 
            {
                if (prevNode == NULL) 
                {
                    paper->contentsHead = contentTemp->next;
                }
                else 
                {
                    prevNode->next = contentTemp->next;
                }
                free(contentTemp->paperContent.keywords);
                free(contentTemp);
                printf("������ %d�� �����Ǿ����ϴ�.\n", deletePage);
                break;
            }
            prevNode = contentTemp;
            contentTemp = contentTemp->next;
        }

        if (contentTemp == NULL) 
        {
            printf("������ %d�� ã�� �� �����ϴ�.\n", deletePage);
        }
    }

    // ������ �� Ű���� ���� �Ǵ� �߰�
    while (1) 
    {
        setConsoleTextColor("\033[31m"); 
        printf("���� ������ �����Ͻðڽ��ϱ�? (y/n): ");
        setConsoleTextColor("\033[0m"); 
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; 
        if (strcmp(input, "y") != 0 && strcmp(input, "Y") != 0) 
        {
            break;
        }

        printf("������ ������ ��ȣ(����: -1): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; 
        if (strcmp(input, "-1") == 0) 
        {
            break;
        }
        int editPage = atoi(input);

        // �Է¹��� ������ ��ȣ�� �ش��ϴ� ��� ã��
        contentTemp = paper->contentsHead;
        ContentNode* prevNode = NULL;
        while (contentTemp != NULL) {
            if (contentTemp->paperContent.page == editPage) 
            {
                // �ش� �������� Ű���� ����
                printf("������ %d�� Ű����(%s): ", contentTemp->paperContent.page, contentTemp->paperContent.keywords);
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = '\0'; 
                if (strcmp(input, "") != 0) 
                {
                    free(contentTemp->paperContent.keywords);
                    contentTemp->paperContent.keywords = my_strdup(input);
                }
                break;
            }
            prevNode = contentTemp;
            contentTemp = contentTemp->next;
        }

        // �Է¹��� ������ ��ȣ�� �ش��ϴ� ��尡 ���� ���, ���ο� ������ �� Ű���� �߰�
        if (contentTemp == NULL) 
        {
            setConsoleTextColor("\033[31m"); 
            printf("������ %d�� ã�� �� �����ϴ�. ���ο� ������ �� Ű���带 �߰��մϴ�.\n", editPage);
            setConsoleTextColor("\033[0m"); 
            printf("�߰��� Ű����: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0'; 

            addPaperContent(paper, editPage, input);
        }
    }
}




// Ư�� �� ������ �߰��ϴ� �Լ�
void addContentToPaper(ResearchPaper* paper) 
{
    int page;
    char keywords[200];

    while (1) 
    {
        printf("�߰��� ������ ��ȣ(-1 �Է½� ����): ");
        scanf("%d", &page);
        getchar(); 

        if (page == -1) 
        {
            break;
        }

        printf("������ %d�� Ű����: ", page);
        fgets(keywords, sizeof(keywords), stdin);
        keywords[strcspn(keywords, "\n")] = '\0'; 

        addPaperContent(paper, page, keywords);
        setConsoleTextColor("\033[31m"); 
        printf("������ %d�� Ű���� '%s' �߰� �Ϸ�.\n", page, keywords);
        setConsoleTextColor("\033[0m"); 
    }
}

// �� ������ ������ ������� �����ϴ� �Լ�
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
        if (sorted == NULL || sorted->paperContent.page > current->paperContent.page) 
        {
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

//�� ���� ���� �Լ�
void removeContentFromPaper(ResearchPaper* paper, int page) 
{
    ContentNode* current = paper->contentsHead;
    ContentNode* prev = NULL;

    while (current != NULL) 
    {
        if (current->paperContent.page == page)
        {
            if (prev == NULL) 
            {
                // ù ��° ��带 ����
                paper->contentsHead = current->next;
            }
            else 
            {
                prev->next = current->next;
            }
            free(current->paperContent.keywords);
            free(current);
            setConsoleTextColor("\033[31m"); 
            printf("������ %d�� �����Ǿ����ϴ�.\n", page);
            setConsoleTextColor("\033[0m"); 
            return;
        }
        prev = current;
        current = current->next;
    }
    setConsoleTextColor("\033[31m"); 
    printf("������ %d�� ã�� �� �����ϴ�.\n", page);
    setConsoleTextColor("\033[0m"); 
}



void selectAndEditPaper() 
{
    char title[100];
    printf("������ ���� ������ �Է��ϼ���: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0'; 

    ResearchPaper* paperTemp = head;
    while (paperTemp != NULL) 
    {
        if (strcmp(paperTemp->title, title) == 0)
        {
            // �� ����
            editPaper(paperTemp);

            // �߰��� ���� �Է�
            setConsoleTextColor("\033[31m"); 
            printf("�߰��� ������ �Է��Ͻðڽ��ϱ�? (y/n): ");
            setConsoleTextColor("\033[0m"); 
            char addMore[3];
            fgets(addMore, sizeof(addMore), stdin);
            addMore[strcspn(addMore, "\n")] = '\0'; 

            if (strcmp(addMore, "y") == 0 || strcmp(addMore, "Y") == 0) 
            {
                addContentToPaper(paperTemp);
                sortPaperContents(paperTemp); 
            }
            clearConsole();
            return;
        }
        paperTemp = paperTemp->next;
    }
    clearConsole();
    setConsoleTextColor("\033[31m"); 
    printf("���� �ش��ϴ� ���� ã�� �� �����ϴ�.\n");
    setConsoleTextColor("\033[0m"); 
}

// Ư�� ���� �������� ã�� ����ϴ� �Լ�
void printPaperByTitle(const char* title) 
{
    ResearchPaper* paperTemp = head;
    while (paperTemp != NULL) 
    {
        if (strcmp(paperTemp->title, title) == 0) 
        {
            printf("����: %s\n", paperTemp->title);
            printf("����: %s\n", paperTemp->author);
            printf("����: %d\n", paperTemp->year);
            printf("�ּ�: %s\n", paperTemp->url);
            openLink(paperTemp->url);
            ContentNode* contentTemp = paperTemp->contentsHead;
            while (contentTemp != NULL) 
            {
                printf("������: %d\n", contentTemp->paperContent.page);
                printf("Ű����: %s\n", contentTemp->paperContent.keywords); 
                contentTemp = contentTemp->next;
            }
            return;
        }
        paperTemp = paperTemp->next;
    }
    clearConsole();
    setConsoleTextColor("\033[31m"); 
    printf("���� �ش��ϴ� ���� ã�� �� �����ϴ�.\n");
    setConsoleTextColor("\033[0m"); 
}


//��� �� ��� �Լ�
void printAllPapers() 
{
    if (head == NULL) 
    {
        clearConsole();
        setConsoleTextColor("\033[31m"); 
        printf("����� ���� �����ϴ�.\n");
        setConsoleTextColor("\033[0m"); 
        return;
    }

    ResearchPaper* paperTemp = head;
    while (paperTemp != NULL) 
    {
        printf("����: %s\n", paperTemp->title);
        printf("����: %s\n", paperTemp->author);
        printf("����: %d\n", paperTemp->year);
        printf("�ּ�: %s\n", paperTemp->url);

        ContentNode* contentTemp = paperTemp->contentsHead;
        while (contentTemp != NULL) 
        {
            printf("������: %d\n", contentTemp->paperContent.page);
            printf("Ű����: %s\n", contentTemp->paperContent.keywords); 
            contentTemp = contentTemp->next;
        }
        printf("\n");
        paperTemp = paperTemp->next;
    }
}

// �� �����͸� ���Ͽ� ���� �Լ�
void writePaperToFile(const ResearchPaper* paper, const char* folderPath) 
{
    char filePath[200];
    snprintf(filePath, sizeof(filePath), "%s\\%s.txt", folderPath, paper->title);

    FILE* file = fopen(filePath, "w");
    if (file == NULL) 
    {
        setConsoleTextColor("\033[31m"); 
        printf("������ �� �� �����ϴ�: %s\n", filePath);
        setConsoleTextColor("\033[0m"); 
        return;
    }

    fprintf(file, "����: %s\n", paper->title);
    fprintf(file, "����: %s\n", paper->author);
    fprintf(file, "����: %d\n", paper->year);
    fprintf(file, "�ּ�: %s\n", paper->url);

    ContentNode* contentTemp = paper->contentsHead;
    while (contentTemp != NULL) 
    {
        fprintf(file, " ������: %d\n Ű����: %s\n", contentTemp->paperContent.page, contentTemp->paperContent.keywords);
        contentTemp = contentTemp->next;
    }

    fclose(file);
}

// ��� �� �����͸� ���Ͽ� ���� �Լ�
void writePapersToFolder(const char* folderPath) 
{
    ResearchPaper* paperTemp = head;
    while (paperTemp != NULL) 
    {
        writePaperToFile(paperTemp, folderPath);
        paperTemp = paperTemp->next;
    }
}

// �������� ��� �ؽ�Ʈ ������ �о���� �Լ�
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

// �޴��� ǥ���ϰ� ������� ������ ó���ϴ� �Լ�
void displayMenu() 
{
    while (1) 
    {
        int choice;
        printf("===== �� ���� �ý��� =====\n");
        printf("1. �� �߰�\n");
        printf("2. �� ����\n");
        printf("3. Ư�� ������ �� ���\n");
        printf("4. ��� �� ���\n");
        printf("5. ����\n");
        printf("==========================\n");
        printf("����: ");
        scanf("%d", &choice);
        getchar(); 
        switch (choice) 
        {
        case 1: 
        {
            clearConsole();

            char title[100];
            char author[100];
            int year;
            char url[300];
            int page;
            char keywords[200];

            printf("����: ");
            fgets(title, sizeof(title), stdin);
            title[strcspn(title, "\n")] = '\0'; 

            printf("����: ");
            fgets(author, sizeof(author), stdin);
            author[strcspn(author, "\n")] = '\0'; 

            printf("����: ");
            scanf("%d", &year);
            getchar(); 

            printf("�ּ�: ");
            fgets(url, sizeof(url), stdin);
            url[strcspn(url, "\n")] = '\0'; 

            addPaper(title, author, year, url);

            ResearchPaper* currentPaper = head;
            while (currentPaper->next != NULL) 
            {
                currentPaper = currentPaper->next;
            }

            while (1) 
            {
                printf("�߰��� ������ ��ȣ(����: -1): ");
                scanf("%d", &page);
                getchar(); 
                if (page == -1) 
                {
                    clearConsole();
                    break;
                }

                printf("�߰��� Ű����: ");
                fgets(keywords, sizeof(keywords), stdin);
                keywords[strcspn(keywords, "\n")] = '\0';

                addPaperContent(currentPaper, page, keywords);
            }
            sortPaperContents(currentPaper);
            break;
        }
        case 2:
        {
            clearConsole();

            selectAndEditPaper();
            break;
        }
        case 3: 
        {
            clearConsole();
            char title[100];
            printf("����� ���� ������ �Է��ϼ���: ");
            fgets(title, sizeof(title), stdin);
            title[strcspn(title, "\n")] = '\0'; 

            printPaperByTitle(title);
            break;
        }
        case 4:
        {
            clearConsole();
            printAllPapers();
            break;
        }
        case 5:
        {
            clearConsole();
            setConsoleTextColor("\033[31m"); 
            printf("�����߽��ϴ�.");
            setConsoleTextColor("\033[0m"); 
            return;
        }
        default:
        {
            setConsoleTextColor("\033[31m");
            printf("�ùٸ� ��ȣ�� �����ϼ���.\n");
            setConsoleTextColor("\033[0m"); 
        }
        }
    }
}

int main() 
{
    const char* folderPath = "C:\\Users\\kimmi\\Desktop\\������\\";
    // ���α׷� ���� �� ��� �ؽ�Ʈ ���� �о����
    readPapersFromFolder(folderPath);

    // ����� �޴� ǥ��
    displayMenu();

    // ���α׷� ���� �� ��� �� �����͸� ���Ϸ� ����
    writePapersToFolder(folderPath);

    return 0;
}
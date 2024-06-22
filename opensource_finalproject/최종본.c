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
    char* keywords; // Ű���� �߰�
} ResearchPaper_contents;

// ResearchPaper ����ü ����
typedef struct ResearchPaper 
{
    char* title;
    char* author;
    int year;
    char* url;
    struct ContentNode* contentsHead; // �� ������ ���Ḯ��Ʈ�� ���� ���
    struct ResearchPaper* next;
} ResearchPaper;

// �� ������ ��� ����ü ����
typedef struct ContentNode 
{
    ResearchPaper_contents paperContent;
    struct ContentNode* next;
} ContentNode;

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
void setConsoleTextColor(const char* colorCode) {
    printf("%s", colorCode);
}

// �� ���Ḯ��Ʈ�� ���� ���
ResearchPaper* head = NULL;



// ���ο� �� ��带 �����ϰ� ���Ḯ��Ʈ�� �߰��ϴ� �Լ�
void addPaper(char* title, char* author, int year, char* url) 
{
    // ���ο� �� ��� ����
    ResearchPaper* newPaper = (ResearchPaper*)malloc(sizeof(ResearchPaper));

    // ResearchPaper �� ����
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
    else {
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

    // ResearchPaper_contents �� ����
    newNode->paperContent.page = page;
    newNode->paperContent.keywords = my_strdup(keywords); // Ű���� �߰�
    newNode->next = NULL;

    // ���� ����Ʈ�� ��������� ���ο� ������ ���� ���� ����
    if (paper->contentsHead == NULL) 
    {
        paper->contentsHead = newNode;
    }
    else {
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
        setConsoleTextColor("\033[31m"); // ������
        printf("������ �� �� �����ϴ�: %s\n", filepath);
        setConsoleTextColor("\033[0m"); // �⺻ ����(���)
        return;
    }

    char title[100];
    char author[100];
    int year;
    char url[300];
    int page;
    char keywords[200]; // Ű���� �߰�

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
            setConsoleTextColor("\033[31m"); // ������
            printf("������ ������ �� �����ϴ�: %s\n", filePath);
            setConsoleTextColor("\033[0m"); // �⺻ ����(���)
        }
        else 
        {
            setConsoleTextColor("\033[31m"); // ������
            printf("���� ���� �Ϸ�: %s\n", filePath);
            setConsoleTextColor("\033[0m"); // �⺻ ����(���)
        }
    }
    else 
    {
        setConsoleTextColor("\033[31m"); // ������
        printf("������ �������� �ʽ��ϴ�: %s\n", filePath);
        setConsoleTextColor("\033[0m"); // �⺻ ����(���)
    }
}

void editPaper(ResearchPaper* paper) {
    char input[100];
    int year;
    char temp[300]; // �ӽ� �迭

    printf("����(%s): ", paper->title);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove newline character
    if (strcmp(input, "") != 0) {
        // ������ �����ϸ� ���� ���� ���� �� ���ο� ���� ����
        deletePaperFile(paper->title, "C:\\Users\\kimmi\\Desktop\\������\\");
        free(paper->title);
        paper->title = my_strdup(input);
    }

    printf("����(%s): ", paper->author);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove newline character
    if (strcmp(input, "") != 0) {
        free(paper->author);
        paper->author = my_strdup(input);
    }

    printf("����(%d): ", paper->year);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0'; // Remove newline character
    if (strcmp(temp, "") != 0) {
        sscanf(temp, "%d", &year);
        paper->year = year;
    }

    printf("�ּ�(%s): ", paper->url);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove newline character
    if (strcmp(input, "") != 0) {
        free(paper->url);
        paper->url = my_strdup(input);
    }

    // ���� ������ �� Ű���� ��� ���
    printf("���� ����� ������ �� Ű����:\n");
    ContentNode* contentTemp = paper->contentsHead;
    while (contentTemp != NULL) {
        printf("������ %d: %s\n", contentTemp->paperContent.page, contentTemp->paperContent.keywords);
        contentTemp = contentTemp->next;
    }

    // ������ �� Ű���� ���� �Ǵ� �߰�
    while (1) {
        setConsoleTextColor("\033[31m"); // ������
        printf("���� ������ �����Ͻðڽ��ϱ�? (y/n): ");
        setConsoleTextColor("\033[0m"); // �⺻ ����(���)
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // Remove newline character
        if (strcmp(input, "y") != 0 && strcmp(input, "Y") != 0) {
            break;
        }

        printf("������ ������ ��ȣ(����: -1): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // Remove newline character
        if (strcmp(input, "-1") == 0) {
            break;
        }
        int editPage = atoi(input);

        // �Է¹��� ������ ��ȣ�� �ش��ϴ� ��� ã��
        contentTemp = paper->contentsHead;
        ContentNode* prevNode = NULL;
        while (contentTemp != NULL) {
            if (contentTemp->paperContent.page == editPage) {
                // �ش� �������� Ű���� ����
                printf("������ %d�� Ű����(%s): ", contentTemp->paperContent.page, contentTemp->paperContent.keywords);
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

        // �Է¹��� ������ ��ȣ�� �ش��ϴ� ��尡 ���� ���, ���ο� ������ �� Ű���� �߰�
        if (contentTemp == NULL) {
            setConsoleTextColor("\033[31m"); // ������
            printf("������ %d�� ã�� �� �����ϴ�. ���ο� ������ �� Ű���带 �߰��մϴ�.\n", editPage);
            setConsoleTextColor("\033[0m"); // �⺻ ����(���)
            printf("�߰��� Ű����: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0'; // Remove newline character

            addPaperContent(paper, editPage, input);
        }
    }
}


// Ư�� �� ������ �߰��ϴ� �Լ�
void addContentToPaper(ResearchPaper* paper) 
{
    int page;
    char keywords[200]; // Ű���� �߰�

    while (1) {
        printf("�߰��� ������ ��ȣ(-1 �Է½� ����): ");
        scanf("%d", &page);
        getchar(); // �Է� ���ۿ��� '\n' ����

        if (page == -1) 
        {
            break;
        }

        printf("������ %d�� Ű����: ", page);
        fgets(keywords, sizeof(keywords), stdin);
        keywords[strcspn(keywords, "\n")] = '\0'; // Remove newline character

        addPaperContent(paper, page, keywords);
        setConsoleTextColor("\033[31m"); // ������
        printf("������ %d�� Ű���� '%s' �߰� �Ϸ�.\n", page, keywords);
        setConsoleTextColor("\033[0m"); // �⺻ ����(���)
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
                // ù ��° ��带 ����
                paper->contentsHead = current->next;
            }
            else {
                prev->next = current->next;
            }
            free(current->paperContent.keywords);
            free(current);
            setConsoleTextColor("\033[31m"); // ������
            printf("������ %d�� �����Ǿ����ϴ�.\n", page);
            setConsoleTextColor("\033[0m"); // �⺻ ����(���)
            return;
        }
        prev = current;
        current = current->next;
    }
    setConsoleTextColor("\033[31m"); // ������
    printf("������ %d�� ã�� �� �����ϴ�.\n", page);
    setConsoleTextColor("\033[0m"); // �⺻ ����(���)
}



void selectAndEditPaper() {
    char title[100];
    printf("������ ���� ������ �Է��ϼ���: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = '\0'; // Remove newline character

    ResearchPaper* paperTemp = head;
    while (paperTemp != NULL) {
        if (strcmp(paperTemp->title, title) == 0) {
            // ������ ���� ���� �����
            setConsoleTextColor("\033[31m"); // ������
            printf("������ �������� �ֽ��ϱ�? (y/n): ");
            setConsoleTextColor("\033[0m"); // �⺻ ����(���)
            char deleteChoice;
            fgets(&deleteChoice, sizeof(deleteChoice), stdin);
            deleteChoice = deleteChoice != '\0' ? deleteChoice : 'n';

            if (deleteChoice == 'y' || deleteChoice == 'Y') {
                while (1) {
                    printf("������ ������ ��ȣ(����: -1): ");
                    fgets(title, sizeof(title), stdin);
                    int pageToDelete = atoi(title);
                    if (pageToDelete == -1) {
                        break;
                    }
                    removeContentFromPaper(paperTemp, pageToDelete);
                }
            }

            // �� ����
            editPaper(paperTemp);


            // �߰��� ���� �Է�
            setConsoleTextColor("\033[31m"); // ������
            printf("�߰��� ������ �Է��Ͻðڽ��ϱ�? (y/n): ");
            setConsoleTextColor("\033[0m"); // �⺻ ����(���)
            char addMore;
            fgets(&addMore, sizeof(addMore), stdin);
            addMore = addMore != '\0' ? addMore : 'n';

            if (addMore == 'y' || addMore == 'Y') {
                addContentToPaper(paperTemp);
                sortPaperContents(paperTemp); // �߰��� ���� ����
            }
            return;
        }
        paperTemp = paperTemp->next;
    }
    setConsoleTextColor("\033[31m"); // ������
    printf("���� �ش��ϴ� ���� ã�� �� �����ϴ�.\n");
    setConsoleTextColor("\033[0m"); // �⺻ ����(���)
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
                printf("Ű����: %s\n", contentTemp->paperContent.keywords); // Ű���� ��� �߰�
                contentTemp = contentTemp->next;
            }
            return;
        }
        paperTemp = paperTemp->next;
    }
    setConsoleTextColor("\033[31m"); // ������
    printf("���� �ش��ϴ� ���� ã�� �� �����ϴ�.\n");
    setConsoleTextColor("\033[0m"); // �⺻ ����(���)
}

void printAllPapers() 
{
    if (head == NULL) 
    {
        setConsoleTextColor("\033[31m"); // ������
        printf("����� ���� �����ϴ�.\n");
        setConsoleTextColor("\033[0m"); // �⺻ ����(���)
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
            printf("Ű����: %s\n", contentTemp->paperContent.keywords); // Ű���� ��� �߰�
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
        setConsoleTextColor("\033[31m"); // ������
        printf("������ �� �� �����ϴ�: %s\n", filePath);
        setConsoleTextColor("\033[0m"); // �⺻ ����(���)
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

            printf("����: ");
            fgets(title, sizeof(title), stdin);
            title[strcspn(title, "\n")] = '\0'; // Remove newline character

            printf("����: ");
            fgets(author, sizeof(author), stdin);
            author[strcspn(author, "\n")] = '\0'; // Remove newline character

            printf("����: ");
            scanf("%d", &year);
            getchar(); // consume the newline character left by scanf

            printf("�ּ�: ");
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
                printf("�߰��� ������ ��ȣ(����: -1): ");
                scanf("%d", &page);
                getchar(); // consume the newline character left by scanf
                if (page == -1) 
                {
                    break;
                }

                printf("�߰��� Ű����: ");
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
            printf("����� ���� ������ �Է��ϼ���: ");
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
            setConsoleTextColor("\033[31m"); // ������
            printf("�ùٸ� ��ȣ�� �����ϼ���.\n");
            setConsoleTextColor("\033[0m"); // �⺻ ����(���)
        }
    }
}

int main() 
{
    const char* folderPath = "C:\\Users\\kimmi\\Desktop\\������\\";
    // ���α׷� ���� �� ��� �ؽ�Ʈ ���� �о����
    readPapersFromFolder(folderPath);

    /*clearConsole();*/

    // ����� �޴� ǥ��
    displayMenu();

    // ���α׷� ���� �� ��� �� �����͸� ���Ϸ� ����
    writePapersToFolder(folderPath);

    return 0;
}
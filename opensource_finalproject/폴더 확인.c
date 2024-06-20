#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <windows.h>
#include <limits.h>

#pragma warning(disable : 4996)

#define DIRECTORY 1
#define FILE_TYPE 0

struct _finddata_t fd;

int isDirectory() {
    if (fd.attrib & _A_SUBDIR)
        return DIRECTORY;
    else
        return FILE_TYPE;
}

void FileSearch(const char* path, char*** fileNames, int* fileCount) {
    intptr_t handle;
    int check = 0;
    char searchPath[_MAX_PATH];

    snprintf(searchPath, sizeof(searchPath), "%s\\*.*", path);

    if ((handle = _findfirst(searchPath, &fd)) == -1) {
        return;
    }

    do {
        char filePath[_MAX_PATH];
        snprintf(filePath, sizeof(filePath), "%s\\%s", path, fd.name);

        check = isDirectory();    // ���丮 �Ǻ�

        if (check == DIRECTORY && fd.name[0] != '.') { //<.>, <..>�� �ƴ� ���丮 
            FileSearch(filePath, fileNames, fileCount);    // ���� ���丮 �˻�
        }
        else if (check == FILE_TYPE && fd.size != 0 && fd.name[0] != '.') {
            // .txt Ȯ���� ����
            char* dot = strrchr(fd.name, '.');
            if (dot && strcmp(dot, ".txt") == 0) {
                *dot = '\0'; // .txt ����
            }
            // ���� �̸� ����
            *fileNames = realloc(*fileNames, (*fileCount + 1) * sizeof(char*));
            (*fileNames)[*fileCount] = strdup(fd.name);
            (*fileCount)++;
        }
    } while (_findnext(handle, &fd) == 0);

    _findclose(handle);
}

// ResearchPaper_contents ����ü ����
typedef struct {
    int page;
    char* contents;
} ResearchPaper_contents;

// ResearchPaper ����ü ����
typedef struct ResearchPaper {
    char* title;
    char* author;
    int year;
    char* url;
    struct ContentNode* contentsHead; // �� ������ ���Ḯ��Ʈ�� ���� ���
    struct ResearchPaper* next;
} ResearchPaper;

// �� ������ ��� ����ü ����
typedef struct ContentNode {
    ResearchPaper_contents paperContent;
    struct ContentNode* next;
} ContentNode;

// ���ڿ��� �����ϴ� �Լ�
char* my_strdup(const char* str) {
    char* copy = (char*)malloc(strlen(str) + 1);
    if (copy) {
        strcpy(copy, str);
    }
    return copy;
}

// �� ���Ḯ��Ʈ�� ���� ���
ResearchPaper* head = NULL;

// ���ο� �� ��带 �����ϰ� ���Ḯ��Ʈ�� �߰��ϴ� �Լ�
void addPaper(char* title, char* author, int year, char* url) {
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
    if (head == NULL) {
        head = newPaper;
    }
    else {
        // ����Ʈ�� ���� ���ο� �� ��� �߰�
        ResearchPaper* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newPaper;
    }
}

// �� ���� ��带 �߰��ϴ� �Լ�
void addPaperContent(ResearchPaper* paper, int page, char* contents) {
    // ���ο� �� ���� ��� ����
    ContentNode* newNode = (ContentNode*)malloc(sizeof(ContentNode));

    // ResearchPaper_contents �� ����
    newNode->paperContent.page = page;
    newNode->paperContent.contents = my_strdup(contents);
    newNode->next = NULL;

    // ���� ����Ʈ�� ��������� ���ο� ������ ���� ���� ����
    if (paper->contentsHead == NULL) {
        paper->contentsHead = newNode;
    }
    else {
        // ���� ����Ʈ�� ���� ���ο� ���� ��� �߰�
        ContentNode* temp = paper->contentsHead;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// ���Ͽ��� �� ������ �о�� ���Ḯ��Ʈ�� �߰��ϴ� �Լ�
void readPaperFromFile(const char* filepath) {
    FILE* file = fopen(filepath, "r");  // file �����͸� ����� �����մϴ�.
    if (file == NULL) {
        printf("������ �� �� �����ϴ�: %s\n", filepath);
        return;
    }

    char title[100];
    char author[100];
    int year;
    char url[100];
    int page;
    char contents[500];

    fscanf(file, "����: %99[^\n]\n", title);
    fscanf(file, "����: %99[^\n]\n", author);
    fscanf(file, "����: %d\n", &year);
    fscanf(file, "�ּ�: %99[^\n]\n", url);

    addPaper(title, author, year, url);
    ResearchPaper* currentPaper = head;
    while (currentPaper->next != NULL) {
        currentPaper = currentPaper->next;
    }

    while (fscanf(file, " ������: %d\n ����: %499[^\n]\n", &page, contents) == 2) {
        addPaperContent(currentPaper, page, contents);
    }

    fclose(file);
}

// ���Ḯ��Ʈ�� ��� ���� �� ������ ����ϴ� �Լ�
void printPapers() {
    ResearchPaper* paperTemp = head;
    while (paperTemp != NULL) {
        printf("����: %s\n", paperTemp->title);
        printf("����: %s\n", paperTemp->author);
        printf("����: %d\n", paperTemp->year);
        printf("�ּ�: %s\n", paperTemp->url);

        ContentNode* contentTemp = paperTemp->contentsHead;
        while (contentTemp != NULL) {
            printf("  ������: %d\n", contentTemp->paperContent.page);
            printf("  ����: %s\n", contentTemp->paperContent.contents);
            contentTemp = contentTemp->next;
        }
        printf("\n");
        paperTemp = paperTemp->next;
    }
}

// Ư�� ���� ���� ��� �ؽ�Ʈ ������ ��ȸ�ϸ鼭 �� ������ �о���� �Լ�
void readAllPapersFromFolder(const char* folderPath) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    char searchPath[MAX_PATH];

    snprintf(searchPath, sizeof(searchPath), "%s\\*.txt", folderPath);
    hFind = FindFirstFile(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("������ �� �� �����ϴ�: %s\n", folderPath);
        return;
    }

    do {
        char filePath[MAX_PATH];
        snprintf(filePath, sizeof(filePath), "%s\\%s", folderPath, findFileData.cFileName);
        readPaperFromFile(filePath);
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

// ���� �Լ�
int main() {
    const char* folderPath = "C:\\Users\\kimmi\\Desktop\\������\\";
    char** fileNames = NULL;
    int fileCount = 0;

    // ���� �̸� �˻�
    FileSearch(folderPath, &fileNames, &fileCount);

    // ���� �̸� ��� �� �� ���� �б�
    for (int i = 0; i < fileCount; i++) {
        char filePath[MAX_PATH];
        snprintf(filePath, sizeof(filePath), "%s\\%s.txt", folderPath, fileNames[i]);
        readPaperFromFile(filePath);

        free(fileNames[i]); // �޸� ����
    }
    free(fileNames); // �޸� ����

    // �� ����Ʈ ���
    printf("\n�� ����Ʈ:\n");
    printPapers();

    return 0;
}

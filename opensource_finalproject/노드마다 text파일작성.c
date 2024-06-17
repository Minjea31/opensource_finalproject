#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// �� ����Ʈ�� ���Ͽ� �����ϴ� �Լ� (�� �� �������� ���� ����)
void savePapersToFile(const char* folderPath) {
    ResearchPaper* paperTemp = head;
    while (paperTemp != NULL) {
        // ���ϸ��� �� �������� ����
        char filename[256]; // ������ ���̷� ����
        snprintf(filename, sizeof(filename), "%s/%s.txt", folderPath, paperTemp->title);

        FILE* file = fopen(filename, "w");
        if (file == NULL) {
            printf("������ �� �� �����ϴ�: %s\n", filename);
        }
        else {
            // ���Ͽ� �� ���� �ۼ�
            fprintf(file, "����: %s\n", paperTemp->title);
            fprintf(file, "����: %s\n", paperTemp->author);
            fprintf(file, "����: %d\n", paperTemp->year);
            fprintf(file, "�ּ�: %s\n", paperTemp->url);

            ContentNode* contentTemp = paperTemp->contentsHead;
            while (contentTemp != NULL) {
                fprintf(file, "  ������: %d\n", contentTemp->paperContent.page);
                fprintf(file, "  ����: %s\n", contentTemp->paperContent.contents);
                contentTemp = contentTemp->next;
            }

            fclose(file);
            printf("���� ���� �Ϸ�: %s\n", filename);
        }

        paperTemp = paperTemp->next;
    }
}

// ���� �Լ�
int main() {
    char title[100];
    char author[100];
    int year;
    char url[100];
    int page;
    char contents[500];
    const char* folderPath = "C:\\Users\\kimmi\\Desktop\\������"; // ������ ���� ���

    while (1) {
        printf("�� ���� (exit �Է½� ����): ");
        scanf("%99s", title);
        if (strcmp(title, "exit") == 0) {
            break;
        }

        printf("����: ");
        scanf("%99s", author);

        printf("����: ");
        scanf("%d", &year);

        printf("URL �ּ�: ");
        scanf("%99s", url);

        addPaper(title, author, year, url);

        ResearchPaper* currentPaper = head;
        while (currentPaper->next != NULL) {
            currentPaper = currentPaper->next;
        }

        while (1) {
            printf("�������� �Է��Ͻÿ� (����: -1): ");
            scanf("%d", &page);
            if (page == -1) {
                break;
            }

            getchar(); // ���� ����
            printf("������ �Է��Ͻÿ�: ");
            fgets(contents, sizeof(contents), stdin);
            contents[strcspn(contents, "\n")] = 0; // fgets�� �Է¹��� ���ڿ����� ���๮�ڸ� ����

            addPaperContent(currentPaper, page, contents);
        }
    }

    printf("\n�� ����Ʈ:\n");
    printPapers();

    // �� ���� ���Ϸ� ����
    savePapersToFile(folderPath);

    return 0;
}
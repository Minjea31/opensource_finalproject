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

// �� ����Ʈ�� ���Ͽ� �����ϴ� �Լ�
void savePapersToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("������ �� �� �����ϴ�.\n");
        return;
    }

    ResearchPaper* paperTemp = head;
    while (paperTemp != NULL) {
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
        fprintf(file, "\n");
        paperTemp = paperTemp->next;
    }

    fclose(file);
}

// ���� �Լ�
int main() {
    char title[100];
    char author[100];
    int year;
    char url[100];
    int page;
    char contents[500];
    const char* filename = "C:\\Users\\kimmi\\Desktop\\test.txt"; // ���� ��θ� ���⿡ ����

    while (1) {
        printf("�� ���� (exit �Է½� ����): ");
        fgets(title, sizeof(title), stdin);
        title[strcspn(title, "\n")] = '\0'; // ���� ���� ����
        if (strcmp(title, "exit") == 0) {
            break;
        }

        printf("����: ");
        fgets(author, sizeof(author), stdin);
        author[strcspn(author, "\n")] = '\0'; // ���� ���� ����

        printf("����: ");
        scanf("%d", &year);
        getchar(); // ���� ����

        printf("URL �ּ�: ");
        fgets(url, sizeof(url), stdin);
        url[strcspn(url, "\n")] = '\0'; // ���� ���� ����

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

        getchar(); // ������ �Է� �� ���� ����
    }

    printf("\n�� ����Ʈ:\n");
    printPapers();

    // �� ����Ʈ�� ���Ͽ� ����
    savePapersToFile(filename);

    return 0;
}

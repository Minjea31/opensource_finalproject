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

// �Լ� ������Ÿ�� ����
void addPaper(char* title, char* author, int year, char* url);
void addPaperContent(ResearchPaper* paper, int page, char* contents);
void readPaperFromFile(const char* filepath);
void editPaper(ResearchPaper* paper);
void addContentToPaper(ResearchPaper* paper);
void readAndEditPaper();
void printPapers();
char* my_strdup(const char* str);

// ResearchPaper ���Ḯ��Ʈ�� ���� ���
ResearchPaper* head = NULL;

// ���ڿ��� �����ϴ� �Լ�
char* my_strdup(const char* str) {
    char* copy = (char*)malloc(strlen(str) + 1);
    if (copy) {
        strcpy(copy, str);
    }
    return copy;
}

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
        // ���� ����Ʈ�� ������ ��ġ�� ã�Ƽ� ����
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

// ���Ͽ��� �� ������ �о�� ���Ḯ��Ʈ�� �߰��ϴ� �Լ�
void readPaperFromFile(const char* filepath) {
    FILE* file = fopen(filepath, "r");
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

    // ����, ����, ����, �ּ� �б�
    fscanf(file, "����: %99[^\n]\n", title);
    fscanf(file, "����: %99[^\n]\n", author);
    fscanf(file, "����: %d\n", &year);
    fscanf(file, "�ּ�: %99[^\n]\n", url);

    // ���ο� �� �߰�
    addPaper(title, author, year, url);
    ResearchPaper* currentPaper = head;
    while (currentPaper->next != NULL) {
        currentPaper = currentPaper->next;
    }

    // �������� ���� �о �� ���뿡 �߰�
    while (fscanf(file, " ������: %d\n ����: %499[^\n]\n", &page, contents) == 2) {
        addPaperContent(currentPaper, page, contents);
    }

    fclose(file);
}

// Ư�� ���� �����ϴ� �Լ�
void editPaper(ResearchPaper* paper) {
    char input[100];
    int year;

    // ���� ����
    printf("����(%s): ", paper->title);
    fgets(input, sizeof(input), stdin);
    if (strcmp(input, "\n") != 0) {
        input[strcspn(input, "\n")] = '\0'; // ���� ���� ����
        free(paper->title);
        paper->title = my_strdup(input);
    }

    // ���� ����
    printf("����(%s): ", paper->author);
    fgets(input, sizeof(input), stdin);
    if (strcmp(input, "\n") != 0) {
        input[strcspn(input, "\n")] = '\0'; // ���� ���� ����
        free(paper->author);
        paper->author = my_strdup(input);
    }

    // ���� ����
    printf("����(%d): ", paper->year);
    fgets(input, sizeof(input), stdin);
    if (strcmp(input, "\n") != 0) {
        sscanf(input, "%d", &year);
        paper->year = year;
    }

    // �ּ� ����
    printf("�ּ�(%s): ", paper->url);
    fgets(input, sizeof(input), stdin);
    if (strcmp(input, "\n") != 0) {
        input[strcspn(input, "\n")] = '\0'; // ���� ���� ����
        free(paper->url);
        paper->url = my_strdup(input);
    }

    // ���� �߰�
    printf("������ �߰��մϴ�.\n");
    addContentToPaper(paper);
}

// Ư�� �� ������ �߰��ϴ� �Լ�
void addContentToPaper(ResearchPaper* paper) {
    int page;
    char contents[500];

    while (1) {
        // ������ ��ȣ �Է�
        printf("�߰��� ������ ��ȣ(����: -1): ");
        scanf("%d", &page);
        getchar(); // scanf �� ���� ���� ó��
        if (page == -1) {
            break;
        }

        // �߰��� ���� �Է�
        printf("�߰��� ����: ");
        fgets(contents, sizeof(contents), stdin);
        contents[strcspn(contents, "\n")] = '\0'; // ���� ���� ����

        // ���� ���� �߰�
        addPaperContent(paper, page, contents);
    }
}

// ���Ͽ��� �� ������ �а� ������ �� �ְ� �ϴ� �Լ�
void readAndEditPaper() {
    while (1) {
        char title[100];
        printf("������ ���� ������ �Է��ϼ��� (���Ḧ �Է��ϸ� ���α׷��� ����˴ϴ�): ");
        fgets(title, sizeof(title), stdin);
        title[strcspn(title, "\n")] = '\0'; // ���� ���� ����

        // ���� ���� Ȯ��
        if (strcmp(title, "����") == 0) {
            break;
        }

        // ���� ��� ����
        char filepath[200];
        sprintf(filepath, "C:\\Users\\kimmi\\Desktop\\������\\%s.txt", title);

        // ���� ���� �õ�
        FILE * file = fopen(filepath, "r");
        if (file == NULL) {
            printf("������ �� �� �����ϴ�: %s\n", filepath);
            printf("�ٽ� �Է��ϰų� '����'�� �Է��ϼ���.\n");
            continue;
        }
        fclose(file);

        // ���Ͽ��� �� ������ �о ���Ḯ��Ʈ�� �߰�
        readPaperFromFile(filepath);

        // ����Ʈ���� �ش� ���� ã�� ����
        ResearchPaper* paperTemp = head;
        while (paperTemp != NULL) {
            if (strcmp(paperTemp->title, title) == 0) {
                editPaper(paperTemp); // �� ����
                break;
            }
            paperTemp = paperTemp->next;
        }
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

// ���� �Լ�
int main() {
    // �� ���� ���
    readAndEditPaper();

    // ���� �� �� ����Ʈ ���
    printf("\n������ �� ����Ʈ:\n");
    printPapers();

    return 0;
}

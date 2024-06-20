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

// ���Ͽ��� �� ������ �о�� ���Ḯ��Ʈ�� �߰��ϴ� �Լ�
void readPaperFromFile(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    while (file == NULL) {
        printf("������ �� �� �����ϴ�: %s\n", filepath);
        printf("�ٽ� �Է��ϼ���: ");
        char new_filepath[200];
        fgets(new_filepath, sizeof(new_filepath), stdin);
        new_filepath[strcspn(new_filepath, "\n")] = '\0'; // ���� ���� ����
        file = fopen(new_filepath, "r");
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

// Ư�� ������ �� ���Ͽ��� ������ �о�� ���Ḯ��Ʈ�� �߰��ϴ� �Լ�
void readPaperByTitle(const char* title) {
    // ���� ��� ����
    char filepath[200];
    sprintf(filepath, "C:\\Users\\kimmi\\Desktop\\������\\%s.txt", title);

    // ���� ���� �õ�
    FILE* file = fopen(filepath, "r");
    while (file == NULL) {
        printf("������ �� �� �����ϴ�: %s\n", filepath);
        printf("�ٽ� �Է��ϼ���: ");
        char new_title[100];
        fgets(new_title, sizeof(new_title), stdin);
        new_title[strcspn(new_title, "\n")] = '\0'; // ���� ���� ����
        sprintf(filepath, "C:\\Users\\kimmi\\Desktop\\������\\%s.txt", new_title);
        file = fopen(filepath, "r");
    }

    char file_title[100];
    char author[100];
    int year;
    char url[100];
    int page;
    char contents[500];

    // ����, ����, ����, �ּ� �б�
    fscanf(file, "����: %99[^\n]\n", file_title);
    fscanf(file, "����: %99[^\n]\n", author);
    fscanf(file, "����: %d\n", &year);
    fscanf(file, "�ּ�: %99[^\n]\n", url);

    // ���ο� �� �߰�
    addPaper(file_title, author, year, url);
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
}

// Ư�� �� ������ �߰��ϴ� �Լ�
void addContentToPaper(ResearchPaper* paper) {
    int page;
    char contents[500];

    while (1) {
        // ������ ��ȣ �Է�
        printf("�߰��� ������ ��ȣ(����: -1): ");
        scanf("%d", &page);
        getchar(); // scanf ���� ���� ���� ���ڸ� ����
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

// �� ������ ������ ������� �����ϴ� �Լ�
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

// Ư�� ���� �˻��ϴ� �Լ�
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

// ��� �� ������ ����ϴ� �Լ�
void printPapers() {
    ResearchPaper* current = head;
    while (current != NULL) {
        printf("����: %s\n����: %s\n����: %d\n�ּ�: %s\n", current->title, current->author, current->year, current->url);

        ContentNode* contentNode = current->contentsHead;
        while (contentNode != NULL) {
            printf(" ������ %d: %s\n", contentNode->paperContent.page, contentNode->paperContent.contents);
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
        printf("\n�޴�:\n");
        printf("1. �� �߰�\n");
        printf("2. �� ���� �߰�\n");
        printf("3. �� ����\n");
        printf("4. �� ���� ����\n");
        printf("5. �� �˻�\n");
        printf("6. �� ���Ͽ��� �ҷ�����\n");
        printf("7. Ư�� ������ �� ���Ͽ��� �ҷ�����\n");
        printf("8. ��� �� ���\n");
        printf("9. ����\n");
        printf("����: ");
        scanf("%d", &choice);
        getchar(); // ���� ���� ����

        switch (choice) {
        case 1:
            printf("����: ");
            fgets(title, sizeof(title), stdin);
            title[strcspn(title, "\n")] = '\0'; // ���� ���� ����
            char author[100];
            int year;
            char url[100];
            printf("����: ");
            fgets(author, sizeof(author), stdin);
            author[strcspn(author, "\n")] = '\0'; // ���� ���� ����
            printf("����: ");
            scanf("%d", &year);
            getchar(); // ���� ���� ����
            printf("�ּ�: ");
            fgets(url, sizeof(url), stdin);
            url[strcspn(url, "\n")] = '\0'; // ���� ���� ����

            addPaper(title, author, year, url);
            break;
        case 2:
            printf("�� ����: ");
            fgets(title, sizeof(title), stdin);
            title[strcspn(title, "\n")] = '\0'; // ���� ���� ����
            paper = searchPaper(title);
            if (paper) {
                addContentToPaper(paper);
            }
            else {
                printf("���� ã�� �� �����ϴ�: %s\n", title);
            }
            break;
        case 3:
            printf("�� ����: ");
            fgets(title, sizeof(title), stdin);
            title[strcspn(title, "\n")] = '\0'; // ���� ���� ����
            paper = searchPaper(title);
            if (paper) {
                editPaper(paper);
            }
            else {
                printf("���� ã�� �� �����ϴ�: %s\n", title);
            }
            break;
        case 4:
            printf("�� ����: ");
            fgets(title, sizeof(title), stdin);
            title[strcspn(title, "\n")] = '\0'; // ���� ���� ����
            paper = searchPaper(title);
            if (paper) {
                sortPaperContents(paper);
                printf("�� ������ ������ ������� ���ĵǾ����ϴ�.\n");
            }
            else {
                printf("���� ã�� �� �����ϴ�: %s\n", title);
            }
            break;
        case 5:
            printf("�� ����: ");
            fgets(title, sizeof(title), stdin);
            title[strcspn(title, "\n")] = '\0'; // ���� ���� ����
            paper = searchPaper(title);
            if (paper) {
                printf("����: %s\n����: %s\n����: %d\n�ּ�: %s\n", paper->title, paper->author, paper->year, paper->url);
                ContentNode* contentNode = paper->contentsHead;
                while (contentNode != NULL) {
                    printf(" ������ %d: %s\n", contentNode->paperContent.page, contentNode->paperContent.contents);
                    contentNode = contentNode->next;
                }
            }
            else {
                printf("���� ã�� �� �����ϴ�: %s\n", title);
            }
            break;
        case 6:
            printf("�ҷ��� ���� ���: ");
            char filepath[200];
            fgets(filepath, sizeof(filepath), stdin);
            filepath[strcspn(filepath, "\n")] = '\0'; // ���� ���� ����
            readPaperFromFile(filepath);
            break;
        case 7:
            printf("�� ����: ");
            fgets(title, sizeof(title), stdin);
            title[strcspn(title, "\n")] = '\0'; // ���� ���� ����
            readPaperByTitle(title);
            break;
        case 8:
            printPapers();
            break;
        case 9:
            return 0;
        default:
            printf("�߸��� �����Դϴ�. �ٽ� �õ��ϼ���.\n");
        }
    }

    return 0;
}

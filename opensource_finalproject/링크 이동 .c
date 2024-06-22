#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <shellapi.h>

#pragma warning(disable : 4996)

// ResearchPaper_contents ����ü ����
typedef struct {
    int page;
    char* keywords; // Ű���� �߰�
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

// ��ũ�� ���� ���� �Լ�
void openLink(const char* url) {
    ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

// Ư�� ������ ���� ����ϰ� ��ũ�� Ŭ���Ͽ� �� �� �ֵ��� ����
void printPaperByTitle(const char* title) {
    ResearchPaper* paperTemp = head;
    while (paperTemp != NULL) {
        if (strcmp(paperTemp->title, title) == 0) {
            printf("����: %s\n", paperTemp->title);
            printf("����: %s\n", paperTemp->author);
            printf("����: %d\n", paperTemp->year);
            printf("�ּ�: %s\n", paperTemp->url);

            // ��ũ�� ���� ���� �Լ� ȣ��
            openLink(paperTemp->url);

            ContentNode* contentTemp = paperTemp->contentsHead;
            while (contentTemp != NULL) {
                printf("������: %d\n", contentTemp->paperContent.page);
                printf("Ű����: %s\n", contentTemp->paperContent.keywords); // Ű���� ��� �߰�
                contentTemp = contentTemp->next;
            }
            return;
        }
        paperTemp = paperTemp->next;
    }

    printf("���� �ش��ϴ� ���� ã�� �� �����ϴ�.\n");
}

int main() {
    // ������ ���� �� ������ �ʱ�ȭ
    ResearchPaper paper1 = { "Sample Paper 1", "John Doe", 2023, "https://www.example.com/paper1", NULL, NULL };
    head = &paper1;

    // Ư�� ������ �� ��� �� ��ũ ���� �׽�Ʈ
    printPaperByTitle("Sample Paper 1");

    return 0;
}

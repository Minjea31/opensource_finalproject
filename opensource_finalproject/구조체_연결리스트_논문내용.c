#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ResearchPaper_contents ����ü ����
typedef struct {
    int page;
    char* contents;
} ResearchPaper_contents;

// ��� ����ü ����
typedef struct Node {
    ResearchPaper_contents paperContent;
    struct Node* next;
} Node;

// ���ڿ��� �����ϴ� �Լ�
char* my_strdup(const char* str) {
    char* copy = (char*)malloc(strlen(str) + 1);
    if (copy) {
        strcpy(copy, str);
    }
    return copy;
}

// ���Ḯ��Ʈ�� ���� ���
Node* head = NULL;

// ���ο� ��带 �����ϰ� ���Ḯ��Ʈ�� �߰��ϴ� �Լ� (������������ ����)
void addPaperContent(int page, char* contents) {
    // ���ο� ��� ����
    Node* newNode = (Node*)malloc(sizeof(Node));

    // ResearchPaper_contents �� ����
    newNode->paperContent.page = page;
    newNode->paperContent.contents = my_strdup(contents);
    newNode->next = NULL;

    // ����Ʈ�� ��������� ���ο� ��带 ���� ���� ����
    if (head == NULL || head->paperContent.page > page) {
        newNode->next = head;
        head = newNode;
    }
    else {
        // ����Ʈ�� ������ ��ġ�� ���ο� ��� ����
        Node* temp = head;
        while (temp->next != NULL && temp->next->paperContent.page < page) {
            temp = temp->next;
        }
        newNode->next = temp->next;
        temp->next = newNode;
    }
}

// ���Ḯ��Ʈ�� ��� ��带 ����ϴ� �Լ�
void printPaperContents() {
    Node* temp = head;
    while (temp != NULL) {
        printf("������ : %d\n", temp->paperContent.page);
        printf("�߿� ���� : %s\n\n", temp->paperContent.contents);
        temp = temp->next;
    }
}

// ���� �Լ�
int main()
{
    int page;
    char contents[500]; // ������ ������ �迭 ũ�⸦ ����� ũ�� ����

    while (1) {
        printf("�������� �Է��Ͻÿ� (���� : -1) : ");
        scanf("%d", &page);
        if (page == -1) {
            break;
        }

        printf("������ �Է��Ͻÿ� : ");
        getchar(); // ���� ����
        fgets(contents, sizeof(contents), stdin);
        contents[strcspn(contents, "\n")] = 0; // fgets�� �Է¹��� ���ڿ����� ���๮�ڸ� ����

        addPaperContent(page, contents);
    }

    printf("\n�� ���� ����Ʈ :\n");
    printPaperContents();

    return 0;
}

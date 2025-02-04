#include<stdio.h>
#include<stdlib.h>

typedef struct Node
{
    int data;
    struct Node* next;
} Node;

struct Node* create(int data)
{
    Node* newNode = malloc(sizeof(Node));
    
    if (newNode == NULL) {
        printf("Failed to allocate memory for Node\n");
        exit(1);
    }
    
    newNode->data = data;
    newNode->next = NULL;

    return newNode;
}

void insertAtEnd(struct Node** head, int data)
{
    struct Node* newNode = create(data);
    if (*head == NULL) {
        *head = newNode;
        
    }
    else {
        struct Node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}



void printList(struct Node* head)
{
    struct Node* current = head;
    while(current != NULL){
    printf("%d , ",current->data);
    current=current->next;}
    printf("end\n");

}
void freelist(struct Node* head)
{
    struct Node* temp;
    while(head !=NULL){
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main()
{
    struct Node* head=NULL;
    int count = 0;
    int input;
    printf("How many numbers: ");
    scanf("%d",&count);
    printf("enter %d numbers:\n", count);

    for(int i=0 ;i< count ;i++){
        scanf("%d",&input);
        insertAtEnd(&head, input);

    }
    printList(head);

    freelist(head);
    getchar();
    getchar();
    return 0;
    
}
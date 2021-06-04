/*************************************
* Lab 1 Exercise 2
* Name: Sebastian Lie
* Matric No: A0164657U
* Lab Group: 15
*************************************/

#include <stdio.h>
#include <stdlib.h> //for malloc() and free()

//Declaration of a Linked List Node

typedef struct NODE{
    int data;
    struct NODE* next;
} node;

//Function Prototypes
node* insertAt(node*, int, int, int);
void printList(node*);
void destroyList(node*);


int main()
{
    node* myList = NULL;    //Empty List
    int position, input, copies;

    //Fill in code for input and processing
    while (scanf("%i %i %i", &position, &input, &copies) == 3){
        myList = insertAt(myList, position, copies, input);
    }
    //Output code coded for you
    printf("My List:\n");
    printList(myList);
    
    destroyList(myList);
    myList = NULL;
    
    printf("My List After Destroy:\n");
    printList(myList);
 
    return 0;
}

//Actual Function Implementations
node* insertAt(node* head, int position, int copies, int newValue)
{   
    // find position
    int counter = position;
    node* nodeAfter = head;
    node* nodeBefore = NULL;
    while (counter > 0 && nodeAfter != NULL) // see if required index or end of list reached first
    {
        nodeBefore = nodeAfter;
        nodeAfter = nodeBefore->next;
        counter--;
    }
    while (copies > 0) // build new list
    {
        node* newNode = (node*)malloc(sizeof(node));
        // printf("%i", newValue);
        newNode->data = newValue;
        newNode->next = nodeAfter;
        nodeAfter = newNode;
        copies--;
    }
    if (nodeBefore != NULL) // node we started at
    {
        nodeBefore->next = nodeAfter;
    }
    else // no node before, need new head
    {
        head = nodeAfter;
    }
    return head;
}
 
void printList(node* head)
//Purpose: Print out the linked list content
//Assumption: the list is properly null terminated
{
    //This function has been implemented for you
    node* ptr = head;

    while (ptr != NULL)  {    //or you can write while(ptr)
        printf("%i ", ptr->data);
        ptr = ptr->next;
    }
    printf("\n");
}

void destroyList(node* head)
{
     
    //Fill in your code here
    //You can use the same implementation as in exercise 1
    node* current = head;
    node* temp = head;
    
    while (temp) {
        current = temp;
   	    temp = current->next;
        free(current);
   }
}

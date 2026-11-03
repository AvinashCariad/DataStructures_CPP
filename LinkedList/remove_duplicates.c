#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM_ELEMENTS_IN_LIST (10)
#define MAX_VALUE (10)

struct node
{
    int data;
    struct node *next;
};

void handle_error(void)
{
    printf("Test failed\n");
    exit(1);
}

/*
head: first node in the linked list
Optimized using a Direct-Address Hashtable
*/
void remove_duplicates(struct node *head)
{
    /* If there are 0 or 1 nodes in the linked list, then simply return */
    if (head == NULL || head->next == NULL)
        return;
            
    /* 
       HASHTABLE REPRESENTATION:
       An array tracking seen values. Indices 0 through 9 represent the 'Keys'.
       Initialized entirely to 0 (False / Not Seen yet).
    */
    int seen_hash_table[MAX_VALUE] = {0};        // initialize all values to 0 (False / Not Seen yet)

    struct node *cur_node = head;
    struct node *prev_node = NULL;

    while (cur_node != NULL) {
        int key = cur_node->data;

        // LOOKUP: O(1) check if the key already exists in our hashtable
        if (seen_hash_table[key] == 1) {
            /* iter_node is a duplicate of cur_node. So remove it */
            prev_node->next = cur_node->next;
            free(cur_node);
            cur_node = prev_node->next;        // move to the next node after deletion
        } else {
            // INSERT: O(1) mark this key as seen in our hashtable
            // Keep Iterating through the list:
            seen_hash_table[key] = 1;
            prev_node = cur_node;
            cur_node = cur_node->next;
        }
    }
}

struct node* construct_list(int num_elements)
{
    int i;
    struct node* head = NULL, *new_node;

    for (i = 0; i < num_elements; ++i) {
        new_node = (struct node*) calloc(1, sizeof(struct node));
        new_node->data = rand() % MAX_VALUE;
        new_node->next = head;
        head = new_node;
    }

    return head;
}

void delete_list(struct node *head)
{
    struct node *cur_node = head;
    struct node *temp_node;

    while (cur_node) {
        temp_node = cur_node->next;
        free(cur_node);
        cur_node = temp_node;
    }
}

void print_list(struct node* head) 
{
    struct node* cur_node = head;

    while (cur_node != NULL) {
        printf("%d ", cur_node->data);
        cur_node = cur_node->next;
    }
    printf("\n");
}

void verify_list(struct node *head)
{
    struct node *cur_node, *iter_node;

    /* If there are 0 or 1 elements in list, then simply return */ 
    if (head == NULL || head->next == NULL)
        return;
            
    cur_node = head;
    while (cur_node) {
        /* Iterate from node after cur_node to the end of the list */
        iter_node = cur_node->next;
        while (iter_node) {
            /* If there is a duplicate, then flag an error */
            if (cur_node->data == iter_node->data) 
                handle_error();
            
            iter_node = iter_node->next;
        }
        cur_node = cur_node->next;
    }
}

// main code harness:
int main(int argc, char** argv)
{
    struct node *head;
    int num_elems;

    /* Test for different list lengths */
    for (num_elems = 0; num_elems <= MAX_NUM_ELEMENTS_IN_LIST; ++num_elems) {
        /* Construct a list with random elements. The list can contain duplicates */
        head = construct_list(num_elems);

        printf("Input  : ");
        print_list(head);

        /* Remove the duplicates using the fast hashtable method */
        remove_duplicates(head);

        printf("Output : ");
        print_list(head);

        /* Verify the list */
        verify_list(head);
        
        delete_list(head);

        printf("__________________________________________________________\n");
    }

    printf("Test passed \n");
    return 0;
}

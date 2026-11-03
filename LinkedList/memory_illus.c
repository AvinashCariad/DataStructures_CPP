# include <stdio.h>
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


// constructs a linked list with num_elements nodes.
// Each node's data is a random value between 0 and MAX_VALUE-1.
// malloc, calloc amd realloc are used in a round-robin fashion
// to demonstrate different memory allocation methods.
struct node* construct_list(int num_elements)
{
    int i;
    struct node* head = NULL;

    for (i = 0; i < num_elements; ++i) {
        struct node *new_node = NULL;

        /*----------------------------------------------------------------------
         * CASE 1: MALLOC (Memory Allocation)
         * Allocates raw memory. Must explicitly initialize both fields 
         * because malloc'd memory contains garbage values.
         *--------------------------------------------------------------------*/
        if (i % 3 == 0) {
            new_node = (struct node*) malloc(sizeof(struct node));
            if (new_node == NULL) handle_error();
            
            new_node->data = rand() % MAX_VALUE; // Explicitly set data
            new_node->next = NULL;               // Explicitly clear pointer garbage
        }
        
        /*----------------------------------------------------------------------
         * CASE 2: CALLOC (Contiguous Allocation)
         * Allocates 1 block of node size and clears it. 
         * new_node->next is automatically initialized to NULL (0).
         *--------------------------------------------------------------------*/
        else if (i % 3 == 1) {
            new_node = (struct node*) calloc(1, sizeof(struct node));
            if (new_node == NULL) handle_error();
            
            new_node->data = rand() % MAX_VALUE; 
            // new_node->next is already NULL thanks to calloc
        }
        
        /*----------------------------------------------------------------------
         * CASE 3: REALLOC (Re-allocation)
         * Modifies an existing block size. To safely demonstrate this, we 
         * start with a tiny 1-byte allocation and resize it to a full struct node.
         *--------------------------------------------------------------------*/
        else {
            // Allocate a placeholder block (too small for a struct node)
            void *temp_ptr = malloc(1);     // Start with a 1-byte allocation 
            if (temp_ptr == NULL) handle_error();

            // Expand the placeholder block to fit our actual structure size
            new_node = (struct node*) realloc(temp_ptr, sizeof(struct node));
            if (new_node == NULL) {
                free(temp_ptr); // Clean up if resizing fails
                handle_error();
            }

            new_node->data = rand() % MAX_VALUE;
            new_node->next = NULL; // Explicitly clear garbage (realloc doesn't zero-out)
        }

        // Attach the newly allocated node to the front of our list
        new_node->next = head;
        head = new_node;
    }

    return head;
}

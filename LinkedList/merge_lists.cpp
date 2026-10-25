#include <iostream>
using namespace std;

struct Node {
    int data;
    Node *next;
};

Node *createNode(int data) {
    Node *node = new Node;
    if (!node) {
        cout << "Error allocating memory!\n";
        return nullptr;
    }
    node->data = data;
    node->next = nullptr;
    return node;
}

void insertNode(Node *&head, int data, int pos) {
    if (!head) {
        head = createNode(data);
        return;
    }
    if (pos == 0) {
        Node *node = createNode(data);
        node->next = head;
        head = node;
        return;
    }
    Node *temp = head;
    int curr_pos = 0;
    while (curr_pos < pos - 1 && temp) {
        ++curr_pos;
        temp = temp->next;
    }
    if (temp) {
        Node *node = createNode(data);
        node->next = temp->next;
        temp->next = node;
    } else {
        cout << "Position is much farther than the length of the list\n";
    }
}

void displayNodes(Node *head) {
    while (head) {
        cout << head->data << "-->";
        head = head->next;
    }
    cout << "NULL\n\n";
}

/**
 * Merges two sorted linked lists in-place.
 * Time Complexity: O(n + m)
 * Space Complexity: O(1)
 */
Node* mergeSortedLists(Node* head1, Node* head2) {
    // A temporary dummy node to act as the anchor for the starting head
    Node dummy; 
    Node* rear = &dummy;

    // Compare nodes from both lists and weave them together
    while (head1 && head2) {
        if (head1->data <= head2->data) {
            rear->next = head1;   // Link node from list 1
            head1 = head1->next;  // Advance list 1
        } else {
            rear->next = head2;   // Link node from list 2
            head2 = head2->next;  // Advance list 2
        }
        rear = rear->next;        // Advance the combined tail pointer
    }

    // Append whichever list still has elements remaining
    rear->next = head1 ? head1 : head2;

    // The actual head is the node following the dummy node
    return dummy.next;
}


// main code harness: 
int main(int argc, char** argv) {
    Node* head1 = createNode(1);
    insertNode(head1, 3, 1);
    insertNode(head1, 8, 2);
    insertNode(head1, 12, 3);
    insertNode(head1, 13, 4);
    cout << "List 1:\n";
    displayNodes(head1);

    Node *head2 = createNode(2);
    insertNode(head2, 3, 1);
    insertNode(head2, 4, 2);
    insertNode(head2, 9, 3);
    insertNode(head2, 16, 4);
    insertNode(head2, 19, 5);
    cout << "List 2:\n";
    displayNodes(head2);

    Node* head = mergeSortedLists(head1, head2);
    cout << "Merged List:\n";
    displayNodes(head);
    
    return 0;
}

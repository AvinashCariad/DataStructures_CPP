#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Helper to push standard "base" limits so we don't pop from an empty tower
void initializeTowers(vector<int> t[], int n) {
    // We add a dummy largest element (n + 1) to act as a floor boundary
    for (int i = n + 1; i >= 1; --i) {
        t[0].push_back(i);
    }
    t[1].push_back(n + 1);        // note the dummy large element is added 
    t[2].push_back(n + 1);        // note the dummy large element is added 
}

// Function to print and execute the legal move between two pegs
void moveDiskBetweenPegs(vector<int> t[], int src, int dest, char names[]) {
    int srcTop = t[src].back();
    int destTop = t[dest].back();

    // If source peg top is smaller, it legally moves to destination
    if (srcTop < destTop) {
        t[dest].push_back(srcTop);
        t[src].pop_back();
        cout << "Move disk " << srcTop << " from Peg " << names[src] << " to Peg " << names[dest] << "\n";
    } 
    // If destination peg top is smaller, it moves to source instead
    else {
        t[src].push_back(destTop);
        t[dest].pop_back();
        cout << "Move disk " << destTop << " from Peg " << names[dest] << " to Peg " << names[src] << "\n";
    }
}

void iterativeHanoi(int n) {
    // Representation of the 3 rods (0 = Source, 1 = Auxiliary, 2 = Destination)
    vector<int> t[3]; 
    char pegNames[] = {'A', 'B', 'C'};
    
    initializeTowers(t, n);

    int total_moves = pow(2, n) - 1;

    // If number of disks is even, destination and auxiliary swap roles
    int src = 0, aux = 1, dest = 2;
    if (n % 2 == 0) {
        swap(aux, dest);
    }

    // Iterate through all moves from 1 to 2^n - 1
    for (int i = 1; i <= total_moves; ++i) {
        if (i % 3 == 1) {
            moveDiskBetweenPegs(t, src, dest, pegNames);
        } else if (i % 3 == 2) {
            moveDiskBetweenPegs(t, src, aux, pegNames);
        } else if (i % 3 == 0) {
            moveDiskBetweenPegs(t, aux, dest, pegNames);
        }
    }
}


// main code harness:
int main(int argc, char** argv) {
    int disks;
    cout << "Enter the number of disks: ";
    cin >> disks;

    if (disks < 1) {
        cout << "Please enter a valid number of disks (>= 1)." << endl;
        return 0;
    }

    cout << "The sequence of moves involved in the Tower of Hanoi:\n";
    iterativeHanoi(disks);

    return 0;
}

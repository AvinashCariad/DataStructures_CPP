#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <cctype>
#include <limits>

using namespace std;

const int BOARD_SIZE = 10;


// define ship struct:
struct Ship {
    string name;
    int size;
    int hits = 0;
    bool isSunk() const { return hits >= size; }
};


// define board:
class Board {
private:
    char displayGrid[BOARD_SIZE][BOARD_SIZE]; // '~' water, 'S' ship, 'X' hit, 'O' miss
    int shipIdGrid[BOARD_SIZE][BOARD_SIZE];   // Stores index of ship (-1 if none)
    vector<Ship> ships;

public:
    Board() {
        for (int r = 0; r < BOARD_SIZE; ++r) {
            for (int c = 0; c < BOARD_SIZE; ++c) {
                displayGrid[r][c] = '~';
                shipIdGrid[r][c] = -1;
            }
        }
        ships = {
            {"Carrier", 5},
            {"Battleship", 4},
            {"Cruiser", 3},
            {"Submarine", 3},
            {"Destroyer", 2}
        };
    }

    bool canPlaceShip(int row, int col, int size, bool horizontal) const {
        if (horizontal) {
            if (col + size > BOARD_SIZE) return false;
            for (int c = col; c < col + size; ++c) {
                if (shipIdGrid[row][c] != -1) return false;
            }
        } else {
            if (row + size > BOARD_SIZE) return false;
            for (int r = row; r < row + size; ++r) {
                if (shipIdGrid[r][col] != -1) return false;
            }
        }
        return true;
    }

    void placeShip(int shipIdx, int row, int col, bool horizontal) {
        int size = ships[shipIdx].size;
        if (horizontal) {
            for (int c = col; c < col + size; ++c) {
                displayGrid[row][c] = 'S';
                shipIdGrid[row][c] = shipIdx;
            }
        } else {
            for (int r = row; r < row + size; ++r) {
                displayGrid[r][col] = 'S';
                shipIdGrid[r][col] = shipIdx;
            }
        }
    }

    void placeShipsRandomly(mt19937& rng) {
        uniform_int_distribution<int> coordDist(0, BOARD_SIZE - 1);
        uniform_int_distribution<int> dirDist(0, 1);

        for (int i = 0; i < (int)ships.size(); ++i) {
            bool placed = false;
            while (!placed) {
                int r = coordDist(rng);
                int c = coordDist(rng);
                bool horiz = (dirDist(rng) == 0);
                if (canPlaceShip(r, c, ships[i].size, horiz)) {
                    placeShip(i, r, c, horiz);
                    placed = true;
                }
            }
        }
    }

    // Returns: -1 = already guessed, 0 = miss, 1 = hit, 2 = hit & sunk
    int receiveAttack(int row, int col, string& sunkShipName) {
        char cell = displayGrid[row][col];
        if (cell == 'X' || cell == 'O') {
            return -1;
        }

        int id = shipIdGrid[row][col];
        if (id != -1) {
            displayGrid[row][col] = 'X';
            ships[id].hits++;
            if (ships[id].isSunk()) {
                sunkShipName = ships[id].name;
                return 2;
            }
            return 1;
        } else {
            displayGrid[row][col] = 'O';
            return 0;
        }
    }

    bool allShipsSunk() const {
        for (const auto& ship : ships) {
            if (!ship.isSunk()) return false;
        }
        return true;
    }

    void print(bool hideShips) const {
        cout << "   0 1 2 3 4 5 6 7 8 9\n";
        for (int r = 0; r < BOARD_SIZE; ++r) {
            cout << " " << (char)('A' + r) << " ";
            for (int c = 0; c < BOARD_SIZE; ++c) {
                char ch = displayGrid[r][c];
                if (hideShips && ch == 'S') {
                    cout << "~ ";
                } else {
                    cout << ch << " ";
                }
            }
            cout << "\n";
        }
    }

    const vector<Ship>& getShips() const { return ships; }
};

bool parseCoordinate(const string& input, int& row, int& col) {
    if (input.length() != 2) return false;
    char rChar = toupper(input[0]);
    char cChar = input[1];
    if (rChar < 'A' || rChar >= 'A' + BOARD_SIZE) return false;
    if (cChar < '0' || cChar >= '0' + BOARD_SIZE) return false;
    row = rChar - 'A';
    col = cChar - '0';
    return true;
}

void manualPlacement(Board& playerBoard) {
    const auto& ships = playerBoard.getShips();
    for (int i = 0; i < (int)ships.size(); ++i) {
        while (true) {
            playerBoard.print(false);
            cout << "\nPlace your " << ships[i].name << " (size " << ships[i].size << ").\n";
            cout << "Enter start coordinate (e.g., A0) and orientation (H/V): ";
            string coord;
            char dir;
            cin >> coord >> dir;
            dir = toupper(dir);

            int r, c;
            if (!parseCoordinate(coord, r, c) || (dir != 'H' && dir != 'V')) {
                cout << "Invalid input. Try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            bool horiz = (dir == 'H');
            if (!playerBoard.canPlaceShip(r, c, ships[i].size, horiz)) {
                cout << "Ship does not fit or overlaps another ship. Try again.\n";
                continue;
            }

            playerBoard.placeShip(i, r, c, horiz);
            break;
        }
    }
}


// main code harness:
int main(int argc, char** argv) {
    mt19937 rng(static_cast<unsigned int>(time(nullptr)));
    Board playerBoard;
    Board aiBoard;

    cout << "=== BATTLESHIP ===\n";
    cout << "Place ships manually (M) or randomly (R)? ";
    char choice;
    cin >> choice;
    if (toupper(choice) == 'M') {
        manualPlacement(playerBoard);
    } else {
        playerBoard.placeShipsRandomly(rng);
    }
    aiBoard.placeShipsRandomly(rng);

    uniform_int_distribution<int> dist(0, BOARD_SIZE - 1);

    while (true) {
        cout << "\n--- ENEMY WATERS ---\n";
        aiBoard.print(true);
        cout << "\n--- YOUR FLEET ---\n";
        playerBoard.print(false);

        // Player Turn
        int r, c;
        while (true) {
            cout << "\nEnter target coordinate (e.g., B4): ";
            string input;
            cin >> input;
            if (!parseCoordinate(input, r, c)) {
                cout << "Invalid format. Use A0 through J9.\n";
                continue;
            }
            string sunkName;
            int result = aiBoard.receiveAttack(r, c, sunkName);
            if (result == -1) {
                cout << "You already fired there. Pick another spot.\n";
            } else if (result == 0) {
                cout << "MISS at " << input << "!\n";
                break;
            } else if (result == 1) {
                cout << "HIT at " << input << "!\n";
                break;
            } else if (result == 2) {
                cout << "HIT! You sunk the enemy's " << sunkName << "!\n";
                break;
            }
        }

        if (aiBoard.allShipsSunk()) {
            cout << "\n*** VICTORY! You destroyed the entire enemy fleet! ***\n";
            break;
        }

        // AI Turn
        while (true) {
            int aiR = dist(rng);
            int aiC = dist(rng);
            string sunkName;
            int result = playerBoard.receiveAttack(aiR, aiC, sunkName);
            if (result != -1) {
                char rowLabel = 'A' + aiR;
                cout << "Enemy fires at " << rowLabel << aiC << ": ";
                if (result == 0) cout << "MISS!\n";
                else if (result == 1) cout << "HIT!\n";
                else if (result == 2) cout << "HIT! Your " << sunkName << " was sunk!\n";
                break;
            }
        }

        if (playerBoard.allShipsSunk()) {
            cout << "\n*** DEFEAT! The enemy sunk all your ships! ***\n";
            break;
        }
    }

    return 0;
}


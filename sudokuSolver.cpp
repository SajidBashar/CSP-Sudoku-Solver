#include <bits/stdc++.h>

using namespace std;

unsigned int nodesExpanded = 0;

// B: basic backtracking search (no forward checking, random variable order and random value order)
bool solveSudoku(vector<vector<int>> &board, vector<pair<int, int>> emptyCells) {
     // Base Case: if no empty cells, board is solved and goal state found
     if (emptyCells.size() == 0) {
          return true;
     }

     // choose a random empty cell to start
     random_shuffle(emptyCells.begin(), emptyCells.end());

     int row = emptyCells[0].first;
     int col = emptyCells[0].second;

     // random shuffle the possible values to try
     vector<int> possibleValues = {1, 2, 3, 4, 5, 6, 7, 8, 9};
     random_shuffle(possibleValues.begin(), possibleValues.end());

     nodesExpanded++;

          for (int i = 0; i < possibleValues.size(); i++) {
          int value = possibleValues[i];
          bool valid = true;

          // check if value is valid in row
          for (int j = 0; j < 9; j++) {
               if (board[row][j] == value) {
                    valid = false;
                    break;
               }
          }

          // check if value is valid in column
          for (int j = 0; j < 9; j++) {
               if (board[j][col] == value) {
                    valid = false;
                    break;
               }
          }

          // check if value is valid in 3x3 box
          int rowBoxIndx = row / 3;
          int colBoxIndx = col / 3;
          for (int j = rowBoxIndx * 3; j < rowBoxIndx * 3 + 3; j++) {
               for (int k = colBoxIndx * 3; k < colBoxIndx * 3 + 3; k++) {
                    if (board[j][k] == value) {
                         valid = false;
                         break;
                    }
               }
               if (!valid) break;
          }

               // if value is valid, then set it and recurse
          if (valid) {
               board[row][col] = value;
               emptyCells.erase(emptyCells.begin());

               if (solveSudoku(board, emptyCells)) {
                    return true;
               }

               board[row][col] = 0;
                    emptyCells.insert(emptyCells.begin(), make_pair(row, col));
          }
     }

     // if no valid values found, return false and backtrack
     return false;
}

// B+FC: backtracking search with forward checking for empty cells (random variable order and random value order)
bool solveSudokuForwardChecking(vector<vector<int>> &board, vector<pair<int, int>> emptyCells, vector<vector<set<int>>> legalValues) {
     // Base Case: if no empty cells, board is solved and goal state found
     if (emptyCells.size() == 0) {
          return true;
     }

     // choose a random empty cell to start
     random_shuffle(emptyCells.begin(), emptyCells.end());

     int row = emptyCells[0].first;
     int col = emptyCells[0].second;

     // random shuffle the possible values to try
     vector<int> possibleValues = {1, 2, 3, 4, 5, 6, 7, 8, 9};
     random_shuffle(possibleValues.begin(), possibleValues.end());

     nodesExpanded++;

     for (int i = 0; i < possibleValues.size(); i++) {
          int value = possibleValues[i];
          bool valid = true;

          // check if value is valid in row
          for (int j = 0; j < 9; j++) {
               if (board[row][j] == value) {
                    valid = false;
                    break;
               }
          }

          // check if value is valid in column
          for (int j = 0; j < 9; j++) {
               if (board[j][col] == value) {
                    valid = false;
                    break;
               }
          }

          // check if value is valid in 3x3 box
          int rowBoxIndx = row / 3;
          int colBoxIndx = col / 3;
          for (int j = rowBoxIndx * 3; j < rowBoxIndx * 3 + 3; j++) {
               for (int k = colBoxIndx * 3; k < colBoxIndx * 3 + 3; k++) {
                    if (board[j][k] == value) {
                         valid = false;
                         break;
                    }
               }
               if (!valid) break;
          }

               // if value is valid, then set it and recurse
          if (valid) {
               board[row][col] = value;
               emptyCells.erase(emptyCells.begin());

               bool forwardCheck = true;
               vector<vector<set<int>>> legalValuesCopy = legalValues;

               // update remaining legal values for empty cells based on the new value assigned to the current empty cell
                    for (int i = 0; i < emptyCells.size(); i++) { 
                         int emptyCellRow = emptyCells[i].first;
                         int emptyCellCol = emptyCells[i].second;

                         // check if value is present in empty cell's row
                         for (int j = 0; j < 9; j++) {
                              if (board[emptyCellRow][j] == value) {
                                   legalValuesCopy[emptyCellRow][emptyCellCol].erase(value);
                                   if (legalValuesCopy[emptyCellRow][emptyCellCol].size() == 0) {
                                        forwardCheck = false;
                                        break;
                                   }
                              }
                         }

                         // check if value is present in empty cell's column
                         for (int j = 0; j < 9; j++) {
                              if (board[j][emptyCellCol] == value) {
                                   legalValuesCopy[emptyCellRow][emptyCellCol].erase(value);
                                   if (legalValuesCopy[emptyCellRow][emptyCellCol].size() == 0) {
                                        forwardCheck = false;
                                        break;
                                   }
                              }
                         }

                         // check if value is present in empty cell's 3x3 box  
                         int rowBoxIndx = emptyCellRow / 3;
                         int colBoxIndx = emptyCellCol / 3;
                         for (int j = rowBoxIndx * 3; j < rowBoxIndx * 3 + 3; j++) {
                              for (int k = colBoxIndx * 3; k < colBoxIndx * 3 + 3; k++) {
                                   if (board[j][k] == value) {
                                        legalValuesCopy[emptyCellRow][emptyCellCol].erase(value);
                                        if (legalValuesCopy[emptyCellRow][emptyCellCol].size() == 0) {
                                             forwardCheck = false;
                                             break;
                                        }
                                   }
                              }
                              if (!forwardCheck) break;
                         }
                    }

               if (forwardCheck && solveSudokuForwardChecking(board, emptyCells, legalValuesCopy)) {
                    return true;
               }

               board[row][col] = 0;
                    emptyCells.insert(emptyCells.begin(), make_pair(row, col));
               
          }
     }

     // if no valid values found, return false and backtrack
     return false;
}

int countConstraints(vector<vector<int>> &board, int row, int col) {
     int count = 0;

     // Count constraints in the same row
     for (int c = 0; c < 9; c++) {
          if (c != col && board[row][c] != 0) {
               count++;
          }
     }

     // Count constraints in the same column
     for (int r = 0; r < 9; r++) {
          if (r != row && board[r][col] != 0) {
               count++;
          }
     }

     // Count constraints in the same block
     int rowBoxIndx = row / 3;
     int colBoxIndx = col / 3;

     for (int r = rowBoxIndx * 3; r < rowBoxIndx * 3 + 3; r++) {
          for (int c = colBoxIndx * 3; c < colBoxIndx * 3 + 3; c++) {
               if (r != row && c != col && board[r][c] != 0) {
                    count++;
               }
          }
     }

     return count;
}

int countConstraintsValue(vector<vector<int>>& board, int row, int col, int value) {
     int constraints = 0;

     // Check if value is present in the same row
     for (int j = 0; j < 9; j++) {
          if (board[row][j] == value) {
               constraints++;
          }
     }

     // Check if value is present in the same column
     for (int i = 0; i < 9; i++) {
          if (board[i][col] == value) {
               constraints++;
          }
     }

     // Check if value is present in the same 3x3 box
     int rowBoxIndx = row / 3;
     int colBoxIndx = col / 3;
     for (int i = rowBoxIndx * 3; i < rowBoxIndx * 3 + 3; i++) {
          for (int j = colBoxIndx * 3; j < colBoxIndx * 3 + 3; j++) {
               if (board[i][j] == value) {
               constraints++;
               }
          }
     }

     return constraints;
}

// B+FC+H: backtracking search with forward checking and the 3 heuristics (break any remaining ties in the order of the variables and values at random)
bool solveSudokuForwardingCheckingHeuristics(vector<vector<int>> &board, vector<pair<int, int>> emptyCells, vector<vector<set<int>>> legalValues) {
     // Base Case: if no empty cells, board is solved and goal state found
     if (emptyCells.size() == 0) {
          return true;
     }

     // apply the most constrained variable and most constraining variable heuristics to find the next empty cell to fill
     int row = emptyCells[0].first;
     int col = emptyCells[0].second;
     int emptyCellIdx = 0; 

     for (int i = 1; i < emptyCells.size(); i++) {
          int emptyCellRow = emptyCells[i].first;
          int emptyCellCol = emptyCells[i].second;

          if (legalValues[emptyCellRow][emptyCellCol].size() < legalValues[row][col].size()) {   // most constrained variable heuristic
               row = emptyCellRow;
               col = emptyCellCol;
               emptyCellIdx = i;
          } else if (legalValues[emptyCellRow][emptyCellCol].size() == legalValues[row][col].size()) {   // break ties using most constraining variable heuristic
               int numConstraints1 = countConstraints(board, emptyCellRow, emptyCellCol);
               int numConstraints2 = countConstraints(board, row, col);

               if (numConstraints1 > numConstraints2) {
                    row = emptyCellRow;
                    col = emptyCellCol;
                    emptyCellIdx = i;
               }
          }
     }

     // least constraining value heuristic
     vector<int> possibleValues = {1, 2, 3, 4, 5, 6, 7, 8, 9};
     for (int i = 0; i < possibleValues.size(); i++) {
          for (int j = i + 1; j < possibleValues.size(); j++) {
               int numConstraints1 = countConstraintsValue(board, row, col, possibleValues[i]);
               int numConstraints2 = countConstraintsValue(board, row, col, possibleValues[j]);

               if (numConstraints1 > numConstraints2) {
                    int temp = possibleValues[i];
                    possibleValues[i] = possibleValues[j];
                    possibleValues[j] = temp;
               }

               if (numConstraints1 == numConstraints2) {  // break ties at random
                    int randNum = rand() % 2;
                    if (randNum == 0) {
                         int temp = possibleValues[i];
                         possibleValues[i] = possibleValues[j];
                         possibleValues[j] = temp;
                    }
               }
          }
     }

     nodesExpanded++;

     for (int i = 0; i < possibleValues.size(); i++) {
          int value = possibleValues[i];
          bool valid = true;

          // check if value is valid in row
          for (int j = 0; j < 9; j++) {
               if (board[row][j] == value) {
                    valid = false;
                    break;
               }
          }

          // check if value is valid in column
          for (int j = 0; j < 9; j++) {
               if (board[j][col] == value) {
                    valid = false;
                    break;
               }
          }

          // check if value is valid in 3x3 box
          int rowBoxIndx = row / 3;
          int colBoxIndx = col / 3;
          for (int j = rowBoxIndx * 3; j < rowBoxIndx * 3 + 3; j++) {
               for (int k = colBoxIndx * 3; k < colBoxIndx * 3 + 3; k++) {
                    if (board[j][k] == value) {
                         valid = false;
                         break;
                    }
               }
               if (!valid) break;
          }

          // if value is valid, then set it and recurse
          if (valid) {
               board[row][col] = value;
               emptyCells.erase(emptyCells.begin() + emptyCellIdx);

               bool forwardCheck = true;
               vector<vector<set<int>>> legalValuesCopy = legalValues;

               // update remaining legal values for empty cells based on the new value assigned to the current empty cell
               for (int i = 0; i < emptyCells.size(); i++) { 
                    int emptyCellRow = emptyCells[i].first;
                    int emptyCellCol = emptyCells[i].second;

                    // check if value is present in empty cell's row
                    for (int j = 0; j < 9; j++) {
                         if (board[emptyCellRow][j] == value) {
                              legalValuesCopy[emptyCellRow][emptyCellCol].erase(value);
                              if (legalValuesCopy[emptyCellRow][emptyCellCol].size() == 0) {
                                   forwardCheck = false;
                                   break;
                              }
                         }
                    }

                    // check if value is present in empty cell's column
                    for (int j = 0; j < 9; j++) {
                         if (board[j][emptyCellCol] == value) {
                              legalValuesCopy[emptyCellRow][emptyCellCol].erase(value);
                              if (legalValuesCopy[emptyCellRow][emptyCellCol].size() == 0) {
                                   forwardCheck = false;
                                   break;
                              }
                         }
                    }

                    // check if value is present in empty cell's 3x3 box
                    int rowBoxIndx = emptyCellRow / 3;
                    int colBoxIndx = emptyCellCol / 3;
                    for (int j = rowBoxIndx * 3; j < rowBoxIndx * 3 + 3; j++) {
                         for (int k = colBoxIndx * 3; k < colBoxIndx * 3 + 3; k++) {
                              if (board[j][k] == value) {
                                   legalValuesCopy[emptyCellRow][emptyCellCol].erase(value);
                                   if (legalValuesCopy[emptyCellRow][emptyCellCol].size() == 0) {
                                        forwardCheck = false;
                                        break;
                                   }
                              }
                         }
                         if (!forwardCheck) break;
                    }
               }

               if (forwardCheck && solveSudokuForwardingCheckingHeuristics(board, emptyCells, legalValuesCopy)) {
                    return true;
               }

               board[row][col] = 0;
               emptyCells.insert(emptyCells.begin() + emptyCellIdx, make_pair(row, col));
          }
     }

     // if no valid values found, return false and backtrack
     return false;
}

// driver function 
int main(int argc, char **argv) {
     if (argc != 2) {
          cout << "Usage: ./sudokuSolver <B|B+FC|B+FC+H> < puzzle.txt" << endl;
          return 1;
     }

     srand(time(0)); // ensure randomization
     vector<vector<int>> board(9, vector<int>(9, 0));

     for (int i = 0; i < 9; i++) {
          for (int j = 0; j < 9; j++) {
               cin >> board[i][j];
          }
     }

          vector<pair<int, int>> emptyCells; // queue of empty cells

     for (int i = 0; i < 9; i++) {
          for (int j = 0; j < 9; j++) {
               if (board[i][j] == 0) {
                    emptyCells.push_back(make_pair(i, j));
               }
          }
     }

     bool solved;

     if (argv[1] == string("B")) {
          cout << "Basic Backtracking Search" << endl;
          solved = solveSudoku(board, emptyCells);
     } else if (argv[1] == string("B+FC") || argv[1] == string("B+FC+H")) {
          if (argv[1] == string("B+FC")) {
               cout << "Backtracking Search with Forward Checking" << endl;
          } else {
               cout << "Backtracking Search with Forward Checking and Heuristic" << endl;
          }

          vector<vector<set<int>>> legalValues;

          for (int i = 0; i < 9; i++) {
               vector<set<int>> row;
               for (int j = 0; j < 9; j++) {
                    set<int> values = {1, 2, 3, 4, 5, 6, 7, 8, 9};
                    row.push_back(values);
               }
               legalValues.push_back(row);
          }

          // update legal values for all empty cells
          for (int i = 0; i < emptyCells.size(); i++) {
               int emptyCellRow = emptyCells[i].first;
               int emptyCellCol = emptyCells[i].second;

               // update legal values for empty cell based on values already present in row
               for (int j = 0; j < 9; j++) {
                    if (board[emptyCellRow][j] != 0) {
                         legalValues[emptyCellRow][emptyCellCol].erase(board[emptyCellRow][j]);
                    }
               }

               // update legal values for empty cell based on values already present in column
               for (int j = 0; j < 9; j++) {
                    if (board[j][emptyCellCol] != 0) {
                         legalValues[emptyCellRow][emptyCellCol].erase(board[j][emptyCellCol]);
                    }
               }

               // update legal values for empty cell based on values already present in 3x3 box
               int rowBoxIndx = emptyCellRow / 3;
               int colBoxIndx = emptyCellCol / 3;
               for (int j = rowBoxIndx * 3; j < rowBoxIndx * 3 + 3; j++) {
                    for (int k = colBoxIndx * 3; k < colBoxIndx * 3 + 3; k++) {
                         if (board[j][k] != 0) {
                              legalValues[emptyCellRow][emptyCellCol].erase(board[j][k]);
                         }
                    }
               }
          }

          if (argv[1] == string("B+FC")) {
               solved = solveSudokuForwardChecking(board, emptyCells, legalValues);
          } else {
               solved = solveSudokuForwardingCheckingHeuristics(board, emptyCells, legalValues);
          }
     } else {
          if (argc)
          cout << "Error: Invalid combination" << endl;
          return 0;
     }

     cout << "------------------" << endl;

     if (solved) {
          for (int i = 0; i < 9; i++) {
               for (int j = 0; j < 9; j++) {
                    cout << board[i][j] << " ";
               }
               cout << endl;
          }
     } else {  
          cout << "No solution!" << endl;
     }
          
     cout << "------------------" << endl;
     cout << "Nodes Expanded: " << nodesExpanded << endl;
     return 0;
}

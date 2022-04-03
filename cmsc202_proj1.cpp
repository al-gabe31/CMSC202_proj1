// File: proj1.cpp
// Project: CMSC 202 proj1, Fall 2021
// Author: Gabe Aquino
// Date: 9/23/2021
// Section: 30/33
// E-mail: lq90575@umbc.edu
// Asks for the user inputs to solve a crossword

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

// Precondition: The file name that the user inputs must be a valid (existing) file
// Postcondition: returns a string of the file name
string getFileName();


// Precondition: all arrays passed must have the matching size as the parameters
// Postcondition: Populates (fills) the grid with the 15x15 letters and remaining with all 10 words to find
void loadFile(char grid[15][15], string remaining[10], string fileName);


// Precondition: Both the grid and remaining variable must have correctly been filled by the loadFile function
// Postcondition: Prints the 15x15 crossword puzzle followed by all remaining words
void displayPuzzle(char grid[15][15], string remaining[10]);


// Precondition: grid variable must have correctly been filled by the loadFile function
// Postcondition: returns a string that's constructed after inputing 2 pairs of coordinates from the crossword, creating a word
string buildWord(char grid[15][15]);


// Precondition: remaining array must have correctly been filled by the loadFile function
// Postcondition: returns a bool where if true, will turn the found word into "" where it will be ignored
bool checkWord(string word, string remaining[10]);


// Precondition: coord must be an integer
// Postcondition: returns true if coord is in the range 0 to 14 (inclusive); false otherwise
bool inRange(int coord);


// Precondition: remaining variable must have correctly been filled by the loadFile function
// Postcondition: returns a bool if all values in the remaining array is "", implying that all words have been found
bool puzzleFinished(string remaining[10]);


int main() {
    const int DIMENSION = 15;
    char grid[DIMENSION][DIMENSION];
    string remaining[10];
    fstream myFile;
    string fileName;

    cout << "Welcome to UMBC Word Search" << endl;

    // Prompts the user which file they want to open
    fileName = getFileName();
    myFile.open(fileName, ios::in);

    // Gets all the letters and words from the file and puts them into the grid and remaining arrays
    loadFile(grid, remaining, fileName);

    // The program should continue until all words have been found
    do {
        displayPuzzle(grid, remaining);
        string word = buildWord(grid);
        // if statement executes only when the user inputted valid coordinates (i.e. buildWord made a word)
        if(word != "") {
            checkWord(word, remaining);
        }
    } while(puzzleFinished(remaining) == false);

    cout << "Thank you for playing UMBC Word Search!" << endl;

    if(myFile.is_open()) {
        myFile.close();
    }
    return 0;
}

string getFileName() {
   string fileName = "";
   fstream testFile;

   do {
       if(testFile.is_open()) {
           testFile.close();
       }

      cout << "What is the name of the file you would like to load?" << endl;
      cin >> fileName;
      testFile.open(fileName, ios::in);
   } while(testFile.is_open() == false);

   return fileName;
}

void loadFile(char grid[15][15], string remaining[10], string fileName) {
    int numLetters = 0;
    int numWords = 0;
    fstream myFile;
    myFile.open(fileName, ios::in);

    // Puts all the letters from the text file into the grid variable
    for(int i = 0; i < 15; i++) {
        myFile >> grid[i][0] >> grid[i][1] >> grid[i][2] >> grid[i][3] >> grid[i][4] >> grid[i][5] >> grid[i][6] >> grid[i][7] >> grid[i][8] >> grid[i][9] >> grid[i][10] >> grid[i][11] >> grid[i][12] >> grid[i][13] >> grid[i][14];
        numLetters += 15;
    }
    cout << numLetters << " letters imported." << endl;

    // Puts all the words from the text file into the remaining variable
    for(int i = 0; i < 10; i++) {
        myFile >> remaining[i];
        numWords++;
    }
    cout << numWords << " words imported." << endl;

    if(myFile.is_open()) {
        myFile.close();
    }
}

void displayPuzzle(char grid[15][15], string remaining[10]) {
    // Displays the top row, which is all numbers
    cout << " ";
    for(int i = 0; i < 10; i++) {
        cout << "   " << i;
    }
    for(int i = 10; i < 15; i++) {
        cout << "  " << i;
    }
    cout << endl;

    // Displays all the other rows: which starts with a row number followed by the letters in the crossword
    for(int i = 0; i < 10; i++) {
        cout << " " << i << "  ";

        for(int j = 0; j < 15; j++) {
            cout << grid[i][j] << "   ";
        }

        cout << endl;
    }
    for(int i = 10; i < 15; i++) {
        cout << i << "  ";

        for(int j = 0; j < 15; j++) {
            cout << grid[i][j] << "   ";
        }

        cout << endl;
    }

    cout << endl;


    // Displays all the remaining words onto the screen
    cout << "*********REMAINING WORDS*********" << endl;
    for(int i = 0; i < 10; i++) {
        // Ignores strings in the remaining array that are ""; indicates that it has been found before
        if(remaining[i] != "") {
            cout << remaining[i] << endl;
        }
    }
}

string buildWord(char grid[15][15]) {
    string word = "";
    int x1, y1, x2, y2;
    int xIncrement, yIncrement;

    cout << "What is the starting X Y (separated by a space)?" << endl;
    cin >> x1 >> y1;
    cout << "What is the ending X Y (separated by space)?" << endl;
    cin >> x2 >> y2;


    // Must first validate that all 4 coordinate inputs are in the inclusive range of 0 to 14
    if(inRange(x1) && inRange(y1) && inRange(x2) && inRange(y2)) {

        // Entry is a straight line if either the x or y difference is 0
        // For clarity, x difference just means x2 - x1
        if(x2 - x1 == 0 || y2 - y1 == 0) {

            // First step is to determine how the x and y values are going to change as we build the word
            //    - If x stays the same: xIncrement = 0 because the x value doesn't increment (or change)
            //    - If x difference is positive: xIncrement is 1 (x increases by 1 by each iteration)
            //    - If x difference is negative: xIncrement is -1 (x decreases by 1 by each iteration)
            if(x2 - x1 == 0) {
                xIncrement = 0;
            } else if(x2 - x1 > 0) {
                xIncrement = 1;
            } else {
                xIncrement = -1;
            }

            // Same rules apply to y
            if(y2 - y1 == 0) {
                yIncrement = 0;
            } else if(y2 - y1 > 0) {
                yIncrement = 1;
            } else {
                yIncrement = -1;
            }


            // This segment deals with actually constructing the word
            int numLetters;
            // If x is the one changing, then it should be used to find the number of letters
            if(abs(x2 - x1) > 0) {
                numLetters = abs(x2 - x1) + 1;
            }
            // If y is the one changing, then it should be used to find the number of letters
            else {
                numLetters = abs(y2 - y1) + 1;
            }
            char wordBuild[numLetters];

            // Builds the word letter by letter from the grid
            for(int i = 0; i < numLetters; i++) {
                // Don't worry too much about this math... it just works ;_;
                wordBuild[i] = grid[x1 + (i * xIncrement)][y1 + (i * yIncrement)];
            }

            for(char letter : wordBuild) {
                word += letter;
            }
        }

        // Entry is a diagonal line if the absolute value of either the x or y difference is greater than 0
        else {

            // A valid diagonal input is when the absolute value of both x & y difference is equal
            // Translated In Code Form: abs(x2 - x1) == abs(y2 - y1)
            if(abs(x2 - x1) == abs(y2 - y1)) {
                // Next step is to determine the x & y increment (i.e. whether the word constructed is forwards or backwards)

                // If x2 - x1 is positive, that tells us that we read from left to right
                if(x2 - x1 > 0) {
                    xIncrement = 1;
                }
                // Otherwise, read the word from right to left
                else {
                    xIncrement = -1;
                }

                // Same logic for y

                // If y2 - y1 is positive, read the word from top to bottom
                if(y2 - y1 > 0) {
                    yIncrement = 1;
                } else {
                    yIncrement = -1;
                }


                // This segment deals with actually constructing the word
                int numLetters = abs(x2 - x1) + 1;
                char wordBuild[numLetters];

                // Builds the word letter by letter from the grid
                for(int i = 0; i < numLetters; i++) {
                    // Don't worry too much about this math... it just works ;_;
                    wordBuild[i] = grid[x1 + (i * xIncrement)][y1 + (i * yIncrement)];
                }

                for(char letter : wordBuild) {
                    word += letter;
                }
            }

            // Otherwise the attempt at a diagonal input is invalid (i.e. the attempt is not a diagonal)
            else {
                cout << "Invalid Input - Failed to create a diagonal" << endl;
            }
        }
    } else {
        cout << "Invalid Input - That point is not in the grid" << endl;
    }

    return word;
}

bool checkWord(string word, string remaining[10]) {
   bool found = false;

   // Checks each item in remaining to see if it matches word
   // If it finds a match, then found will be true
   for(int i = 0; i < 10; i++) {
      if(word == remaining[i]) {
         found = true;
         remaining[i] = "";
      }
   }

   if(found == true) {
      cout << "You found the word " << word << "!" << endl;
   } else {
      cout << "You didn't find a word on the list" << endl;
   }

   return found;
}

bool inRange(int coord) {
   if(0 <= coord && coord <= 14) {
      return true;
   } else {
      return false;
   }
}

bool puzzleFinished(string remaining[10]) {
   int numFound = 0;

   // Increments numFound everytime an item in remaining is "", again implying that the word was found earlier
   for(int i = 0; i < 10; i++) {
      if(remaining[i] == "") {
         numFound++;
      }
   }

   // Only returns true if numFound equals 10
   if(numFound == 10) {
      return true;
   } else {
      return false;
   }
}


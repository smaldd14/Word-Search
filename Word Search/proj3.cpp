/* Name: Devin Smaldore
 * Date: 10-7-15
 * Class: csc1720
 * Project Description: The goal is create a program that resembles that of a word search game that could be found at wonderword.com
 * 	User will enter the word number they wish to find along with the start row, start column and end row and end column
 * 	Program will check to see which way the word is going, then print out whether user got the guess correct or not.
 * 
 * To Compile:
 * 	g++ -Wall proj3.cpp
 * To Run:
 * 	./a.out
 * 	The Command Line will prompt you for the rest (testdata.txt)
 */



#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <iomanip>


const char EOL = '\n';  // end of line character

using namespace std;


//global structs
struct wordSearchType {
        char **wordGrid;
        int rows;
        int columns;
        int numOfWords;
        string *wordsToSolve;
};

struct wordToFindType {
        int usersWord;
        int startRow;
        int startCol;
        int endRow;
        int endCol;
   };

//global string that has same components of wordsToSolve, but everything is lowcased
string *lowcasedCorrectWord;

//named prototypes
char** loadWordSearchGrid(ifstream &inStream, int &rows, int &columns);
string* loadWordSearchWords(ifstream &inStream, string *solvedWords, int &numOfWords);
void displayWordSearch(const wordSearchType& wordSearch);
void solveWordSearch(wordSearchType& wordSearch, wordToFindType& wordToFind, int wordsGuessed);
bool horizontal(wordToFindType& wordToFind, const wordSearchType& wordSearch, const string *lowcasedCorrectWord);
bool vertical(wordToFindType& wordToFind, const wordSearchType& wordSearch, const string *lowcasedCorrectWord);
bool diagonalUp(wordToFindType& wordToFind, const wordSearchType& wordSearch, const string *lowcasedCorrectWord);
bool diagonalDown(wordToFindType& wordToFind, const wordSearchType& wordSearch, const string *lowcasedCorrectWord);
bool checkIfInputIsRight(wordToFindType& wordToFind, const wordSearchType& wordSearch, const string *lowcasedCorrectWord);
bool checkBoundaries(const wordToFindType& wordToFind, const wordSearchType& wordSearch);

int main()
{
   ifstream inputFile;


//need to add a part of the code where if user doesnt enter a file name, i use a default file name

//variable for input and output file streams
   string infileName;
   string defaultFile = "testdata.txt";

   cout << "Enter file from which we will read from: ";
   getline(cin, infileName);
   //open files for reading
   if(infileName == "")
      infileName = defaultFile;
   // files assumed to be in same directory as running program
   cout << "Opening file for reading..." << endl << endl;
   inputFile.open(infileName.c_str());
   
   // verify if the file actually opened, if not prompt again
   while(!inputFile.is_open()){
      cerr << "Unable to open input file: " << infileName << "please enter a valid file name: ";
      getline(cin, infileName);
      if(infileName == "")
         infileName = defaultFile;
      cout << "Opening file for reading..." << endl << endl;
      inputFile.open(infileName.c_str());
   }
  
   //struct used for the puzzle solving process 
   wordSearchType wordSearch; 
   //struct for users input
   wordToFindType wordToFind;
  
   
   wordSearch.wordGrid = loadWordSearchGrid(inputFile, wordSearch.rows, wordSearch.columns); //function that loads in grid diameters and grid itself  
   wordSearch.wordsToSolve = loadWordSearchWords(inputFile, wordSearch.wordsToSolve, wordSearch.numOfWords); //function that loads in amount of words and words themselves
   //displayWordSearch(wordSearch); //displays the data exactly how it looks in the file


   //counter that gets incremented every time the user correctly guesses a word.. Once all words have been guessed, user wins!
   int wordsGuessed = 0;
   
   solveWordSearch(wordSearch, wordToFind, wordsGuessed); //prints out grid with coordinates and prompts user to enter guess
   //while loop that runs as long as the number of words guess is less than the number of words in the puzzle.    
   while(wordsGuessed<wordSearch.numOfWords){
      //if/else if statements that check to see if each "directional" function is true
      if(horizontal(wordToFind, wordSearch, lowcasedCorrectWord)){
         wordsGuessed++;
         system ("clear"); //clears screen so the output looks cleaner
         cout << endl << "Congrats! you matched " << wordSearch.wordsToSolve[wordToFind.usersWord] << "." << endl;
         wordSearch.wordsToSolve[wordToFind.usersWord] = "***"; //changes the solved word to "***" so it will be ignored when the word lst is printed out
         solveWordSearch(wordSearch, wordToFind, wordsGuessed);
      }
      else if(vertical(wordToFind, wordSearch, lowcasedCorrectWord)){
         wordsGuessed++;
         system ("clear"); //clears screen so the output looks cleaner
         cout << endl << "Congrats! you matched " << wordSearch.wordsToSolve[wordToFind.usersWord] << "." << endl;
         wordSearch.wordsToSolve[wordToFind.usersWord] = "***"; //changes the solved word to "***" so it will be ignored when the word lst is printed out
         solveWordSearch(wordSearch, wordToFind, wordsGuessed);
      }
      else if(diagonalUp(wordToFind, wordSearch, lowcasedCorrectWord)){
         wordsGuessed++;
         system ("clear"); //clears screen so the output looks cleaner
         cout << endl << "Congrats! you matched " << wordSearch.wordsToSolve[wordToFind.usersWord] << "." << endl; 
         wordSearch.wordsToSolve[wordToFind.usersWord] = "***"; //changes the solved word to "***" so it will be ignored when the word lst is printed out
         solveWordSearch(wordSearch, wordToFind, wordsGuessed);
      }
      else if(diagonalDown(wordToFind, wordSearch, lowcasedCorrectWord)){
         wordsGuessed++;
         system ("clear"); //clears screen so the output looks cleaner
         cout << endl << "Congrats! you matched " << wordSearch.wordsToSolve[wordToFind.usersWord] << "." << endl;
         wordSearch.wordsToSolve[wordToFind.usersWord] = "***"; //changes the solved word to "***" so it will be ignored when the word lst is printed out
         solveWordSearch(wordSearch, wordToFind, wordsGuessed);
      }
      else{
         //if the user did not enter a correct coordinate, they will be prompted to enter again
         cout << "Your input was incorrect, ";
         cout << "please enter the word and its start and end coordinates within the grid, row column row column. i.e. 1 8 6 12 10. ";
         cout << "If you wish to quit, type in 0 five times... 0 0 0 0 0: " << endl;
         cin >> wordToFind.usersWord >> wordToFind.startRow >> wordToFind.startCol >> wordToFind.endRow >> wordToFind.endCol;
         if(wordToFind.usersWord == 0 && wordToFind.startRow == 0 && wordToFind.startCol == 0 && wordToFind.endRow == 0 && wordToFind.endCol == 0){
            cout << "Uh Oh! I'm sorry you wanted to quit. Better luck next time!" << endl;
            exit(1);
         }
         wordToFind.usersWord--;
         wordToFind.startRow--;
         wordToFind.startCol--;
         wordToFind.endRow--;
         wordToFind.endCol--;
      }
   }

    //close the input file so we do not prevent another individual from using file
   inputFile.close();
   return 0;
}


/* 
	function that takes in the 2D array grid, the rows and columns in the grid,
	the number of words to be solved, and the words to be solved.
	Prints out exactly what was in the file to be loaded.
*/
void displayWordSearch(const wordSearchType& wordSearch)
{
   //prints the number of rows and columns in grid
   cout << wordSearch.rows << " " << wordSearch.columns << endl << endl;

  
  //loop to print out the grid itself
   for(int j=0; j<wordSearch.rows; j++){
      for(int k = 0; k< wordSearch.columns; k++){   
         cout << wordSearch.wordGrid[j][k]; //prints letters to the screen to form grid
         cout << " ";
      }
        cout << endl;
   }
   cout << endl;
   //prints the number of words in the word list
   cout << wordSearch.numOfWords << endl;
   int x = 0;
   //loop to print out the words to be solved
   for(int i = 0; i<wordSearch.numOfWords; i++){
      x++;
      cout << wordSearch.wordsToSolve[i] << " ";
      cout << endl;
   }
   cout << endl;
}

/* Loads the data for the word search grid from the given already opened inFileStream.
        Data will be loaded into dynamically allocated  array.  inFileStream will remain open
        on exiting the function.
        
        INPUTS:
        ------
        inStream - reference to already open stream to read from
	rows - called by reference in main, amount of rows in the grid
	columns - called by reference in main, amount of columns in the grid
*/
char** loadWordSearchGrid(ifstream &inStream, int &rows, int &columns)
{
   char** grid;
   //grabs first number in file which is rows 
   inStream >> rows;
   //grabs second number which is columns
   inStream >> columns;

   while(inStream.peek() == EOL)
      inStream.ignore(1,'\n'); //ignores newline character at the end of each input line

   //creates rows of wordGrid
   grid = new char* [rows];
   //creates columns of wordGrid
   for(int row = 0; row<rows; row++)
      grid[row] = new char[columns];
   //loop to fill in letters in each position
   for(int i = 0; i<rows; i++){
      int j = 0; //variable for inner, faster varying array
      while(inStream.peek() != EOL){
         inStream >> grid[i][j];
         grid[i][j] = tolower(grid[i][j]);
         j++;
      }
      while(inStream.peek() == EOL)
         inStream.ignore(1,'\n'); //ignores newline character at the end of each input line
   }
   
   return grid;
}

/* Loads the data for the word search grid from the given already opened inFileStream.
        Data will be loaded into dynamically allocated  array.  inFileStream will remain open
        on exiting the function.
        
        INPUTS:
        ------
        inStream - reference to already open stream to read from
	solvedWords - string array that is dynamically allocated that loads in each word to be solved
	numOfWords - called by reference which is the number of words that need to by solved
*/
string* loadWordSearchWords(ifstream &inStream, string *solvedWords, int &numOfWords)  
{  
   //read in number of words to solve
   inStream >> numOfWords;
   solvedWords = new string[numOfWords];
   //loop to read in each word that needs to be solved
   for( int k = 0; k<numOfWords; k++)
      inStream >> solvedWords[k];

   return solvedWords;
}

/* Prints the data that was loaded in in the previous two functions.
 * INPUTS: reference to struct wordSearchType
	   reference to struct WordToFindType
 * 	User is prompted to type in their guess, and if the guess is outside of the "boundaries",
 * 	the user is prompted again to enter something inside the boundaries.
 */

void solveWordSearch(wordSearchType& wordSearch, wordToFindType& wordToFind, int wordsGuessed)
{
   
   // sets column numbers to right above first letter in grid
   cout  << setw(4);
   //loop to print out the numbers of the columns in the word search
   for(int i = 1; i<wordSearch.rows+1; i++){
     cout << i%10 << " ";
   }
   cout << endl;
   
   //sets dashes to right above first letter in grid
   cout << setw(4);
   //loop to print out a "border" on the top of the word search
   for(int i = 0; i<wordSearch.rows; i++){
     cout << "-" << " ";
   }
   cout << endl;




  //loop to print out the grid itself
   for(int j=0; j<wordSearch.rows; j++){
      //if/else statement to make sure rows numbers are printed on left side
      if(j>=9)
         cout << j+1 << setw(2);
      else
         cout << j+1 << setw(3);
      for(int k = 0; k< wordSearch.columns; k++){
             cout << wordSearch.wordGrid[j][k]; //prints letters to the screen to form grid
         cout << " ";
      }
      //if/else statement to make sure rows numbers are printed on right side
      if(j>=9)
         cout << setw(3) << j+1;
      else
         cout << setw(2) << j+1;
      cout << endl;
   }

   //sets dashes to right below first letter in grid
   cout << setw(4);
   //loop to print out a "border" on the bottom of the word search
   for(int i = 0; i<wordSearch.rows; i++){
     cout << "-" << " ";
   }
   cout << endl;

   // sets column numbers to right below first letter in grid
   cout  << setw(4);
   //loop to print out the numbers of the columns in the word search
   for(int i = 1; i<wordSearch.rows+1; i++){
     cout << i%10 << " ";
   }
   cout << endl << endl;
   
   int x = 0; //counter for the number of words on each line
   //loop to print out the words to be solved
   for(int i = 0; i<wordSearch.numOfWords; i++){
      x++;
      if(wordSearch.wordsToSolve[i] != "***")
         cout << i+1 << ") " << wordSearch.wordsToSolve[i] << " ";
      if(x == 5){
         x = 0;
         cout << endl;
      }
   }
   cout << endl << endl;
   
   //creates another array with the same words as the words to solve but all lowercased
   lowcasedCorrectWord = new string[wordSearch.numOfWords];
   for(int k = 0; k<wordSearch.numOfWords; k++){
      lowcasedCorrectWord[k] = wordSearch.wordsToSolve[k];   
      for(int y = 0; y<(int)lowcasedCorrectWord[k].length(); y++){   
         lowcasedCorrectWord[k][y] = tolower(lowcasedCorrectWord[k][y]);
      }
   }
   
   //checks to see if all words have been solved
   if(wordsGuessed == wordSearch.numOfWords){
      cout << "YOU DID IT! YOU'RE THE BEST! PLEASE FEEL FREE TO PLAY AGAIN!" << endl;
      exit(1);
   }   
   //prompts user for word and its starting and ending coordinates
   cout << "Enter the word number and its start and end coordinates, row column row column. i.e. 1 8 6 12 10. " << endl;
   cout << "If you wish to quit, type in 0 five times... 0 0 0 0 0: " << endl;
   cin >> wordToFind.usersWord >> wordToFind.startRow >> wordToFind.startCol >> wordToFind.endRow >> wordToFind.endCol;
   if(wordToFind.usersWord == 0 && wordToFind.startRow == 0 && wordToFind.startCol == 0 && wordToFind.endRow == 0 && wordToFind.endCol == 0){
      cout << "Uh Oh! I'm sorry you wanted to quit. Better luck next time!" << endl;
      exit(1);
   }
   //subtracts one from all of the inputs because what the user puts in is actually one less then that.
   wordToFind.usersWord--;
   wordToFind.startRow--;
   wordToFind.startCol--;
   wordToFind.endRow--;
   wordToFind.endCol--;
  

   //while loops that run while the users input is outside of the stated boundaries.  
   while(!checkBoundaries(wordToFind, wordSearch)){
      cout << "Your input is out of the boundaries," << endl;
      //prompts user for word and its starting and ending coordinates
      cout << "enter the word and its start and end coordinates within boundaries, row column row column. i.e. Apple 8 6 12 10, ";
      cout << "or enter 0 0 0 0 0 to quit: " << endl;
      cin >> wordToFind.usersWord >> wordToFind.startRow >> wordToFind.startCol >> wordToFind.endRow >> wordToFind.endCol;
      if(wordToFind.usersWord == 0 && wordToFind.startRow == 0 && wordToFind.startCol == 0 && wordToFind.endRow == 0 && wordToFind.endCol == 0){
         cout << "Uh Oh! I'm sorry you wanted to quit. Better luck next time!" << endl;
         exit(1);
      }
      wordToFind.usersWord--;
      wordToFind.startRow--;
      wordToFind.startCol--;
      wordToFind.endRow--;
      wordToFind.endCol--;   
   }
}


//function that takes two structs and checks to see if the user entered word equals the newly made string from left to right
bool horizontal(wordToFindType& wordToFind, const wordSearchType& wordSearch, const string *lowcasedCorrectWord)
{
   //variable that will be incremented so the loop can go through the word that needs to be found
   int i = wordToFind.startCol;

   string wordBetweenPositions; //new string that will be compared with the word the user entered
   
   if(wordToFind.startCol<wordToFind.endCol && wordToFind.startRow == wordToFind.endRow){ 
      while(i<=wordToFind.endCol){
         wordSearch.wordGrid[wordToFind.startRow][i] = tolower(wordSearch.wordGrid[wordToFind.startRow][i]);
         wordBetweenPositions += wordSearch.wordGrid[wordToFind.startRow][i]; // goes thru grid and appends letters to this new string
         i++;
      }
   }
   else if(wordToFind.startCol>wordToFind.endCol && wordToFind.startRow == wordToFind.endRow){
      while(i>=wordToFind.endCol){
         wordSearch.wordGrid[wordToFind.startRow][i] = tolower(wordSearch.wordGrid[wordToFind.startRow][i]);
         wordBetweenPositions += wordSearch.wordGrid[wordToFind.startRow][i];
         i--;
      }
   }
   // initialize j counter
   int j = wordToFind.startCol;
   if(wordBetweenPositions == lowcasedCorrectWord[wordToFind.usersWord]){  //if the word that the grid created equals the correct word guess, its a match
      if(j<wordToFind.endCol){
         while(j<=wordToFind.endCol){
            wordSearch.wordGrid[wordToFind.startRow][j] = toupper(wordSearch.wordGrid[wordToFind.startRow][j]);
            j++;
         }
         return true;
      }
      else{
         while(j>=wordToFind.endCol){
            wordSearch.wordGrid[wordToFind.startRow][j] = toupper(wordSearch.wordGrid[wordToFind.startRow][j]);
            j--;
         }
         return true;
      }
   }
   return false;
}


//function that takes two structs and checks to see if the user entered word equals the newly made string from bottom to top
bool vertical(wordToFindType& wordToFind, const wordSearchType& wordSearch, const string *lowcasedCorrectWord)
{
   //variable that will be incremented so the loop can go through the word that needs to be found
   int i = wordToFind.startRow;

   string wordBetweenPositions; //new string that will be compared with the word the user entered
   if(wordToFind.startRow>wordToFind.endRow && wordToFind.startCol == wordToFind.endCol){
      while(i>=wordToFind.endRow){
         wordSearch.wordGrid[i][wordToFind.startCol] = tolower(wordSearch.wordGrid[i][wordToFind.startCol]);
         wordBetweenPositions += wordSearch.wordGrid[i][wordToFind.startCol]; 
         i--;
      }
   }

   else if(wordToFind.startRow<wordToFind.endRow && wordToFind.startCol == wordToFind.endCol){
      while(i<=wordToFind.endRow){
         wordSearch.wordGrid[i][wordToFind.startCol] = tolower(wordSearch.wordGrid[i][wordToFind.startCol]);
         wordBetweenPositions += wordSearch.wordGrid[i][wordToFind.startCol];
         i++;
      }
   }
   //reinitialize j
   int j = wordToFind.startRow;
   if(wordBetweenPositions == lowcasedCorrectWord[wordToFind.usersWord]){
      if(j>wordToFind.endRow){
         while(j>=wordToFind.endRow){
            wordSearch.wordGrid[j][wordToFind.startCol] = toupper(wordSearch.wordGrid[j][wordToFind.startCol]);
            j--;
         }
         return true;
      }
      else{
         while(j<=wordToFind.endRow){
            wordSearch.wordGrid[j][wordToFind.startCol] = toupper(wordSearch.wordGrid[j][wordToFind.startCol]);
            j++;
         }
         return true;
      }
   }
   return false;
}



//function that takes two structs and checks to see if the user entered word equals the newly made string going diagonal up and to the right
bool diagonalUp(wordToFindType& wordToFind, const wordSearchType& wordSearch, const string *lowcasedCorrectWord)
{
   //variables that will be incremented so the loop can go through the word that needs to be found
   int i = wordToFind.startRow;
   int j = wordToFind.startCol;

   string wordBetweenPositions; //new string that will be compared with the word the user entered
   //diagonal up and to the right if statement
   if(wordToFind.startRow>wordToFind.endRow && wordToFind.startCol<wordToFind.endCol){
      while(i>=wordToFind.endRow){
         wordSearch.wordGrid[i][j] = tolower(wordSearch.wordGrid[i][j]);
         wordBetweenPositions += wordSearch.wordGrid[i][j];
         i--;
         j++;
      }
   }
   //diagonal up and to the left if statement
   else if(wordToFind.startRow>wordToFind.endRow && wordToFind.startCol>wordToFind.endCol){
      while(i>=wordToFind.endRow){
         wordSearch.wordGrid[i][j] = tolower(wordSearch.wordGrid[i][j]);
         wordBetweenPositions += wordSearch.wordGrid[i][j];
         i--;
         j--;
      }
   }
   
   //reinitialize i,j to increment
   i = wordToFind.startRow;
   j = wordToFind.startCol;
   if(wordBetweenPositions == lowcasedCorrectWord[wordToFind.usersWord]){
      if(i>wordToFind.endRow && j<wordToFind.endCol){
         while(i>=wordToFind.endRow){
            wordSearch.wordGrid[i][j] = toupper(wordSearch.wordGrid[i][j]);
            i--;
            j++;
         }
         return true;
      }
      else{
         while(i>=wordToFind.endRow){
            wordSearch.wordGrid[i][j] = toupper(wordSearch.wordGrid[i][j]);
            i--;
            j--;
         }
         return true;
      }
   }
   return false;
}



//function that takes two structs and checks to see if the user entered word equals the newly made string going diagonal down and to the right
bool diagonalDown(wordToFindType& wordToFind, const wordSearchType& wordSearch, const string *lowcasedCorrectWord)
{
   //variables that will be incremented so the loop can go through the word that needs to be found
   int i = wordToFind.startRow;
   int j = wordToFind.startCol;

   string wordBetweenPositions; //new string that will be compared with the word the user entered
   //diagonal down and to the right if statement
   if(wordToFind.startRow<wordToFind.endRow && wordToFind.startCol<wordToFind.endCol){
      while(i<=wordToFind.endRow){
         wordSearch.wordGrid[i][j] = tolower(wordSearch.wordGrid[i][j]);
         wordBetweenPositions += wordSearch.wordGrid[i][j];
         i++;
         j++;
      }
   }
   //diagonal down and to the left if statement
   else if(wordToFind.startRow<wordToFind.endRow && wordToFind.startCol>wordToFind.endCol){
      while(i<=wordToFind.endRow){
         wordSearch.wordGrid[i][j] = tolower(wordSearch.wordGrid[i][j]);
         wordBetweenPositions += wordSearch.wordGrid[i][j];
         i++;
         j--;
      }
   }
   // reinitialize i,j to increment
   i = wordToFind.startRow;
   j = wordToFind.startCol;
   if(wordBetweenPositions == lowcasedCorrectWord[wordToFind.usersWord]){
      if(i<wordToFind.endRow && j<wordToFind.endCol){
         while(i<=wordToFind.endRow){
            wordSearch.wordGrid[i][j] = toupper(wordSearch.wordGrid[i][j]);
            i++;
            j++;
         }
         return true;
      }
      else{
         while(i<=wordToFind.endRow){
            wordSearch.wordGrid[i][j] = toupper(wordSearch.wordGrid[i][j]);
            i++;
            j--;
         }
         return true;
      }
   }
   return false;
}


//function that looks at the users input and compares it with what the files says, if the input is out of the said boundaries, the user must enter input again
bool checkBoundaries(const wordToFindType& wordToFind, const wordSearchType& wordSearch)
{

   if(wordToFind.usersWord >= wordSearch.numOfWords || wordToFind.usersWord < 0)
      return false;
   else if(wordToFind.startRow >= wordSearch.rows || wordToFind.startRow < 0)
      return false;
   else if(wordToFind.endRow >= wordSearch.rows || wordToFind.endRow < 0)
      return false;
   else if(wordToFind.startCol >= wordSearch.columns || wordToFind.startCol < 0)
      return false;
   else if(wordToFind.endCol >= wordSearch.columns || wordToFind.endCol < 0)
      return false;
   
   return true;
}

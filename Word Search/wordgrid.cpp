void loadWordSearchData(ifstream &inStream, char** grid, int &rows, int &columns, int &numOfWords, string *solvedWords)
{
   
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

   //read in number of words to solve
   inStream >> numOfWords;
   solvedWords = new string[numOfWords];
   //loop to read in each word that needs to be solved
   for( int k = 0; k<numOfWords; k++)
      inStream >> solvedWords[k];
   cout << solvedWords << endl;
}



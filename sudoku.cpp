/*
For the grid function, I am printing out which grid the errors have been located. Starting with grid 1 in the top left and counting to the right.

grid 1    grid 2    grid 3

grid 4    grid 5    grid 6

grid 7    grid 8    grid 9

*/

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <fstream>
using namespace std;

mutex m;
bool columnErrors[9] = {false, false, false, false, false, false, false, false, false};
bool rowErrors[9] = {false, false, false, false, false, false, false, false, false};
bool gridErrors[9] = {false, false, false, false, false, false, false, false, false};

void checkCol(int col[][9],int numInts, int threadId) 
{
  int temp;
  int count;
  int numbers[9];

  //  m.lock();

  for(int i = 0; i < numInts; i++)
    {
      for(int j = 0; j < numInts; j++)
	{
	  count = 0;
	  temp = col[j][i];
	  for(int k = 0; k < numInts; k++)
	    {
	      if(col[k][i] == temp)
		  count++;
	    }
	  
	  if( count >= 2)
	    {
	      m.lock();
	      columnErrors[i + 1] = true;
	      numbers[temp] = i + 1;
	      m.unlock();
	    }
	}
    }

  // m.unlock();
}


void checkRow(int row[][9], int numInts, int threadId)
{
  int temp;
  int count;
  int numbers[9];

  // m.lock();
  for(int i = 0; i < numInts; i++)
    {
      for(int j = 0; j < numInts; j++)
	{
	  count = 0;
	  temp = row[i][j];
	  for(int k = 0; k < numInts; k++)
	    {
	      if(row[i][k] == temp)
		count++;
	    }
	  
	  if(count >= 2)
	    {
	      //	      m.lock();
	      rowErrors[i + 1] = true;
	      numbers[temp] = i + 1;
	      //  m.unlock();
	    }
	}
    }
  // m.unlock();
}

void checkGrid(int grid[][9], int numInts, int threadId, bool *isGridCorrect)
{
  *isGridCorrect = true;
  int count;
  int temp;
  int numbers[9];

  // m.lock();
      
  for(int i = 0; i < numInts; i++)
    {
      for(int j = 0; j < 3; j++) //choose a row
	{
	  for(int k = 0; k < 3; k++) //choose a col
	    {
	      temp = grid[j + (i / 3) * 3][k + (i % 3) * 3];
	      count = 0;
	      for(int m = 0; m < 3; m++) //choose a row to compare
		{
		  for(int n = 0; n < 3; n++)
		    { //choose a col to compare
		      if(grid[m + (i / 3) * 3][n + (i % 3) * 3] == temp)
			count++;
		      if(count > 1)
			{
			  //m.lock();
			  *isGridCorrect = false;
			  gridErrors[i + 1] = true;
			  numbers[temp] = i + 1;
			  // m.unlock();
			}
		    }
		}
	    }
	}
    }
  
  //  m.unlock();
}

void getInput(int grid[][9], int numInts)
{
  for(int i = 0; i < numInts; i++)
    {
      for (int j = 0; j < numInts; j++)
	{
	  if(!(cin >> grid[i][j]))
	    {
	      cout << "Error while reading the file." << endl;
	      break;
	    }
	  if(!cin)
	    break;
	}
    }
}



int main()
{
  int grid[9][9];
  int numInts = 9;

  int threads = 27;
  thread t[threads];
 
  getInput(grid, numInts);		

  bool isGridCorrect = true;
  
  for(int i = 0; i < threads; i+=3)
   {
      t[i]= thread(checkRow, grid, numInts, i);
      t[i+1] = thread(checkCol, grid, numInts,i + 1);
      t[i+2] = thread(checkGrid, grid, numInts, i+2, &isGridCorrect);
   }
  
 
  for(int i = 0; i < threads; i++)
    t[i].join();

  for(int m = 0; m < numInts; m++)
   {
     if(columnErrors[m])
	cout << "Column " << m  << " doesn't have the required values." << endl;
   }

  for(int k = 0; k < numInts; k++)
    {
      if(rowErrors[k])
	cout << "Row " << k << " doesn't have the required values." << endl;
    }
 
  if(isGridCorrect)
    cout << "This sudoku has correct input" << endl;
  else
    {
      for(int n = 0; n < numInts; n++)
	{
	  if(gridErrors[n])
	    cout << "Grid " << n  << " doesn't have the required values." << endl;
	}
      cout << "The input is not a valid Sudoku." << endl;
    }
  return 0;
}


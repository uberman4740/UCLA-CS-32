//History implementation

#include "History.h"
#include "Pit.h"
#include "globals.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
    m_row = nRows;
    m_col = nCols;
    for (int i = 1; i <= m_row; i++) //Creates a grid of dots
    {
        for (int k = 1; k <= m_col; k++)
        {
            deadSnakes[i][k] = '.';
        }
    }
}

bool History::record(int r, int c)
{
    if ((r >= 0 && r < m_row) && (c >= 0 && c < m_col))   //Sets the deadsnakes to represent A-Z for num. of dead snakes
    {
        if (deadSnakes[r][c] >= 'A' && deadSnakes[r][c] <= 'Z')
            deadSnakes[r][c] += 1;
        else
            deadSnakes[r][c] = 'A';
        return true;
    }
    else
        return false;
}
void History::display() const
{
    clearScreen();      //Clears Screen and then draws the new grid with positions of dead snakes
    for (int r = 1; r <= m_row; r++)
    {
        for (int c = 1; c <= m_col; c++)
            cout << deadSnakes[r][c];
        cout << endl;
    }
    cout << endl;    
}

//
//  mazesueue.cpp
//  Homework 2(q3)
//
//  Created by Jahan Cherian on 4/23/15.
//  Copyright (c) 2015 Jahan Cherian. All rights reserved.
//

#include <iostream>
#include <queue>
using namespace std;

//////////////////////////////-----------FUNCTION PROTOTYPES----------///////////////////////////////

bool pathExists(char maze[][10], int sr, int sc, int er, int ec);
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise.

//////////////////////////////-----------COORDINATE CLASS----------///////////////////////////////

class Coord     //Unchandged from Specifications
{
	public:
		Coord(int rr, int cc): m_r(rr), m_c(cc) {}
		int r() const { return m_r;}
		int c() const { return m_c;}
	private:
		int m_r;
		int m_c;
};

//////////////////////////////-----------PATHEXISTS FUNCTION IMPLEMENTATION----------///////////////////////////////

bool pathExists(char maze[][10], int sr, int sc, int er, int ec)
{
	queue<Coord> coordqueue;        // Create a queue of coordinates
	Coord startpoint(sr,sc);
	coordqueue.push(startpoint);    // Initially enqueue the starting coordinates
	maze[sr][sc] = '@';             // Indicate the change
    
	int row = sr;
	int col = sc;
	
	while(!coordqueue.empty())   // While our queue is not empty
	{
		startpoint = coordqueue.front();
		row = startpoint.r();
		col = startpoint.c();
		coordqueue.pop();
		
		if(row == er && col == ec)
			return true;
        
		if(row != 0 && maze[row-1][col] != 'X' && maze[row-1][col] != '@') //Can move NORTH, by checking if we arent in 0th row, and we havent already visited that spot
		{
			Coord move(row-1, col);
			coordqueue.push(move);      // Enqueue the North Coordinates
			maze[row-1][col] = '@';     // Indicate that we've visited the spot
		}
		if(col != 9 && maze[row][col+1] != 'X' && maze[row][col +1] != '@') //Can move EAST, by checking if we arent in 9th col, and we havent already visited that spot
		{
			Coord move(row, col + 1);
			coordqueue.push(move);      // Enqueue the East Coordinates
			maze[row][col + 1] = '@';   // Indicate that we've visited the spot
		}
		
		if(row != 9 && maze[row + 1][col] != 'X' && maze[row+1][col] != '@') //Can move SOUTH, by checking if we arent in 9th row, and we havent already visited that spot
		{
			Coord move(row+1, col);
			coordqueue.push(move);      // Enqueue the South Coordinates
			maze[row+1][col] = '@';     // Indicate that we've visited the spot
		}
		if(col != 0 && maze[row][col-1] != 'X' && maze[row][col-1] != '@') //Can move WEST, by checking if we arent in 0th col, and we havent already visited that spot
		{
			Coord move(row, col-1);
			coordqueue.push(move);      // Enqueue the West Coordinates
			maze[row][col-1] = '@';     // Indicate that we've visited the spot
		}
	}
	return false;
}

//////////////////////////////-----------MAIN----------///////////////////////////////

int main()  // Test Case
{
    char maze[10][10] = {
        { 'X','X','X','X','X','X','X','X','X','X'},
        { 'X','.','.','.','.','.','.','.','.','X'},
        { 'X','X','.','X','.','X','X','X','X','X'},
        { 'X','.','.','X','.','X','.','.','.','X'},
        { 'X','.','.','X','.','.','.','X','.','X'},
        { 'X','X','X','X','.','X','X','X','.','X'},
        { 'X','.','X','.','.','.','.','X','X','X'},
        { 'X','.','.','X','X','.','X','X','.','X'},
        { 'X','.','.','.','X','.','.','.','.','X'},
        { 'X','X','X','X','X','X','X','X','X','X'}
    };
    
    if (pathExists(maze, 6,4, 1,1))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
    return 0;
}

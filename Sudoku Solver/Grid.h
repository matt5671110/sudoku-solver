#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stack>
#include <cctype>

class Grid
{
public:
	//structs
	struct GuessState
	{
		int grid_state[9][9][9];
		int x, y;
		std::stack<int> possibilities;
	};
	//functions
	Grid();
	~Grid();

	//drawing grid
	void drawGridWithPossibilities();
	void drawGridWithKnowns();
	void resetGrid(bool keep_initial = false);

	//setting values
	void setKnownValue(int x, int y, int value, bool initial = true);

	//guess stuff
	bool isSolved();
	bool isUnsolvable();
	void copyGrid(int(&destination)[9][9][9]) const;
	void setGridState(int(&source)[9][9][9]);

	//Solving Stuff
	bool doAllEliminations();
	bool rowEliminations();
	bool columnEliminations();
	bool houseEliminations();

	bool doAllSingles();
	bool rowSingles();
	bool columnSingles();
	bool houseSingles();

	//save and load test file
	void saveInitialKnowns() const;
	void loadInitialKnowns();

	//validity testing
	bool checkAllDuplicates();
	bool checkRowDuplicates();
	bool checkColumnDuplicates();
	bool checkHouseDuplicates();
private:
	int grid[9][9][9];
	int initial_knowns[9][9];

	int hasKnownValue(int x, int y); //returns 0 if value is unknown or value if it is known
};


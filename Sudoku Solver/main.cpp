//identify OS
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#define SYSTEM_WINDOWS
#endif

#include <fcntl.h>
#include <cctype>
#include <stack>
#include <chrono>
#include <limits>

#include "Grid.h"

#ifdef SYSTEM_WINDOWS
#include <io.h>
#endif

typedef std::chrono::steady_clock::time_point steady_time;


void drawMenu();
char getMenuChar();
void enterKnowns(Grid& grid);

void createGuess(std::stack<Grid::GuessState>& stack, Grid& grid);
void nextGuess(std::stack<Grid::GuessState>& stack, Grid& grid);

//Definitions
int main(int argc, const char* argv[])
{
#ifdef SYSTEM_WINDOWS
	//set console for wide character support
	_setmode(_fileno(stdout), _O_U16TEXT);
#endif

	Grid grid;

	bool flag = true;

	while (flag)
	{
		char selection = getMenuChar();

		if (selection == 'K')
			grid.drawGridWithKnowns();
		else if (selection == 'P')
			grid.drawGridWithPossibilities();
		else if (selection == 'E')
			enterKnowns(grid);
		else if (selection == 'S')
		{
			if (!grid.checkAllDuplicates())
			{
				steady_time begin = std::chrono::steady_clock::now();

				std::stack<Grid::GuessState> guess_stack;

				int num_guesses = 0;
				int num_iterations = 0;

				do
				{

					bool changes_made = false;
					do
					{
						bool elim, singles;
						elim = grid.doAllEliminations();
						singles = grid.doAllSingles();

						changes_made = elim || singles;

						num_iterations++;
					} while (changes_made);

					if (!grid.isSolved())
					{
						if (grid.isUnsolvable())
						{
							nextGuess(guess_stack, grid);
						}
						else
						{
							//stack isn't empty, grid still isn't solved, push on another guess
							createGuess(guess_stack, grid);
							nextGuess(guess_stack, grid);
							num_guesses++;
						}
					}

				} while (!grid.isSolved());

				steady_time end = std::chrono::steady_clock::now();

				std::wcout << std::endl;

				grid.drawGridWithKnowns();

				std::wcout << std::endl << "Piece of Cake!" << std::endl;
				std::wcout << "Num Iterations: " << num_iterations << std::endl;
				std::wcout << "Num Guesses: " << num_guesses << std::endl;
				std::wcout << "Execution Time (sec): " <<
					std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0 << std::endl << std::endl;
			}
			else
			{
				std::wcout << "Invalid Puzzle!" << std::endl;
			}
		}
		else if (selection == 'R')
		{
			grid.resetGrid();
			std::wcout << "Grid Reset" << std::endl;
		}
		else if (selection == 'T')
		{
			grid.resetGrid(true);
			std::wcout << "Grid Reset With User-Set Values Intact" << std::endl;
		}
		else if (selection == 'W')
			grid.saveInitialKnowns();
		else if (selection == 'L')
			grid.loadInitialKnowns();
		else
			flag = false;
	}

	return 0;
}

void drawMenu()
{
	std::wcout << "Sudoku Solver:" << std::endl;
	std::wcout << "\tK - Draw Grid With Known Values" << std::endl;
	std::wcout << "\tP - Draw Grid With Possible Values" << std::endl;
	std::wcout << "\tE - Enter Known Values Into Grid" << std::endl;
	std::wcout << "\tS - Solve Puzzle" << std::endl;
	std::wcout << "\tR - Reset Grid" << std::endl;
	std::wcout << "\tT - Reset Grid Leaving User-Set Known Values" << std::endl;
	std::wcout << "\tW - Write User-Set Known Values To File" << std::endl;
	std::wcout << "\tL - Load User-Set Known Values From File" << std::endl;
	std::wcout << "\tQ - Quit" << std::endl;
	std::wcout << std::endl << "Input: ";
}

char getMenuChar()
{
	bool flag = true;
	char menu_char = ' ';
	while (flag)
	{
		drawMenu();
		std::string input;
		std::getline(std::cin, input);
		if (input.length() > 0)
			menu_char = toupper(input.at(0));
		else
		{
			std::wcout << "Invalid Input!" << std::endl;
			continue;
		}

		if (menu_char == 'K' || menu_char == 'P' || menu_char == 'E' ||
			menu_char == 'S' || menu_char == 'R' || menu_char == 'T' || 
			menu_char == 'W' || menu_char == 'L' ||menu_char == 'Q')
		{
			flag = false;
		}
		else
		{
			std::wcout << std::endl << "Invalid Input!" << std::endl;
		}
	}
	return menu_char;
}

void enterKnowns(Grid& grid)
{
	std::wcout << "Enter known values in the format 'x y value' or enter 0 0 0 to stop." << std::endl;
	std::wcout << "Enter 0 as value to clear a cell. ex(5 5 0)" << std::endl;
	
	bool flag = true;
	while (flag)
	{
		int x, y, value;
		std::wcout << "Enter Value (x y value): ";
		
		std::cin >> x >> y >> value;

		if (std::cin.fail())
		{
			std::wcout << "Invalid Input!" << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}

		if (x == 0 && y == 0 && value == 0)
			flag = false;
		else
		{
			if (x < 1 || x > 9)
			{
				std::wcout << "X coordinate invalid!" << std::endl;
				continue;
			}
			if (y < 1 || y > 9)
			{
				std::wcout << "Y coordinate invalid!" << std::endl;
				continue;
			}
			if (value < 0 || value > 9)
			{
				std::wcout << "Value is invalid!" << std::endl;
			}
			grid.setKnownValue(x, y, value);
		}
	}

	//flush input buffer before returning to menu
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void createGuess(std::stack<Grid::GuessState>& stack, Grid& grid)
{
	//save state of grid
	Grid::GuessState state;
	grid.copyGrid(state.grid_state);

	//find cell with lowest number of possibilities
	int lowest_possibilities = 9;
	state.x = 0;
	state.y = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			int num_possibilities = 0;
			for (int k = 0; k < 9; k++)
			{
				if (state.grid_state[i][j][k] != 0)
					num_possibilities++;
			}

			if (num_possibilities > 1 && num_possibilities < lowest_possibilities)
			{
				state.x = i;
				state.y = j;
				lowest_possibilities = num_possibilities;
			}
		}
	}

	//push possibilities for that cell onto possibility stack
	for (int k = 0; k < 9; k++)
	{
		if (state.grid_state[state.x][state.y][k] != 0)
			state.possibilities.push(state.grid_state[state.x][state.y][k]);
	}

	//push new guess onto stack
	stack.push(state);
}

void nextGuess(std::stack<Grid::GuessState>& stack, Grid& grid)
{
	if (!stack.empty())
	{
		Grid::GuessState& state = stack.top();
		grid.setGridState(state.grid_state);
		if (!state.possibilities.empty())
		{
			grid.setKnownValue(state.x + 1, state.y + 1, state.possibilities.top(), false);
			state.possibilities.pop();
		}
		else
		{
			stack.pop();
			nextGuess(stack, grid);
		}
	}
}
#include "Grid.h"



Grid::Grid()
{
	//Initialize grid
	resetGrid();
}

Grid::~Grid()
{

}

void Grid::drawGridWithPossibilities()
{
	std::wcout << L"╔═══╤═══╤═══╗╔═══╤═══╤═══╗╔═══╤═══╤═══╗" << std::endl;
	for (int j = 0; j < 9; j++)
	{
		std::wcout << L"║";
		for (int i = 0; i < 9; i++)
		{
			for (int k = 0; k < 3; k++)
			{
				int num = grid[i][j][k];
				if (num != 0)
					std::wcout << num;
				else
					std::wcout << " ";
			}
			std::wcout << (((i + 1) % 3 == 0) ? L"║" : L"│");
			if ((i + 1) % 3 == 0 && (i + 1) != 9)
				std::wcout << L"║";
		}
		std::wcout << std::endl;


		std::wcout << L"║";
		for (int i = 0; i < 9; i++)
		{
			for (int k = 3; k < 6; k++)
			{
				int num = grid[i][j][k];
				if (num != 0)
					std::wcout << num;
				else
					std::wcout << " ";
			}
			std::wcout << (((i + 1) % 3 == 0) ? L"║" : L"│");
			if ((i + 1) % 3 == 0 && (i + 1) != 9)
				std::wcout << L"║";
		}
		std::wcout << std::endl;

		std::wcout << L"║";
		for (int i = 0; i < 9; i++)
		{
			for (int k = 6; k < 9; k++)
			{
				int num = grid[i][j][k];
				if (num != 0)
					std::wcout << num;
				else
					std::wcout << " ";
			}
			std::wcout << (((i + 1) % 3 == 0) ? L"║" : L"│");
			if ((i + 1) % 3 == 0 && (i + 1) != 9)
				std::wcout << L"║";
		}
		std::wcout << std::endl;

		if ((j + 1) % 3 != 0)
			std::wcout << L"╟───┼───┼───╢╟───┼───┼───╢╟───┼───┼───╢" << std::endl;
		if ((j + 1) % 3 == 0)
		{
			std::wcout << L"╚═══╧═══╧═══╝╚═══╧═══╧═══╝╚═══╧═══╧═══╝" << std::endl;
			if ((j + 1) != 9)
				std::wcout << L"╔═══╤═══╤═══╗╔═══╤═══╤═══╗╔═══╤═══╤═══╗" << std::endl;
		}
	}
}

void Grid::drawGridWithKnowns()
{
	std::wcout << L"┌─┬─┬─┐┌─┬─┬─┐┌─┬─┬─┐" << std::endl;
	for (int j = 0; j < 9; j++)
	{
		std::wcout << L"│";
		for (int i = 0; i < 9; i++)
		{
			int num_possible = 0;
			int value = 0;
			for (int k = 0; k < 9; k++)
			{
				if (grid[i][j][k] != 0)
				{
					num_possible++;
					value = grid[i][j][k];
				}
			}
			if (num_possible == 1)
				std::wcout << value;
			else
				std::wcout << " ";
			std::wcout << L"│";
			if ((i + 1) % 3 == 0 && (i + 1) != 9)
				std::wcout << L"│";
		}
		std::wcout << std::endl;

		if ((j + 1) % 3 == 0)
		{
			std::wcout << L"└─┴─┴─┘└─┴─┴─┘└─┴─┴─┘" << std::endl;
			if (j + 1 != 9)
				std::wcout << L"┌─┬─┬─┐┌─┬─┬─┐┌─┬─┬─┐" << std::endl;
		}
		else
		{
			std::wcout << L"├─┼─┼─┤├─┼─┼─┤├─┼─┼─┤" << std::endl;
		}
	}
}

void Grid::resetGrid(bool keep_initial)
{
	for (int i = 0; i < 9; i++)
	{
	for (int j = 0; j < 9; j++)
	{
		for (int k = 0; k < 9; k++)
		{
			grid[i][j][k] = k + 1;
		}
		if (!keep_initial)
			initial_knowns[i][j] = 0;
		else if (initial_knowns[i][j] != 0)
			setKnownValue(i + 1, j + 1, initial_knowns[i][j]);
	}
	}
}

void Grid::setKnownValue(int x, int y, int value, bool initial)
{
	if (value == 0)
	{
		for (int k = 0; k < 9; k++)
		{
			grid[x-1][y-1][k] = k + 1;
		}
	}
	else
	{
		for (int i = 0; i < 9; i++)
		{
			if (i == value - 1)
			{
				grid[x - 1][y - 1][i] = value;
			}
			else
			{
				grid[x - 1][y - 1][i] = 0;
			}
		}
	}

	if(initial)
		initial_knowns[x - 1][y - 1] = value;
}

bool Grid::isSolved()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (hasKnownValue(i, j) == 0)
				return false;
		}
	}
	return true;
}

bool Grid::isUnsolvable()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			int num_possibilities = 0;
			for (int k = 0; k < 9; k++)
			{
				if (grid[i][j][k] != 0)
				{
					num_possibilities++;
				}
			}

			if (num_possibilities == 0)
				return true;
		}
	}
	return false;
}

void Grid::copyGrid(int(&destination)[9][9][9]) const
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			for (int k = 0; k < 9; k++)
			{
				destination[i][j][k] = grid[i][j][k];
			}
		}
	}
}

void Grid::setGridState(int(&source)[9][9][9])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			for (int k = 0; k < 9; k++)
			{
				grid[i][j][k] = source[i][j][k];
			}
		}
	}
}

bool Grid::doAllEliminations()
{
	bool row, column, house;
	row = rowEliminations();
	column = columnEliminations();
	house = houseEliminations();

	return row || column || house;
}

bool Grid::rowEliminations()
{
	bool changes_made = false;

	//for each row
	for (int j = 0; j < 9; j++)
	{
		//look for a known value, and eliminate it as possible for the rest of the row
		for (int i = 0; i < 9; i++)
		{
			int known_value = hasKnownValue(i, j);
			if (known_value != 0)
			{
				//if value is known, go through row again and eliminate it as possible for rest of row;
				for (int k = 0; k < 9; k++)
				{
					if (k != i && grid[k][j][known_value - 1] != 0)
					{
						grid[k][j][known_value - 1] = 0;
						changes_made = true;
					}
				}
			}
		}
	}

	return changes_made;
}

bool Grid::columnEliminations()
{
	bool changes_made = false;

	//for each column
	for (int i = 0; i < 9; i++)
	{
		//look for known value, and eliminate from the rest of the column
		for (int j = 0; j < 9; j++)
		{
			int known_value = hasKnownValue(i, j);
			if (known_value != 0)
			{
				//if value is known go through column again and eliminate it as possible for rest of column
				for (int k = 0; k < 9; k++)
				{
					if (k != j && grid[i][k][known_value - 1] != 0)
					{
						grid[i][k][known_value - 1] = 0;
						changes_made = true;
					}
				}
			}
		}
	}

	return changes_made;
}

bool Grid::houseEliminations()
{
	bool changes_made = false;

	//for each house
	for (int ii = 0; ii < 3; ii++)
	{
		for (int jj = 0; jj < 3; jj++)
		{
			//loop through each square in house
			for (int i = (ii * 3); i < (ii * 3) + 3; i++)
			{
				for (int j = (jj * 3); j < (jj * 3) + 3; j++)
				{
					//look for known values, and eliminate it from the rest of the house
					int known_value = hasKnownValue(i, j);
					if (known_value != 0)
					{
						//if there is a known value, loop through house again and eliminate it as possible for rest of house
						for (int k = (ii * 3); k < (ii * 3) + 3; k++)
						{
							for (int l = (jj * 3); l < (jj * 3) + 3; l++)
							{
								if (k != i || l != j)
								{
									if (grid[k][l][known_value - 1] != 0)
									{
										grid[k][l][known_value - 1] = 0;
										changes_made = true;
									}
								}
							}
							
						}
					}
				}
			}
		}
	}

	return changes_made;
}

bool Grid::doAllSingles()
{
	bool row, column, house;
	row = rowSingles();
	column = columnSingles();
	house = houseSingles();
	return row || column || house;
}

bool Grid::rowSingles()
{
	bool changes_made = false;

	//for each row
	for (int j = 0; j < 9; j++)
	{
		//initialize array
		int num_possible[9];
		for (int i = 0; i < 9; i++)
		{
			num_possible[i] = 0;
		}

		//count the number of possible cells in the row that can be a 1, 2, 3, ..., 9
		for (int i = 0; i < 9; i++)
		{
			for (int k = 0; k < 9; k++)
			{
				if (grid[i][j][k] != 0)
				{
					num_possible[k]++;
				}
			}
		}

		//if the number of possible cells that can be a 1, 2, ..., 9 is exactly one in that row, the cell with that possible value must be that vavlue
		for (int k = 0; k < 9; k++)
		{
			if (num_possible[k] == 1)
			{
				//go back through the row and determine the cell to set
				for (int i = 0; i < 9; i++)
				{
					if (grid[i][j][k] != 0 && hasKnownValue(i,j) == 0)
					{
						setKnownValue(i + 1, j + 1, k + 1, false);
						changes_made = true;
						break;
					}
				}
			}
		}
	}

	if(doAllEliminations()) changes_made = true;
	return changes_made;
}

bool Grid::columnSingles()
{
	bool changes_made = false;

	//for each column
	for (int i = 0; i < 9; i++)
	{
		//initialize array
		int num_possible[9];
		for (int i = 0; i < 9; i++)
		{
			num_possible[i] = 0;
		}

		//count the number of possible cells in the column that can be a 1, 2, 3, ..., 9
		for (int j = 0; j < 9; j++)
		{
			for (int k = 0; k < 9; k++)
			{
				if (grid[i][j][k] != 0)
				{
					num_possible[k]++;
				}
			}
		}


		//if the number of possible cells that can be a 1, 2, ..., 9 is exactly one in that column, the cell with that possible value must be that vavlue
		for (int k = 0; k < 9; k++)
		{
			if (num_possible[k] == 1)
			{
				//go back through the column and determine the cell to set
				for (int j = 0; j < 9; j++)
				{
					if (grid[i][j][k] != 0 && hasKnownValue(i,j) == 0)
					{
						setKnownValue(i + 1, j + 1, k + 1, false);
						changes_made = true;
						break;
					}
				}
			}
		}
	}

	if(doAllEliminations()) changes_made = true;
	return changes_made;
}

bool Grid::houseSingles()
{
	bool changes_made = false;

	//for each house
	for (int ii = 0; ii < 3; ii++)
	{
		for (int jj = 0; jj < 3; jj++)
		{
			//initialize array
			int num_possible[9];
			for (int i = 0; i < 9; i++)
			{
				num_possible[i] = 0;
			}

			//loop through each cell in house and count number of possible 1, 2, 3, ..., 9s in house
			for (int i = (ii * 3); i < (ii * 3) + 3; i++)
			{
				for (int j = (jj * 3); j < (jj * 3) + 3; j++)
				{
					//count number of possible 1, 2, 3, ..., 9s
					for (int k = 0; k < 9; k++)
					{
						if (grid[i][j][k] != 0)
							num_possible[k]++;
					}
				}
			}

			//If the number of possible cells that can be a 1, 2, 3, ..., 9 is exactly 1 that cell must be that possible value
			for (int k = 0; k < 9; k++)
			{
				if (num_possible[k] == 1)
				{
					//go back through each cell in the house and find which value to set
					for (int i = (ii * 3); i < (ii * 3) + 3; i++)
					{
						for (int j = (jj * 3); j < (jj * 3) + 3; j++)
						{
							if (grid[i][j][k] != 0 && hasKnownValue(i,j) == 0)
							{
								setKnownValue(i + 1, j + 1, k + 1, false);
								changes_made = true;
								break;
							}
						}
					}
				}
			}
		}
	}

	if (doAllEliminations()) changes_made = true;
	return changes_made;
}

void Grid::saveInitialKnowns() const
{
	std::string filename;
	std::wcout << "Enter filename to write: ";
	std::getline(std::cin, filename);

	if (filename.length() == 0)
	{
		std::wcout << "No Filename. Nothing written." << std::endl;
		return;
	}

	//test if file exists
	std::ifstream test(filename.c_str());
	if (test.good())
	{
		std::wcout << "File already exists, overwrite? (Y/N): ";
		std::string input;
		std::getline(std::cin, input);
		char choice = toupper(input.at(0));
		if (choice != 'Y')
		{
			std::wcout << "File Not Saved." << std::endl;
			test.close();
			return;
		}
	}
	test.close();

	std::ofstream file(filename.c_str());
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if(initial_knowns[i][j] != 0)
				file << i + 1 << " " << j + 1 << " " << initial_knowns[i][j] << std::endl;
		}
	}
	file.close();
	std::wcout << "File Saved." << std::endl;
}

void Grid::loadInitialKnowns()
{
	std::string filename;
	std::wcout << "Enter filename to load: ";
	std::getline(std::cin, filename);

	if (filename.length() == 0)
	{
		std::wcout << "No Filename. Nothing Loaded." << std::endl;
		return;
	}

	std::ifstream file(filename.c_str());
	if (file.good())
	{
		std::string line;
		while (std::getline(file, line))
		{
			std::istringstream stream(line);
			int x, y, value;
			stream >> x >> y >> value;
			setKnownValue(x, y, value);
		}
	}
	else
	{
		std::wcout << "Couldn't read file!" << std::endl;
		file.close();
		return;
	}
	file.close();

	std::wcout << "File Loaded!" << std::endl;
}

bool Grid::checkAllDuplicates()
{
	return checkRowDuplicates() || checkColumnDuplicates() || checkHouseDuplicates();
}

bool Grid::checkRowDuplicates()
{
	//for each row
	for (int j = 0; j < 9; j++)
	{
		//Initialize array
		int num_knowns[9];
		for (int i = 0; i < 9; i++)
		{
			num_knowns[i] = 0;
		}

		//go through the row and check number of known 1, 2, 3, ..., 9s
		for (int i = 0; i < 9; i++)
		{
			if (hasKnownValue(i, j) != 0)
				num_knowns[hasKnownValue(i, j) - 1]++;
		}

		//if there are multiple known 1, 2, 3, ..., 9s in the row there are duplicates
		for (int i = 0; i < 9; i++)
		{
			if (num_knowns[i] > 1)
				return true;
		}
	}

	return false;
}

bool Grid::checkColumnDuplicates()
{
	//for each column
	for (int i = 0; i < 9; i++)
	{
		//Initialize array
		int num_knowns[9];
		for (int i = 0; i < 9; i++)
		{
			num_knowns[i] = 0;
		}

		//go through the column and check number of known 1, 2, 3, ..., 9s
		for (int j = 0; j < 9; j++)
		{
			if (hasKnownValue(i, j) != 0)
				num_knowns[hasKnownValue(i, j) - 1]++;
		}

		//if there are multiple known 1, 2, 3, ..., 9s in the column there are duplicates
		for (int i = 0; i < 9; i++)
		{
			if (num_knowns[i] > 1)
				return true;
		}
	}

	return false;
}

bool Grid::checkHouseDuplicates()
{
	//for each house
	for (int ii = 0; ii < 3; ii++)
	{
		for (int jj = 0; jj < 3; jj++)
		{
			//Initialize array
			int num_knowns[9];
			for (int i = 0; i < 9; i++)
			{
				num_knowns[i] = 0;
			}

			//go through each cell in house and determine the number of known 1, 2, 3, ..., 9s
			for (int i = (ii * 3); i < (ii * 3) + 3; i++)
			{
				for (int j = (jj * 3); j < (jj * 3) + 3; j++)
				{
					if (hasKnownValue(i, j) != 0)
						num_knowns[hasKnownValue(i, j) - 1]++;
				}
			}

			//if number of known 1, 2, 3, ..., 9s in house is more than 1 there are duplicates
			for (int i = 0; i < 9; i++)
			{
				if (num_knowns[i] > 1)
					return true;
			}
		}
	}

	return false;
}

int Grid::hasKnownValue(int x, int y)
{
	int num_possible = 0;
	int value;
	for (int i = 0; i < 9; i++)
	{
		if (grid[x][y][i] != 0)
		{
			num_possible++;
			value = grid[x][y][i];
		}
	}

	return (num_possible == 1) ? value : 0;
}
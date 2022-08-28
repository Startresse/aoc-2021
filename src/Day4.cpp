#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <array>

#include "Day4.h"

std::vector<int> getFirstLine(std::ifstream &file)
{
	std::vector<int> values;

	std::string line;
	std::getline(file, line);
	std::istringstream ssline(line);

	int value;

	while (ssline >> value)
	{
		values.push_back(value);
		char coma;
		ssline >> coma;
	}

	return values;
}

class ValidatedValue : private std::pair<int, bool>
{
public:
	ValidatedValue() : ValidatedValue(0){};
	ValidatedValue(int x) : ValidatedValue(x, false){};
	ValidatedValue(int x, bool b)
	{
		first = x;
		second = b;
	};
	int value() const { return first; }
	int &value() { return first; }

	bool found() const { return second; }
	bool &found() { return second; }

	friend std::ostream &operator<<(std::ostream &os, const ValidatedValue &value);
};

std::ostream &operator<<(std::ostream &os, const ValidatedValue &value)
{
	os << "(" << value.value() << ", " << value.found() << ") ";
	return os;
}

#define BOARD_LENGTH 5
#define BOARD_WIDTH 5
class Board
{
public:
	Board() { default_constructor(); };
	Board(std::ifstream &file);

	void find(int value);
	bool finished();
	int sum_unfound();

	void display() const;

	operator bool() const
	{
		return board_values[0][0].value() != -1;
	}

private:
	std::array<std::array<ValidatedValue, BOARD_WIDTH>, BOARD_LENGTH> board_values;
	void default_constructor() { board_values[0][0] = -1; }
};

Board::Board(std::ifstream &file)
{
	// skip line
	char endl;
	if (!(file >> endl))
	{
		default_constructor();
		return;
	}

	int value;
	for (unsigned int i = 0; i < BOARD_LENGTH; ++i)
	{
		std::string line;
		std::getline(file, line);
		std::istringstream ssline(line);

		for (unsigned int j = 0; j < BOARD_WIDTH; ++j)
		{
			ssline >> value;
			board_values[i][j] = value;
		}
	}
}

void Board::find(int value)
{
	for (int i = 0; i < BOARD_LENGTH; ++i)
	{
		for (int j = 0; j < BOARD_WIDTH; ++j)
		{
			auto &res = board_values[i][j];
			if (res.value() == value)
				res.found() = true;
		}
	}
}

bool Board::finished()
{
	for (int i = 0; i < BOARD_LENGTH; ++i)
	{
		bool line_OK = true;
		for (int j = 0; j < BOARD_WIDTH; ++j)
		{
			if (!board_values[i][j].found())
			{
				line_OK = false;
				break;
			}
		}
		if (line_OK)
			return true;
	}

	for (int i = 0; i < BOARD_WIDTH; ++i)
	{
		bool column_OK = true;
		for (int j = 0; j < BOARD_LENGTH; ++j)
		{
			if (!board_values[j][i].found())
			{
				column_OK = false;
				break;
			}
		}
		if (column_OK)
			return true;
	}

	if (BOARD_LENGTH == BOARD_WIDTH)
	{
		bool diag_OK = true;
		for (int i = 0; i < BOARD_LENGTH; ++i)
		{
			if (!board_values[i][i].found())
			{
				diag_OK = false;
				break;
			}
		}
		if (diag_OK)
			return true;

		diag_OK = true;
		for (int i = 0; i < BOARD_LENGTH; ++i)
		{
			if (!board_values[i][BOARD_LENGTH - i].found())
			{
				diag_OK = false;
				break;
			}
		}
		if (diag_OK)
			return true;
	}

	return false;
}

int Board::sum_unfound()
{
	int res = 0;
	for (int i = 0; i < BOARD_LENGTH; ++i)
	{
		for (int j = 0; j < BOARD_WIDTH; ++j)
		{
			auto &x = board_values[i][j];
			if (!x.found())
				res += x.value();
		}
	}
	return res;
}

void Board::display() const
{
	for (unsigned int i = 0; i < BOARD_LENGTH; ++i)
	{
		for (unsigned int j = 0; j < BOARD_WIDTH; ++j)
		{
			const auto &res = board_values[i][j];
			std::cout << res << " ";
		}
		std::cout << std::endl;
	}
}

void Day4::run() const
{
	std::ifstream file;
	file.open("ressources/Day4_p1.txt");

	if (!file.is_open())
	{
		throw std::invalid_argument("Couldn't open file");
	}

	std::vector<int> values = getFirstLine(file);

	std::cout << "There are " << values.size() << " values." << std::endl;

	std::vector<Board> boards;
	do
	{
		Board b(file);
		if (!b)
			break;
		boards.push_back(b);
	} while (true);

	std::cout << "There are " << boards.size() << " boards." << std::endl;

	for (int value : values)
	{
		for (Board &b : boards)
		{
			b.find(value);
		}

		bool finished = false;
		for (Board &b : boards)
		{
			if (b.finished())
			{
				finished = true;
				std::cout << "Value " << value << " ended the game.\n";
				std::cout << "Completed Board:\n";
				b.display();
				std::cout << "Res: " << value * b.sum_unfound() << std::endl;
			}
		}
		if (finished)
			break;
	}
}

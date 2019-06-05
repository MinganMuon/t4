// t4 = tttt = tiny tic tac toe

#include <string>
#include <iostream>

int main()
{
	std::cout << "t4 - a tiny tic tac toe program\n";
	std::cout << "-------------------------------\n\n";

	std::cout << "Play as X or O? ";
	std::string choice;
	std::getline(std::cin, choice);
	if (!(choice == "X" || choice == "x" || choice == "O" || choice == "o"))
	{
		std::cout << "Error: choose a character in {X,x,O,o}.\n";
		return 0;
	}

	return 0;
}


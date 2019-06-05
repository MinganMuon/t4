// t4 = tttt = tiny tic tac toe

#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <random>
#include <iostream>

enum class Player {X, O};
enum class GameStatus {X_Won, O_Won, Draw, NotCompleted};
enum class Tile {X, O, Empty};

class State
{
	public:
		State() : board(std::array<Tile,9>()), playertomove(Player::X) { board.fill(Tile::Empty); };

		std::array<Tile,9> getBoard() const { return board; };
		Player getPlayerToMove() const { return playertomove; };

		GameStatus findGameStatus() const;

		// for the current player
		bool makeMove(int tile);
		std::vector<int> getListOfMoves() const;

	private:
		// The board's tiles are enumerated like so:
		// 0 | 1 | 2
		// - + - + -
		// 3 | 4 | 5
		// - + - + -
		// 6 | 7 | 8
		std::array<Tile,9> board;
		Player playertomove;
};

GameStatus State::findGameStatus() const
{
	// check for a win
	// checks for the first winning combination of three --- having more than one winning combination is illogical, shouldn't ever come up, and is thus not considered
	const auto winstates = std::vector<std::vector<int>>{ {0,1,2}, {3,4,5}, {6,7,8}, {0,3,6}, {1,4,7}, {2,5,8}, {0,4,8}, {2,4,6} };
	for (const auto i : winstates)
	{
		if (board[i[0]] != Tile::Empty && board[i[1]] == board[i[0]] && board[i[2]] == board[i[0]])
		{
			return (board[i[0]] == Tile::X) ? GameStatus::X_Won : GameStatus::O_Won;
		}
	}

	// check for a draw
	// important to do this after checking for a win because a player could still win even if the whole board is covered
	auto drawiter = std::find(board.begin(), board.end(), Tile::Empty);
	if (drawiter == board.end())
		return GameStatus::Draw;

	// no end state has been reached
	return GameStatus::NotCompleted;
}

bool State::makeMove(int tile)
{
	if (tile < 0 && tile > 8)
		return false;

	if (board[tile] != Tile::Empty)
		return false;

	if (playertomove == Player::X)
	{
		board[tile] = Tile::X;
		playertomove = Player::O;
	}
	else
	{
		board[tile] = Tile::O;
		playertomove = Player::X;
	}

	return true;
}

std::vector<int> State::getListOfMoves() const
{
	std::vector<int> moves;

	// the list of possile moves is simply the list of empty tiles
	for (int i=0; i < 9; i++)
		if (board[i] == Tile::Empty)
			moves.push_back(i);

	return moves;
}

void drawgame(const State &game)
{
	std::array<Tile,9> board = game.getBoard();
	std::array<std::string,9> boardchar;
	for (int i=0; i < 9; i++)
	{
		switch (board[i]) {
			case Tile::X:
				boardchar[i] = "X";
				break;
			case Tile::O:
				boardchar[i] = "O";
				break;
			case Tile::Empty:
				boardchar[i] = std::to_string(i);
				break;
		}
	}
	std::cout << boardchar[0] << "|" << boardchar[1] << "|" << boardchar[2] << "\n"
		  <<     "-"      << "+" <<     "-"      << "+" <<    "-"       << "\n"
		  << boardchar[3] << "|" << boardchar[4] << "|" << boardchar[5] << "\n"
		  <<     "-"      << "+" <<     "-"      << "+" <<    "-"       << "\n"
		  << boardchar[6] << "|" << boardchar[7] << "|" << boardchar[8] << "\n";
}

int main(int argc, char **argv)
{
	std::cout << "t4 - a tiny tic tac toe program\n";
	std::cout << "-------------------------------\n\n";

	if (argc < 3 || std::string(argv[1]) != "play" || (std::string(argv[2]) != "self" && std::string(argv[2]) != "random-ai" && std::string(argv[2]) != "perfect-ai"))
	{
		std::cout << "Usage: t4 play <opponent> where opponent is \"self\", \"random-ai\", or \"perfect-ai\".\n";
		return 0;
	}

	std::string opponentchoice = std::string(argv[2]);

	Player playerchoice = Player::X;
	if (opponentchoice != "self")
	{
		std::cout << "Play as X or O? ";
		std::string choice;
		std::getline(std::cin, choice);
		if (choice == "X" || choice == "x") {
			playerchoice = Player::X;
		} else if (choice == "O" || choice == "o") {
			playerchoice = Player::O;
		} else {
			std::cout << "Error: choose a character in {X,x,O,o}.\n";
			return 0;
		}
	}

	if (opponentchoice == "perfect-ai")
	{
		std::cout << "perfect-ai not implemented.\n";
		return 0;
	}

	State game = State();

	bool exit = false;
	while (!exit)
	{
		drawgame(game);

		int move = -1;
		if (game.getPlayerToMove() == playerchoice)
		{
			std::cout << "You are player " << (game.getPlayerToMove() == Player::X ? "X" : "O") << ".\n"
				  << "Select a tile to place a mark: ";
			std::string tilechoice;
			std::getline(std::cin, tilechoice);
			move = std::stoi(tilechoice);
		} else {
			if (opponentchoice == "random-ai")
			{
				std::vector<int> moves = game.getListOfMoves();
				std::random_device random_device;
				std::mt19937 engine{random_device()};
				std::uniform_int_distribution<int> dist(0, moves.size() - 1);
				move = moves[dist(engine)];
			}
			/*
			if (opponentchoice == "perfect-ai")
			{
				
			}
			*/
			std::cout << "The " << opponentchoice << " played in tile " << move << ".\n";
		}

		if (!game.makeMove(move))
		{
			std::cout << "You or the AI didn't pick a valid tile!\n";
		} else {
			std::cout << "\n";
			GameStatus gs = game.findGameStatus();
			switch (gs) {
				case GameStatus::NotCompleted:
					break;
				case GameStatus::Draw:
					std::cout << "Draw!\n";
					drawgame(game);
					exit = true;
					break;
				case GameStatus::X_Won:
					std::cout << "Player X won!\n";
					drawgame(game);
					exit = true;
					break;
				case GameStatus::O_Won:
					std::cout << "Player O won!\n";
					drawgame(game);
					exit = true;
					break;
			}
			if (opponentchoice == "self")
				playerchoice = (playerchoice == Player::X) ? Player::O : Player::X;
		}

	}

	return 0;
}


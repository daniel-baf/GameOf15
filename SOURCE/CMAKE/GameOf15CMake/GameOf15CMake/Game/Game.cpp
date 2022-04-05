#include "Game.h"
/// <summary>
/// CONSTRUCTOR
/// </summary>
Game::Game() {
	this->boards = std::vector<OrtogonalMatrix*>(1);
	this->user_nick = "MrMayoneso";
	this->steps = 0;
	this->levels = 1;
	this->current_board = 0;
	this->final_points = 0;
}

/// <summary>
/// DESTRUCTOR
/// </summary>
Game::~Game() {
	// delete boards
	for (int i = 0; i < this->boards.size(); i++)
	{
		// this->boards[i]->~OrtogonalMatrix();
	}
	// delete board array
}

/// <summary>
/// CREATE A NEW GAME, THIS METHOD IS NOT PART OF THE CONSTRUCTOR, SO U CAN CREATE MULTIPLE GAMES FOR SAME OBJECT
/// </summary>
/// <param name="lines">n orotogonal asides</param>
/// <param name="columns">n ortogonal headers</param>
/// <param name="to_fill">the data of each ortogonal node</param>
/// <param name="autofill">create data with rng, or insert custom</param>
void Game::create_game(int lines, int columns, vector<vector<int>> to_fill, bool autofill) {
	// start index = 0;
	int start_index = 0;
	// add dinamically levels
	for (int i = 0; i < this->levels; i++)
	{
		// get array of data
		vector<int> current_data_to_fill;
		if (autofill) {
			current_data_to_fill = create_array(lines, columns, start_index);
			start_index += lines * columns - 1;
		}
		else {
			current_data_to_fill = to_fill[i];
		}
		// save data to new orotognal matrix
		OrtogonalMatrix* board = new OrtogonalMatrix();
		board->set_highest(i + 1); // new max value for formatting, if matrix highest is 88 -> new matrix max length is 88*(i+1)
		board->fill(lines, columns, current_data_to_fill);
		// increases 
		this->boards[i] = board;
	}
}

/// <summary>
///  Create Matrix's data using a random rng, the rng is generated by a computer seed
/// </summary>
/// <param name="cols">columns of matrix</param>
/// <param name="rows">matrix rows</param>
/// <param name="start_index">where to start the random numbers</param>
/// <returns>the vector with the data to be inserted into matrix</returns>
vector<int> Game::create_array(int cols, int rows, int start_index) {
	std::vector<int> intArray(cols * rows);
	// fill array
	for (int i = 0; i < cols * rows - 1; i++)
	{
		intArray[i] = start_index + i + 1;
	}
	cout << endl;
	// shuffle array
	std::random_device rd;
	auto rng = std::default_random_engine(rd());
	std::shuffle(intArray.begin(), intArray.end() - 1, rng); // i always want the last as 0
	return intArray;
};

/// <summary>
/// Changes the values of 2 nodes if there is a valid movement, check is not null and value is = 0
/// </summary>
/// <param name="value"></param>
/// <param name="action"></param>
/// <returns></returns>
bool Game::move_cell(int value, BoardMovements action) {
	OrtogonalNode* to_move = this->boards[this->current_board]->findByValue(value);
	if (to_move != NULL) {
		switch (action)
		{
		case UP:
			return move_node(to_move, to_move->up);
		case DOWN:
			return move_node(to_move, to_move->down);
		case LEFT:
			return move_node(to_move, to_move->left);
		case RIGHT:
			return move_node(to_move, to_move->right);
		default:
			cout << "MOVIMIENTO INVALIDO" << endl;
			return false;
		}
	}
	return false;
};

/// <summary>
/// Swap two nodes info
/// </summary>
/// <param name="origin"></param>
/// <param name="destination"></param>
/// <returns></returns>
bool Game::move_node(OrtogonalNode* origin, OrtogonalNode* destination) {
	// check nodes are not null
	if (origin != NULL && destination != NULL) {
		if (destination->value == 0) {
			// increase steps
			this->steps++;
			int data_recover = destination->value;
			destination->value = origin->value;
			origin->value = data_recover;
			return true;
		}
	}
	cout << "DESTINO INVALIDO" << endl;
	return false;
}

/// <summary>
/// Check if all cells are sorted lower to higher, once finds a x > x1 return false
/// </summary>
/// <returns></returns>
bool Game::is_game_won() {
	// view in all nodes
	AsideNode* current = this->boards[current_board]->asides->tail;
	int points_partial = 0;
	OrtogonalNode* node_check;
	// check tail by tail if is ordered, from last wich must be "0", and all wich should be sorted from higher to lower
	while (current != NULL) {
		node_check = current->row->tail;
		while (node_check != NULL) {
			if (node_check->right != NULL) {
				if (node_check->value > node_check->right->value) {
					// check if is higher 'case is higher than 0
					if (node_check->right != this->boards[current_board]->asides->head->row->head) {
						points_partial += 2;
						// add result when board ends
						this->final_points += points_partial;
						return false;
					}
				}
				else {
					points_partial += 2;
				}
			}
			node_check = node_check->right;
		}
		current = current->next;
	}
	return true;
}

int Game::check_is_won() {
	if (this->is_game_won()) {
		if (this->current_board >= this->boards.size() - 1) {
			forward_board();
			cout << "Has ganado todos los niveles del juego " << endl;
			return -1;
		}
		else {
			cout << "Has ganado el nivel " << this->current_board << " cambiando de tablero..." << endl;
			forward_board();
		}
	}
	return 0;
}

/// <summary>
/// Print matrix
/// </summary>
void Game::print_board() {
	try
	{
		cout << "BOARD: " << this->current_board << endl;
		this->boards[this->current_board]->print();
	}
	catch (const std::exception&)
	{
		cout << "ERROR PRINTING BOARD: " << endl;
	}
}

/// <summary>
/// Changes to next matrix board
/// </summary>
void Game::forward_board() {
	if (this->current_board < this->levels - 1) {
		this->current_board++;
	}
}

/// <summary>
/// Changes to previous matrix board
/// </summary>
void Game::backward_board() {
	if (this->current_board > 0) {
		this->current_board--;
	}
}

/// <summary>
/// Set the levels, it means the number of board to generate when call create_game method
/// </summary>
/// <param name="levels"></param>
void Game::set_levels(int levels) {
	this->levels = levels;
	this->boards = std::vector<OrtogonalMatrix*>(levels);
}

int Game::get_levels() {
	return this->levels;
}

int Game::get_steps() {
	return this->steps;
}

int Game::get_final_points() {
	return this->final_points;
}
#include "Game.h"

using namespace std;

Game::Game() : game_field(), timer()
{
	srand(time(NULL));
	srand((int)time(0));
	temp_time = 0;
	score = 0;
}

Game::~Game()
{
}

void Game::set_field(Field _game_field) {
	game_field = _game_field;
}

Field Game::get_field() {
	return game_field;
}

// ����� ������ ������� delete � ������ ���������� ��� ���������� �������� �� ������� (���� ������ ������).
//����� ������� delete ������ (������� ��������, double delete).
//����� ������� �� �� ����� ��������� : delete ������ delete[] ��� ��������(������������� ���������, undefined behavior).
//����� ������������ ������ ����� ������ delete (dangling pointer).
// ������ ��������� ����� ����, ������ ��������� ������ �����������
// �������� ������ �� ������ ���������, ������ ������� �� ���� ������ ������. �� � ����� ����, ��
// ��������� ��������������� �����������, �� ���� ������
//��� �� ������ � ����������� ��������, ��� ��� �������� ����� � �++ ������� ������������ ����������� ��������� std::vector**.�� ��� ������� ������ � ������������ � ��������� � � �����������.� ���� ��, �� ��� ����� ������ ���� ������ �� ����� �����
bool Game::Is_falling_figure_collision(int offset_x, int offset_y) {
	Coord figure_point;
	array<Coord, 4>  temp_figure_coord = game_field.get_figure().get_figure_coord(); // � � ����� ������ ���� ���������� ������� � ���������
	for (int k = 0; k < temp_figure_coord.size(); k++) {
		for (int i = 0; i < game_field.get_width(); i++)
		{
			for (int j = 0; j < game_field.get_height(); j++)
			{
				figure_point.y = temp_figure_coord[k].y + game_field.get_figure_left_top_point().y;
				figure_point.x = temp_figure_coord[k].x + game_field.get_figure_left_top_point().x; 
				if (figure_point.y + offset_y == game_field.get_height())
					{
						return true;
					}
				else if (((figure_point.x + offset_x == i) && (figure_point.y + offset_y) == j)
					&& (game_field.get_cell_value_by_indexes(figure_point.x + offset_x, figure_point.y + offset_y) == 2)) // ���������� ����� 2 (� 1 ���� ���� ��������)
				{
					return true;
				}			
			}
		}
	}
	return false;
}

bool Game::Is_falling_figure_collision_diagonal(int offset_x, int offset_y) { // ������ �� ������ ����������� ������, �����
	Coord figure_point;
	array<Coord, 4>  temp_figure_coord = game_field.get_figure().get_figure_coord(); 
	for (int k = 0; k < temp_figure_coord.size(); k++) {
		for (int i = 0; i < game_field.get_width(); i++)
		{
			for (int j = 0; j < game_field.get_height(); j++)
			{
				figure_point.y = temp_figure_coord[k].y + game_field.get_figure_left_top_point().y;
				figure_point.x = temp_figure_coord[k].x + game_field.get_figure_left_top_point().x;
				if (figure_point.y + offset_y == game_field.get_height())
				{
					return true;
				}
				else if ((offset_y > 0 && offset_x != 0) && ((figure_point.x + offset_x == i) && (figure_point.y + offset_y) == j)
					&& (game_field.get_cell_value_by_indexes(figure_point.x + offset_x, figure_point.y + offset_y) == 2)
					&& (game_field.get_cell_value_by_indexes(figure_point.x + offset_x, figure_point.y) != 2))
				{
					return true;
				}
			}
		}
	}
	return false;
}

	

bool Game::Is_falling_figure_collision_by_side_only(int offset_x) {
	Coord figure_point;
	array<Coord, 4>  temp_figure_coord = game_field.get_figure().get_figure_coord();
	for (int k = 0; k < temp_figure_coord.size(); k++) {
		for (int i = 0; i < game_field.get_width(); i++)
		{
			for (int j = 0; j < game_field.get_height(); j++)
			{
				figure_point.y = temp_figure_coord[k].y + game_field.get_figure_left_top_point().y;
				figure_point.x = temp_figure_coord[k].x + game_field.get_figure_left_top_point().x;
				if (figure_point.y + offset_x == 0 && figure_point.y + offset_x == game_field.get_width())
				{
					return true;
				}
				else if (((offset_x != 0) && (figure_point.x + offset_x == i) && (figure_point.y) == j)
					&& (game_field.get_cell_value_by_indexes(figure_point.x + offset_x, figure_point.y) == 2)
					&& (game_field.get_cell_value_by_indexes(figure_point.x + offset_x, figure_point.y + 1) == 2)) // ���������� ����� 2 (� 1 ���� ���� ��������)
				{
					return true;
				}
			}
		}
	}
	return false;
}


// places figure on the field and saves previous figure state
void Game::Place_figure() { 
	array <Coord, 4> figure_coord = game_field.get_figure().get_figure_coord();
	array <Coord, 4> temp_figure_on_field_state = figure_coord;
	Coord left_top_point = game_field.get_figure_left_top_point();

	for (int i = 0; i < game_field.get_width(); i++)
	{
		for (int j = 0; j < game_field.get_height(); j++)
		{
			for (int k = 0; k < figure_coord.size(); k++)
			{ 
				if (((figure_coord[k].x + left_top_point.x) == i) && ((figure_coord[k].y + left_top_point.y) == j)
					&& (game_field.get_cell_value_by_indexes(i, j) != 2)) // 2 - ��� ����������  � ���� ������
				{			
						game_field.set_cell_value_by_indexes(1, i, j);
						temp_figure_on_field_state[k].x = i;
						temp_figure_on_field_state[k].y = j;
				}		
			}
		}
	}
	game_field.set_previous_figure_state_on_field(temp_figure_on_field_state);
	}


void Game::Remove_previous_figure_state() { 
	array <Coord, 4> temp_figure_on_field_state = game_field.get_previous_figure_state().get_figure_coord();
	for (int k = 0; k < temp_figure_on_field_state.size(); k++) {
		for (int i = 0; i < game_field.get_width(); i++)
		{
			for (int j = 0; j < game_field.get_height(); j++) {
				if (temp_figure_on_field_state[k].x == i && temp_figure_on_field_state[k].y == j)
				{
					game_field.set_cell_value_by_indexes(0, i, j);
				}
			}
		}
	}
}

void Game::Set_figure_as_a_field_part() {
	array <Coord, 4> figure_coord = game_field.get_figure().get_figure_coord();
	Coord left_top_point = game_field.get_figure_left_top_point();
	for (int i = 0; i < game_field.get_width(); i++)
	{
		for (int j = 0; j < game_field.get_height(); j++)
		{
			for (int k = 0; k < figure_coord.size(); k++)
			{
				if (((figure_coord[k].x + left_top_point.x) == i) && ((figure_coord[k].y + left_top_point.y) == j)
					&& (game_field.get_cell_value_by_indexes(i, j) == 1))
				{
					game_field.set_cell_value_by_indexes(2, i, j); // �������� �������� ���� �� 2, ��� ���������� ������
				}
			}
		}
	}
}

int Game::Field_check_full_row_index() {
	int counter = 0;
	for (int j = 0; j < game_field.get_height(); j++)
	{
		for (int i = 0; i < game_field.get_width(); i++)
		{
			if (game_field.get_cell_value_by_indexes(i, j) == 2)
			{
				counter++;
			}
		}
		if (counter >= game_field.get_width() - 2) { // ����� 1 � ������ -1
			return j;
		}
		else counter = 0;
	}
	return -1;
}

void Game::Erase_complete_row(int index) { // ��� ����� ���� ����������
	int **temp_field_cells;
	temp_field_cells = new int*[game_field.get_width()];
	for (int i = 0; i < game_field.get_width(); i++)
	{
		temp_field_cells[i] = new int[game_field.get_height()];
		for (int j = 0; j < game_field.get_height(); j++) {
			temp_field_cells[i][j] = game_field.get_cell_value_by_indexes(i, j);
		}
	}
	for (int j = 0; j < game_field.get_height() - 1; j++) // ����� 1 ������ ��� j + 1
	{
		for (int i = 0; i < game_field.get_width(); i++)
		{
			if (j == 0)
			{
				game_field.set_cell_value_by_indexes(0, i, j);
			}
			else if (j >= index - 1)
			{
				if (j < game_field.get_height() - 2) {
					game_field.set_cell_value_by_indexes(temp_field_cells[i][j], i, j + 2);
				}
				break;
			}
				game_field.set_cell_value_by_indexes(temp_field_cells[i][j], i, j + 1);
		}
	}
	for (int i = 0; i < game_field.get_width(); i++)
	{
		delete[](temp_field_cells[i]);
	}
	delete[](temp_field_cells);
}


void Game::Update_field() {
	int row_index = 0;
	Place_figure();
	row_index = Field_check_full_row_index();
//	Erase_complete_row(row_index); // Erase_complete_row ����� ����������
//���� ������ ������ ������ ����. ����� �������� � ��������� ����, ����� ����� ��������� �� ������ ������
	if (row_index != -1) cout << endl << row_index; 
}

void Game::StartGame() {
	int movement_offset_x;
	int falling_offset_y;
	std::string EnumStrings[6] = { "UP", "LEFT", "RIGHT", "DOWN", "MIN", "MAX" };
	View view(game_field);
	view.DrawField(game_field);
	while (1)
	{
		movement_offset_x = 0;
		falling_offset_y = 0;
		if(timer.Elapsed().count() - temp_time > 0){			
		temp_time = timer.Elapsed().count();
		action = view.ViewAction(game_field);
		if (strlen(action) > 1 ) {
			// ����������� ��������� ������ � ���� �� keyboard interaction
			if (!strcmp(action, "UP")) { 
				game_field.set_rotated_figure(*game_field.get_figure().Rotate_figure_up());
			}
			else if (!strcmp(action, "LEFT")) { // ������� �������� �� ������������ � ����� (������ 2)
				if (game_field.get_figure_left_top_point().x > 1) { // ������� �� 1
				//	game_field.Move_figure_left(); 
					movement_offset_x = -1;
				}
			}
			else if (!strcmp(action, "RIGHT")) { 
				view.DrawField(game_field);
				int max_x = game_field.get_figure_left_top_point().x;
				array<Coord, 4>  temp_figure_coord = game_field.get_figure().get_figure_coord();
				for (int i = 0; i < temp_figure_coord.size(); i++) {
					if ((temp_figure_coord[i].x + game_field.get_figure_left_top_point().x) > max_x) {
						max_x = temp_figure_coord[i].x + game_field.get_figure_left_top_point().x;
					}
				}
				if (max_x < game_field.get_width() - 1) { // ������� �� ������ ���� ����� 1
					//game_field.Move_figure_right();
					movement_offset_x = 1;
				}
			}
			else if (!strcmp(action, "DOWN")) { 
				game_field.set_rotated_figure(*game_field.get_figure().Rotate_figure_down());				
			}
		}
		// ��� ���� ����� ������ �� ������� � ����� ���������� �����, � ������ ��������� ����, ��������:
		if (Is_falling_figure_collision_by_side_only(movement_offset_x))
		{
			movement_offset_x = 0;
		}

		// ������� ������
		falling_offset_y = 1;
		if (Is_falling_figure_collision_diagonal(movement_offset_x, falling_offset_y))
		{
			movement_offset_x = 0;
		}
		if (Is_falling_figure_collision(movement_offset_x, falling_offset_y) == false) {
				game_field.Move_figure_by_offset(movement_offset_x, falling_offset_y); // change figure coordinates
				// ������ ������-�� �� ���������
				Remove_previous_figure_state();
			}
		// set figure as a field part by not removing it's previous state
		else {
			Set_figure_as_a_field_part();
			figure.CreateFigure();
			game_field.set_new_figure(figure);
		}
		Update_field();
		view.DrawField(game_field);
	}
	}
}



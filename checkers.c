#include "ctype.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void print_table(char* table) {
	printf("   | a | b | c | d | e | f | g | h\n");
	for (int y = 0; y < 8; y++) {
		printf("---+---+---+---+---+---+---+---+---\n");
		printf(" %d ", y + 1);
		for (int x = 0; x < 8; x++)
			printf("| %c ", table[x + 8 * y]);
		printf("\n");
	}
}

char check_win(char* table) {
	char x = 0, o = 0;
	for (int i = 0; i < 8 * 8; i++) {
		if (tolower(table[i]) == 'x')
			x = 1;
		else if (tolower(table[i]) == 'o')
			o = 1;
		if (x && o)
			return ' ';
	}
	return x ? 'x' : 'o';
}

int encode_move(int x0, int y0, int x1, int y1) {
	return y1 * 512 + x1 * 64 + y0 * 8 + x0;
}

char get_square(char* table, int x, int y) {
	if (x < 0 || x > 7 || y < 0 || y > 7)
		return -1;
	return table[x + 8 * y];
}

char get_opponent(char player) {
	return tolower(player) == 'x' ? 'o' : 'x';
}

int check_move(char* table, char player, int i, int dx, int dy) {
	int x = i % 8, y = i / 8;
	int sq = get_square(table, x + dx, y + dy);
	if (sq == ' ')
		return encode_move(x, y, x + dx, y + dy);
	else if ((sq == get_opponent(player) || sq == toupper(get_opponent(player))) &&
				get_square(table, x + 2 * dx, y + 2 * dy) == ' ')
		return encode_move(x, y, x + 2 * dx, y + 2 * dy);
	return -1;
}

int* available_moves(char* table, char player) {
	int* moves = malloc(48 * sizeof(int));
	int move_count = 0;
	for (int i = 0; i < 8 * 8; i++) {
		if (tolower(table[i]) != player)
			continue;
		int move;
		if (table[i] == 'o' || table[i] == 'X' || table[i] == 'O') {
			move = check_move(table, table[i], i, -1, -1);
			if (move != -1)
				moves[move_count++] = move;
			move = check_move(table, table[i], i, +1, -1);
			if (move != -1)
				moves[move_count++] = move;
		}
		if (table[i] == 'x' || table[i] == 'X' || table[i] == 'O') {
			move = check_move(table, table[i], i, +1, +1);
			if (move != -1)
				moves[move_count++] = move;
			move = check_move(table, table[i], i, -1, +1);
			if (move != -1)
				moves[move_count++] = move;
		}
	}
	if (move_count == 0) {
		free(moves);
		moves = NULL;
	} else if (move_count < 48) {
		moves[move_count] = -1;
	}
	return moves;
}

int get_score(char* table, char player) {
	int score = 0;
	for (int i = 0; i < 8 * 8; i++) {
		if (table[i] == player)
			score += 1;
		else if (table[i] == toupper(player))
			score += 5;
		else if (table[i] == get_opponent(player))
			score -= 1;
		else if (table[i] == toupper(get_opponent(player)))
			score -= 5;
	}
	return score;
}

char* copy_table(char* table) {
	char* new_table = malloc(8 * 8);
	for (int i = 0; i < 8 * 8; i++)
		new_table[i] = table[i];
	return new_table;
}

void do_move(char* table, int move) {
	int x0 = (move) % 8;
	int y0 = (move / 8) % 8;
	int x1 = (move / 64) % 8;
	int y1 = (move / 512) % 8;
	table[x1 + 8 * y1] = table[x0 + 8 * y0];
	table[x0 + 8 * y0] = ' ';
	if (table[x1 + 8 * y1] == 'o' && y1 == 0)
		table[x1 + 8 * y1] = 'O';
	else if (table[x1 + 8 * y1] == 'x' && y1 == 7)
		table[x1 + 8 * y1] = 'X';
	if (x0 - x1 == 2 || x0 - x1 == -2)
		table[(x0 + x1) / 2 + 8 * ((y0 + y1) / 2)] = ' ';
}

int negamax(char* table, int depth, char player) {
	if (depth == 3 || check_win(table) != ' ')
		return get_score(table, player);
	int* moves = available_moves(table, player);
	if (moves == NULL)
		return -1000;
	int best_score = -1000;
	int best_move = 0;
	for (int i = 0; i < 48; i++) {
		if (moves[i] == -1)
			break;
		char* new_table = copy_table(table);
		do_move(new_table, moves[i]);
		int score = -negamax(new_table, depth + 1, get_opponent(player));
		free(new_table);
		if (score > best_score) {
			best_score = score;
			best_move = moves[i];
		}
	}
	free(moves);
	if (depth == 0) {
		printf("%c> %c%c %c%c\n", player,
				(best_move) % 8 + 'a', (best_move / 8) % 8 + '1',
				(best_move / 64) % 8 + 'a', (best_move / 512) % 8 + '1');
		do_move(table, best_move);
	}
	return best_score;
}

char is_valid_move(int* moves, int move) {
	for (int i = 0; i < 48; i++) {
		if (moves[i] == -1)
			return 0;
		if (moves[i] == move)
			return 1;
	}
	return 0;
}

int get_input(char* table, char player) {
	char *line = NULL;
	size_t line_length = 0;

	int* moves = available_moves(table, player);
	int move = -1;
	while (!is_valid_move(moves, move)) {
		printf("%c> ", player);
		int read = getline(&line, &line_length, stdin);
		if (read != 6)
			continue;
		int x0 = line[0] - 'a';
		int y0 = line[1] - '1';
		int x1 = line[3] - 'a';
		int y1 = line[4] - '1';
		if (x0 < 0 || x0 > 7 || y0 < 0 || y0 > 7 ||
			x1 < 0 || x1 > 7 || y1 < 0 || y1 > 7)
			continue;
		move = encode_move(x0, y0, x1, y1);
	}
	return move;
}

int main(int argc, char** argv) {
	char table[8 * 8];
	for (int i = 0; i < 8 * 8; i++) {
		if (i < 8) table[i] = i % 2 == 1 ? 'x' : ' ';
		else if (i < 16) table[i] = i % 2 == 0 ? 'x' : ' ';
		else if (i < 24) table[i] = i % 2 == 1 ? 'x' : ' ';
		else if (i > 55) table[i] = i % 2 == 0 ? 'o' : ' ';
		else if (i > 47) table[i] = i % 2 == 1 ? 'o' : ' ';
		else if (i > 39) table[i] = i % 2 == 0 ? 'o' : ' ';
		else table[i] = ' ';
	}

	char turn = 'x';
	char ai = ' ';
	if (argc == 2 && !strcmp(argv[1], "--ai")) {
		ai = 'o';
	} else if (argc == 3 && !strcmp(argv[1], "--ai")) {
		ai = tolower(argv[2][0]);
		if (strlen(argv[2]) > 1 || (ai != 'x' && ai != 'o')) {
			printf("Invalid AI turn. Should be 'x' or 'o'\n");
			exit(1);
		}
	} else if (argc != 1) {
		printf("Invalid arguments. Options:\n");
		printf("\t%s --ai [x|o]\n", argv[0]);
		exit(1);
	}

	char win = ' ';
	while (win == ' ') {
		print_table(table);
		if (turn == ai) {
			if (negamax(table, 0, turn) == -1000)
				win = get_opponent(turn);
		} else {
			do_move(table, get_input(table, turn));
		}
		turn = get_opponent(turn);
		win = check_win(table);
	}
	printf("%c won!\n", win);
}

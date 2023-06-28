#pragma warning(disable:4996)
#include<cstring>
#include<iostream>
#include <algorithm>
using namespace std;
long save_num = 0;
char save_arr[300000000] = { 0 };
int sudoku_arr[9][9] = { 0 };
int col_record[9][10] = { 0 };
int row_record[9][10] = { 0 };
int block_record[9][10] = { 0 };
struct SudokuSpace {
	int x, y;	//数独空白点的位置
	int count;			//计数器，用来排序
}blank_arr[100];
int generateSudokuEndGame(int n) {
	int row_list[9] = { 0,1,2,3,4,5,6,7,8 };
	int	number_list[9] = { 1,2,3,4,5,6,7,8,9 };
	string sudoku_template[9] = {
		"123456789",
		"456789123",
		"789123456",
		"234567891",
		"567891234",
		"891234567",
		"345678912",
		"678912345",
		"912345678"
	};
	FILE* se;
	if (n > 0 && n <= 1000000) {
		remove("./sudoku_endgame.txt");
		se = fopen("./sudoku_endgame.txt", "wt");
		if (se == NULL) {
			cout << "open file error" << endl;
			return 1;
		}
	}
	else
	{
		cout << "n should in 1~1000000" << endl;
		return 1;
	}

	save_num = 0;
	memset(save_arr, 0, sizeof(save_arr));

	do {
		do {
			do {
				do {
					for (int i = 0; i < 9; i++) {
						for (int j = 0; j < 9; j++) {
							int tmp = (int)(sudoku_template[row_list[i]][j] - '0') - 1;

							save_arr[save_num] = number_list[tmp] + '0';
							save_num++;
							if (j != 8) {
								save_arr[save_num] = ' ';
								save_num++;
							}
							else
							{
								save_arr[save_num] = '\n';//end of a row
								save_num++;
							}
						}
					}
					save_arr[save_num++] = '\n';
					n--;
					if (n == 0) {
						fwrite(save_arr, sizeof(char), save_num, se);
						cout << "finish generation" << endl;
						fclose(se);
						return 0;
					}
				} while (next_permutation(number_list, number_list + 9));//exchange in number_list
			} while (next_permutation(row_list + 6, row_list + 9));//exchange row 7~9
		} while (next_permutation(row_list + 3, row_list + 6));//exchange row 4~6
	} while (next_permutation(row_list, row_list + 3));//exchange row 1~3

	return 1;
}
int getBlockIndex(int x, int y) {
	if (x < 0 || y < 0 || x >= 9 || y>8) return -1;
	return ((x / 3) * 3 + (y / 3));
}
void updateRecord(int x, int y, int number, int flag) {
	row_record[x][number] = flag;
	col_record[y][number] = flag;
	block_record[getBlockIndex(x, y)][number] = flag;
}
bool canFill(int x, int y, int number)
{
	if (row_record[x][number] == 0 && col_record[y][number] == 0 && block_record[getBlockIndex(x, y)][number] == 0)
		return true;
	else
		return false;
}

bool fillNumber(int index, int size)
{
	if (index != size)
	{
		for (int i = 1; i < 10; i++)
		{
			if (canFill(blank_arr[index].x, blank_arr[index].y, i))
			{
				sudoku_arr[blank_arr[index].x][blank_arr[index].y] = i;
				updateRecord(blank_arr[index].x, blank_arr[index].y, i, 1);
				if (fillNumber(index + 1, size))
					return true;
				updateRecord(blank_arr[index].x, blank_arr[index].y, i, 0);
				sudoku_arr[blank_arr[index].x][blank_arr[index].y] = 0;
			}
		}
		return false;
	}
	else
	{
		return true;
	}
}
bool cmp(struct SudokuSpace& a, struct SudokuSpace& b) {
	return a.count > b.count;
}
int solveSudokuGame(char* questpath) {
	char content[1000];
	FILE* sg = fopen(questpath, "r");
	if (sg == NULL) {
		cout << "open question file error" << endl;
		return 1;
	}
	save_num = 0;
	memset(save_arr, 0, sizeof(save_arr));

	while (!feof(sg)) {
		memset(content, 0, sizeof(content));
		memset(blank_arr, 0, sizeof(blank_arr));
		memset(row_record, 0, sizeof(row_record));
		memset(col_record, 0, sizeof(col_record));
		memset(block_record, 0, sizeof(block_record));
		if (fread(content, sizeof(char), 163 * sizeof(char), sg) < 161) {
			cout << "read file error" << endl;
			return -1;
		};
		int x = 0;
		int y = 0;

		for (int i = 0; i < 162; i++) {

			if (content[i] == ' ' || content[i] == '$' || content[i] == '\n')
				continue;
			sudoku_arr[x][y] = content[i] - '0';
			if (y >= 8) {
				y = 0;
				x++;
			}
			else
				y++;
		}
		//******************************
		int num_blank = 0;
		int col_num[9] = { 0 };
		int row_num[9] = { 0 };
		int block_num[9] = { 0 };

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (sudoku_arr[i][j] != 0) //not blank then update 
				{
					updateRecord(i, j, sudoku_arr[i][j], 1);
					row_num[i]++;
					col_num[j]++;
					block_num[getBlockIndex(i, j)]++;
				}
				else//blank then record
				{
					blank_arr[num_blank].x = i;
					blank_arr[num_blank].y = j;
					num_blank++;
				}

			}
		}
		for (int i = 0; i < num_blank; i++)
		{
			blank_arr[i].count = col_num[blank_arr[i].y] + row_num[blank_arr[i].x] + block_num[getBlockIndex(blank_arr[i].x, blank_arr[i].y)];
		}
		sort(blank_arr, blank_arr + num_blank, cmp);
		//******************************
		if (fillNumber(0, num_blank) == false)
			cout << "fail to solve this game" << endl;
		else
		{
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					save_arr[save_num] = sudoku_arr[i][j] + '0';
					save_num++;
					if (save_arr[save_num] == '0')
						save_arr[save_num] = '$';
					if (j != 8) {
						save_arr[save_num] = ' ';
						save_num++;
					}
					else
					{
						save_arr[save_num] = '\n';//end of a row
						save_num++;
					}
				}
			}
			save_arr[save_num] = '\n';//end of a row
			save_num++;
		}
	}
	FILE* result = fopen("./sudoku.txt", "wt");
	fwrite(save_arr, sizeof(char), save_num, result);
	cout << "finish solve" << endl;
	fclose(result);
	fclose(sg);
	return 0;
}
bool fillNumberNotN(int index, int size, int n, int x, int y, int tmp[9][9])
{
	if (index != size)
	{
		for (int i = 1; i < 10; i++)
		{
			if ((blank_arr[index].x == x) && (blank_arr[index].y == y) && (i == n)) {
				//cout << i << endl;
				continue;
			}

			if (canFill(blank_arr[index].x, blank_arr[index].y, i))
			{
				tmp[blank_arr[index].x][blank_arr[index].y] = i;

				updateRecord(blank_arr[index].x, blank_arr[index].y, i, 1);
				if (fillNumberNotN(index + 1, size, n, x, y, tmp))
					return true;
				updateRecord(blank_arr[index].x, blank_arr[index].y, i, 0);
				tmp[blank_arr[index].x][blank_arr[index].y] = 0;
			}
		}
		return false;
	}
	else
	{
		return true;
	}
}
bool hasUniqueSolution(int sudoku[9][9], int n, int x, int y) {
	int tmp[9][9];
	memcpy(tmp, sudoku, 9 * 9 * sizeof(int));
	memset(blank_arr, 0, sizeof(blank_arr));
	memset(row_record, 0, sizeof(row_record));
	memset(col_record, 0, sizeof(col_record));
	memset(block_record, 0, sizeof(block_record));
	int num_blank = 0;
	int col_num[9] = { 0 };
	int row_num[9] = { 0 };
	int block_num[9] = { 0 };
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (tmp[i][j] != 0) //not blank then update 
			{
				updateRecord(i, j, sudoku_arr[i][j], 1);
				row_num[i]++;
				col_num[j]++;
				block_num[getBlockIndex(i, j)]++;
			}
			else//blank then record
			{
				blank_arr[num_blank].x = i;
				blank_arr[num_blank].y = j;
				num_blank++;
			}
		}
	}
	for (int i = 0; i < num_blank; i++)
	{
		blank_arr[i].count = col_num[blank_arr[i].y] + row_num[blank_arr[i].x] + block_num[getBlockIndex(blank_arr[i].x, blank_arr[i].y)];
	}
	sort(blank_arr, blank_arr + num_blank, cmp);

	if (fillNumberNotN(0, num_blank, n, x, y, tmp) == false)
		return true;
	else
		return false;
}
int test(char* path)
{
	char content[1000];
	memset(content, 0, sizeof(content));
	FILE* sg = fopen(path, "r");
	if (sg == NULL) {
		cout << "open test file error" << endl;
		return 1;
	}
	int x = 0;
	int y = 0;
	fread(content, sizeof(char), 163 * sizeof(char), sg);
	for (int i = 0; i < 162; i++) {
		if (content[i] == ' ' || content[i] == '$' || content[i] == '\n')
			continue;
		sudoku_arr[x][y] = content[i] - '0';
		if (y >= 8) {
			y = 0;
			x++;
		}
		else
			y++;
	}

	if (hasUniqueSolution(sudoku_arr, 1, 0, 0))
		cout << "has unique solution" << endl;
	else
		cout << "has no unique solution" << endl;
	return 0;
}
void creat_blank()
{
	int round = 25;
	int temp_row = -1;
	int temp_col = -1;
	int temp_num = -1;

	for (int i = 0; i < round; i++)
	{
		int j = rand() % 10;
		int k = rand() % 10;

		if (sudoku_arr[j][k] != 0)
		{
			temp_row = j;
			temp_col = k;
			temp_num = sudoku_arr[j][k];
			sudoku_arr[j][k] = 0;
		}
	}
}

void creat_blank_level(int level)
{
	int round = 0;
	if (level == 1)
		round = 25;
	else
		if (level == 2)
			round = 40;
		else
			if (level == 3)
				round = 60;
	int temp_row = -1;
	int temp_col = -1;
	int temp_num = -1;

	for (int i = 0; i < round; i++)
	{
		int j = rand() % 10;
		int k = rand() % 10;

		if (sudoku_arr[j][k] != 0)
		{
			temp_row = j;
			temp_col = k;
			temp_num = sudoku_arr[j][k];
			sudoku_arr[j][k] = 0;
		}
		else
			i--;
	}
}

int generateLevel(int n, int m)
{
	generateSudokuEndGame(n);
	char content[1000];
	FILE* sg = fopen("./sudoku_endgame.txt", "r");
	if (sg == NULL) {
		cout << "open question file error" << endl;
		return 1;
	}
	save_num = 0;
	memset(save_arr, 0, sizeof(save_arr));
	int round = 0;

	while (!feof(sg)) {
		memset(content, 0, sizeof(content));
		memset(blank_arr, 0, sizeof(blank_arr));
		memset(row_record, 0, sizeof(row_record));
		memset(col_record, 0, sizeof(col_record));
		memset(block_record, 0, sizeof(block_record));
		int size = fread(content, sizeof(char), 163 * sizeof(char), sg);
		if (size < 161 && size>0) {
			cout << "read file error" << endl;
			return -1;
		}
		else if (size == 0)
			break;
		int x = 0;
		int y = 0;

		for (int i = 0; i < 162; i++) {

			if (content[i] == ' ' || content[i] == '$' || content[i] == '\n')
				continue;
			sudoku_arr[x][y] = content[i] - '0';
			if (y >= 8) {
				y = 0;
				x++;
			}
			else
				y++;
		}
		creat_blank_level(m);

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				save_arr[save_num] = sudoku_arr[i][j] + '0';

				if (save_arr[save_num] == '0')
					save_arr[save_num] = '$';
				save_num++;
				if (j != 8) {
					save_arr[save_num] = ' ';
					save_num++;
				}
				else
				{
					save_arr[save_num] = '\n';//end of a row
					save_num++;
				}
			}
		}
		save_arr[save_num] = '\n';//end of a row
		save_num++;
		round++;
		if (round >= n)break;
	}
	FILE* result = fopen("./sudoku_level.txt", "wt");
	fwrite(save_arr, sizeof(char), save_num, result);
	cout << "finish creat " << n << " Level " << m << " game" << endl;
	fclose(result);
	fclose(sg);
	return 0;
}


int generateSudokuGame(int n) {
	generateSudokuEndGame(n);
	char content[1000];
	FILE* sg = fopen("./sudoku_endgame.txt", "r");
	if (sg == NULL) {
		cout << "open question file error" << endl;
		return 1;
	}
	save_num = 0;
	memset(save_arr, 0, sizeof(save_arr));

	while (!feof(sg)) {
		memset(content, 0, sizeof(content));
		memset(blank_arr, 0, sizeof(blank_arr));
		memset(row_record, 0, sizeof(row_record));
		memset(col_record, 0, sizeof(col_record));
		memset(block_record, 0, sizeof(block_record));
		int size = fread(content, sizeof(char), 163 * sizeof(char), sg);
		if (size < 161 && size>0) {
			cout << "read file error" << endl;
			return -1;
		}
		else if (size == 0)
			break;
		int x = 0;
		int y = 0;

		for (int i = 0; i < 162; i++) {

			if (content[i] == ' ' || content[i] == '$' || content[i] == '\n')
				continue;
			sudoku_arr[x][y] = content[i] - '0';
			if (y >= 8) {
				y = 0;
				x++;
			}
			else
				y++;
		}
		creat_blank();
		/*for (int i = 0;i < 9;i++)
		{
			for (int j = 0;j < 9;j++)
			{
				cout << sudoku_arr[i][j] << " ";
			}
			cout << endl;
		}*/
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				save_arr[save_num] = sudoku_arr[i][j] + '0';

				if (save_arr[save_num] == '0')
					save_arr[save_num] = '$';
				save_num++;
				if (j != 8) {
					save_arr[save_num] = ' ';
					save_num++;
				}
				else
				{
					save_arr[save_num] = '\n';//end of a row
					save_num++;
				}
			}
		}
		save_arr[save_num] = '\n';//end of a row
		save_num++;
	}
	FILE* result = fopen("./sudoku_game.txt", "wt");
	fwrite(save_arr, sizeof(char), save_num, result);
	cout << "finish solve" << endl;
	fclose(result);
	fclose(sg);
	return 0;
}
int main(int argc, char** argv)
{
	if (argc == 1)
	{
		cout << "no argument" << endl;
		return 0;
	}
	else
	{
		if (argc == 2 && strcmp(argv[1], "-c") == 0) {
			cout << "please input the number of sukodu endgames" << endl;
			return 1;
		}

		if (strcmp(argv[1], "-c") == 0)
		{
			int n = atoi(argv[2]);
			if (generateSudokuEndGame(n))
			{
				cout << "fail to generate" << endl;
				return 1;
			}

		}
		else if (strcmp(argv[1], "-s") == 0)
		{
			if (solveSudokuGame(argv[2]))
			{
				cout << "fail to solve" << endl;
				return 1;
			}
		}
		else if (strcmp(argv[1], "-n") == 0 && strcmp(argv[3], "-m") == 0)
		{
			int n = atoi(argv[2]);
			int m = atoi(argv[4]);
			if (generateLevel(n, m))
			{
				cout << "fail to generate corresponding level games" << endl;
				return 1;
			}
		}
		else if (strcmp(argv[1], "-n") == 0)
		{
			int n = atoi(argv[2]);
			if (generateSudokuGame(n))
			{
				cout << "fail to find unique solution" << endl;
				return 1;
			}
		}
		else if (strcmp(argv[1], "-u") == 0)
		{
			if (test(argv[2]))
			{
				cout << "fail to find unique solution" << endl;
				return 1;
			}
		}
	}
}



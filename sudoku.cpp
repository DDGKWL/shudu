#pragma warning(disable:4996)
#include<cstring>
#include<iostream>
#include <algorithm>
using namespace std;
long save_num = 0;
char save_arr[300000000] = { 0 };
int generateSudokuEndGame(int n) {
	int col_list[9] = { 0,1,2,3,4,5,6,7,8 };
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
							int tmp = (int)(sudoku_template[col_list[i]][j] - '0') - 1;

							save_arr[save_num] = number_list[tmp] + '0';
							save_num++;
							if (j != 8) {
								save_arr[save_num] = '$';
								save_num++;
							}
							else
							{
								save_arr[save_num] = '\n';//end of a col
								save_num++;
							}
						}
					}
					save_arr[save_num++] = '\n';
					n--;
					if (n == 0) {
						fwrite(save_arr, sizeof(char),save_num, se);
						cout << "finish generation" << endl;
						fclose(se);
						return 0;
					}
				} while (next_permutation(number_list, number_list + 9));//exchange in number_list
			} while (next_permutation(col_list + 6, col_list + 9));//exchange col 7~9
		} while (next_permutation(col_list + 3, col_list + 6));//exchange col 4~6
	} while (next_permutation(col_list, col_list + 3));//exchange col 1~3
	
	return 1;
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

		if(strcmp(argv[1],"-c")==0)
		{
			int n = atoi(argv[2]);
			if (generateSudokuEndGame(n))
			{
				cout << "fail to generate" << endl;
				return 1;
			}
			
		}
	}
}
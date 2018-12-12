// sudoku1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include<fstream>
#include<string.h>
#include <algorithm>
using namespace std;
int finality[10000000][10];
class sudo {

private:
	int puzzle[10][10];
	ifstream input;
	ofstream output;

public:
	bool judge(int, int, int);
	bool solve(int, int);
	void data_input(string);
	void data_output();
	void generate_sudo(int);
	void print_generate_sudo(int);
};
bool sudo::judge(int x, int y, int num) {
	for (int i = 0; i < 9; i++) //检验行是否有重复的
		if (puzzle[x][i] == num)
			return false;
	for (int j = 0; j < 9; j++) //检验列是否有重复的
		if (puzzle[j][y] == num)
			return false;
	for (int i = x / 3 * 3; i < x / 3 * 3 + 3; i++) { //检验宫是否有重复的
		for (int j = y / 3 * 3; j < y / 3 * 3; j++)
			if (puzzle[i][j] == num)
				return false;
	}
	return true;
}
bool sudo::solve(int row, int column) {		//解一个数独
	if (row == 8 && column == 9)
		return true;		//结束条件
	if (column == 9) {
		row++;
		column = 0;			//行走到最后一个 走到下一列的第一个
	}
	if (puzzle[row][column] != 0) {
		return solve(row, column + 1); //若不是空 则往右走
	}
	for (int i = 1; i < 10; i++) {
		if (judge(row, column, i) == true) {
			puzzle[row][column] = i;		//若符合要求 则填入
			if (solve(row, column + 1))
				return true;				//递归往后求解
		}

	}
	puzzle[row][column] = 0;
	return false;		//无解 回溯

}
void sudo::data_input(string filename) {
	input.open(filename);
	output.open("out.txt");
	char enter;
	while (!input.eof()) {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++)
				input >> puzzle[i][j];	//输入一个数组题目
		}
			
		solve(0, 0);
		data_output();
		output << endl;

	}
	input.close();
	output.close();
}
void sudo::data_output() {
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++) {
			output << puzzle[i][j];
			if (j != 8) output << " ";
			else output << endl;
		}
}
void sudo::generate_sudo(int sudo_amount) {
	int sudo_num = 0;
	int temp[9];
	for (int i = 0; i < 8; i++)
		temp[i] = i + 2;	//第一个是2 为（1+9）% 9 + 1
	temp[8] = 1;
	int swap[9] = { 0,1,2,3,4,5,6,7,8 };
	int move[9] = { 0,3,6,1,4,7,2,5,8 };
	do { //对第一行进行全排列 可生成8!个基础终局
		for (int i = 0; i < 9; i++) { //向右移动
			for (int j = 0; j < 9; j++)
				puzzle[i][j] = temp[(j - move[i] + 9) % 9];

		}
		/*****成功生成了一个基础终局*****/
		do {
			do {
				for (int i = 0; i < 6; i++) {	/*改这里啦哦！*/
					for (int j = 0; j < 9; j++) {
						finality[i + 9 * sudo_num][j] = puzzle[swap[i]][j];
					}
				}
				for (int i = 6; i < 9; i++) {
					for (int j = 0; j < 9; j++) {
						finality[i + 9 * sudo_num][j] = puzzle[swap[i]][j];
					}
				}
				sudo_num++;
				if (sudo_num == sudo_amount) {
					return;
				}
			} while (next_permutation(swap + 6, swap + 9));
		} while (next_permutation(swap + 3, swap + 6));

		/*****对行进行交换*****/
		do {
			do {
				for (int i = 0; i < 9; i++) {
					for (int j = 0; j < 9; j++) {
						finality[i + 9 * sudo_num][j] = puzzle[i][swap[j]];
					}
				}
				sudo_num++;
				if (sudo_num == sudo_amount) {
					return;
				}
			} while (next_permutation(swap + 6, swap + 9));
		} while (next_permutation(swap + 3, swap + 6));
	} while (next_permutation(temp + 1, temp + 9));


}
void sudo::print_generate_sudo(int n) {
	output.open("finality.txt");
	for (int i = 0; i < n; i++) {
		for (int j = 9 * i; j < 9 * i + 9; j++) {
			for (int k = 0; k < 9; k++) {
				output << finality[j][k];
				if (k != 8) output << " ";
				else output << endl;
			}
		}
		output << endl;
	}
		
}
int main(int argc, char * argv[])
{
	sudo problem;
	string filename;
	char order[10]; //存储是 -c 还是 -s
	char num[100]; //存的数独题目的地址或者生成终局的数目
	strcpy(order, argv[1]);
	strcpy(num, argv[2]);
	if (strcmp(order, "-c") == 0) {	//生成终局
		int flag = 1;
		int i = 0, sudoNum = 0;
		while (i < strlen(num)) {
			if (num[i] - '0' >= 0 && num[i] - '0' <= 9) {
				sudoNum = sudoNum * 10 + num[i] - '0';
				i++;
			}
			else {
				flag = 0;
				break;
			}

		}
		if (!flag) printf("Wrong input\n");
		else {	//生成终局
			problem.generate_sudo(sudoNum);
			//problem.print_generate_sudo(sudoNum);
		}
	}
	if (strcmp(order, "-s") == 0) { //解数独
		filename = num;
		problem.data_input(filename);
	}
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

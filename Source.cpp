
#include <iostream>
using namespace std;
char grid[5][5];
 
int input;
int input2;

int main() {

	char grid[5][5] = { ' ','|',' ','|', ' ' ,
						'-','-','-','-','-',
						' ', '|',' ','|', ' ',
						'-', '-','-', '-', '-',
						' ', '|',' ', '|', ' ', };
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cout << grid[i][j];
		}
		cout << " " << endl;;
	}
	char input;
	int input1;
	int input2;
	while (1) {
		cout << "enter X or O" << endl;
		cin >> input;
		cout << "what row" << endl;
		cin >> input1;
		cout << "what collumn" << endl;
		cin >> input2;
		
		
		if (input1 == 1)input1 = 0;
		if (input1 == 2)input1 = 2;
		if (input1 == 3)input1 = 4;

		if (input2 == 1)input2 = 0;
		if (input2 == 2)input2 = 2;
		if (input2 == 3)input2 = 4;

		if (input == 'X' || input == 'x') {
			grid[input1][input2] = 'X';
		}

		bool placed = false;
		while(1) {
			int a = rand() % 5;
			int b = rand() % 5;
			if (grid[a][b] == ' ') {
				grid[a][b] = 'O';
				break;
			}

		}
		if (input1 == 1)input1 = 0;
		if (input1 == 2)input1 = 2;
		if (input1 == 3)input1 = 4;

		if (input2 == 1)input2 = 0;
		if (input2 == 2)input2 = 2;
		if (input2 == 3)input2 = 4;

		if (input == 'O' || input == 'o') {
			grid[input1][input2] = 'O';

		}

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				cout << grid[i][j];
			}
			cout << " " << endl;
	}

	}
}

	

	

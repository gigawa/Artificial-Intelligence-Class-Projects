#include <stdio.h>
#include <iostream>
#include <set>
#include <queue>

using namespace std;

//The 2D board for our 8-puzzle in format board[x][y]
int** board;
//The output logging file. Don't worry about this one
FILE* output;

struct node {
	int** board;
	int g;
	int f;
	vector<node> children;

	bool operator<(const node &rhs) const {
		return !(f < rhs.f);
	}
};

struct position {
	int a;
	int b;
};

/*
ReadBoardFile: Read board data in from a file to populate our board structure.

Arguments:
char* fname: The name of the file to read

This just reads a simple file format of a b c each line. There should be three lines.
Each of the 9 entries should go from 0-8 inclusive. If you spot a problem let us know.

Returns bool: Returns true if the file was opened and read successfuly, and false if the file could not be read
*/
bool ReadBoardFile(char* fname)
{
	FILE* file = fopen(fname, "r");
	if (file)
	{
		rewind(file);
		int a, b, c;

		board = new int*[3];
		for (int u = 0; u < 3; u++)
		{
			board[u] = new int[3];

			for (int v = 0; v < 3; v++)
			{
				board[u][v] = 0;
			}
		}

		int i = 0;
		//while (fscanf_s(file, "%d %d %d", &a, &b, &c) != EOF)
		while (fscanf(file, "%d %d %d", &a, &b, &c) != EOF)
		{
			board[0][i] = a;
			board[1][i] = b;
			board[2][i] = c;
			i++;
		}

		fclose(file);

		return true;
	}
	else
	{
		printf("Failed to read file...\n");
		return false;
	}
}

/*
LogCell: Prints the current cell traveled to and its value and saves the output to a file

Arguments:
int x: The x of the cell you are visiting
int y: The y of the cell you are visiting

In order for us to grade you, you must print the cell you are travelling to in your algorithm.
This function makes it easier for you to print the proper data. It also saves the data to a file.

Returns void: No values returned
*/
void LogCell(int x, int y)
{
	printf("(%d, %d): %d\n", x, y, board[x][y]);
	fprintf(output, "(%d, %d): %d\n", x, y, board[x][y]);
}

/*
SetartLogging: Opens the logging file for later writing

Arguments:
None

Pretty much just a wrapper for fopen so that we make sure you are writing your logging data to the right file.

Returns void: No values returned
*/
void StartLogging()
{
	output = fopen("log.txt", "w");
}

/*
EndLogging: Closes the logging file properly

Arguments:
None

Pretty much just a wrapper for fclose so that we make sure you close the right file.

Returns void: No values returned
*/
void EndLogging()
{
	fclose(output);
}

void printBoard (int** currentBoard){
	cout << endl;
	cout << currentBoard[0][0] << " " << currentBoard[1][0] << " " << currentBoard[2][0] << " " << endl;
	cout << currentBoard[0][1] << " " << currentBoard[1][1] << " " << currentBoard[2][1] << " " << endl;
	cout << currentBoard[0][2] << " " << currentBoard[1][2] << " " << currentBoard[2][2] << " " << endl;
}

int** copyBoard (int** currentBoard) {
	int ** newBoard = new int*[3];
	for (int u = 0; u < 3; u++)
	{
		newBoard[u] = new int[3];

		for (int v = 0; v < 3; v++)
		{
			newBoard[u][v] = 0;
		}
	}

	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			newBoard[i][j] = currentBoard[i][j];
		}
	}
	return newBoard;
}

bool GoalTest(int** current) {
	bool misplaced = false;
	for(int i = 0; i < 3; i++){
		if(current[i][0] != i) {
			misplaced = true;
		}
	}
	for(int i = 3; i < 6; i++){
		if(current[i-3][1] != i) {
			misplaced = true;
		}
	}
	for(int i = 6; i < 9; i++){
		if(current[i-6][2] != i) {
			misplaced = true;
		}
	}

	return !misplaced;
}

int heuristic(int** currentBoard) {
	int misplaced = 0;
	for(int i = 0; i < 3; i++){
		if(currentBoard[i][0] != i) {
			misplaced++;
		}
	}
	for(int i = 3; i < 6; i++){
		if(currentBoard[i-3][1] != i) {
			misplaced++;
		}
	}
	for(int i = 6; i < 9; i++){
		if(currentBoard[i-6][2] != i) {
			misplaced++;
		}
	}

	return misplaced;
}

position GetBlankPosition(int** currentBoard) {
	position pos;
	for(int j = 0; j < 3; j++) {
		for(int i = 0; i < 3; i++) {
			//cout << "(" << i << ", " << j << ")"<< " = " << currentBoard[i][j] << endl;
			if(currentBoard[i][j] == 0) {
				position pos;
				pos.a = i;
				pos.b = j;
			}
		}
	}
	return pos;
}

int** moveRight(int** currentBoard) {
	position blankPosition = GetBlankPosition(currentBoard);
	int currentRight = currentBoard[blankPosition.a+1][blankPosition.b];
	currentBoard[blankPosition.a+1][blankPosition.b] = 0;
	currentBoard[blankPosition.a][blankPosition.b] = currentRight;
	//printBoard();
	return currentBoard;
}

int** moveLeft(int** currentBoard) {
	position blankPosition = GetBlankPosition(currentBoard);
	int currentLeft = currentBoard[blankPosition.a-1][blankPosition.b];
	currentBoard[blankPosition.a-1][blankPosition.b] = 0;
	currentBoard[blankPosition.a][blankPosition.b] = currentLeft;
	//printBoard();
	return currentBoard;
}

int** moveUp(int** currentBoard) {
	position blankPosition = GetBlankPosition(currentBoard);
	int currentUp = currentBoard[blankPosition.a][blankPosition.b-1];
	currentBoard[blankPosition.a][blankPosition.b-1] = 0;
	currentBoard[blankPosition.a][blankPosition.b] = currentUp;
	//printBoard();
	return currentBoard;
}

int** moveDown(int** currentBoard) {
	position blankPosition = GetBlankPosition(currentBoard);
	int currentDown = currentBoard[blankPosition.a][blankPosition.b+1];
	currentBoard[blankPosition.a][blankPosition.b+1] = 0;
	currentBoard[blankPosition.a][blankPosition.b] = currentDown;
	//printBoard();
	return currentBoard;
}

vector<node> CreateChildren(node current, int g) {
	vector<node> children;
	position blankPosition = GetBlankPosition(current.board);
	if(blankPosition.a > 0) {
		node left;
		left.board = copyBoard(current.board);
		left.board = moveLeft(left.board);
		left.f = heuristic(left.board) + g;
		left.g = g++;
		children.push_back(left);
	}
	if(blankPosition.a < 2) {
		node right;
		right.board = copyBoard(current.board);
		right.board = moveRight(right.board);
		right.f = heuristic(right.board) + g;
		right.g = g++;
		children.push_back(right);
	}
	if(blankPosition.b > 0) {
		node up;
		up.board = copyBoard(current.board);
		up.board = moveUp(up.board);
		up.f = heuristic(up.board) + g;
		up.g = g++;
		children.push_back(up);
	}
	if(blankPosition.b < 2) {
		node down;
		down.board = copyBoard(current.board);
		down.board = moveDown(down.board);
		down.f = heuristic(down.board) + g;
		down.g = g++;
		children.push_back(down);
	}
	return children;
}

bool solve() {
	priority_queue<node> frontier;
	set<node> explored;
	set<node> inFrontier;

	node start;
	start.board = board;
	start.g = 0;
	start.f = 0
	frontier.push(start);
	inFrontier.insert(start);

	//while(frontier.size() > 0) {
	for(int k = 0; k < 10; k++) {
		node current = frontier.top();
		cout << endl << "Current Board:";
		printBoard(current.board);
		frontier.pop();
		inFrontier.erase(current);
		if(GoalTest(current.board)){
			return true;
		}
		explored.insert(current);
		if(current.children.size() == 0) {
			current.children = CreateChildren(current, current.g);
		}
		for(int i = 0; i < current.children.size(); i++) {
			frontier.push(current.children[i]);
			cout << endl << "Child:";
			printBoard(current.children[i].board);
			cout << "F: " << current.children[i].f << endl;
			//node childInFrontier = inFrontier.find(current.children[i]);
			if(explored.find(current.children[i]) == explored.end() && inFrontier.find(current.children[i]) == inFrontier.end()) {
				frontier.push(current.children[i]);
			}/*else if(!(childInFrontier == inFrontier.end()) && current.children[i].f > childInFrontier) {
				childInFrontier = current.childrent[i];
			}*/
		}
	}

	return false;
}

int main(int argc, char** argv)
{
	//Setup
	//-----------------------------------
	if (argc == 2)
	{
		if (!ReadBoardFile(argv[1]))
			return 2;
	}
	else
	{
		printf("No file argument given...\n");
		return 1;
	}
	StartLogging();
	//-------------------------------------


	solve();

	//Cleanup
	//-------------------------------------
	EndLogging();
	getchar();
	//-------------------------------------

	return 0;
}

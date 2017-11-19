#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include <string.h>

#include <iostream>
#include <vector>

using namespace std;

#define EMPTY 0
#define X -1
#define O 1

int playerTurn;
int xCount = 0;
int oCount = 0;

struct node {
	int board[4][4][4];
	vector<node> children;
};

struct Move {
	int x;
	int y;
	int z;
};

vector<Move> moveList;

int Won(int board[4][4][4]);

/*
	read_from_file: reads the board from a file
	args:
		char* inputfilename: filename to be read
		int board[][4][4]: array to store board

	returns: 0 if all is well; -2 if the file cannot be opened for reading
*/

int read_from_file( char* inputfilename, int board[][4][4] );

/*
	write_to_file: writes the board from a file
	args:
		char* outputfilename: filename to be written
		int board[4][4][4]: array to store board

	returns: 0 if all is well; -2 if the file cannot be opened for writing
*/

int write_to_file( char* outputfilename, int board[4][4][4] );

/*
	print_move_to_screen: prints the board so that the autograder should be able to read your answer
	args:
		Move m: struct storing the x,y,z coordinate of the square to move to

	returns: void
*/
void print_move_to_screen( Move m);

vector<node> generate_moves(node board, int turn, int ply);

void print_children(node board);

int minimax(node board, int depth, bool maximizing);

int heuristic(node board, int turn);

int main( int argc, char* argv[] ) {
	char* inputfilename = NULL, *outputfilename = NULL;
	node currentBoard;
	Move m = {0,0,0};
	int ply = 0;

	// parse command-line arguments
	for( int i = 1; i < argc; i++ )
	{
		// input file
		if( !strcmp(argv[i], "-i" ) )
		{
			inputfilename = argv[i+1];
			i++;
		}
		// output file
		else if( !strcmp( argv[i], "-o") )
		{
			outputfilename = argv[i+1];
			i++;
		}
		// number of ply to search ahead
		else if( !strcmp( argv[i], "-p") )
		{
			ply = atoi(argv[i+1]);
			i++;
		}
	}

	// check to make sure command-line arguments have been specified
	if( inputfilename == NULL || outputfilename == NULL )
	{
		printf( "input and output filenames need to be specified on the command line (-i <filename> -o <filename>\n");
		return -1;
	}

	if( ply <= 0 )
	{
		printf( "need to have ply set to be greater than 0 (use -p <ply>)\n");
		return -1;
	}

	// debug info
	//printf( "input file: [%s]\n", inputfilename);
	//printf( "output file: [%s]\n", outputfilename);

	// read from file
	read_from_file( inputfilename, currentBoard.board );

	// debug
	//printf( "starting tictactoe\n");

	//sleep(5);

	//debug into
	//printf( "finishing tictactoe\n");

	//cout << "Player Turn: " << playerTurn << endl;

	currentBoard.children = generate_moves(currentBoard, playerTurn, ply);

	int moveScore = -1000;
	node newBoard;
	for(int i = 0; i < currentBoard.children.size(); i++) {
		int minimaxScore = minimax(currentBoard.children[i], ply, true);
		//cout << "Minimax Score: " << minimaxScore << endl;
		if(moveScore < minimaxScore) {
			moveScore = minimaxScore;
			m = moveList[i];
			newBoard = currentBoard.children[i];
		}
	}

	//cout << "Selected Move Score: " << moveScore << endl;

	//print_children(currentBoard);

	//cout << "Winner: " << Won(currentBoard.board) << endl;

	print_move_to_screen( m );

	//write board state to file and exit
	return write_to_file(outputfilename, newBoard.board);
}

int heuristic(node board, int turn) {
	int wonBoard = Won(board.board);
	if(wonBoard == turn) {
		return 100;
	}else if(Won(board.board) == -1 * turn) {
		return -100;
	}

	int oRow = 0;
	int xRow = 0;

	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			int oCurrent = 0;
			int xCurrent = 0;
			for(int k = 0; k < 4; k++) {
				if(board.board[k][j][i] == O) {
					oCurrent++;
				}else if(board.board[j][k][i]) {
					xCurrent++;
				}
			}
			if(oCurrent > 1) {
				oRow += 2;
				if(oCurrent > 2) {
					oRow += 10;
				}
			}
			if(xCurrent > 1) {
				xRow += 2;
				if(xCurrent > 2) {
					xRow += 10;
				}
			}
		}
	}

	if(turn == O) {
		return oRow - xRow;
	}else {
		return xRow - oRow;
	}

	return 0;
}

int minimax(node board, int depth, bool maximizing) {
	if(depth == 0 || board.children.size() == 0) {
		int value = heuristic(board, playerTurn);
		//cout << "Heuristic" << endl;
		if(value > -10)
		return value;
	}

	if(maximizing) {
		int bestValue = -100;
		for(auto& child : board.children) {
			int v = minimax(child, depth-1, false);
			bestValue = max(bestValue, v);
		}
		//cout << "Maximizing" << endl;
		return bestValue;
	}else {
		int bestValue = 100;
		for(auto& child : board.children) {
			int v = minimax(child, depth-1, true);
			bestValue = min(bestValue, v);
		}
		//cout << "Minimizing" << endl;
		return bestValue;
	}
}

void print_children(node board) {
	cout << "Printing children" << endl;
	vector<node> childrenVector = board.children;
	for(auto& children : childrenVector) {
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				for(int k = 0; k < 4; k++) {
					cout << children.board[i][j][k] << " ";
				}
				cout << endl;
			}
			cout << endl << endl;
		}
		cout << endl << endl << endl;
	}
}

void copyBoard(int board1[4][4][4], int board2[4][4][4]) {
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			for(int k = 0; k < 4; k++) {
				board2[i][j][k] = board1[i][j][k];
			}
		}
	}
}

vector<node> generate_moves(node board, int turn, int ply) {
	vector<node> children;
	if(ply > 0) {
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				for(int k = 0; k < 4; k++) {
					if(board.board[i][j][k] == EMPTY) {
						node newBoard;
						copyBoard(board.board, newBoard.board);
						newBoard.board[i][j][k] = turn;
						newBoard.children = generate_moves(newBoard, turn*-1, ply-1);
						Move newMove;
						newMove.x = i;
						newMove.y = j;
						newMove.z = k;
						moveList.push_back(newMove);
						children.push_back(newBoard);
						//cout << "new board " << newBoard.board[i][j][k] << endl;
					}
				}
			}
		}
	}
	return children;
}

int Won(int board[4][4][4]) {
  if(board[0][0][0] == board[1][1][1] && board[0][0][0] == board[2][2][2] && board[0][0][0] == board[3][3][3] && board[0][0][0] != EMPTY) {
    return board[0][0][0];
  }else if(board[0][0][3] == board[1][1][2] && board[0][0][3] == board[2][2][1] && board[0][0][3] == board[3][3][0] && board[0][0][3] != EMPTY) {
    return board[0][0][3];
  }else if(board[3][0][0] == board[2][1][1] && board[3][0][0] == board[1][2][2] && board[3][0][0] == board[0][3][3] && board[3][0][0] != EMPTY) {
    return board[3][0][0];
  }else if(board[3][0][3] == board[2][1][2] && board[3][0][3] == board[1][2][1] && board[3][0][3] == board[0][3][0] && board[3][0][3] != EMPTY) {
    return board[3][0][3];
  }
	for(int j = 0; j < 4; j++) {
		//Diagonal
		if(board[0][0][j] == board[1][1][j] && board[0][0][j] == board[2][2][j] && board[0][0][j] == board[3][3][j] && board[0][0][j] != EMPTY) {
			return board[0][0][j];
		}else if(board[3][0][j] == board[2][1][j] && board[3][0][j] == board[1][2][j] && board[3][0][j] == board[0][3][j] && board[3][0][j] != EMPTY) {
			return board[3][0][j];
		}

		for(int i = 0; i < 4; i++) {
			//checks columns
	    if(board[i][0][j] == board[i][1][j] && board[i][0][j] == board[i][2][j] && board[i][0][j] == board[i][3][j] && board[i][0][j] != EMPTY) {
	      return board[i][0][j];
	    }

			//checks rows
			if(board[0][i][j] == board[1][i][j] && board[0][i][j] == board[2][i][j] && board[0][i][j] == board[3][i][j] && board[0][i][j] != EMPTY) {
	      return board[0][i][j];
	    }
		}
	}

	return 0;
}

int read_from_file( char* inputfilename, int board[][4][4] ) {
	FILE *ifile = fopen( inputfilename, "r" );
	if( !ifile )
	{
		printf( "could not open input file [%s] for reading\n", inputfilename );
		return -2;
	}

	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			for( int k = 0; k < 4; k++ )
			{
				char c = '.';
				fscanf(ifile, " %c", &c );
				switch( c )
				{
					case '.':
						board[k][j][i] = EMPTY;
						break;
					case 'x':
						board[k][j][i] = X;
						xCount++;
						break;
					case 'o':
						board[k][j][i] = O;
						oCount++;
						break;
					default:
						board[k][j][i] = EMPTY;
						break;
				}

			}
		}
	}

	if(oCount > xCount) {
		playerTurn = X;
	}else {
		playerTurn = O;
	}
	fclose( ifile );
	return 0;
}

int write_to_file( char* outputfilename, int board[4][4][4] ) {
	FILE *ofile = fopen( outputfilename, "w" );
	if( !ofile )
	{
		printf( "could not open output file [%s] for writing\n", outputfilename );
		return -2;
	}

	// iterate through all squares on the board
	for( int i = 0; i < 4; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			for( int k = 0; k < 4; k++ )
			{
				// write the appropriate character to the file
				switch( board[k][j][i])
				{
					case EMPTY: fprintf( ofile, "%c", '.'); break;
					case O: fprintf( ofile, "%c", 'o'); break;
					case X: fprintf( ofile, "%c", 'x'); break;
					default: fprintf( ofile, "%c", '.'); break;
				}
			}
			fprintf( ofile, "\n");
		}
		fprintf( ofile, "\n");
	}

	// close the output file
	fclose (ofile);
	return 0;
}

void print_move_to_screen( Move m) {
	printf( "%d %d %d\n", m.x, m.y, m.z);
}

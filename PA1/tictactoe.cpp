#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include <string.h>

#include <iostream>

using namespace std;

#define EMPTY 0
#define X -1
#define O 1

int playerTurn;

int xCount = 0;
int oCount = 0;

struct Move {
	int x;
	int y;
	int z;
};

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

int main( int argc, char* argv[] )
{
	char* inputfilename = NULL, *outputfilename = NULL;
	int board[4][4][4];
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
	read_from_file( inputfilename, board );

	// debug
	//printf( "starting tictactoe\n");

	//sleep(5);

	//debug into
	//printf( "finishing tictactoe\n");

	cout << "Player Turn: " << playerTurn << endl;

	cout << "Winner: " << Won(board) << endl;

	print_move_to_screen( m );

	//write board state to file and exit
	return write_to_file(outputfilename, board);
}

int Won(int board[4][4][4]) {
	for(int j = 0; j < 4; j++) {
		//Diagonal
		cout << "Diagonal 1: " << board[0][0][j] << board[1][1][j] << board[2][2][j] << board[3][3][j] << endl;
		cout << "Diagonal 2: " << board[3][0][j] << board[2][1][j] << board[1][2][j] << board[0][3][j] << endl;
		if(board[0][0][j] == board[1][1][j] == board[2][2][j] == board[3][3][j] != EMPTY) {
			if(board[0][0][j] != EMPTY) {
				cout << "D1" << endl;
				return board[0][0][j];
			}
		}else if(board[3][0][j] == board[2][1][j] == board[1][2][j] == board[0][3][j] != EMPTY) {
			if(board[3][0][j] != EMPTY) {
				cout << "D2" << endl;
				return board[3][0][j];
			}
		}

		for(int i = 0; i < 4; i++) {
			//checks columns
	    if(board[j][0][i] == board[j][1][i] == board[j][2][i] == board[j][3][i] != EMPTY) {
	      return board[j][0][i];
	    }

			//checks rows
			if(board[0][j][i] == board[1][j][i] == board[2][j][i] == board[3][j][i] != EMPTY) {
	      return board[0][j][i];
	    }
		}
	}

	return 0;
}

int read_from_file( char* inputfilename, int board[][4][4] )
{
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
					case '.': board[i][j][k] = EMPTY;
										break;
					case 'x': board[i][j][k] = X;
										xCount++;
										break;
					case 'o': board[i][j][k] = O;
										oCount++;
										break;
					default: board[i][j][k] = EMPTY;
										break;
				}

			}
		}
	}

	if(oCount > xCount) {
		playerTurn = X;
	}

	fclose( ifile );
	return 0;
}

int write_to_file( char* outputfilename, int board[4][4][4] )
{
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
				switch( board[i][j][k])
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

void print_move_to_screen( Move m)
{
	printf( "%d %d %d\n", m.x, m.y, m.z);
}

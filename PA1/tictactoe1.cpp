#include <iostream>
#include <fstream>

using namespace std;

#define EMPTY 0
#define X 1
#define O 2

//board - x, y, z
int inputBoard [4][4][4];

bool WonRow(int column, int id) {
  for(int i = 0; i < 4; i++) {
    if(board[column][0][i] == board[column][1][i] == board[column][2][i] == board[column][3][i] == id) {
      return true;
    }/*else if(board[column][i][0] == board[column][i][1] == board[column][i][2] == board[column][i][3] == id) {
      return true;
    }*/
  }
  return false;
}

bool WonColumnn(int row, int id) {
  for(int i = 0; i < 4; i++) {
    if(board[0][row][i] == board[1][row][i] == board[2][row][i] == board[3][row][i] == id) {
      return true;
    }
  }
  return false;
}

bool WonDiagonal(int id) {
  for(int i = 0; i < 4; i++) {
    if(board[0][0][i] == board[1][1][i] == board[2][2][i] == board[3][3][i] == id) {
      return true;
    }else if(board[3][0][i] == board[2][1][i] == board[1][2][i] == board[0][3][i] == id) {
      return true;
    }
  }
}

/*bool WinTest(int board [4][4][4]) {
  bool won = false;
  if(board[0][0][0] == board[1][0][0] == board[2][0][0] == board[3][0][0]) {
    won = true;
  }
  for(int i = 0; i < 4; i++) {
    if(board[i][0][0] == board[i][1][0] == board[i][2][0] == board[i][3][0]) {
      won = true;
    }else if(board[i][0][0] == board[i][0][1] == board[i][0][2] == board[i][0][3]) {
      won = true;
    }else if(board[i][0][0] == board[i][0][0] == board[i][0][0] == board[i][0][0]) {
      won = true;
    }
  }
  return false;
}*/

int main(int argc, char ** argv) {
  WinTest(inputBoard);
  return 0;
}

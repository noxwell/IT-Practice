#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define ERR_INVALID_ARGS 1
#define ERR_PLAYER_CHEATS 2
#define ERR_PLAYER1_CHEATS 2
#define ERR_PLAYER2_CHEATS 3
const int maxStep = 100;
char field[2][15][10]; //player's fields
int step;
int score[2];
int vx, vy;
string playerExe[2];
ifstream ifs;
ofstream logFile[2], ofs;

void usage()
{
	cout << "Squares 2014 Judge checker\n";
	cout << "Usage: checker.exe [player_1_executable] [player_2_executable]\n";
}

void LogStep(int player, int stepType, char competitorField[][10],  char myField[][10])
{
	logFile[player] << "Step " << step << ":\n<input.txt>\n";
	//print input file given to player
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			logFile[player] << field[player][i][j];
		}
		logFile[player] << endl;
	}
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			logFile[player] << field[(player + 1) % 2][i][j];
		}
		logFile[player] << endl;
	}
	logFile[player] << "<output.txt>\n";
	//print output file returned by player
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			logFile[player] << competitorField[i][j];
		}
		logFile[player] << endl;
	}
	if (stepType == 3)
	{
		logFile[player] << vx << " " << vy << endl;
	}
	else
	{
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				logFile[player] << myField[i][j];
			}
			logFile[player] << endl;
		}
	}
}

void InitPlayer(int player)
{
	//prepare log files
	if (player == 0) logFile[player].open("logfile_1.txt", ofstream::out);
	else logFile[player].open("logfile_2.txt", ofstream::out);
	//reset field and score
	score[player] = 0;
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			field[player][i][j] = '.';
		}
		ofs << endl;
	}
}

void ExecPlayer(int player)
{
	ofs.open("input.txt", ofstream::out);
	for (int r = 0; r < 2; r++)
	{
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				ofs << field[(player + r) % 2][i][j];
			}
			ofs << endl;
		}
	}
	ofs.close();
	system(playerExe[player].c_str());
}

int ValidateField(char oldField[][10], char newField[][10])
{
	//validate symbols
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (newField[i][j] != '.' && newField[i][j] != 'R' && newField[i][j] != 'G' && newField[i][j] != 'B' && newField[i][j] != 'Y' && newField[i][j] != 'O') return -1;
		}
	}
	//check if we trying to fill competitor's field
	bool unfilled = false;
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (oldField[i][j] == '.') unfilled = true;
		}
	}
	if (unfilled)
	{
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (newField[i][j] == '.' || (oldField[i][j] != '.') && (oldField[i][j] != newField[i][j])) return -1;
			}
		}
		return 0;
	}
	//check that numbers of element of each colour didn't change
	int counter[2][5];
	memset(counter, 0, sizeof counter);
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (oldField[i][j] == 'R') counter[0][0]++;
			if (oldField[i][j] == 'G') counter[0][1]++;
			if (oldField[i][j] == 'B') counter[0][2]++;
			if (oldField[i][j] == 'Y') counter[0][3]++;
			if (oldField[i][j] == 'O') counter[0][4]++;
		}
	}
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (newField[i][j] == 'R') counter[1][0]++;
			if (newField[i][j] == 'G') counter[1][1]++;
			if (newField[i][j] == 'B') counter[1][2]++;
			if (newField[i][j] == 'Y') counter[1][3]++;
			if (newField[i][j] == 'O') counter[1][4]++;
		}
	}
	for (int i = 0; i < 5; i++)
		if (counter[0][i] != counter[1][i])
			return -1;
	//check if we want to shuffle row
	int changed = 0;
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (oldField[i][j] != newField[i][j])
			{
				changed++;
				vx = i;
				break;
			}
		}
	}
	if (changed == 0) return 0; //if we didn't anything
	if (changed == 1) return 1;
	//check if we want to shuffle column
	changed = 0;
	for (int j = 0; j < 10; j++)
	{
		for (int i = 0; i < 15; i++)
		{
			if (oldField[i][j] != newField[i][j])
			{
				changed++;
				vy = j;
				break;
			}
		}
	}
	if (changed == 1) return 2;
	return -1;
}

int neighbours[][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

int RecursiveRemove(char myField[][10], int x, int y, char colour)
{
	if (x < 0 || x >= 15 || y < 0 || y >= 10 || myField[x][y] != colour || myField[x][y] == '.') return 0;
	myField[x][y] = '.';
	int res = 1;
	for (int i = 0; i < 4; i++)
		res += RecursiveRemove(myField, x + neighbours[i][0], y + neighbours[i][1], colour);
	return res;
}

void Fall(char myField[][10])
{
	for (int j = 0; j < 10; j++)
	{
		for (int i = 13; i >= 0; i--)
		{
			while (i < 14 && myField[i][j] != '.' && myField[i + 1][j] == '.')
			{
				swap(myField[i][j], myField[i + 1][j]);
				i++;
			}
		}
	}
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		//print help
		usage(); 
		return ERR_INVALID_ARGS;
	}

	//prepare environment
	for (int i = 0; i < 2; i++)
	{
		playerExe[i] = argv[i + 1];
		InitPlayer(i);
	}

	step = 0;
	char myField[15][10], competitorField[15][10], tc; //where we save player's steps
	int stepType, ti;

	while (step < maxStep)
	{
		for (int r = 0; r < 2; r++)
		{
			ExecPlayer(r); //prepare input for player and execute him
			//read competitor field
			ifs.open("output.txt", ifstream::in);
			for (int i = 0; i < 15; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					ifs >> competitorField[i][j];
				}
			}
			stepType = ValidateField(field[(r + 1) % 2], competitorField);
			if (stepType != 0) //if player did something wrong
			{
				return ERR_PLAYER_CHEATS + r;
			}
			//check if we want to remove connected area
			int filePos = ifs.tellg(); //save current position in file
			ifs >> tc; //read one symbol
			ifs.seekg(filePos); //restore current position in file
			if (tc >= '0' && tc <= '9') //if that symbol was digit - we want to remove connected area
			{
				ifs >> vx >> vy;
				if (vx < 1 || vx > 15 || vy < 1 || vy > 10) return ERR_PLAYER_CHEATS + r;
				stepType = 3;
				memcpy(myField, field[r], sizeof myField);
				ti = RecursiveRemove(myField, vx - 1, vy - 1, field[r][vx - 1][vy - 1]);
				score[r] += ti * ti;
				Fall(myField);
			}
			else
			{
				//read player's field
				for (int i = 0; i < 15; i++)
				{
					for (int j = 0; j < 10; j++)
					{
						ifs >> myField[i][j];
					}
				}
				stepType = ValidateField(field[r], myField);
				if (stepType == -1 && step != 0) //if player did something wrong
				{
					return ERR_PLAYER_CHEATS + r;
				}
			}
			ifs.close();
			LogStep(r, stepType, competitorField, myField);
			//save modified fields
			memcpy(field[r], myField, sizeof field[r]);
			memcpy(field[(r + 1) % 2], competitorField, sizeof field[r]);
		}
		step++;
	}
	if (score[0] > score[1])
	{
		cout << "Player 1 Wins!\n";
		logFile[0] << "You Win!";
		logFile[1] << "You Loose! :(";
	}
	else
	{		
		cout << "Player 2 Wins!\n";
		logFile[1] << "You Win!";
		logFile[0] << "You Loose! :(";
	}
	cout << "Score: " << score[0] << " " << score[1];
	logFile[0] << endl << "Score: " << score[0] << " " << score[1];
	logFile[1] << endl << "Score: " << score[0] << " " << score[1];
}
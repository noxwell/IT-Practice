#include <cstdio>
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

char f[2][15][10];

char random_color()
{
	int rnd = rand() % 5;
	if (rnd == 0) return 'R';
	if (rnd == 1) return 'G';
	if (rnd == 2) return 'B';
	if (rnd == 3) return 'Y';
	if (rnd == 4) return 'O';
}

int main()
{
	srand(time(0));
	freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);

	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			cin >> f[0][i][j];
		}
	}
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			cin >> f[1][i][j];
			if (f[1][i][j] == '.') f[1][i][j] = random_color();
		}
	}
	int step_type = rand() % 10;

	if (step_type == 0)
	{
		int rnd = rand() % 10;
		for (int i = 0; i < 100; i++)
			swap(f[0][rand() % 15][rnd], f[0][rand() % 15][rnd]);
	}
	if (step_type == 1)
	{
		int rnd = rand() % 15;
		for (int i = 0; i < 100; i++)
			swap(f[0][rnd][rand() % 10], f[0][rnd][rand() % 10]);
	}
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			cout << f[1][i][j];
		}
		cout << endl;
	}
	if (step_type >= 2)
	{
		cout << (rand() % 15 + 1) << " " << (rand() % 10 + 1);
		return 0;
	}
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			cout << f[0][i][j];
		}
		cout << endl;
	}
	return 0;
}
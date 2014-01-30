/* Printer.h - 
** Written by Nicholas Denaro
*/

#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>
using namespace std;

class Printer
{
public:
	ostream& operator <<(const char* cs);

	ostream& operator <<(const string& s);

	static void MoveBackLine(int count=1);

	static void ClearLine();

	static string CreateColor(string text, string bg);

	static string GetBGColor(const string s);

	static int StringToInt(string s);

	static Printer pout;
};
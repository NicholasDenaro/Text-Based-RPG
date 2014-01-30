/* Printer.cpp
** Written by Nicholas Denaro
*/

#include "Printer.h"
using namespace std;

ostream& Printer::operator<<(const char* cs)
{
	string s=cs;
	//cout<<s<<": ";
	HANDLE hConsole;
	hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
		
	while(s.length()>0)
	{
		int i=s.find('#');
		int j=s.find('#',i+3);
		string phrase;
		stringstream ss;
		if((i!=string::npos)&&(j!=string::npos))
		{
			phrase=s.substr(i+4,j-(i+4));
			int val;
			ss<<s.substr(i+1,3);
			ss>>val;
			SetConsoleTextAttribute(hConsole,val);
			s=s.substr(j);
		}
		else if(i!=string::npos)
		{
			phrase=s.substr(i+4);
			int val;
			ss<<s.substr(i+1,3);
			ss>>val;
			SetConsoleTextAttribute(hConsole,val);
			s="";
		}
		else
		{
			phrase=s;
			s="";
		}
		cout<<phrase;
	}
	SetConsoleTextAttribute(hConsole,15);
	return(cout);
}

ostream& Printer::operator <<(const string& s)
{
	return(Printer::operator <<(s.c_str()));
}

void Printer::ClearLine()
{
	CONSOLE_SCREEN_BUFFER_INFO SBInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&SBInfo);
	cout<<string(SBInfo.dwMaximumWindowSize.X,' ');
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), SBInfo.dwCursorPosition);
}

void Printer::MoveBackLine(int count)
{
	CONSOLE_SCREEN_BUFFER_INFO SBInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&SBInfo);
	COORD coord=SBInfo.dwCursorPosition;
	coord.Y-=count;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

string Printer::CreateColor(string text, string bg)
{
	stringstream textColor;
	textColor<<text.substr(1,3);
	int textVal;
	textColor>>textVal;
		
	stringstream bgColor;
	bgColor<<bg.substr(1,3);
	int bgVal;
	bgColor>>bgVal;

	int newval=textVal+bgVal;
	stringstream output;
	output<<newval;
	string out;
	output>>out;
	while(out.size()<3)
		out='0'+out;
	return('#'+out);
}

string Printer::GetBGColor(const string s)
{
	//convert string to int
	stringstream ss;
	ss<<s.substr(1,3);
	int bg;
	ss>>bg;

	int bgVal=(bg/16)*16;
	string newVal;
	stringstream output;
	output<<bgVal;//insert new value
	output>>newVal;//int to string
	while(newVal.size()<3)//make sure string is correct size
		newVal='0'+newVal;
	return('#'+newVal);
}

int Printer::StringToInt(string s)
{
	stringstream ss;
	ss<<s;
	int value;
	ss>>value;
	return(value);
}

Printer Printer::pout;
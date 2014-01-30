/* main.cpp TextRPG main entry point
** Written by Nicholas Denaro
*/

#include "GameFlow.h"
using namespace std;

#define DIR "Data\\"

void LoadExternal();

void SetConsoleAttributes();


void main()
{
	//SetConsoleAttributes();
	
	LoadExternal();

	cout<<"Welcome to TextRPG"<<endl;
	GameFlow flow;
	flow.Run();

	system("pause");
}

void LoadExternal()
{
	PlayerRace::LoadRaces(DIR,"Races.xml");
	PlayerClass::LoadClasses(DIR,"Classes.xml");
	Monster::LoadMonsters(DIR,"Monsters.xml");
	Monster::LoadBosses(DIR,"Bosses.xml");
	Item::LoadItems(DIR,"Items.xml");
}

void SetConsoleAttributes()
{
	/*HANDLE outcon = GetStdHandle(STD_OUTPUT_HANDLE);//you don't have to call this function every time

    CONSOLE_FONT_INFOEX font;//CONSOLE_FONT_INFOEX is defined in some windows header
    GetCurrentConsoleFontEx(outcon, false, &font);//PCONSOLE_FONT_INFOEX is the same as CONSOLE_FONT_INFOEX*
    font.dwFontSize.X = 8;
    font.dwFontSize.Y = 8;
    SetCurrentConsoleFontEx(outcon, false, &font);*/
}
#ifndef DPUTILS_H_INCLUDED
#define DPUTILS_H_INCLUDED

#include <bits/stdc++.h>
#include <SDL.h>

using namespace std;

//**************************************************************


const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 720;
const string WINDOW_TITLE = "Dixel Pungeon";
const string mapFile = "Map.txt";

void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();


//**************************************************************
// Defining a cell
struct cell
{
    int x;
    int y;
    int type = 0; // 0 space 1 ground 2 wall 3 player 4 slime 5 exit 6 sword 7 skeleton
    int I; // Coordinates within the internal layout
    int J; // Coordinates within the internal layout
    bool seen = 0;
};

struct entity
{
    int I;
    int J;
    bool agitated;
    int chillOut;
    int lastSeenI;
    int lastSeenJ;
    int VIS;
    int damage = 2;
    int health = 10;
};

#endif // DPUTILS_H_INCLUDED

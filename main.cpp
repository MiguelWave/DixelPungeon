#include <bits/stdc++.h>
#include <SDL.h>
#include "DPUtils.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>

using namespace std;

const int NumberOfMapFiles = 1;
int CurrentMap = 0;
bool Used[NumberOfMapFiles];


int GameStage = 0; //0. menu 1.game 2.inventory 3.gameover 4. win

//Elements
int winCon = 0; //-1 means loss, 0 means game cancelled, 1 means win
int cellDim = 55; // Dimensions of each cells (in pixels)
int playerVIS = 5;
int entityVIS = 3;
int renderDistance = 12;

vector<vector<cell>> cellLayout;
vector<vector<vector<cell>>> Maps;
vector<entity> listOfEntities; // Also works as a turn base
entity player;

void RenderGame(SDL_Window* window, SDL_Renderer* renderer);
void GenerateMap();


// Player movement
void pMoveW (entity &ent);
void pMoveN (entity &ent);
void pMoveS (entity &ent);
void pMoveE (entity &ent);
void pMoveNE(entity &ent);
void pMoveSE(entity &ent);
void pMoveNW(entity &ent);
void pMoveSW(entity &ent);


cell getCellAtXY(int x, int y);
int getTypeAtXY(int x, int y);
bool IsWithinVision(const cell &a, const cell &b, int VIS);
bool checkForWallsBetween(const cell &a, const cell &b);


void RenderMenu(SDL_Window* window, SDL_Renderer* renderer, const int& SelectorPosition);
void RenderGameOver(SDL_Window* window, SDL_Renderer* renderer);
void RenderWin(SDL_Window* window, SDL_Renderer* renderer);
SDL_Texture* loadTexture(const string &file, SDL_Renderer *ren);

void NPCMove(entity &ent);
void moveToLastSeen(entity &ent);
void moveRandom(entity &ent);
void Attack(const entity& attacker, entity& defender);

void DrawHPBar(float CurrentHP, float MaxHP, SDL_Window* window, SDL_Renderer* renderer);

//void GetClick() {}
vector<SDL_Texture*> assets;
vector<SDL_Texture*> MenuAssets;
SDL_Texture* GameOver;
SDL_Texture* Win;



//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[]){
//    // Window initialization
//    SDL_Window* window;
//    SDL_Renderer* renderer;
//    initSDL(window, renderer);
//
//    // Event variable
//    SDL_Event e;
//
//    // Importing map
//    for (int i = 0; i < NumberOfMapFiles; i++) {
//        Used[i] = false;
//    }
//    GenerateMap();
//    if (Maps.size() == 0){
//        cout<<"No map detected";
//        return 0;
//    }
//
//    // Importing graphics
//    assets.push_back(loadTexture("Assets/space.bmp", renderer));
//    assets.push_back(loadTexture("Assets/floor1.bmp", renderer));
//    assets.push_back(loadTexture("Assets/wall.bmp", renderer));
//    assets.push_back(loadTexture("Assets/player.bmp", renderer));
//    assets.push_back(loadTexture("Assets/enemy.bmp", renderer));
//    assets.push_back(loadTexture("Assets/exit.bmp", renderer));
//    assets.push_back(loadTexture("Assets/floor2.bmp", renderer));
//    assets.push_back(loadTexture("Assets/wall2.bmp", renderer));
//
//    MenuAssets.push_back(loadTexture("Assets/title.PNG", renderer));
//    MenuAssets.push_back(loadTexture("Assets/start.PNG", renderer));
//    MenuAssets.push_back(loadTexture("Assets/settings.PNG", renderer));
//    MenuAssets.push_back(loadTexture("Assets/quit.PNG", renderer));
//    MenuAssets.push_back(loadTexture("Assets/selector.PNG", renderer));
//
//    GameOver = loadTexture("Assets/game_over.PNG", renderer);
//    Win = loadTexture("Assets/win.PNG", renderer);
//
//    int Selector = 1;
//
//    // Render
//    while (1) {
//        if (GameStage == 0) {
//            RenderMenu(window, renderer, Selector);
//
////             Idling
//            SDL_Delay(10);
//            if ( SDL_WaitEvent(&e) == 0) continue;
//
////             Exit via closing window
//            if (e.type == SDL_QUIT) break;
//
//            // Event of user input
//            if (e.type == SDL_KEYDOWN) {
//                if (e.key.keysym.sym == SDLK_RETURN) {
//                    switch (Selector) {
//                    case 1:
//                        GameStage = 1;
//                        break;
//                    case 2:
//                        break;
//                    case 3:
//                        return 0;
//                    }
//                } else if (e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_UP) {
//                    if (Selector > 1) Selector--;
//                    RenderMenu(window, renderer, Selector);
//                } else if (e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_DOWN) {
//                    if (Selector < 3) Selector++;
//                    RenderMenu(window, renderer, Selector);
//                }
//            }
//        } else if (GameStage == 1) {
//            RenderGame(window, renderer);
//            // Idling
//            SDL_Delay(10);
//            if ( SDL_WaitEvent(&e) == 0) continue;
//
//            // Exit via big red X
//            if (e.type == SDL_QUIT) break;
//
//            // Player input analyzation
//            if (e.type == SDL_KEYDOWN) {
//                bool playermoved = false;
//                if (e.key.keysym.sym == SDLK_ESCAPE) break;     // Exit via ESC
//                else switch (e.key.keysym.sym) {
//                case SDLK_a:
//                    playermoved = 1;
//                    pMoveW(player);
//                    break;
//                case SDLK_d:
//                    playermoved = 1;
//                    pMoveE(player);
//                    break;
//                case SDLK_s:
//                    playermoved = 1;
//                    pMoveS(player);
//                    break;
//                case SDLK_w:
//                    playermoved = 1;
//                    pMoveN(player);
//                    break;
//                case SDLK_e:
//                    playermoved = 1;
//                    pMoveNE(player);
//                    break;
//                case SDLK_q:
//                    playermoved = 1;
//                    pMoveNW(player);
//                    break;
//                case SDLK_c:
//                    playermoved = 1;
//                    pMoveSE(player);
//                    break;
//                case SDLK_z:
//                    playermoved = 1;
//                    pMoveSW(player);
//                    break;
//                case SDLK_SPACE:
//                    playermoved = 1;
//                    break;
//                }
//                if (playermoved) for (int i = 0; i < (int)listOfEntities.size(); i++) NPCMove(listOfEntities[i]);
//                RenderGame(window, renderer);
//            }
//            // Check win condition
//            if (player.health <= 0) winCon = -1;
//            if (CurrentMap == NumberOfMapFiles - 1) winCon = 1;
//
//            if (winCon == -1)   GameStage = 3;
//            else if (winCon == 1)   GameStage = 4;
//        } else if (GameStage == 3) {
//            RenderGameOver(window, renderer);
//            if (SDL_WaitEvent(&e) == 0) continue;
//
//            // Exit via closing window
//            if (e.type == SDL_QUIT) break;
//
//            // Event of user input
//            if (e.type == SDL_KEYDOWN) break;
//        } else if (GameStage == 4) {
//            RenderWin(window, renderer);
//            if (SDL_WaitEvent(&e) == 0) continue;
//
//            // Exit via closing window
//            if (e.type == SDL_QUIT) break;
//
//            // Event of user input
//            if (e.type == SDL_KEYDOWN) break;
//        }
//
//    }
//    switch (winCon) {
//        case -1:
//            cout<<"Game over! Better luck next time.";
//            break;
//        case 0:
//            cout<<"Game cancelled";
//            break;
//        case 1:
//            cout<<"Congratulations, you win!";
//            break;
//    }
    return 0;
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------


// CHECKED!!!!!
void GenerateMap(){
    vector<vector<cell>> NewMap;

    // Get random map
    string path = "maps/map";
    srand((unsigned) time(NULL));
    int random = rand() % NumberOfMapFiles;
    while (Used[random]) random = rand() % NumberOfMapFiles;
    path += to_string(random);
    path += ".txt";
    Used[random] = true;

    // Input from file
    string line;
    int i = 0;
    ifstream inpfile(path);
    cell temp;

    while (!inpfile.eof()){
        // Add a new line to the internal layout
        getline(inpfile, line);
        NewMap.push_back(vector<cell>());

        for (int j = 0; j < int(line.length()); ++j){
            //Coordinates
            temp.x = cellDim * j;
            temp.y = cellDim * i;

            // Internal coordinates
            temp.I = i;
            temp.J = j;

            //Determining cell type
            switch (line[j]){
                case '.':
                    temp.type=1;
                    break;
                case '*':
                    temp.type=2;
                    break;
                case 'Y':
                    temp.type=3;
                    player.I=temp.I;
                    player.J=temp.J;
                    player.agitated=0; //????
                    player.VIS = playerVIS; // To be modified
                    break;
                case 'E':{
                    temp.type=4;
                    entity NewEnt;
                    NewEnt.I=temp.I;
                    NewEnt.J=temp.J;
                    NewEnt.agitated = 0;
                    NewEnt.VIS=entityVIS;
                    listOfEntities.push_back(NewEnt);
                    break;
                }
                case 'O':
                    temp.type=5;
                    break;
            }
            NewMap[i].push_back(temp);
        }
        i++;//Number of lines in the internal layout
    }
    inpfile.close();
    Maps.push_back(NewMap);
}


//----------------------------------------------------------------------------------------------------------------------
//---Game Rendering---
//----------------------------------------------------------------------------------------------------------------------

void RenderMenu(SDL_Window* window, SDL_Renderer* renderer, const int& SelectorPosition) {
    //Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);   //background color
    SDL_RenderClear(renderer);

    // Rendering title
    SDL_Rect temp;
    temp.x = 200;
    temp.y = 100;
    temp.w = SCREEN_WIDTH - 2 * temp.x;
    temp.h = 100;
    SDL_RenderCopy(renderer, MenuAssets[0], NULL, &temp);

    // Rendering Start button
    SDL_Rect Start;
    Start.x = 500;
    Start.y = 250;
    Start.w = SCREEN_WIDTH - 2 * Start.x;
    Start.h = 100;
    SDL_RenderCopy(renderer, MenuAssets[1], NULL, &Start);

    // Rendering Settings button
    SDL_Rect Settings;
    Settings.x = 500;
    Settings.y = 400;
    Settings.w = SCREEN_WIDTH - 2 * Settings.x;
    Settings.h = 100;
    SDL_RenderCopy(renderer, MenuAssets[2], NULL, &Settings);

    // Rendering Quit button
    SDL_Rect Quit;
    Quit.x = 550;
    Quit.y = 550;
    Quit.w = SCREEN_WIDTH - 2 * Quit.x;
    Quit.h = 100;
    SDL_RenderCopy(renderer, MenuAssets[3], NULL, &Quit);

    // Rendering Selector
    switch (SelectorPosition) {
    case 1:
        temp.x = Start.x - 100;
        temp.y = Start.y;
        break;
    case 2:
        temp.x = Settings.x - 100;
        temp.y = Settings.y;
        break;
    case 3:
        temp.x = Quit.x - 100;
        temp.y = Quit.y;
    }
    temp.w = 50;
    temp.h = 100;
    SDL_RenderCopy(renderer, MenuAssets[4], NULL, &temp);
    SDL_RenderPresent(renderer);
}


// CHECKED!!!!!
void RenderGame(SDL_Window* window, SDL_Renderer* renderer) {
    //Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);   // background color: black
    SDL_RenderClear(renderer);

    // Cell check variables
    bool CellIsVisible;
    int AssetID;
    SDL_Rect temp;

    //Redrawing the entire thing from scratch
    for (int i = player.I - renderDistance; i < player.I + renderDistance; ++i)
        for (int j = player.J - renderDistance; j < player.J + renderDistance; ++j) {// Only render from a certain number of cells away
            if ((i >= 0) && (j >= 0)
                && (i < int(Maps[CurrentMap].size()))
                && (j < int(Maps[CurrentMap][i].size()))) continue;// Only render cells in map boundaries

            temp.x = int(SCREEN_WIDTH/2 - cellDim/2 + (j-player.J)*cellDim); //Centered on the player
            temp.y = int(SCREEN_HEIGHT/2 - cellDim/2 + (i-player.I)*cellDim);
            temp.w = cellDim;
            temp.h = cellDim;

            CellIsVisible = (IsWithinVision(Maps[CurrentMap][player.I][player.J], Maps[CurrentMap][i][j], playerVIS)
                    && (!checkForWallsBetween(Maps[CurrentMap][player.I][player.J], Maps[CurrentMap][i][j])));
            AssetID = 0;

            switch (Maps[CurrentMap][i][j].type){
//            case 0:
//                to be added: background elements
            case 1:
                if (CellIsVisible) AssetID = 1;
                else if (Maps[CurrentMap][i][j].seen) AssetID = 6;
                break;
            case 2:
                if (CellIsVisible) AssetID = 2;
                else if (Maps[CurrentMap][i][j].seen) AssetID = 7;
                break;
            case 3:
                AssetID = 3; // To be added: render floor then render player
                break;
            case 4:
                if (CellIsVisible) AssetID = 4;
                else if (Maps[CurrentMap][i][j].seen) AssetID = 6;
                break;
            case 5:
//                if (CellIsVisible)
                    AssetID = 5; // Since we only have 1 asset for the exit, to be added later
//                else if (Maps[CurrentMap][i][j].seen) AssetID = 5;
                break;
            }
            if (AssetID != 0) SDL_RenderCopy(renderer, assets[AssetID], NULL, &temp);
            if (CellIsVisible) Maps[CurrentMap][i][j].seen = 1;
        }

    DrawHPBar(player.health * 1.0, 10, window, renderer);
    SDL_RenderPresent(renderer);
}

void RenderGameOver(SDL_Window* window, SDL_Renderer* renderer) {
    //Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);   //background color
    SDL_RenderClear(renderer);

    // Rendering title
    SDL_Rect temp;
    temp.x = 200;
    temp.y = 100;
    temp.w = SCREEN_WIDTH - 2 * temp.x;
    temp.h = 100;
    SDL_RenderCopy(renderer, GameOver, NULL, &temp);
    SDL_RenderPresent(renderer);
}

void RenderWin(SDL_Window* window, SDL_Renderer* renderer) {
    //Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);   //background color
    SDL_RenderClear(renderer);

    // Rendering title
    SDL_Rect temp;
    temp.x = 200;
    temp.y = 100;
    temp.w = SCREEN_WIDTH - 2 * temp.x;
    temp.h = 100;
    SDL_RenderCopy(renderer, Win, NULL, &temp);
    SDL_RenderPresent(renderer);
}
//----------
// Swap cells
void swapCells(cell &from, cell &to)
{
    cell temp;
    temp.type = from.type;
    from.type = to.type;
    to.type = temp.type;
}

void NextMap(){
    CurrentMap++;
    listOfEntities.clear();
    if (CurrentMap == NumberOfMapFiles) GameStage = 4;
    else GenerateMap();

}



//Movement
void pMoveW(entity &ent){
    int i=ent.I;
    int j=ent.J;

    switch (Maps[CurrentMap][i][j-1].type){
    case 1:
        swapCells(Maps[CurrentMap][i][j], Maps[CurrentMap][i][j-1]);
        ent.J--;
        break;
    case 5:
        NextMap();
        break;
    case 4:
        winCon=-1;
        break;
    }
}
void pMoveE(entity &ent){
    int i=ent.I;
    int j=ent.J;

    switch (Maps[CurrentMap][i][j+1].type){
    case 1:
        swapCells(Maps[CurrentMap][i][j], Maps[CurrentMap][i][j+1]);
        ent.J++;
        break;
    case 5:
        NextMap();
        break;
    case 4:
        winCon=-1;
        break;
    }
}
void pMoveN(entity &ent){
    int i=ent.I;
    int j=ent.J;

    switch (Maps[CurrentMap][i-1][j].type){
    case 1:
        swapCells(Maps[CurrentMap][i][j], Maps[CurrentMap][i-1][j]);
        ent.I--;
        break;
    case 5:
        NextMap();
        break;
    case 4:
        winCon=-1;
        break;
    }
}
void pMoveS(entity &ent){
    int i=ent.I;
    int j=ent.J;

    switch (Maps[CurrentMap][i+1][j].type){
    case 1:
        swapCells(Maps[CurrentMap][i][j], Maps[CurrentMap][i+1][j]);
        ent.I++;
        break;
    case 5:
        NextMap();
        break;
    case 4:
        winCon=-1;
        break;
    }
}
void pMoveNE(entity &ent){
    int i=ent.I;
    int j=ent.J;

    switch (Maps[CurrentMap][i-1][j+1].type){
    case 1:
        swapCells(Maps[CurrentMap][i][j], Maps[CurrentMap][i-1][j+1]);
        ent.I--;
        ent.J++;
        break;
    case 5:
        NextMap();
        break;
    case 4:
        winCon=-1;
        break;
    }
}
void pMoveSE(entity &ent){
    int i=ent.I;
    int j=ent.J;

    switch (Maps[CurrentMap][i+1][j+1].type){
    case 1:
        swapCells(Maps[CurrentMap][i][j], Maps[CurrentMap][i+1][j+1]);
        ent.I++;
        ent.J++;
        break;
    case 5:
        NextMap();
        break;
    case 4:
        winCon=-1;
        break;
    }
}
void pMoveNW(entity &ent){
    int i=ent.I;
    int j=ent.J;

    switch (Maps[CurrentMap][i-1][j-1].type){
    case 1:
        swapCells(Maps[CurrentMap][i][j], Maps[CurrentMap][i-1][j-1]);
        ent.I--;
        ent.J--;
        break;
    case 5:
        NextMap();
        break;
    case 4:
        winCon=-1;
        break;
    }
}
void pMoveSW(entity &ent){
    int i=ent.I;
    int j=ent.J;

    switch (Maps[CurrentMap][i+1][j-1].type){
    case 1:
        swapCells(Maps[CurrentMap][i][j], Maps[CurrentMap][i+1][j-1]);
        ent.I++;
        ent.J--;
        break;
    case 5:
        NextMap();
        break;
    case 4:
        winCon=-1;
        break;
    }
}





// To be made into separate functions (e.g. getTypeXY or getHealthXY) to save time importing other unnecessary stats
cell getCellAtXY(int x, int y){
    return Maps[CurrentMap][int(y / cellDim)][int(x / cellDim)];
}

int getTypeAtXY(int x, int y){
    return Maps[CurrentMap][int(y/cellDim)][int(x/cellDim)].type;
}

bool IsWithinVision(const cell &a, const cell &b, int VIS){
    if ((a.I-b.I)*(a.I-b.I) + (a.J-b.J)*(a.J-b.J) < (VIS*VIS)) return true;
    else return false;
}

// Note: true means theres a wall between, not the other way around
bool checkForWallsBetween(const cell &a, const cell &b)
{
    // VTCP:
    int uX = b.x - a.x;
    int uY = b.y - a.y;

    // Shuttle variables
    int iX;
    int iY;

    // Toggle variable, true means this is the last pixel of that cell, false, not
    bool isLast = false;

    // Taking X into shuttling
    if (uX != 0)
    {
        // Determine the direction of the vector for initialization of shuttle variable
        if (uX>0){
            iX = a.x + cellDim; // iX will be running from the start of the next cell to the final side of the cell adjacent to the destination cell
        } else {
            iX = a.x - 1;
        }

        // Shuttling
        while ( int(iX/cellDim) != b.J ) // While the current query variable isn't in the destination cell
        {
            // Determine Y based on X
            iY = int ( uY * ( iX - (a.x+cellDim/2) ) / uX + (a.y+cellDim/2) );

            // Examine if (x,y) is a wall
            if (getTypeAtXY(iX, iY) == 2) return true;

            // Incrementing X with regards to whether or not the current x is the last side of the cell
            if (isLast) {
                iX += int(uX/abs(uX));
            } else {       /*signum*/
                iX += int( (uX/abs(uX)) * (cellDim-1) );
            }
            isLast=!isLast;
        }
    }

    isLast=false;

    // Taking Y into shuttling
    if (uY != 0)
    {
        // Determine the direction of the vector for initialization of shuttle variable
        if (uY>0) {
            iY = a.y + cellDim; // iX will be running from the start of the next cell to the final side of the cell adjacent to the destination cell
        } else {
            iY = a.y - 1;
        }

        // Shuttling
        while (int(iY/cellDim) != b.I) // While the current query variable isn't in the destination cell
        {
            // Determine X based on Y
            iX = int ( uX * ( iY - (a.y+cellDim/2) ) / uY + (a.x+cellDim/2) );

            // Examine if (x,y) is a wall
            if (getTypeAtXY(iX, iY) == 2) return true;

            // Incrementing Y with regards to whether or not the current y is the last side of the cell
            if (isLast) {
                iY += int(uY/abs(uY));
            } else {       /*signum*/
                iY += int( (uY/abs(uY)) * (cellDim-1) );
            }
            isLast=!isLast;
        }
    }
    return false;
}
//************************1.9.3: Graphics****************************
SDL_Texture* loadTexture(const string &file, SDL_Renderer *ren){
	SDL_Texture *texture = nullptr;
    SDL_Surface *loadedImage = IMG_Load(file.c_str());

	if (loadedImage != nullptr) {
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);
		if (texture == nullptr) {
			logSDLError(cout, "CreateTextureFromSurface");

		SDL_FreeSurface(loadedImage);
		}
	} else {
		logSDLError(cout, "LoadBMP");
	}
	return texture;
}

//************************1.10: Enemy AI*****************************
void NPCMove(entity &ent){
    // Check for player (before moving)
    if ((IsWithinVision(Maps[CurrentMap][ent.I][ent.J], Maps[CurrentMap][player.I][player.J], ent.VIS)) && (!checkForWallsBetween(Maps[CurrentMap][ent.I][ent.J],Maps[CurrentMap][player.I][player.J]) ))
    {
        ent.agitated = 1;
        ent.chillOut = 0;
        ent.lastSeenI = player.I;
        ent.lastSeenJ = player.J;
    }


    if (ent.agitated) {
        if ((abs(ent.J - player.J) <= 1) && (abs(ent.I - player.I) <= 1))// Attack range to be added in place of "1"
        {
            Attack(ent, player);
            return;
        }
        else moveToLastSeen(ent);
        if (ent.chillOut==3)
        {
            ent.agitated = 0;
            ent.chillOut = 0;
        }
    }
    else moveRandom(ent);

    // Check for player (after moving)
    if ((IsWithinVision(Maps[CurrentMap][ent.I][ent.J], Maps[CurrentMap][player.I][player.J], ent.VIS)) && (!checkForWallsBetween(Maps[CurrentMap][ent.I][ent.J],Maps[CurrentMap][player.I][player.J])))
    {
        ent.agitated = 1;
        ent.chillOut = 0;
        ent.lastSeenI = player.I;
        ent.lastSeenJ = player.J;
    }
}

void moveToLastSeen(entity &ent)
{
    int aX = ent.J;
    int aY = ent.I;

    // If the entity has reached the last seen location and it hasn't been reassigned a new location (has not seen the player), move randomly
    if ((aX==ent.lastSeenJ) && (aY==ent.lastSeenI))
    {
        moveRandom(ent);
        ent.chillOut++;
        return;
    }

    // Unit vectors of VTCP, used for 1 cell movement
    int iX;
    if (ent.J!=ent.lastSeenJ){
        iX = int((ent.lastSeenJ - ent.J)/abs(ent.lastSeenJ - ent.J));
    } else {
        iX = 0;
    }

    int iY;
    if (ent.I!=ent.lastSeenI){
        iY = int((ent.lastSeenI - ent.I)/abs(ent.lastSeenI - ent.I));
    } else {
        iY = 0;
    }


    // Go in the direction of the last seen location
    if (Maps[CurrentMap][aY+iY][aX+iX].type==1)
    {
        swapCells(Maps[CurrentMap][aY+iY][aX+iX], Maps[CurrentMap][aY][aX]);
        ent.J+=iX;
        ent.I+=iY;
    }
    else if (Maps[CurrentMap][aY][aX+iX].type==1)
    {
        swapCells(Maps[CurrentMap][aY][aX+iX], Maps[CurrentMap][aY][aX]);
        ent.J+=iX;
    }
    else if (Maps[CurrentMap][aY+iY][aX].type==1)
    {
        swapCells(Maps[CurrentMap][aY+iY][aX], Maps[CurrentMap][aY][aX]);
        ent.I+=iY;
    }
    else
    {
        moveRandom(ent);
        ent.chillOut++;
    }
}

void moveRandom(entity &ent)
{
    int randI = 1 - (rand() % 3); // 1, 0 or -1;
    int randJ = 1 - (rand() % 3);
    switch (Maps[CurrentMap][ent.I+randI][ent.J+randJ].type)
    {
        case 1:
            swapCells(Maps[CurrentMap][ent.I+randI][ent.J+randJ], Maps[CurrentMap][ent.I][ent.J]);
            ent.I+=randI;
            ent.J+=randJ;
            break;
        case 2:
            moveRandom(ent);
            break;
        case 4:
            moveRandom(ent);
            break;
        case 5:
            moveRandom(ent);
            break;
    }
}

void Attack(const entity &attacker, entity& defender) {
    defender.health -= attacker.damage;
}

//------------------------------
void DrawHPBar(float CurrentHP, float MaxHP, SDL_Window* window, SDL_Renderer* renderer) {
    float percentage = CurrentHP / MaxHP;

    const int BarWidth = 200;
    const int BarHeight = 20;
    const int BarThickness = 3;
    const int BarGap = 3;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Outline
    for (int i = 0; i < BarThickness; i++) SDL_RenderDrawLine(renderer, 0, i, BarWidth, i);
    for (int i = 0; i < BarThickness; i++) SDL_RenderDrawLine(renderer, i, 0, i, BarHeight);
    for (int i = BarHeight; i < BarHeight + BarThickness; i++) SDL_RenderDrawLine(renderer, 0, i, BarWidth - BarHeight, i);
    for (int i = BarWidth - BarThickness / 2; i <= BarWidth + BarThickness / 2; i++) SDL_RenderDrawLine(renderer, i, 0, i - BarHeight, BarHeight + BarThickness - 1);

    // Fill hp bar relative to current hp percentage
    for (int i = BarThickness + BarGap; i < BarHeight - BarGap; i++)
        SDL_RenderDrawLine(renderer, BarThickness + BarGap, i,
                           percentage * (BarWidth - BarGap - i - BarThickness/2 - (BarThickness + BarGap)) + (BarThickness + BarGap), i);

//    int TextSize = 24;
//    TTF_Font* TextFont = TTF_OpenFont("fonts\PixelifySans-Regular.ttf", TextSize);
//    SDL_Color TempColor = {255, 255, 255, 255};
//
//    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(TextFont, "test", TempColor);
//
//    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
//
//    SDL_Rect temp;
//    temp.x = 0;
//    temp.y = 0;
//    temp.w = surfaceMessage->w;
//    temp.h = surfaceMessage->h;
//    SDL_RenderCopy(renderer, Message, NULL, &temp);

//    SDL_FreeSurface(surfaceMessage);
//    SDL_DestroyTexture(Message);

//    string inptext = "test";// + std::to_string(score);
//    SDL_Color textColor = { 255, 255, 255, 255 };
//    SDL_Surface* textSurface = TTF_RenderText_Solid(Sans, inptext.c_str(), textColor);
//    SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, textSurface);
//
//    int text_width = textSurface->w;
//    int text_height = textSurface->h;
//    SDL_Rect renderQuad = { 0, 0, text_width, text_height };
//    SDL_RenderCopy(renderer, text, NULL, &renderQuad);
//
//    SDL_DestroyTexture(text);
//    SDL_FreeSurface(textSurface);

    SDL_RenderPresent(renderer);
    return;
}

//bool CheckEntityAlive(entity )

#include <bits/stdc++.h>
#include <SDL.h>
#include "DPUtils.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>

using namespace std;

//struct button{
//    int x;
//    int y;
//    int w;
//    int h;
//    SDL_Texture* Idle;
//    SDL_Texture* Selected;
//
//};

//Elements
int winCon = 0; //-1 means loss, 0 means game cancelled, 1 means win
int cellDim = 55; // Dimensions of each cells (in pixels)
int playerVIS = 5;
int entityVIS = 3;
int renderDistance = 8;


vector<vector<cell>> cellLayout;
vector<entity> listOfEntities; // Also works as a turn base
entity player;

void RenderGame(SDL_Window* window, SDL_Renderer* renderer);
void importLayout(const string &path);


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
bool inSight(const cell &a, const cell &b, int VIS);
bool checkForWallsBetween(const cell &a, const cell &b);


void RenderMenu(SDL_Window* window, SDL_Renderer* renderer, const int& SelectorPosition);
void RenderGameOver(SDL_Window* window, SDL_Renderer* renderer);
SDL_Texture* loadTexture(const string &file, SDL_Renderer *ren);

void NPCMove(entity &ent);
void moveToLastSeen(entity &ent);
void moveRandom(entity &ent);
void NPCAttack(const entity &ent);

void DrawHPBar(float CurrentHP, float MaxHP, SDL_Window* window, SDL_Renderer* renderer);

//void GetClick() {}
vector<SDL_Texture*> assets;
vector<SDL_Texture*> MenuAssets;
SDL_Texture* GameOver;

int main(int argc, char* argv[])
{
    // Window initialization
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);

    DrawHPBar(0.5, 10, window, renderer);
    // Event variable
    SDL_Event e;
//
//    // Importing map
//    importLayout(mapFile);
//
//    if (cellLayout.size()==0){
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
//
//
//    bool GameStarted = false;
//    int GameStage = 0; //0. menu 1.game 2.inventory 3.gameover
//    int Selector = 1;
//
//    // Render
//
    while (1) {
//        if (GameStage == 0) {
//            RenderMenu(window, renderer, Selector);
//
            // Idling
            SDL_Delay(10);
            if ( SDL_WaitEvent(&e) == 0) continue;

            // Exit via closing window
            if (e.type == SDL_QUIT) break;
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
//                if (playermoved) for (int i=0; i<listOfEntities.size(); i++) NPCMove(listOfEntities[i]);
//                RenderGame(window, renderer);
//            }
//            // Check win condition
//            if (winCon!=0)   GameStage = 3;
//        } else if (GameStage == 3) {
//            RenderGameOver(window, renderer);
//            if ( SDL_WaitEvent(&e) == 0) continue;
//
//            // Exit via closing window
//            if (e.type == SDL_QUIT) break;
//
//            // Event of user input
//            if (e.type == SDL_KEYDOWN) break;
//        }
//
    }
//    switch (winCon){
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

void importLayout(const string &path){
    string line;
    int i=0;
    ifstream inpfile(path);


    while (!inpfile.eof()){
        getline(inpfile,line);
        // Add a new line to the internal layout
        cellLayout.push_back(vector<cell>());

        //Analyzing line i
        for (int j=0; j<int (line.length()); ++j){
            cell temp;
            //Coordinates
            temp.x =   ( cellDim   ) * j;
            temp.y =   ( cellDim  ) * i;

            // Internal coordinates
            temp.I=i;
            temp.J=j;

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
                    player.agitated=0;
                    player.VIS = playerVIS;
                    break;
                case 'E':
                    temp.type=4;
                    entity temp2;
                    temp2.I=temp.I;
                    temp2.J=temp.J;
                    temp2.agitated = 0;
                    temp2.VIS=entityVIS;
                    listOfEntities.push_back(temp2);
                    break;
                case 'O':
                    temp.type=5;
                    break;
            }
            cellLayout[i].push_back(temp);
        }
        i++;//Number of lines in the internal layout
    }
    inpfile.close();
}

//---Game Rendering---
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
    temp.x = 400;
    temp.y = 250;
    temp.w = SCREEN_WIDTH - 2 * temp.x;
    temp.h = 100;
    SDL_RenderCopy(renderer, MenuAssets[1], NULL, &temp);

    // Rendering Settings button
    temp.x = 350;
    temp.y = 400;
    temp.w = SCREEN_WIDTH - 2 * temp.x;
    temp.h = 100;
    SDL_RenderCopy(renderer, MenuAssets[2], NULL, &temp);

    // Rendering Quit button
    temp.x = 400;
    temp.y = 550;
    temp.w = SCREEN_WIDTH - 2 * temp.x;
    temp.h = 100;
    SDL_RenderCopy(renderer, MenuAssets[3], NULL, &temp);

    // Rendering Selector
    switch (SelectorPosition) {
    case 1:
        temp.x = 300;
        temp.y = 250;
        break;
    case 2:
        temp.x = 250;
        temp.y = 400;
        break;
    case 3:
        temp.x = 300;
        temp.y = 550;
    }
    temp.w = 50;
    temp.h = 100;
    SDL_RenderCopy(renderer, MenuAssets[4], NULL, &temp);
    SDL_RenderPresent(renderer);
}

void RenderGame(SDL_Window* window, SDL_Renderer* renderer) {
    int pPosI = player.I;
    int pPosJ = player.J;

    //Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);   //background color
    SDL_RenderClear(renderer);

    //Redrawing the entire thing from scratch
    SDL_Rect temp;
    for (int i=player.I-renderDistance; i<player.I+renderDistance; ++i){
        for (int j=player.J-renderDistance; j<player.J+renderDistance; ++j)// Only render from a certain number of cells away
        {
            if ((i>=0)&&(j>=0)&&(i<cellLayout.size())&&(j<cellLayout[i].size()))// Making sure that cell exists *****************
            {
                temp.x=int(SCREEN_WIDTH/2 - cellDim/2 + (j-pPosJ)*cellDim); //Centered on the player
                temp.y=int(SCREEN_HEIGHT/2 - cellDim/2 + (i-pPosI)*cellDim);
                temp.w=cellDim;
                temp.h=cellDim;
                switch (cellLayout[i][j].type){
    //                case 0:
    //                    SDL_RenderCopy(renderer, assets[0], NULL, &temp);
    //                    break;
                    case 1:
                        if (inSight(cellLayout[pPosI][pPosJ], cellLayout[i][j], playerVIS)&&(!checkForWallsBetween(cellLayout[pPosI][pPosJ], cellLayout[i][j])))
                        {
                            SDL_RenderCopy(renderer, assets[1], NULL, &temp);
                            cellLayout[i][j].seen=1;
                        }
                        else
                        {
                            if (cellLayout[i][j].seen==1) SDL_RenderCopy(renderer, assets[6], NULL, &temp);
                        }
                        break;



                    case 2:
                        if (inSight(cellLayout[pPosI][pPosJ], cellLayout[i][j], playerVIS)&&(!checkForWallsBetween(cellLayout[pPosI][pPosJ], cellLayout[i][j])))
                        {
                            SDL_RenderCopy(renderer, assets[2], NULL, &temp);
                            cellLayout[i][j].seen=1;
                        }
                        else
                        {
                            if (cellLayout[i][j].seen==1) SDL_RenderCopy(renderer, assets[7], NULL, &temp);
                        }
                        break;



                    case 3:
                        if (inSight(cellLayout[pPosI][pPosJ], cellLayout[i][j], playerVIS)&&(!checkForWallsBetween(cellLayout[pPosI][pPosJ], cellLayout[i][j])))
                        {
                            SDL_RenderCopy(renderer, assets[3], NULL, &temp);
                            cellLayout[i][j].seen=1;
                        }
                        else
                        {
                            if (cellLayout[i][j].seen==1) SDL_RenderCopy(renderer, assets[3], NULL, &temp);
                        }
                        break;



                    case 4:
                        if (inSight(cellLayout[pPosI][pPosJ], cellLayout[i][j], playerVIS)&&(!checkForWallsBetween(cellLayout[pPosI][pPosJ], cellLayout[i][j])))
                        {
                            SDL_RenderCopy(renderer, assets[4], NULL, &temp);
                            cellLayout[i][j].seen=1;
                        }
                        else
                        {
                            if (cellLayout[i][j].seen==1) SDL_RenderCopy(renderer, assets[6], NULL, &temp);
                        }
                        break;



                    case 5:
                        if (inSight(cellLayout[pPosI][pPosJ], cellLayout[i][j], playerVIS)&&(!checkForWallsBetween(cellLayout[pPosI][pPosJ], cellLayout[i][j])))
                        {
                            SDL_RenderCopy(renderer, assets[5], NULL, &temp);
                            cellLayout[i][j].seen=1;
                        }
                        else
                        {
                            if (cellLayout[i][j].seen==1) SDL_RenderCopy(renderer, assets[5], NULL, &temp);
                        }
                        break;
                }

            }
        }
    }
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
//----------
// Swap cells
void swapCells(cell &from, cell &to)
{
    cell temp;
    temp.type = from.type; from.type = to.type; to.type = temp.type;
}

//Movement
void pMoveW(entity &ent){
    int i=ent.I;
    int j=ent.J;

    switch (cellLayout[i][j-1].type){
    case 1:
        swapCells(cellLayout[i][j], cellLayout[i][j-1]);
        ent.J--;
        break;
    case 5:
        winCon=1;
        break;
    case 4:
        winCon=-1;
        break;
    }
}
void pMoveE(entity &ent){
    int i=ent.I;
    int j=ent.J;

    switch (cellLayout[i][j+1].type){
    case 1:
        swapCells(cellLayout[i][j], cellLayout[i][j+1]);
        ent.J++;
        break;
    case 5:
        winCon=1;
        break;
    case 4:
        winCon=-1;
        break;
    }
}
void pMoveN(entity &ent){
    int i=ent.I;
    int j=ent.J;

    switch (cellLayout[i-1][j].type){
    case 1:
        swapCells(cellLayout[i][j], cellLayout[i-1][j]);
        ent.I--;
        break;
    case 5:
        winCon=1;
        break;
    case 4:
        winCon=-1;
        break;
    }
}
void pMoveS(entity &ent){
    int i=ent.I;
    int j=ent.J;

    switch (cellLayout[i+1][j].type){
    case 1:
        swapCells(cellLayout[i][j], cellLayout[i+1][j]);
        ent.I++;
        break;
    case 5:
        winCon=1;
        break;
    case 4:
        winCon=-1;
        break;
    }
}
void pMoveNE(entity &ent){
    int i=ent.I;
    int j=ent.J;

    switch (cellLayout[i-1][j+1].type){
    case 1:
        swapCells(cellLayout[i][j], cellLayout[i-1][j+1]);
        ent.I--;
        ent.J++;
        break;
    case 5:
        winCon=1;
        break;
    case 4:
        winCon=-1;
        break;
    }
}
void pMoveSE(entity &ent){
    int i=ent.I;
    int j=ent.J;

    switch (cellLayout[i+1][j+1].type){
    case 1:
        swapCells(cellLayout[i][j], cellLayout[i+1][j+1]);
        ent.I++;
        ent.J++;
        break;
    case 5:
        winCon=1;
        break;
    case 4:
        winCon=-1;
        break;
    }
}
void pMoveNW(entity &ent){
    int i=ent.I;
    int j=ent.J;

    switch (cellLayout[i-1][j-1].type){
    case 1:
        swapCells(cellLayout[i][j], cellLayout[i-1][j-1]);
        ent.I--;
        ent.J--;
        break;
    case 5:
        winCon=1;
        break;
    case 4:
        winCon=-1;
        break;
    }
}
void pMoveSW(entity &ent){
    int i=ent.I;
    int j=ent.J;

    switch (cellLayout[i+1][j-1].type){
    case 1:
        swapCells(cellLayout[i][j], cellLayout[i+1][j-1]);
        ent.I++;
        ent.J--;
        break;
    case 5:
        winCon=1;
        break;
    case 4:
        winCon=-1;
        break;
    }
}

// To be made into separate functions (e.g. getTypeXY or getHealthXY) to save time importing other unnecessary stats
cell getCellAtXY(int x, int y){
    int j = int( x / cellDim );
    int i = int( y / cellDim );
    cell temp;
    temp.seen = cellLayout[i][j].seen;
    temp.I = cellLayout[i][j].I;
    temp.J = cellLayout[i][j].J;
    temp.type = cellLayout[i][j].type;
    temp.x = cellLayout[i][j].x;
    temp.y = cellLayout[i][j].y;
    return temp;
}

int getTypeAtXY(int x, int y){
    return cellLayout[int(y/cellDim)][int(x/cellDim)].type;
}

bool inSight(const cell &a, const cell &b, int VIS){
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
        if (uY>0){
            iY = a.y + cellDim; // iX will be running from the start of the next cell to the final side of the cell adjacent to the destination cell
        } else {
            iY = a.y - 1;
        }

        // Shuttling
        while ( int(iY/cellDim) != b.I ) // While the current query variable isn't in the destination cell
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
    if ((inSight(cellLayout[ent.I][ent.J], cellLayout[player.I][player.J], ent.VIS)) && (!checkForWallsBetween(cellLayout[ent.I][ent.J],cellLayout[player.I][player.J]) ))
    {
        ent.agitated = 1;
        ent.chillOut = 0;
        ent.lastSeenI = player.I;
        ent.lastSeenJ = player.J;
    }


    if (ent.agitated) {
        if ((abs(ent.J-player.J)<=1)&&(abs(ent.I-player.I)<=1))// Attack range to be added in place of "1"
        {
            NPCAttack(ent);
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
    if ((inSight(cellLayout[ent.I][ent.J], cellLayout[player.I][player.J], ent.VIS)) && (!checkForWallsBetween(cellLayout[ent.I][ent.J],cellLayout[player.I][player.J])))
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
    if (cellLayout[aY+iY][aX+iX].type==1)
    {
        swapCells(cellLayout[aY+iY][aX+iX], cellLayout[aY][aX]);
        ent.J+=iX;
        ent.I+=iY;
    }
    else if (cellLayout[aY][aX+iX].type==1)
    {
        swapCells(cellLayout[aY][aX+iX], cellLayout[aY][aX]);
        ent.J+=iX;
    }
    else if (cellLayout[aY+iY][aX].type==1)
    {
        swapCells(cellLayout[aY+iY][aX], cellLayout[aY][aX]);
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
    switch (cellLayout[ent.I+randI][ent.J+randJ].type)
    {
        case 1:
            swapCells(cellLayout[ent.I+randI][ent.J+randJ], cellLayout[ent.I][ent.J]);
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

void NPCAttack(const entity &ent)
{
    winCon =-1;
}

//**************************************************************
void DrawHPBar(float CurrentHP, float MaxHP, SDL_Window* window, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);   //background color
    SDL_RenderClear(renderer);

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

    for (int i = BarThickness + BarGap; i < BarHeight - BarGap; i++) SDL_RenderDrawLine(renderer, BarThickness + BarGap, i, percentage * (BarWidth - BarGap - i - BarThickness/2 - (BarThickness + BarGap)) + (BarThickness + BarGap), i);

    SDL_RenderPresent(renderer);
    return;
}


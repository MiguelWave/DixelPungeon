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


int GameStage = 0; //0. menu 1.game 2.inventory 3.gameover 4. win -1. game is no longer running (exit game)

//Elements
int winCon = 0; //-1 means loss, 0 means game cancelled, 1 means win
int cellDim = 55; // Dimensions of each cells (in pixels)
int playerVIS = 5;
int entityVIS = 5;
int renderDistance = 8;

float Sanity = 100;
float SanityDecayPerTurn = 0.5;
float DecayMultiplierPerTurn = 1.1;
void DecaySanity();
void SpawnRandom();



// Assets
vector<Mix_Music*> Sounds;
vector<SDL_Texture*> assets;
vector<SDL_Texture*> MenuAssets;
SDL_Texture* GameOver;
SDL_Texture* Win;
vector<vector<vector<cell>>> Maps;
vector<entity> listOfEntities; // Also works as a turn base
entity player;

// Rendering (reading)
void RenderMenu(SDL_Window* window, SDL_Renderer* renderer, const int& SelectorPosition);
void RenderGameOver(SDL_Window* window, SDL_Renderer* renderer);
void RenderWin(SDL_Window* window, SDL_Renderer* renderer);
SDL_Texture* loadTexture(const string &file, SDL_Renderer *ren);
void RenderGame(SDL_Window* window, SDL_Renderer* renderer);

// Generating (writing)
void GenerateMap();

void pMove(char input);

cell getCellAtXY(int x, int y);
int getTypeAtXY(int x, int y);
bool IsWithinVision(const cell &a, const cell &b, int VIS);
bool checkForWallsBetween(const cell &a, const cell &b);



void NPCMove(entity &ent);
void moveToLastSeen(entity &ent);
void moveRandom(entity &ent);
void Attack(const entity& attacker, entity& defender);

void DrawHPBar(float CurrentHP, float MaxHP, SDL_Window* window, SDL_Renderer* renderer);

//void GetClick() {}

void InitNewGame(){
    // Clear player inventory
    player.health = 10;
    player.damage = 2;
    CurrentMap = 0;
    Maps.clear();
    listOfEntities.clear();
    for (int i = 0; i < NumberOfMapFiles; i++) Used[i] = false;
    GenerateMap();
    cout << "-----Initializing new game-----" << endl;
}


//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[]){
    // Window initialization
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);
//    Mix_Init(MIX_INIT_MP3);

    // Event variable
    SDL_Event e;

    // Init audio
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
        cout << "Could not init" << endl;
        return 0;
    }

    // Importing graphics
    assets.push_back(loadTexture("Assets/space.bmp", renderer)); // 0 space
    assets.push_back(loadTexture("Assets/floor1.bmp", renderer));// 1 floor
    assets.push_back(loadTexture("Assets/wall.bmp", renderer));  // 2 wall
    assets.push_back(loadTexture("Assets/player.bmp", renderer));// 3 player (placeholder)
    assets.push_back(loadTexture("Assets/enemy.bmp", renderer)); // 4 slime (placeholder)
    assets.push_back(loadTexture("Assets/exit.bmp", renderer));  // 5 exit
    assets.push_back(loadTexture("Assets/floor2.bmp", renderer));// 6 floor (dark)
    assets.push_back(loadTexture("Assets/wall2.bmp", renderer)); // 7 wall (dark
    assets.push_back(loadTexture("Assets/Iron_Sword_JE2_BE2.png", renderer));// 8 sword
    assets.push_back(loadTexture("Assets/Dungeon_Character_2.png", renderer));// 9 character sheet
    assets.push_back(loadTexture("Assets/chest_3.png", renderer)); // 10 chest
    assets.push_back(loadTexture("Assets/Potion_of_Saturation_BE2.png", renderer));// 11 potion

    MenuAssets.push_back(loadTexture("Assets/title.PNG", renderer));
    MenuAssets.push_back(loadTexture("Assets/start.PNG", renderer));
    MenuAssets.push_back(loadTexture("Assets/settings.PNG", renderer));
    MenuAssets.push_back(loadTexture("Assets/quit.PNG", renderer));
    MenuAssets.push_back(loadTexture("Assets/selector.PNG", renderer));

    GameOver = loadTexture("Assets/game_over.PNG", renderer);
    Win = loadTexture("Assets/win.PNG", renderer);

    Sounds.push_back(Mix_LoadMUS("Assets/Sounds/Cave3.ogg"));//0 ambient
    Sounds.push_back(Mix_LoadMUS("Assets/Sounds/Cave4.ogg"));//1 ambient
    Sounds.push_back(Mix_LoadMUS("Assets/Sounds/Cave10.ogg"));//2 ambient
    Sounds.push_back(Mix_LoadMUS("Assets/Sounds/Cave11.ogg"));//3 ambient
    Sounds.push_back(Mix_LoadMUS("Assets/Sounds/Cave12.ogg"));//4 ambient
    Sounds.push_back(Mix_LoadMUS("Assets/Sounds/Cave13.ogg"));//5 ambient
    Sounds.push_back(Mix_LoadMUS("Assets/Sounds/Deepslate_break1.ogg"));//6 player move
    Sounds.push_back(Mix_LoadMUS("Assets/Sounds/Deepslate_break2.ogg"));//7 player move
    Sounds.push_back(Mix_LoadMUS("Assets/Sounds/Deepslate_break3.ogg"));//8 player move
    Sounds.push_back(Mix_LoadMUS("Assets/Sounds/Deepslate_break4.ogg"));//9 player move

    int Selector = 1;




    // Debugging
//    RenderGame(window, renderer);
//    while (1) {
//        if ( SDL_WaitEvent(&e) == 0) continue;
//        if (e.type == SDL_QUIT) {
//            break;
//        }
//        if (e.type == SDL_KEYDOWN) Mix_PlayMusic(test, 0);
//    }

    // Render
    while (GameStage != -1) {
        switch (GameStage) {
        case 0: {
            RenderMenu(window, renderer, Selector);

//             Idling
            SDL_Delay(10);
            if ( SDL_WaitEvent(&e) == 0) continue;

//             Exit via closing window
            if (e.type == SDL_QUIT) {
                GameStage = -1;
                break;
            }

            // Event of user input
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_RETURN: // User pressed Enter
                    switch (Selector) {
                    case 1:
                        GameStage = 1;
                        InitNewGame();
                        break;
                    case 2:
                    // To be added: settings
                        break;
                    case 3:
                        GameStage = -1;
                        break;
                    }
                    break;

                case SDLK_w:
                case SDLK_UP:
                    if (Selector > 1) Selector--;
                    RenderMenu(window, renderer, Selector);
                    break;

                case SDLK_s:
                case SDLK_DOWN:
                    if (Selector < 3) Selector++;
                    RenderMenu(window, renderer, Selector);
                    break;
                }
            }
            break;
        }
        case 1: {
            RenderGame(window, renderer);
            // Idling
            SDL_Delay(10);
            if (SDL_WaitEvent(&e) == 0) continue;

            // User closes window
            if (e.type == SDL_QUIT) {
                GameStage = -1;
                break;
            }

            // Player input analyzation
            if (e.type == SDL_KEYDOWN) {
                bool playermoved = false;
                switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    GameStage = -1;
                    break;
                case SDLK_a:
                    playermoved = 1;
                    pMove('a');
                    break;
                case SDLK_d:
                    playermoved = 1;
                    pMove('d');
                    break;
                case SDLK_s:
                    playermoved = 1;
                    pMove('s');
                    break;
                case SDLK_w:
                    playermoved = 1;
                    pMove('w');
                    break;
                case SDLK_e:
                    playermoved = 1;
                    pMove('e');
                    break;
                case SDLK_q:
                    playermoved = 1;
                    pMove('q');
                    break;
                case SDLK_c:
                    playermoved = 1;
                    pMove('c');
                    break;
                case SDLK_z:
                    playermoved = 1;
                    pMove('z');
                    break;
                case SDLK_SPACE:
                    playermoved = 1;
                    break;
                }
                if (playermoved) for (int i = 0; i < (int)listOfEntities.size(); i++) NPCMove(listOfEntities[i]);
//                RenderGame(window, renderer);
            }
            // Check win condition
            if (GameStage == -1) break;
            if (player.health <= 0) GameStage = 3;
            break;
        }
        case 3: {
            RenderGameOver(window, renderer);
            if (SDL_WaitEvent(&e) == 0) continue;

            // Exit via closing window
            if (e.type == SDL_QUIT) GameStage = -1;

            // Press any key to continue
            if (e.type == SDL_KEYDOWN) GameStage = 0;

            break;
        }
        case 4: {
            RenderWin(window, renderer);
            // To be added: display stats
            if (SDL_WaitEvent(&e) == 0) continue;

            // Exit via closing window
            if (e.type == SDL_QUIT) GameStage = -1;

            // Press any key to continue
            if (e.type == SDL_KEYDOWN) GameStage = 0;

            break;
        }
        }
    }
    switch (winCon) {
        case -1:
            cout<<"Game over! Better luck next time.";
            break;
        case 0:
            cout<<"Game cancelled";
            break;
        case 1:
            cout<<"Congratulations, you win!";
            break;
    }

    return 0;
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------


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
    SDL_Rect temp;

    //Redrawing the entire thing from scratch
    for (int i = player.I - renderDistance; i <= player.I + renderDistance; ++i) {
        for (int j = player.J - renderDistance; j <= player.J + renderDistance; ++j) {// Only render from a certain number of cells away
            if ((i >= 0) && (j >= 0)
                && (i < int(Maps[CurrentMap].size()))
                && (j < int(Maps[CurrentMap][i].size()))) {// Only render cells in map boundaries

            temp.x = int(SCREEN_WIDTH/2 - cellDim/2 + (j - player.J)*cellDim); //Centered on the player
            temp.y = int(SCREEN_HEIGHT/2 - cellDim/2 + (i - player.I)*cellDim);
            temp.w = cellDim;
            temp.h = cellDim;

            CellIsVisible = (IsWithinVision(Maps[CurrentMap][player.I][player.J], Maps[CurrentMap][i][j], playerVIS)
                    && (!checkForWallsBetween(Maps[CurrentMap][player.I][player.J], Maps[CurrentMap][i][j])));

            switch (Maps[CurrentMap][i][j].type){
//            case 0:
//                to be added: background elements
            case 1:
                if (CellIsVisible) SDL_RenderCopy(renderer, assets[1], NULL, &temp);
                else if (Maps[CurrentMap][i][j].seen) SDL_RenderCopy(renderer, assets[6], NULL, &temp);
                break;
            case 2:
                if (CellIsVisible) SDL_RenderCopy(renderer, assets[2], NULL, &temp);
                else if (Maps[CurrentMap][i][j].seen) SDL_RenderCopy(renderer, assets[7], NULL, &temp);
                break;
            case 3:
                SDL_RenderCopy(renderer, assets[1], NULL, &temp); // To be added: render floor then render player
                SDL_Rect character;
                character.h = 16; character.w = 16;
                character.x = 16 * 5; character.y = 16 * 0;
                SDL_RenderCopy(renderer, assets[9], &character, &temp);
                break;
            case 4:
                if (CellIsVisible) SDL_RenderCopy(renderer, assets[4], NULL, &temp);
                else if (Maps[CurrentMap][i][j].seen) SDL_RenderCopy(renderer, assets[6], NULL, &temp);
                break;
            case 5:
                if (CellIsVisible) SDL_RenderCopy(renderer, assets[5], NULL, &temp); // Since we only have 1 asset for the exit, to be added later
                else if (Maps[CurrentMap][i][j].seen) SDL_RenderCopy(renderer, assets[5], NULL, &temp);
                break;
            case 6:
                if (CellIsVisible) {
                    SDL_RenderCopy(renderer, assets[1], NULL, &temp);
                    SDL_RenderCopy(renderer, assets[8], NULL, &temp);
                } else if (Maps[CurrentMap][i][j].seen) SDL_RenderCopy(renderer, assets[6], NULL, &temp);
                break;
            case 7:
                if (CellIsVisible) {
                    SDL_RenderCopy(renderer, assets[1], NULL, &temp); // To be added: render floor then render player
                    SDL_Rect character;
                    character.h = 16; character.w = 16;
                    character.x = 16 * 4; character.y = 16 * 1;
                    SDL_RenderCopy(renderer, assets[9], &character, &temp);
                } else if (Maps[CurrentMap][i][j].seen) SDL_RenderCopy(renderer, assets[6], NULL, &temp);
                break;
            case 8:
                if (CellIsVisible) {
                    SDL_RenderCopy(renderer, assets[1], NULL, &temp);
                    SDL_RenderCopy(renderer, assets[10], NULL, &temp);
                } else if (Maps[CurrentMap][i][j].seen) SDL_RenderCopy(renderer, assets[6], NULL, &temp);
                break;
            case 9:
                if (CellIsVisible) {
                    SDL_RenderCopy(renderer, assets[1], NULL, &temp);
                    SDL_RenderCopy(renderer, assets[11], NULL, &temp);
                } else if (Maps[CurrentMap][i][j].seen) SDL_RenderCopy(renderer, assets[6], NULL, &temp);
                break;
            }
            if (CellIsVisible) Maps[CurrentMap][i][j].seen = 1;
            }
        }
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
    temp.w = 930;
    temp.h = 140;
    temp.x = SCREEN_WIDTH/2 - temp.w/2;
    temp.y = SCREEN_HEIGHT/2 - temp.h/2;
    SDL_RenderCopy(renderer, GameOver, NULL, &temp);
    SDL_RenderPresent(renderer);
}

void RenderWin(SDL_Window* window, SDL_Renderer* renderer) {
    //Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);   //background color
    SDL_RenderClear(renderer);

    // Rendering title
    SDL_Rect temp;
    temp.w = 930;
    temp.h = 140;
    temp.x = SCREEN_WIDTH/2 - temp.w/2;
    temp.y = SCREEN_HEIGHT/2 - temp.h/2;
    SDL_RenderCopy(renderer, Win, NULL, &temp);
    // To be added: display stats
    SDL_RenderPresent(renderer);
}
//----------
// Gane=
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

//void SpawnRandom(){
//    srand((unsigned) time(NULL));
//    int random1 = rand() % 100; // Out of 100%
//    if (random1 > Sanity) { // Chance to spawn random
//        // Finding free cell - Spawn range shall be twice the player vision field
//        random = rand() % (playerVIS*2) + 1 - playerVIS;
//        while ()
//
//
//        random = rand() % 2; // 2 being number of monster types there are (more to be added later)
//        switch(random) {
//        case 1:
//            break;
//        case 2:
//            break;
//        }
//        cout << "A noise echoes from afar..." << endl;
//    }
//}
//
//void DecaySanity(){
//    Sanity -= SanityDecayPerTurn;
//    SanityDecayPerTurn *= DecayMultiplierPerTurn;
//}

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
                default:{
                    temp.type = 0;
                    break;}
                case '.':{
                    temp.type=1;
                    break;}
                case '*':{
                    temp.type=2;
                    break;}
                case 'Y':{
                    temp.type=3;
                    player.I=temp.I;
                    player.J=temp.J;
                    player.agitated=0; //????
                    player.VIS = playerVIS; // To be modified
                    break;}
                case 'E':{
                    temp.type=4; // Slime
                    entity NewEnt;
                    NewEnt.I=temp.I;
                    NewEnt.J=temp.J;
                    NewEnt.agitated = 0;
                    NewEnt.VIS=entityVIS;
                    listOfEntities.push_back(NewEnt);
                    break;
                }
                case 'O':{
                    temp.type = 5; // Exit
                    break;}
                case 's':{
                    temp.type = 6; // A sword (item)
                    break;}
                case 'S':{
                    temp.type = 7; // Skeleton
                    entity NewEnt;
                    NewEnt.I=temp.I;
                    NewEnt.J=temp.J;
                    NewEnt.agitated = 0;
                    NewEnt.VIS=entityVIS;
                    NewEnt.damage = 4;
                    NewEnt.health = 8;
                    listOfEntities.push_back(NewEnt);
                    break;}
                 case 'C':{
                    temp.type = 8; // A chest
                    break;}
                case 'P':{
                    temp.type = 9; // potion
                    break;}
            }
            NewMap[i].push_back(temp);
        }
        i++;//Number of lines in the internal layout
    }
    inpfile.close();
    Maps.push_back(NewMap);
}



//Movement
void pMove(char input){
    // Determining internal coordinates of the target cell
    int i = player.I, j = player.J;
    switch (input) {
    case 'a':
        j--;
        break;
    case 'd':
        j++;
        break;
    case 's':
        i++;
        break;
    case 'w':
        i--;
        break;
    case 'e':
        i--;
        j++;
        break;
    case 'c':
        i++;
        j++;
        break;
    case 'z':
        i++;
        j--;
        break;
    case'q':
        i--;
        j--;
        break;
    }

    // Randomness in sound played
    srand((unsigned) time(NULL));
    int random;

    // Player interaction with the target cell
    switch (Maps[CurrentMap][i][j].type){
    case 1:
        swapCells(Maps[CurrentMap][player.I][player.J], Maps[CurrentMap][i][j]);
        player.J = j;
        player.I = i;

        random = rand() % 4 + 6; // 4 being the number of sounds for player moving, and 6 being its position in the Sounds vector

        Mix_PlayMusic(Sounds[random], 0);
        break;
    case 5:
        NextMap();
        break;
    case 4:
        for (int k = 0; k < (int)listOfEntities.size(); k++)
            if (listOfEntities[k].I == i && listOfEntities[k].J == j) {
                Attack(player, listOfEntities[k]);
                cout << "You hit slime for " << player.damage << " damage. ";
                if (listOfEntities[k].health <= 0) {
                    listOfEntities.erase(listOfEntities.begin()+k);
                    Maps[CurrentMap][i][j].type = 1;
                    cout << "As you finish your assault, its skin disintegrates, letting its innards spill onto the floor." << endl;
                } else cout << "It has " << listOfEntities[k].health << " health left."<< endl;
            }
        break;
    case 6: // Pick up sword
        player.damage += 2;
        cout << "You picked up a sword. It boosts your attack damage by 2.\n";
        Maps[CurrentMap][i][j].type = 1;
        break;
    case 7: // Attack skeleton
        for (int k = 0; k < (int)listOfEntities.size(); k++)
            if (listOfEntities[k].I == i && listOfEntities[k].J == j) {
                Attack(player, listOfEntities[k]);
                cout << "You hit skeleton for " << player.damage << " damage. ";

                // Check if dead
                if (listOfEntities[k].health <= 0) {
                    listOfEntities.erase(listOfEntities.begin()+k);
                    Maps[CurrentMap][i][j].type = 1;
                    cout << "The skeleton crumbles to your might." << endl;
                } else cout << "It has " << listOfEntities[k].health << " health left."<< endl;
            }
        break;
    case 8:
        switch (rand() % 2) { // 2 being the number items this chest can drop
        case 0:
            Maps[CurrentMap][i][j].type = 6;
            break;
        case 1:
            Maps[CurrentMap][i][j].type = 9;
            break;
        }
        break;
    case 9: // Pick up potion
        player.health = min(player.health + 5, 10);
        cout << "You picked up a potion. You feel rejuvinated.\n";
        Maps[CurrentMap][i][j].type = 1;
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
            cout << &ent << " hit you for " << ent.damage << " damage." << endl;
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

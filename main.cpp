#include <bits/stdc++.h>
#include <SDL.h>

using namespace std;

//**************************************************************
const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 720;
const string WINDOW_TITLE = "Dixel Pungeon";

void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal = false);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();
//**************************************************************
//Elements
int winCon = 0;
const int cellDim = 48;
const int border = 10;

// Defining a cell
struct cell{
    int x;
    int y;
    const int w = cellDim;
    const int h = cellDim;
    int type = 0; // 0 space 1 ground 2 wall 3 player 4 enemy 5 exit
};


void refreshScreen(SDL_Window* window, SDL_Renderer* renderer, const vector<vector<cell>> &b);
void importLayout(const string &path, vector<vector<cell>> &b);

// Player movement
void pMoveLeft  (vector<vector<cell>> &b);
void pMoveUp    (vector<vector<cell>> &b);
void pMoveDown  (vector<vector<cell>> &b);
void pMoveRight (vector<vector<cell>> &b);

int main(int argc, char* argv[])
{
    // Window initialization
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);
    // Event variable
    SDL_Event e;
    // Input
    vector<vector<cell>> cells;

    importLayout("map.txt", cells);

    // Render

    refreshScreen(window, renderer, cells);
    while (true) {
        // Idling
        SDL_Delay(10);
        if ( SDL_WaitEvent(&e) == 0) continue;
        // Exit via big red X
        if (e.type == SDL_QUIT) break;
        // Player input analyzation
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) break;     // Exit via ESC
            if (e.key.keysym.sym == SDLK_a) pMoveLeft(cells);
            if (e.key.keysym.sym == SDLK_d) pMoveRight(cells);
            if (e.key.keysym.sym == SDLK_s) pMoveDown(cells);
            if (e.key.keysym.sym == SDLK_w) pMoveUp(cells);
            refreshScreen(window, renderer, cells);
        }
        // Check win condition
        if (winCon!=0)   break;
    }
    switch (winCon){
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


//*****************************************************

void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);


    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);

    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}
//******************************Made by the_Wave**********************************************






void importLayout(const string &path, vector<vector<cell>> &b){
    string line;
    int i=0;
    ifstream inpfile(path);


    while (!inpfile.eof()){
        getline(inpfile,line);
        b.push_back(vector<cell>());

        //Analyzing line i
        for (int j=0; j<int (line.length()); ++j){
            cell temp;
            //Coordinates
            temp.x = border + ( cellDim + border ) * j;
            temp.y = border + ( cellDim + border ) * i;

            //Determining type
            switch (line[j]){
                case '.':
                    temp.type=1;
                    break;
                case '*':
                    temp.type=2;
                    break;
                case 'Y':
                    temp.type=3;
                    break;
                case 'E':
                    temp.type=4;
                    break;
                case 'O':
                    temp.type=5;
                    break;
            }
            b[i].push_back(temp);
        }
        i++;
    }
    inpfile.close();
}


//New frame
void refreshScreen(SDL_Window* window, SDL_Renderer* renderer, const vector<vector<cell>> &b)
{
    //Clear screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);   // white
    SDL_RenderClear(renderer);

    //Redrawing the entire thing from scratch
    SDL_Rect temp;
    for (int i=0; i<b.size(); ++i){
        for (int j=0; j<b[i].size(); ++j){
            temp.x=b[i][j].x;
            temp.y=b[i][j].y;
            temp.w=b[i][j].w;
            temp.h=b[i][j].h;
            switch (b[i][j].type){
                case 0:
                    SDL_SetRenderDrawColor(renderer,    255,   255, 255, 255);
                    break;
                case 1:
                    SDL_SetRenderDrawColor(renderer,    192,   192, 192, 255);
                    break;
                case 2:
                    SDL_SetRenderDrawColor(renderer,    64,   64, 64, 255);
                    break;
                case 3:
                    SDL_SetRenderDrawColor(renderer,    0,   0, 255, 255);
                    break;
                case 4:
                    SDL_SetRenderDrawColor(renderer,    255,   0, 0, 255);
                    break;
                case 5:
                    SDL_SetRenderDrawColor(renderer,    0,   255, 0, 255);
                    break;
            }
            SDL_RenderFillRect(renderer, &temp);
        }
    }
    SDL_RenderPresent(renderer);

}
//***********************************************************************
//Movement
void pMoveLeft(vector<vector<cell>> &b){
    int moved=0;
    for (int i=0; i<b.size(); ++i){
        for (int j=0; j<b[i].size(); ++j){
            if ( (b[i][j].type==3) && (moved==0) ){
                switch (b[i][j-1].type){
                    case 1:
                        b[i][j].type=1;
                        b[i][j-1].type=3;
                        moved=1;
                        break;
                    case 5:
                        winCon=1;
                        moved=1;
                        break;
                    case 4:
                        winCon=-1;
                        moved=1;
                        break;
                }
            }
        }
    }
}

void pMoveRight(vector<vector<cell>> &b){
    int moved=0;
    for (int i=0; i<b.size(); ++i){
        for (int j=0; j<b[i].size(); ++j){
            if ( (b[i][j].type==3) && (moved==0) ){
                switch (b[i][j+1].type){
                    case 1:
                        b[i][j].type=1;
                        b[i][j+1].type=3;
                        moved=1;
                        break;
                    case 5:
                        winCon=1;
                        moved=1;
                        break;
                    case 4:
                        winCon=-1;
                        moved=1;
                        break;
                }
            }
        }
    }
}
void pMoveUp(vector<vector<cell>> &b){
    int moved=0;
    for (int i=0; i<b.size(); ++i){
        for (int j=0; j<b[i].size(); ++j){
            if ( (b[i][j].type==3) && (moved==0) ){
                switch (b[i-1][j].type){
                    case 1:
                        b[i][j].type=1;
                        b[i-1][j].type=3;
                        moved=1;
                        break;
                    case 5:
                        winCon=1;
                        moved=1;
                        break;
                    case 4:
                        winCon=-1;
                        moved=1;
                        break;
                }
            }
        }
    }
}
void pMoveDown(vector<vector<cell>> &b){
    int moved=0;
    for (int i=0; i<b.size(); ++i){
        for (int j=0; j<b[i].size(); ++j){
            if ( (b[i][j].type==3) && (moved==0) ){
                switch (b[i+1][j].type){
                    case 1:
                        b[i][j].type=1;
                        b[i+1][j].type=3;
                        moved=1;
                        break;
                    case 5:
                        winCon=1;
                        moved=1;
                        break;
                    case 4:
                        winCon=-1;
                        moved=1;
                        break;
                }
            }
        }
    }
}


//**************************************************************


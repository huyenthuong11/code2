
#include<bits/stdc++.h>
#include<SDL.h>
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include <deque>
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "Snake";

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Mix_Chunk* chunk = NULL;
Mix_Music* music = NULL;
TTF_Font* font = NULL;
SDL_Event e;
SDL_Rect head {300,400,10,10};
SDL_Rect food{rand()%30*20,rand()%30*20,10,10};
SDL_Rect tail;
bool running = false;

void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal = false);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();


void logSDLError(std::ostream& os,const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}


SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{

	SDL_Texture *texture = nullptr;

	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());

	if (loadedImage != nullptr){
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);
		SDL_FreeSurface(loadedImage);

		if (texture == nullptr){
			logSDLError(std::cout, "CreateTextureFromSurface");
		}
	}
	else {
		logSDLError(std::cout, "LoadBMP");
	}
	return texture;
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

void renderTexture( SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h)
{

	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
    dst.w = w;
    dst.h = h;

	SDL_RenderCopy(ren, tex, NULL, &dst);
    SDL_RenderCopy(ren, tex, NULL, &dst);
    SDL_RenderPresent(ren);


}

void board(int score, int level){
    SDL_SetRenderDrawColor(renderer, 61, 72, 73, 0xFF);

    SDL_Rect fillRect = {0, 0, SCREEN_WIDTH, 120 };
    SDL_Rect fillRect2 = {0,0, 10, SCREEN_HEIGHT};
    SDL_Rect fillRect3 = {0,590,SCREEN_WIDTH,10};
    SDL_Rect fillRect4 = {790,0,10,SCREEN_HEIGHT};

	stringstream ss, sl;
	ss << (score);
	sl << (level);
    string str = ss.str();
    string str2 = sl.str();
	const char* scoreChar = str.c_str();
	const char* levelChar = str2.c_str();

	TTF_Init();
	int texW = 0;
	int texH = 0;

	TTF_Font* font = TTF_OpenFont("arial.ttf", 30);
	SDL_Color color = { 255, 255, 255 };


	SDL_Surface* surfaceTwo = TTF_RenderText_Solid(font,
		"SCORE: ", color);
	SDL_Texture* textureTwo = SDL_CreateTextureFromSurface(renderer, surfaceTwo);
	SDL_QueryTexture(textureTwo, NULL, NULL, &texW, &texH);
	SDL_Rect dstrectTwo = { 230, 70, texW, texH };

	SDL_Surface* surfaceThree = TTF_RenderText_Solid(font,
		scoreChar, color);
	SDL_Texture* textureThree = SDL_CreateTextureFromSurface(renderer, surfaceThree);
	SDL_QueryTexture(textureThree, NULL, NULL, &texW, &texH);
	SDL_Rect dstrectThree = { 350, 70, texW, texH };

	SDL_Surface* surfaceFour = TTF_RenderText_Solid(font,
		levelChar, color);
	SDL_Texture* textureFour = SDL_CreateTextureFromSurface(renderer, surfaceFour);
	SDL_QueryTexture(textureFour, NULL, NULL, &texW, &texH);
	SDL_Rect dstrectFour = { 350, 30, texW, texH };

    SDL_Surface* surfaceFive = TTF_RenderText_Solid(font,
		"LEVEL: ", color);
	SDL_Texture* textureFive = SDL_CreateTextureFromSurface(renderer, surfaceFive);
	SDL_QueryTexture(textureFive, NULL, NULL, &texW, &texH);
	SDL_Rect dstrectFive = { 230, 30,texW, texH };


	SDL_RenderFillRect(renderer, &fillRect);
    SDL_RenderFillRect(renderer, &fillRect2);
    SDL_RenderFillRect(renderer, &fillRect3);
    SDL_RenderFillRect(renderer, &fillRect4);

	SDL_RenderCopy(renderer, textureTwo, NULL, &dstrectTwo);
	SDL_RenderCopy(renderer, textureThree, NULL, &dstrectThree);
	SDL_RenderCopy(renderer,textureFour,NULL,&dstrectFour);
	SDL_RenderCopy(renderer, textureFive,NULL,&dstrectFive);

}

bool start = true;
bool lose = false;
bool game = true;

int level =1;

void startgame(){

    SDL_Texture* menu=loadTexture("menu.bmp",renderer);
    SDL_Texture* opensound = loadTexture("open.bmp", renderer);
    SDL_Texture* closesound = loadTexture("close.bmp", renderer);
    font = TTF_OpenFont("arial.ttf", 10);
    SDL_Color color = { 243, 156, 18 };
    SDL_Surface* surface1 = TTF_RenderText_Solid(font, "GAME MENU", color);
    SDL_Surface* surface2 = TTF_RenderText_Solid(font, "PLAY", color);
    SDL_Surface* surfaceB = TTF_RenderText_Solid(font,"QUIT", color);
    SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer,surface1);
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer,surface2);
    SDL_Texture* m2 = SDL_CreateTextureFromSurface(renderer,surfaceB);
    renderTexture(menu, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    renderTexture(opensound,renderer,5,5,50,50);
    renderTexture(texture1,renderer,100,200,300,100);
    renderTexture(texture2,renderer,100,350,200,70);
    renderTexture(m2,renderer,100,450,200,70);

if(!(Mix_PlayingMusic()))
            Mix_PlayMusic(music,0);
    while (start){

        while(SDL_PollEvent(&e)){

            if(e.type == SDL_QUIT) {
                start = false;
                lose = false;
                running = false;
                game = false;
            }
            else if(e.type == SDL_MOUSEBUTTONDOWN){
                if(e.button.x>=100 && e.button.x<=300 && e.button.y>=350 && e.button.y<=420){
                    start = false;
                    running = true;
                    SDL_RenderClear(renderer);
                }
                else if(e.button.x >= 100 && e.button.x <= 300 && e.button.y >= 450 && e.button.y <= 520){
                    SDL_Quit();
                }
                else if(e.button.x>=5 && e.button.x<=55 && e.button.y>=5 && e.button.y<=55){

                    if(Mix_PlayingMusic()){
                        Mix_PauseMusic();
                    }
                    else if(Mix_PausedMusic()){
                        Mix_ResumeMusic();
                    }
                }

            }
        }
    }
}

void endgame(){
    TTF_Font* font = TTF_OpenFont("arial.ttf", 30);
	SDL_Color color = { 0, 255, 255 };
    SDL_Surface* surface = TTF_RenderText_Solid(font, "YOU LOSE!", color);
    SDL_Texture* loseMessage = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_Surface* PAsurface = TTF_RenderText_Solid(font, "PLAY AGAIN", color);
    SDL_Texture* PA = SDL_CreateTextureFromSurface(renderer,PAsurface);
    SDL_Surface* Quitsurface = TTF_RenderText_Solid(font," QUIT", color);
    SDL_Texture* quitTexture = SDL_CreateTextureFromSurface(renderer,Quitsurface);


    SDL_FreeSurface(surface);

    while(lose){
            renderTexture(loseMessage,renderer,250,200,300,100);
            renderTexture(PA, renderer, 300,350,200,70);
            renderTexture(quitTexture,renderer,290,450,150,70);

        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT) {
                start = false;
                lose = false;
                running = false;
                game = false;
            }
            if(e.type == SDL_MOUSEBUTTONDOWN){
                if(e.button.x>=300 && e.button.x<=550 && e.button.y>=350 && e.button.y<=420){
                        start = true;
                        lose = false;
                }
                else if(e.button.x >= 290 && e.button.x <= 440 && e.button.y >= 450 && e.button.y <= 520){
                        lose = false;
                        game = false;
                }
            }
        }
    }
}



int main(int argc, char* argv[]){
     initSDL(window, renderer);

     if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
	{
		cout<<"MIX ERROR";
		return -1;
	}
	TTF_Init();
	//load sound effect
	music = Mix_LoadMUS("background.mp3");
	if (music == NULL)
	{
		cout<<" Mix_GetError";
	}
	chunk = Mix_LoadWAV("applause.wav");
    if (chunk == NULL)
	{
		cout<<" Mix_GetError";
		return -1;
	}
	//load image
	SDL_Texture* snakehead=loadTexture("head.bmp", renderer);
	SDL_Texture* snakefood=loadTexture("food.bmp", renderer);
	SDL_Texture* snakebody=loadTexture("body.bmp", renderer);

	if (snakefood == nullptr || snakehead == nullptr){
        cout<<"error";
    }






    while(game){
            startgame();
            SDL_Rect head {300,400,10,10};
             int score=0;

    enum Direction
    {
        DOWN,
        LEFT,
        RIGHT,
        UP
    };

    deque<SDL_Rect> rq;
    int size = 1;

    int dir = 0;
    while(running){

        if(Mix_PlayingMusic() == 1)
        Mix_PlayMusic(music, 0);

        while(SDL_PollEvent(&e)){

            if(e.type == SDL_QUIT) {
                start = false;
                lose = false;
                running = false;
                game = false;
            }
            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_DOWN){dir = DOWN;}
                if(e.key.keysym.sym == SDLK_UP) {dir = UP;}
                if(e.key.keysym.sym == SDLK_LEFT) {dir = LEFT;}
                if(e.key.keysym.sym == SDLK_RIGHT) {dir = RIGHT;}
            }
        }
        switch(dir){
            case DOWN:
                head.y += 10; break;
            case UP:
                head.y -= 10; break;
            case LEFT:
                head.x -= 10; break;
            case RIGHT:
                head.x += 10; break;
        }

        if(head.x>=food.x && head.x<=food.x+20 && head.y>=food.y && head.y<=food.y+20 ){
            Mix_PlayChannel(-1, chunk, 0);
            food.x=rand() % 30 * 10;
            food.y=(rand() % 30) * 10;
            score+=10;
            size+=1;
        }

        for(int i=0; i<size; i++){
                 if(head.x==rq[i].x&&head.y==rq[i].y){
                    running = false;
                 }
        }
        rq.push_front(head);
        while(rq.size()>size){
            rq.pop_back();
        }
         if(head.x < 10||head.x >= 780||head.y >= 580  ||head.y < 120){
            running = false;
            lose = true;
        }
        if(food.x < 10||food.x >= 780 || food.y >= 580|| food.y < 120){
            food.x=rand() % 30 * 20;
            food.y=(rand() % 39 + 6) * 20;
        }

        if(score<=100) level = 1;
        else if(score>100 && score<=300) level = 2;
        else if(score>300&&score<=500) level = 3;
        else if(score>500 && score<=1000) level = 4;
        else level=5;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer,snakehead,NULL,&rq[0]);

        for(int i=1;i<size;i++){
                    SDL_RenderCopy(renderer,snakebody,NULL,&rq[i]);
        }
        SDL_RenderCopy(renderer,snakefood,NULL,&food);
        cout<<food.x<<" "<<food.y;
        board(score,level);
        SDL_RenderPresent(renderer);
        if(level == 1) SDL_Delay(150);
        else if(level == 2) SDL_Delay(100);
        else if(level == 3) SDL_Delay(70);
        else if(level == 4) SDL_Delay(50);
        else SDL_Delay(25);
    }
    endgame();



    }

    SDL_RenderClear(renderer);
    Mix_CloseAudio();
    TTF_Quit();
    quitSDL(window, renderer);

}

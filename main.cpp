#include <iostream>
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Transform.h"

using namespace std;
using namespace transform;

const int SCREEN_WID		= 1280;
const int SCREEN_HEI		= 960;
const int WINDOW_WID		= SCREEN_WID;
const int WINDOW_HEI		= SCREEN_HEI;

int main(int argc, char* argv[]){
	if (SDL_Init(SDL_INIT_EVERYTHING) >0){
		cout << "Error: SDL failed to initialize." << endl;
		return 0;
	}

	SDL_Window*	window = SDL_CreateWindow("SDL Image Manipulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WID, WINDOW_HEI, NULL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Event event;

	//Load and modify image
	SDL_Surface* image = IMG_Load("Resources/test.jpg");
	SDL_Surface* filtered = CopySurface(image);
	SDL_Surface* graphA	= EmptySurface(image);
	SDL_Surface* graphB = EmptySurface(image);

	Interlace(filtered, 2, CreateColor(20,20,20,20), IL_DOTTED, BLEND_ADD);

	Fill(graphA, CreateColor(30, 30, 30, 255));
	DrawGridlines(graphA, CreateColor(45, 45, 45, 255), GRID_DEFAULT, 64, 4, 0, 0);
	Histogram(image, graphA, CHANNEL_ALL, GRAPH_FILLED, 64, 1, 5000);

	Fill(graphB, CreateColor(30, 30, 30, 255));
	DrawGridlines(graphB, CreateColor(45, 45, 45, 255), GRID_DEFAULT, 64, 4, 0, 0);
	Histogram(filtered, graphB, CHANNEL_ALL, GRAPH_FILLED, 64, 1, 5000);

	SDL_Texture* imageTex = SDL_CreateTextureFromSurface(renderer, image);
	SDL_Texture* post = SDL_CreateTextureFromSurface(renderer, filtered);
	SDL_Texture* gtexA = SDL_CreateTextureFromSurface(renderer, graphA); 
	SDL_Texture* gtexB = SDL_CreateTextureFromSurface(renderer, graphB);

	SDL_FreeSurface(image);
	SDL_FreeSurface(filtered);
	SDL_FreeSurface(graphA);
	SDL_FreeSurface(graphB);

	//Main loop
	bool quit = false;

	while (quit != true){
		while (SDL_PollEvent(&event) != 0){
			switch (event.key.keysym.sym){
				case SDLK_ESCAPE:
					quit = true;
					break;
			}
		}

		//Render media
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, imageTex, NULL, &CreateRect(0,0, SCREEN_WID/2, SCREEN_HEI/2));
		SDL_RenderCopy(renderer, post, NULL, &CreateRect(0, SCREEN_HEI/2, SCREEN_WID/2, SCREEN_HEI/2));
		SDL_RenderCopy(renderer, gtexA, NULL, &CreateRect(SCREEN_WID/2, 0, SCREEN_WID/2, SCREEN_HEI/2));
		SDL_RenderCopy(renderer, gtexB, NULL, &CreateRect(SCREEN_WID/2, SCREEN_HEI/2, SCREEN_WID/2, SCREEN_HEI/2));
		SDL_RenderPresent(renderer);
	}

	//Clean up
	SDL_Quit();

	return 0;
}
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "res_path.h"
#include "cleanup.h"
#define GET_VARIABLE_NAME(Variable) (#Variable)
const int SCREEN_WIDTH = 104;
const int SCREEN_HEIGHT = 100;
const int TILE_SIZE = 40;
const int MVRIGHT = 0;
const int MVLEFT = 1;
const int MVUP = 2;
const int MVDOWN = 3;

void logSDLError(std::ostream& os, const std::string& msg) {
	os << msg << " error: " << SDL_GetError() << std::endl;
}

SDL_Texture* loadTexture(const std::string& file, SDL_Renderer* renderer) {
	
	SDL_Texture* texture = IMG_LoadTexture(renderer,file.c_str());
	if (texture == nullptr) {
		logSDLError(std::cout, "LoadTexture");
	}
	return texture;
}

/*void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y, int w, int h) {
	SDL_Rect destinationRectangle;
	destinationRectangle.x = x;
	destinationRectangle.y = y;
	destinationRectangle.w = w;
	destinationRectangle.h = h;
	SDL_RenderCopy(renderer, texture, NULL, &destinationRectangle);

}
 

void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y) {
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	renderTexture(texture, renderer, x, y, w, h);
}
*/

void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, SDL_Rect destinationRectangle, SDL_Rect* clip = nullptr) {
	SDL_RenderCopy(renderer, texture, clip, &destinationRectangle);
}

void updateMovementState(bool (&mvmtState)[4],const Uint8*keystate) {
	
	if (keystate[SDL_SCANCODE_D]) {
		mvmtState[MVRIGHT] = true;
	}
	else {
		mvmtState[MVRIGHT] = false;
	}

	if (keystate[SDL_SCANCODE_A]) {
		mvmtState[MVLEFT] = true;
	}
	else {
		mvmtState[MVLEFT] = false;
	}

	if (keystate[SDL_SCANCODE_W]) {
		mvmtState[MVUP] = true;
	}
	else {
		mvmtState[MVUP] = false;
	}

	if (keystate[SDL_SCANCODE_S]) {
		mvmtState[MVDOWN] = true;
	}
	else {
		mvmtState[MVDOWN] = false;
	}
	/*
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_d) {
			mvmtState[MVRIGHT] = true;
		}
		else if (event.key.keysym.sym == SDLK_a) {
			mvmtState[MVLEFT] = true;
		}
		else if (event.key.keysym.sym == SDLK_w) {
			mvmtState[MVUP] = true;
		}
		else if (event.key.keysym.sym == SDLK_s) {
			mvmtState[MVDOWN] = true;
		}
	}
	else if (event.type == SDL_KEYUP) {
		if (event.key.keysym.sym == SDLK_d) {
			mvmtState[MVRIGHT] = false;
		}
		else if (event.key.keysym.sym == SDLK_a) {
			mvmtState[MVLEFT] = false;
		}
		else if (event.key.keysym.sym == SDLK_w) {
			mvmtState[MVUP] = false;
		}
		else if (event.key.keysym.sym == SDLK_s) {
			mvmtState[MVDOWN] = false;
		}
	}*/
}

void updateVelocityFromMvmtState(bool mvmtState[4], int(&velocity)[2]) {
	velocity[0] = mvmtState[MVRIGHT] & 1 - mvmtState[MVLEFT] & 1;
	velocity[1] = mvmtState[MVUP] & 1 - mvmtState[MVDOWN] & 1;
}
void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y, SDL_Rect *clip=nullptr) {
	SDL_Rect destinationRectangle;
	destinationRectangle.x = x;
	destinationRectangle.y = y;
	if (clip != nullptr) {
		destinationRectangle.w = clip->w;
		destinationRectangle.h = clip->h;
	}
	else {
		SDL_QueryTexture(texture, NULL, NULL, &destinationRectangle.w, &destinationRectangle.h);
	}
	renderTexture(texture, renderer, destinationRectangle, clip);
}

int main(int, char**) {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		logSDLError(std::cout, "SDL_Init");
	}

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		logSDLError(std::cout, "IMG_Init");
		SDL_Quit();
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Lesson2", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		logSDLError(std::cout, "CreateWindow");
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		logSDLError(std::cout, "CreateRenderer");
		cleanup(window);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	const std::string resPath = getResourcePath();
	SDL_Texture* footTexture = loadTexture(resPath + "feet.png", renderer);
	if (footTexture == nullptr) {
		cleanup(footTexture, window, renderer);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}
	int imageWidth = 104, imageHeight = 100;
	int imageX = SCREEN_WIDTH / 2 - imageWidth / 2;
	int imageY = SCREEN_HEIGHT / 2 - imageHeight / 2;
	int sheetSizeX,  numberOfSprites;
	SDL_QueryTexture(footTexture, NULL, NULL, &sheetSizeX, NULL);
	numberOfSprites = sheetSizeX / imageWidth;
	SDL_Rect *clips = NULL;
	clips = new SDL_Rect[numberOfSprites];
	for (int i = 0; i < numberOfSprites; i++) {
		clips[i].x = i * imageWidth;
		clips[i].y = 0;
		clips[i].w = imageWidth;
		clips[i].h = imageHeight;
	}
	std::cout << numberOfSprites << std::endl;
	
	int useClip = 0;
	SDL_Event event;
	bool quit = false;

	bool mvmtState[4] = { false };
	bool prevState[4] = { false };
	int velocity[2] = { 0 };
	int j = 0;
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
	while(!quit) {
		for (int i = 0; i < 4; i++) {
			prevState[i] = mvmtState[i];
		}
		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				quit = true;
			}
			
		}
		updateMovementState(mvmtState, keystate);
		bool comp = true;
		for (int i = 0; i < 4; i++) {
			if (mvmtState[i] != prevState[i]) {
				comp = false;
			}
		}
		if (!comp){
			for (int i = 0; i < 4; i++) {
				std::cout << mvmtState[i];
			}
			std::cout << std::endl;		
		}
		
		updateVelocityFromMvmtState(mvmtState, velocity);
		
		SDL_RenderClear(renderer);

		renderTexture(footTexture, renderer, imageX, imageY, &clips[j]);
		SDL_RenderPresent(renderer);
		j+=1;
		if (j == 8) {
			j = 0;
		}
		SDL_Delay(70);
	}
	cleanup(footTexture, renderer, window);
	//delete(mvmtState);
	//delete(velocity);
	IMG_Quit();
	SDL_Quit();
	return 0;
}
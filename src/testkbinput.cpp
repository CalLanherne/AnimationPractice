#include <iostream>
#include <SDL.h>
enum MVDIRECTIONS {
    MVRIGHT,
    MVLEFT,
    MVUP,
    MVDOWN,
};
void updateMovementState(bool(&mvmtState)[4], const Uint8* keystate) {

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
}

int main(int, char**) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout<<"SDL_Init Error: "<<SDL_GetError()<<std::endl;
    }
    SDL_Window* window = SDL_CreateWindow("Lesson2", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout<< "CreateWindow Error: "<<SDL_GetError()<<std::endl;
        SDL_Quit();
        return 1;
    }

    bool mvmtState[4] = { false };
    bool prevState[4] = { false };
    
    bool quit = false;
    SDL_Event event;
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    while (!quit) {
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
        if (!comp) {
            for (int i = 0; i < 4; i++) {
                std::cout << mvmtState[i];
            }
            std::cout << std::endl;
        }
    }
    SDL_Quit();
    return 0;
}
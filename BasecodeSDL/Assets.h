#pragma once

#include "Settings.h"

typedef struct Assets_s
{
    //grid and background
    SDL_Texture* textureExample;
    SDL_Texture* MenuFrame;
    SDL_Texture* GameFrame;
    SDL_Texture* DifficultyFrame;
    SDL_Texture* GameModeFrame;
    SDL_Texture* column;
    //button
    SDL_Texture* MenuB;
    SDL_Texture* RestartB;
    SDL_Texture* PlayB;
    SDL_Texture* ExitB;
    //button game mode
    SDL_Texture* AIvsAI;
    SDL_Texture* AIvsPL;
    SDL_Texture* PvP;
    //pawn
    SDL_Texture* redPawn;
    SDL_Texture* yellowPawn;
    //win
    SDL_Texture* WinOne;
    SDL_Texture* WinTwo;
    SDL_Texture* Tie;
    //Turn
    SDL_Texture* PLTurn;
    SDL_Texture* Pone;
    SDL_Texture* Ptwo;
} Assets;

Assets *Assets_New(SDL_Renderer *renderer);
void Assets_Delete(Assets *self);

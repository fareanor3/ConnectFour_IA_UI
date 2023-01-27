#include <stdio.h>
#include <SDL.h>
#include "Assets.h"
#include "Game.h"
#include "Settings.h"
#include "AI.h"

//a function that create a button and place it on the coordinate(x;y)
void CreateButton(SDL_Renderer* renderer, float x, float y, float w, float h, SDL_Texture* image) {

    SDL_FRect buttonRect = { 0,0,0,0 };
    buttonRect.x = x;
    buttonRect.y = y;
    buttonRect.w = w;
    buttonRect.h = h;
    SDL_RenderCopyF(renderer, image, NULL, &buttonRect);
}

void IsHover(SDL_Renderer* renderer, float x, float y, float w, float h, SDL_Texture* image) {

    SDL_FRect buttonRect = { 0,0,0,0 };
    buttonRect.x = x;
    buttonRect.y = y;
    buttonRect.w = w;
    buttonRect.h = h;

    //get the cursor position and check if it's on the button to activate the hover effect
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    if (mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w && mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h) {
        //reduce image opacity by 10% when hovered
        SDL_SetTextureAlphaMod(image, 90);
        //printf("TEST!!!!!!!!!!!\n");
    }
    else {
        //set the image opacity to full when not hovered
        SDL_SetTextureAlphaMod(image, 255);
    }
    SDL_RenderCopyF(renderer, image, NULL, &buttonRect);
}

void ButtonClick(SDL_Renderer* renderer, float x, float y, float w, float h, SDL_Texture* image) {

    SDL_FRect buttonRect = { 0,0,0,0 };
    buttonRect.x = x;
    buttonRect.y = y;
    buttonRect.w = w;
    buttonRect.h = h;

    int mouseX, mouseY;

    //get the cursor position and check if it's on the button to activate the click effect
    SDL_GetMouseState(&mouseX, &mouseY);
    if (mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w && mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h) {
        SDL_SetTextureAlphaMod(image, 255);
    }
    SDL_RenderCopyF(renderer, image, NULL, &buttonRect);
}

//return column number
int ColumnClick(SDL_Renderer* renderer, SDL_Texture* image) {

    SDL_FRect columnRect = { 0,0,0,0 };
    columnRect.x = 0;
    columnRect.y = 5;
    columnRect.w = 97.5;
    columnRect.h = 710;

    int columnN = -1;
    float abscissa[7] = { 20.5, 135.5, 252, 368, 483, 599, 715 };
    int mouseX, mouseY;

    for (int i = 0; i < 7; i++) {
        SDL_GetMouseState(&mouseX, &mouseY);
        if (mouseX >= abscissa[i] && mouseX <= abscissa[i] + columnRect.w) {
            columnRect.x = abscissa[i];
            //SDL_RenderCopyF(renderer, image, NULL, &columnRect);
            SDL_SetTextureAlphaMod(image, 90);
            //printf("column n: %d !!!!\n", i);
            columnN = i;
        }
    }
    return columnN;
}

void ComlumnSelect(SDL_Renderer* renderer, SDL_Texture* image) {

    SDL_FRect columnRect = { 0,0,0,0 };
    columnRect.x = 0;
    columnRect.y = 5;
    columnRect.w = 97.5;
    columnRect.h = 710;

    //get the cursor position and check if it's on the column to activate the hover effect
    float abscissa[7] = { 20.5, 135.5, 252, 368, 483, 599, 715 };
    int mouseX, mouseY;

    for (int i = 0; i < 7; i++) {
        SDL_GetMouseState(&mouseX, &mouseY);
        if (mouseX >= abscissa[i] && mouseX <= abscissa[i] + columnRect.w) {
            columnRect.x = abscissa[i];
            SDL_RenderCopyF(renderer, image, NULL, &columnRect);
        }
    }
}
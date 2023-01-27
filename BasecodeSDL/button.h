#pragma once


void CreateButton(SDL_Renderer* renderer, float x, float y, float w, float h, SDL_Texture* image);
void IsHover(SDL_Renderer* renderer, float x, float y, float w, float h, SDL_Texture* image);
void ButtonClick(SDL_Renderer* renderer, float x, float y, float w, float h, SDL_Texture* image);
int ColumnClick(SDL_Renderer* renderer, SDL_Texture* image);
void ComlumnSelect(SDL_Renderer* renderer, SDL_Texture* image);
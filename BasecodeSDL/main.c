#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "Settings.h"
#include "Game.h"
#include "AI.h"

#include "Common.h"
#include "Input.h"
#include "Assets.h"
#include "button.h"

char* colorCodes[] = { "\x1b[0m", "\x1b[31m", "\x1b[32m" };

int main()
{
    Game* game = Game_New();
    FILE* fp;
    fp = fopen("log.csv", "a");
    srand(time(0));

    int eval_test[GRID_H][GRID_W];
    int eval_1[GRID_H][GRID_W];

    for (int i = 0; i < GRID_H; i++)
    {
        for (int j = 0; j < GRID_W; j++)
        {
            eval_test[i][j] = 0;
            eval_1[i][j] = 0;
        }
    }

    //for (int i = 0; i < 4; i++)
    //{
    //    int column = rand() % 7;
    //    Game_PlayTurn(game, column);
    //}

    calculateWinningAlignments(eval_1, eval_test);
    //printEvaluationMatrix(eval_test);

    game->nbTurn = 0;

    //-----------------------------------------[SDL]-----------------------------------------------------------
    //-----------------------------------------[SDL]-----------------------------------------------------------
    //-----------------------------------------[SDL]-----------------------------------------------------------

    // Initialisation de la SDL
    App_Init(SDL_INIT_VIDEO, IMG_INIT_PNG);

    // Crée la fenêtre et le moteur de rendu
    int sdlFlags = 0;
#ifdef FULLSCREEN
    sdlFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
#endif

    SDL_Window* window = SDL_CreateWindow(
        u8"Basecode SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, sdlFlags
    );

    if (!window)
    {
        printf("ERROR - Create window %s\n", SDL_GetError());
        assert(false); abort();
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    if (!renderer)
    {
        printf("ERROR - Create renderer %s\n", SDL_GetError());
        assert(false); abort();
    }

    // Crée le temps global du jeu
    g_time = Timer_New();
    AssertNew(g_time);

    // Crée le gestionnaire des entrées utilisateur
    Input* input = Input_New();

    // Chargement des assets
    Assets* assets = Assets_New(renderer);

    //--------------------------------------------------------------------------
    // Boucle de rendu

    // Position et vitesse du logo (exemple)
    float x = 0.0f;
    float y = 0.0f;
    float w = 832.0f;
    float h = 720.0f;
    float vx = 200.0f;
    float vy = 200.0f;

    // Boucle de jeu
    while (Game_GetState(game) == GAME_IN_PROGRESS)
    {
        // netoye la console :
        //system("clear");
        //printf("----------------------------------------\n");
        //printf("Tour du joueur %s%d%s\n\n", colorCodes[Game_GetPlayerID(game)], Game_GetPlayerID(game), colorCodes[0]);

        // Met à jour le temps
        Timer_Update(g_time);

        // Met à jour les entrées
        Input_Update(input);

        if (input->quitPressed)
            break;

        Game_Print(game);
        //printf("\n");

        if (Game_GetPlayerID(game) == 1)
        {
            // Choix de la colonne
            int column = -1;
            if (input->mouseClick) {

                column = ColumnClick(renderer, assets->column);
                if (Game_CanPlayAt(game, column)) {
                    Game_PlayTurn(game, column, fp);
                }
                input->mouseClick = false;

            }


            //column = AI_ComputeMove(game, 1);
            //   Action du joueur

            //printf("\n");
        }
        else
        {
            // Choix de la colonne
            int column = AI_ComputeMove(game, 2);

            // Action du joueur
            Game_PlayTurn(game, column, fp);
            //printf("\n");
        }
        //game->nbTurn++;

    //-----------------------------------------[SDL]-----------------------------------------------------------
    //-----------------------------------------[SDL]-----------------------------------------------------------
    //-----------------------------------------[SDL]-----------------------------------------------------------
        // Efface le rendu précédent
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);


        //background
        SDL_FRect bgRect = { 0,0,0,0 };
        bgRect.x = 0;
        bgRect.y = 0;
        bgRect.w = 1280;
        bgRect.h = 720;
        SDL_RenderCopyF(renderer, assets->GameFrame, NULL, &bgRect);

        //Show current Column
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        ComlumnSelect(renderer, assets->column);

        //if (input->mouseClick) {
        //    int column = ColumnClick(renderer, assets->column);
        //    //ButtonClick(renderer, (float)mouseX, assets->column);
        //    input->mouseClick = false;
        //}


        // Dessinez ici votre rendu
        SDL_FRect dstRect = { 0,0,0,0 };
        dstRect.x = x;
        dstRect.y = y;
        dstRect.w = w;
        dstRect.h = h;
        SDL_RenderCopyF(renderer, assets->textureExample, NULL, &dstRect);


        float abscissa[7] = { 20.5, 135.5, 252, 368, 483, 599, 715 };
        float ordinate[6] = { 603, 486.5 , 370, 253.5 , 137, 20.5 };

        // Parcours de la grille et affiche les pions des joueurs
        for (int i = GRID_H - 1; i >= 0; i--)
        {
            for (int j = 0; j < GRID_W; j++)
            {
                // Affiche la bille du joueur ou un espace vide et une barre verticale
                SDL_FRect pawnRect = { 0,0,0,0 };
                pawnRect.x = 0;
                pawnRect.y = 0;
                pawnRect.w = 97.5;
                pawnRect.h = 97.5;

                if (game->grid[i][j] == 1) {
                    pawnRect.x = abscissa[j];
                    pawnRect.y = ordinate[i];
                    SDL_RenderCopyF(renderer, assets->yellowPawn, NULL, &pawnRect);
                }
                else if (game->grid[i][j] == 2) {
                    pawnRect.x = abscissa[j];
                    pawnRect.y = ordinate[i];
                    SDL_RenderCopyF(renderer, assets->redPawn, NULL, &pawnRect);
                }
            }
        }

        // call function that create button from images
        CreateButton(renderer, 1022.0f, 569.0f, 100.0f, 100.0f, assets->MenuB);
        CreateButton(renderer, 900.0f, 569.0f, 100.0f, 100.0f, assets->RestartB);
        CreateButton(renderer, 1144.0f, 569.0f, 100.0f, 100.0f, assets->ExitB);
        // call Hover effect function on button
        IsHover(renderer, 1022.0f, 569.0f, 100.0f, 100.0f, assets->MenuB);
        IsHover(renderer, 900.0f, 569.0f, 100.0f, 100.0f, assets->RestartB);
        IsHover(renderer, 1144.0f, 569.0f, 100.0f, 100.0f, assets->ExitB);
        if (input->mouseClick) {
            // call Click effect function on button
            ButtonClick(renderer, 1022.0f, 569.0f, 100.0f, 100.0f, assets->MenuB);
            ButtonClick(renderer, 900.0f, 569.0f, 100.0f, 100.0f, assets->RestartB);
            ButtonClick(renderer, 1144.0f, 569.0f, 100.0f, 100.0f, assets->ExitB);
            input->mouseClick = false;
        }

        // Affiche le nouveau rendu
        SDL_RenderPresent(renderer);
    }

    // Affichage du resultat
    printf("----------------------------------------\n");
    switch (Game_GetState(game))
    {
    case GAME_P1_WON:
        printf("Le joueur gagne %s 1 %s gagne\n", colorCodes[1], colorCodes[0]);
        printf("LA PARTIE EST REMPORTEE DANS LA CONFIGURATION :\n\n");
        Game_Print(game);
        break;

    case GAME_P2_WON:
        printf("Le joueur %s 2 %s gagne\n", colorCodes[2], colorCodes[0]);
        printf("LA PARTIE EST REMPORTEE DANS LA CONFIGURATION :\n\n");
        Game_Print(game);
        break;

    case GAME_IS_TIED:
        printf("Egalite parfaite\n");
        break;

    default:
        printf("ERREUR\n");
        assert(false);
        break;
    }
    Game_Delete(game);
    //--------------------------------------------------------------------------
   // Libération de la mémoire

    Assets_Delete(assets);
    assets = NULL;

    Input_Delete(input);
    input = NULL;

    Timer_Delete(g_time);
    g_time = NULL;

    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    App_Quit();

    return EXIT_SUCCESS;
}
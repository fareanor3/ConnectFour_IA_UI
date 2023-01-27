#include "Assets.h"

typedef struct TextureSpec_s
{
    SDL_Texture **ptr;
    char *path;
} TextureSpec;

Assets *Assets_New(SDL_Renderer *renderer)
{
    Assets *self = (Assets *)calloc(1, sizeof(Assets));
    AssertNew(self);

    // -------------------------------------------------------------------------
    // Chargement des textures

    TextureSpec texSpecs[] = {
        { &self->textureExample, "../Assets/connect4.png" },
        { &self->MenuFrame, "../Assets/MenuFrame.png" },
        { &self->GameFrame, "../Assets/GameFrame.png" },
        { &self->DifficultyFrame, "../Assets/SelectGameDifficulty.png" },
        { &self->GameModeFrame, "../Assets/SelectGameMode.png" },
        { &self->WinOne, "../Assets/WinnerOne.png" },
        { &self->WinTwo, "../Assets/WinnerTwo.png" },
        { &self->WinTwo, "../Assets/Tie.png" },
        { &self->MenuB, "../Assets/Menu.png" },
        { &self->RestartB, "../Assets/Restart.png" },
        { &self->PlayB, "../Assets/Play.png" },
        { &self->ExitB, "../Assets/Exit.png" },
        { &self->redPawn, "../Assets/redPawn.png" },
        { &self->yellowPawn, "../Assets/yellowPawn.png" },
        { &self->column, "../Assets/Column.png" },
        { &self->AIvsAI, "../Assets/AIvsAI.png" },
        { &self->AIvsPL, "../Assets/AIvsPL.png" },
        { &self->PvP, "../Assets/PvP.png" },
        { &self->PLTurn, "../Assets/PLTurn.png" },
        { &self->Pone, "../Assets/P1Turn.png" },
        { &self->Ptwo, "../Assets/P2Turn.png" },
        
    };
    int texSpecCount = sizeof(texSpecs) / sizeof(TextureSpec);

    for (int i = 0; i < texSpecCount; i++)
    {
        SDL_Texture **texPtr = texSpecs[i].ptr;
        char *path = texSpecs[i].path;

        *texPtr = IMG_LoadTexture(renderer, path);
        if (*texPtr == NULL)
        {
            printf("ERROR - Loading texture %s\n", path);
            printf("      - %s\n", SDL_GetError());
            assert(false);
            abort();
        }
    }

    return self;
}

void Assets_Delete(Assets *self)
{
    if (!self) return;

    // -------------------------------------------------------------------------
    // Libère les textures

    SDL_Texture** texPointers[] = {
        &self->textureExample, &self->MenuB, &self->RestartB, &self->redPawn, &self->yellowPawn, &self->PlayB, & self->ExitB,
        &self->GameFrame, &self->MenuFrame, & self->GameModeFrame, & self->DifficultyFrame, &self->AIvsAI, &self->AIvsPL, &self->PvP,
        & self->WinOne,& self->WinTwo,& self->Tie,& self->PLTurn,& self->Pone,& self->Ptwo,
    };
    int count = sizeof(texPointers) / sizeof(SDL_Texture **);

    for (int i = 0; i < count; i++)
    {
        if (*texPointers[i])
            SDL_DestroyTexture(*(texPointers[i]));
    }

    free(self);
}
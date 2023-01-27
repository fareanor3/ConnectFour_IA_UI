#include "Game.h"

Game *Game_New()
{
    Game *self = (Game *)calloc(1, sizeof(Game));
    AssertNew(self);

    self->state = GAME_IN_PROGRESS;
    self->playerID = PLAYER_1;

    return self;
}

void Game_Delete(Game *self)
{
    if (!self)
        return;
    free(self);
}

Game *Game_Copy(Game *self)
{
    // TODO : Allouez une nouvelle partie puis copiez les membres de self
    Game *copy = Game_New();
    copy->playerID = self->playerID;
    copy->state = self->state;
    for (int i = 0; i < GRID_H; i++)
    {
        for (int j = 0; j < GRID_W; j++)
        {
            copy->grid[i][j] = self->grid[i][j];
        }
    }
    return copy;
}

/// @brief Met � jour l'�tat de la partie.
/// Cette m�thode est appel�e par Game_PlayTurn().
/// @param self la partie.
void Game_UpdateState(Game *self)
{
    /////////
    // TODO : Calculez ici l'�tat de la partie
    // Parcourez tous les alignements possibles
    // Le r�sultat est ensuite affect� � self->state
    // Check rows
    // Initialise l'état à GAME_IN_PROGRESS
    self->state = GAME_IN_PROGRESS;

    // Vérifie les lignes
    for (int i = 0; i < GRID_H; i++)
    {
        if (Game_CheckLine(self->grid, i, PLAYER_1))
        {
            self->state = 1;
            return;
        }
        if (Game_CheckLine(self->grid, i, PLAYER_2))
        {
            self->state = 2;
            return;
        }
    }

    // Vérifie les colonnes
    for (int j = 0; j < GRID_W; j++)
    {
        if (Game_CheckColumn(self->grid, j, PLAYER_1))
        {
            self->state = 1;
            return;
        }
        if (Game_CheckColumn(self->grid, j, PLAYER_2))
        {
            self->state = 2;
            return;
        }
    }

    // Vérifie les diagonales
    if (Game_CheckDiagonal1(self->grid, PLAYER_1))
    {
        self->state = 1;
        return;
    }
    if (Game_CheckDiagonal1(self->grid, PLAYER_2))
    {
        self->state = 2;
        return;
    }
    if (Game_CheckDiagonal2(self->grid, PLAYER_1))
    {
        self->state = 1;
        return;
    }
    if (Game_CheckDiagonal2(self->grid, PLAYER_2))
    {
        self->state = 2;
        return;
    }
    if (Game_IsFull(self))
    {
        self->state = GAME_IS_TIED;
        return;
    }
}

bool Game_CanPlayAt(Game *self, int column)
{
    assert(self);

    // Vérifie que la colonne est valide
    if (column < 0 || column >= GRID_W)
        return false;

    // Vérifie qu'il reste au moins une case vide dans la colonne
    for (int row = 0; row < GRID_H; row++)
    {
        if (self->grid[row][column] == 0)
            return true;
    }

    return false;
}

void Game_PlayTurn(Game *self, int column)
{
    assert(self);
    assert(Game_CanPlayAt(self, column));

    // Trouve la première ligne vide dans la colonne
    int row;
    for (row = 0; row < GRID_H; row++)
    {
        if (self->grid[row][column] == 0)
            break;
    }

    // Place la bille du joueur courant dans la première ligne vide de la colonne
    self->grid[row][column] = self->playerID;

    // Met à jour l'état de la partie
    Game_UpdateState(self);

    // Change le joueur courant
    self->playerID = PlayerID_GetOther(self->playerID);
}

void Game_Print(Game *self)
{
    // Tableau de caractères contenant les codes de couleur pour chaque joueur
    char *colorCodes[] = {"\x1b[0m", "\x1b[31m", "\x1b[32m"};

    // Affiche le numéro de chaque colonne en haut de la grille
    printf(" 0 | 1 | 2 | 3 | 4 | 5 | 6 \n");

    // Ligne de séparation entre le haut de la grille et les cases
    printf("---+---+---+---+---+---+---\n");

    // Parcours de la grille
    for (int i = GRID_H - 1; i >= 0; i--)
    {
        for (int j = 0; j < GRID_W; j++)
        {
            // Affiche la bille du joueur ou un espace vide et une barre verticale
            int player = self->grid[i][j];
            char *colorCode = (player > 0) ? colorCodes[player] : "";
            printf(" %s0%s |", colorCode, colorCodes[0]);
        }

        printf("\n");

        // Ligne de séparation entre les lignes de la grille
        if (i < GRID_H)
            printf("---+---+---+---+---+---+---\n");
    }
}

bool Game_CheckLine(int grid[GRID_H][GRID_W], int line, int player)
{
    assert(grid);
    assert(line >= 0 && line < GRID_H);
    // Vérifie si la ligne contient 4 jetons du joueur donné côte à côte
    for (int j = 0; j < GRID_W - 3; j++)
    {
        if (grid[line][j] == player && grid[line][j + 1] == player && grid[line][j + 2] == player && grid[line][j + 3] == player)
        {
            return true;
        }
    }

    return false;
}

bool Game_CheckColumn(int grid[GRID_H][GRID_W], int column, int player)
{
    assert(grid);
    assert(column >= 0 && column < GRID_W);
    // Vérifie si la colonne contient 4 jetons du joueur donné côte à côte
    for (int i = 0; i < GRID_H - 3; i++)
    {
        if (grid[i][column] == player && grid[i + 1][column] == player && grid[i + 2][column] == player && grid[i + 3][column] == player)
        {
            return true;
        }
    }

    return false;
}

// pas assez d'argument pour un appel de fonction
bool Game_CheckDiagonal1(int grid[GRID_H][GRID_W], int player)
{
    assert(grid);
    // Vérifie si la première diagonale contient 4 jetons du joueur donné côte à côte
    for (int i = 0; i < GRID_H - 3; i++)
    {
        for (int j = 0; j < GRID_W - 3; j++)
        {
            if (grid[i][j] == player && grid[i + 1][j + 1] == player && grid[i + 2][j + 2] == player && grid[i + 3][j + 3] == player)
            {
                return true;
            }
        }
    }

    return false;
}

bool Game_CheckDiagonal2(int grid[GRID_H][GRID_W], int player)
{
    assert(grid);
    // Vérifie si la deuxième diagonale contient 4 jetons du joueur donné côte à côte
    for (int i = 0; i < GRID_H - 3; i++)
    {
        for (int j = 3; j < GRID_W; j++)
        {
            if (grid[i][j] == player && grid[i + 1][j - 1] == player && grid[i + 2][j - 2] == player && grid[i + 3][j - 3] == player)
            {
                return true;
            }
        }
    }

    return false;
}

bool Game_IsFull(Game *game)
{
    assert(game);
    for (int i = 0; i < GRID_H; i++)
    {
        for (int j = 0; j < GRID_W; j++)
        {
            if (game->grid[i][j] == 0)
            {
                return false;
            }
        }
    }
    return true;
}

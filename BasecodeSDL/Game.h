#ifndef _GAME_H_
#define _GAME_H_

#include "Settings.h"

#define GRID_H 6
#define GRID_W 7

/// @brief Enum�ration des identifiants des joueurs.
typedef enum PlayerID_e
{
    /// @brief Identifiant du joueur 1.
    PLAYER_1 = 1,
    /// @brief Identifiant du joueur 2.
    PLAYER_2 = 2
} PlayerID;

/// @brief Renvoie l'identifiant de l'autre joueur.
/// On a donc PlayerID_GetOther(PLAYER_1) = PLAYER_2 et inversement.
/// @param playerID l'identifiant du joueur.
/// @return L'identifiant de l'autre joueur.
INLINE int PlayerID_GetOther(int playerID)
{
    return 3 - playerID;
}

/// @brief Enum�ration des �tats possibles d'une case du plateau.
typedef enum CellState_e
{
    /// @brief Indique que la case du plateau est vide.
    CELL_EMPTY,
    /// @brief Indique que la case du plateau
    /// est occup�e par un pion du joueur 1.
    CELL_P1 = 1,
    /// @brief Indique que la case du plateau
    /// est occup�e par un pion du joueur 2.
    CELL_P2 = 2
} CellState;

/// @brief Enum�ration des �tats possibles de la partie.
typedef enum GameState_e
{
    /// @brief Indique que la partie est en cours.
    GAME_IN_PROGRESS,
    /// @brief Indique que la partie est remport�e par le joueur 1.
    GAME_P1_WON = 1,
    /// @brief Indique que la partie est remport�e par le joueur 2.
    GAME_P2_WON = 2,
    /// @brief Indique que la partie s'est termin�e sur une �galit�e.
    GAME_IS_TIED
} GameState;

/// @brief Structure repr�sentant une partie de puissance 4.
typedef struct Game_s
{
    /// @brief Plateau du jeu.
    /// L'indice de ligne 0 d�signe la ligne du bas.
    /// L'indice de colonne 0 d�signe la colonne de gauche.
    /// Les valeurs possibles des cases sont d�finies dans CellState.
    int grid[GRID_H][GRID_W];

    /// @brief Identifiant du joueur courant.
    /// Les valeurs possibles sont d�finies dans PlayerID.
    int playerID;

    /// @brief Etat de la partie.
    /// Les valeurs possibles sont d�finies dans GameState.
    int state;
    /// @brief Nombre de tours jou�s.
    /// La partie commence au tour 0.
    int nbTurn;
} Game;

/// @brief Cr�e une nouvelle partie de puissance 4.
/// @return La partie cr��e.
Game *Game_New();

/// @brief D�truit une partie pr�alablement cr��e avec Game_New() ou Game_Copy().
/// @param self la partie.
void Game_Delete(Game *self);

/// @brief Copie une partie.
/// @param self la partie.
/// @return Une copie en profondeur de la partie.
Game *Game_Copy(Game *self);

/// @brief Affiche une partie sur la sortie standard.
/// @param self la partie.
void Game_Print(Game *self);

/// @brief Indique si le joueur courant peut jouer son pion dans une certaine colonne.
/// Cette fonction renvoie false si l'indice de la colonne n'est pas valide,
/// si la colonne est d�j� remplie ou si la partie est d�j� termin�e.
/// Elle renvoie true autrement.
/// @param self la partie.
/// @param column l'indice de la colonne.
/// @return true s'il est possible de jouer dans la colonne, false sinon.
bool Game_CanPlayAt(Game *self, int column);

/// @brief Effectue l'action du joueur courant puis met � jour la partie.
/// On suppose qu'il est possible de jouer dans la colonne indiqu�e.
/// L'utilisateur doit le v�rifier avec Game_CanPlayAt() avant d'appeler cette m�thode.
/// @param self la partie.
/// @param column l'indice de la colonne.
void Game_PlayTurn(Game *self, int column);

bool Game_CheckColumn(int grid[GRID_H][GRID_W], int column, int player);
bool Game_CheckLine(int grid[GRID_H][GRID_W], int row, int player);
bool Game_CheckDiagonal2(int grid[GRID_H][GRID_W], int player);
bool Game_CheckDiagonal1(int grid[GRID_H][GRID_W], int player);
bool Game_IsFull(Game *self);

/// @brief Renvoie l'�tat de la partie.
/// Les valeurs possibles sont d�finies dans l'�num�ration GameState.
/// @param self la partie.
/// @return L'�tat de la partie.
INLINE int Game_GetState(Game *self)
{
    return self->state;
}

/// @brief Revoie l'identifiant du joueur courant.
/// @param self la partie.
/// @return L'identifiant du joueur courant.
INLINE int Game_GetPlayerID(Game *self)
{
    return self->playerID;
}

#endif

#ifndef _AI_H_
#define _AI_H_

#include "Settings.h"
#include "Game.h"

/// @brief Fonction calculant l'indice de la colonne calcul  par votre IA.
/// @param game la partie courante.
/// @param playerID l'identifiant de votre IA.
/// @return L'indice de la colonne choisie par l'IA.
int AI_ComputeMove(Game *game, int playerID);

//------------------------------------------------------------------------------
//  IMPORTANT
//  Les structures et prototypes suivants repr sentent une suggestion
//  d'impl mentation, il ne sont pas impos s.
//------------------------------------------------------------------------------

/// @brief Structure repr sentant un noeud dans l'arbre de l'algorithme MinMax.
typedef struct AINode_s
{
    /// @brief Partie associ e au noeud.
    /// Il s'agit d'une copie (modifi e) de la partie en cours.
    Game *gameState;

    /// @brief Score associ e au noeud.
    int score;

    /// @brief Si le noeud n'est pas une feuille, ce champ d signe l'indice de
    /// colonne choisi par l'algorithme pour la partie associ e   ce noeud.
    /// En particulier, l'action choisie par l'algorithme est la valeur
    /// du champ move de la racine de l'arbre.
    int move;

    /// @brief Pointeurs vers les 7 enfants du noeud.
    /// S'il n'est plus possible de jouer en colonne c, children[c] vaut NULL.
    /// Si le noeud est une feuille, les 7 pointeurs vallent NULL.
    struct AINode_s *children[GRID_W];
} AINode;

/// @brief Structure repr sentant l'arbre de l'algorithme MinMax.
typedef struct AITree_s
{
    /// @brief Pointeur vers la racine de l'arbre.
    AINode *root;
} AITree;

/// @brief Structure repr sentant un noeud dans l'arbre de l'algorithme Monte carlo
typedef struct AINode_NewMCT_s
{
    Game *gameState;
    int move;
    int visits;
    int totalScore;
    int childrenCount;
    AINode *children[GRID_W];
} AINode_NewMCT;

/// @brief Structure repr sentant l'arbre de l'algorithme Monte carlo
typedef struct AITree_NewMCTS_s
{
    AINode_NewMCT *root;
} AITree_NewMCTS;

/// @brief Cr e l'arbre de l'algorithme MinMax.
/// @param game la partie courante.
/// @param depth la hauteur maximale de la racine de l'arbre.
/// @param playerID identifiant du joueur dont l'algorithme calcule l'action.
/// @return L'arbre cr   (et initialis ).
AITree *AITree_NewMinMax(Game *game, int depth, int playerID);

/// @brief D truit l'arbre MinMax.
/// @param self l'arbre.
void AITree_Delete(AITree *self);

void printArray(int grid[GRID_H][GRID_W], int rows, int cols);
void printEvaluationMatrix(int eval_matrix[GRID_H][GRID_W]);
void calculateWinningAlignments(int grid[GRID_H][GRID_W], int eval_test[][GRID_W]);
int Game_Eval(Game *self, int playerID, int eval_weights[6][7]);

void AINode_DeleteRec(AINode *self);
AINode *AINode_NewMinMax(Game *game, int depth, bool minLayer, int playerID, int alpha, int beta);
void AINode_Print(AINode *self);
void AITree_Print(AITree *self);
void AINode_PrintTree(AINode *self, int depth, int isLeft);
void AITree_PrintTree(AITree *self);
int Game_EvalLine(int grid[GRID_H][GRID_W], int row, int playerID);
int Game_EvalColumn(int grid[GRID_H][GRID_W], int row, int playerID);
int Game_EvalDiagonal1(int grid[GRID_H][GRID_W], int playerID);
int Game_EvalDiagonal2(int grid[GRID_H][GRID_W], int playerID);
int AITree_GetBestMove(AINode *root);

#endif

#include "AI.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

INLINE int Int_Max(int a, int b) { return a > b ? a : b; }
INLINE int Int_Min(int a, int b) { return a < b ? a : b; }

#define INT_Pinf 1000000
#define INT_Pbig 999999
#define INT_Ninf -1000000
#define INT_Nbig -999999

FILE *fp;

int eval_weights[6][7] = {
    {3, 4, 5, 7, 5, 4, 3},
    {4, 6, 8, 10, 8, 6, 4},
    {5, 8, 11, 13, 11, 8, 5},
    {5, 8, 11, 13, 11, 8, 5},
    {4, 6, 8, 10, 8, 6, 4},
    {3, 4, 5, 7, 5, 4, 3}};

void printEvaluationMatrix(int eval_matrix[][GRID_W])
{
    for (int i = 0; i < GRID_H; i++)
    {
        for (int j = 0; j < GRID_W; j++)
        {
            printf("%d ", eval_matrix[i][j]);
        }
        printf("\n");
    }
}

int Game_Eval(Game *self, int playerID, int eval_weights[6][7])
{
    int evaluation = 0;
    for (int i = 0; i < GRID_H; i++)
    {
        for (int j = 0; j < GRID_W; j++)
        {
            if (self->grid[i][j] == playerID)
            {
                evaluation += eval_weights[i][j];
            }
            else if (self->grid[i][j] == CELL_EMPTY)
            {
                evaluation += 0;
            }
            else
            {
                evaluation -= eval_weights[i][j];
            }
        }
    }
    return evaluation;
}

void calculateWinningAlignments(int grid[][GRID_W], int eval_test[][GRID_W])
{
    int numAlignments;
    // Loop through each cell in the grid
    for (int i = 0; i < GRID_H; i++)
    {
        for (int j = 0; j < GRID_W; j++)
        {
            numAlignments = 0;
            if (grid[i][j] == CELL_EMPTY)
            {
                // Check for horizontal alignments
                if (j <= GRID_W - 4)
                {
                    for (int k = j; k < j + 4; k++)
                    {
                        numAlignments += (grid[i][k] == CELL_EMPTY);
                    }
                }
                // Check for vertical alignments
                if (i <= GRID_H - 4)
                {
                    for (int k = i; k < i + 4; k++)
                    {
                        numAlignments += (grid[k][j] == CELL_EMPTY);
                    }
                }
                // Check for diagonal alignments (top-left to bottom-right)
                if (i <= GRID_H - 4 && j <= GRID_W - 4)
                {
                    for (int k = 0; k < 4; k++)
                    {
                        numAlignments += (grid[i + k][j + k] == CELL_EMPTY);
                    }
                }
                // Check for diagonal alignments (top-right to bottom-left)
                if (i <= GRID_H - 4 && j >= 3)
                {
                    for (int k = 0; k < 4; k++)
                    {
                        numAlignments += (grid[i + k][j - k] == CELL_EMPTY);
                    }
                }
                eval_test[i][j] = numAlignments;
            }
        }
    }
}

bool Game_IsOver(Game *self)
{
    return self->state != GAME_IN_PROGRESS;
}

GameState Game_GetWinner(Game *self)
{
    return self->state;
}

/// @brief D�truit r�cursivement un noeud et tous ses sous-arbres.
/// @param self le noeud.
void AINode_DeleteRec(AINode *self)
{
    // Destruction des sous-arbres
    for (int i = 0; i < GRID_W; i++)
    {
        if (self->children[i])
        {
            AINode_DeleteRec(self->children[i]);
        }
    }
    // On détruit la copie de la partie du noeud courant.
    Game_Delete(self->gameState);

    // On détruit le noeud courant.
    free(self);
}

/// @brief Cr�e un noeud de l'arbre de l'algorithme Alpha-beta pruning Minimax.
/// Cette m�thode est appel�e par AITree_NewMinMax().
/// @param game la partie associ�e au noeud.
/// @param depth la hauteur maximale du noeud.
/// @param minLayer indique si le noeud est un noeud MIN.
/// @param playerID identifiant du joueur dont l'algorithme calcule l'action.
/// @return Le noeud cr�� (et initialis�).
AINode *AINode_NewMinMax(Game *game, int depth, bool minLayer, int playerID, int alpha, int beta)
{
    AINode *self = (AINode *)calloc(1, sizeof(AINode));
    AssertNew(self);
    self->gameState = game;

    // Check if the game is over and return a terminal node with the appropriate score
    if (Game_IsOver(game))
    {
        if (Game_GetWinner(game) == playerID)
        {
            self->score = INT_Pbig;
        }
        else if (Game_GetWinner(game) == GAME_IS_TIED)
        {
            self->score = 0;
        }
        else
        {
            assert(Game_GetWinner(game) == PlayerID_GetOther(playerID));
            self->score = INT_Nbig;
        }
        return self;
    }

    if (depth == 0)
    {
        self->score = Game_Eval(game, playerID, eval_weights);
        return self;
    }

    self->score = minLayer ? INT_Pinf : INT_Ninf;

    for (int column = 0; column < GRID_W; column++)
    {
        int tempColumn = GRID_W / 2 + (1 - 2 * (column % 2)) * (column + 1) / 2;
        if (Game_CanPlayAt(game, tempColumn))
        {

            Game *gameCopy = Game_Copy(game);
            Game_PlayTurn(gameCopy, tempColumn);
            self->children[tempColumn] = AINode_NewMinMax(gameCopy, depth - 1, !minLayer, playerID, alpha, beta);

            if (minLayer)
            {
                if (self->score > self->children[tempColumn]->score)
                {
                    self->score = self->children[tempColumn]->score;
                    self->move = tempColumn;
                }

                beta = Int_Min(beta, self->score);
            }
            else
            {
                if (self->score < self->children[tempColumn]->score)
                {
                    self->score = self->children[tempColumn]->score;
                    self->move = tempColumn;
                }

                alpha = Int_Max(alpha, self->score);
            }

            AINode_DeleteRec(self->children[tempColumn]);
            self->children[tempColumn] = NULL;

            if (alpha >= beta)
                return self;
        }
    }
    return self;
}

AITree *AITree_NewMinMax(Game *game, int depth, int playerID)
{
    AITree *self = (AITree *)calloc(1, sizeof(AITree));
    AssertNew(self);

    Game *curGame = Game_Copy(game);
    self->root = AINode_NewMinMax(curGame, depth, false, playerID, INT_Ninf, INT_Pinf);

    return self;
}

void AITree_Delete(AITree *self)
{
    if (!self)
        return;

    AINode_DeleteRec(self->root);
    free(self);
}

int AI_ComputeMove(Game *game, int playerID)
{
    /////////
    // TODO : Cette fonction repr�sente votre IA.
    // Modifiez-la comme vous le souhaitez.
    fp = fopen("log.csv", "a");
    fprintf(fp, "move,nbTurn,PlayerID\n");
    int depth = 10;
    AITree *aiTree = AITree_NewMinMax(game, depth, playerID);
    int move = aiTree->root->move;
    AITree_Delete(aiTree);
    fprintf(fp, "%d,%d,%d,\n", move, game->nbTurn, Game_GetPlayerID(game));
    fclose(fp);
    return move;
}

/////////////
void print_tree(AINode *node, int depth)
{
    // Print out the information for the current node
    printf("Node at depth %d: score = %d\n", depth, node->score);
    // Print out the information for each child
    for (int i = 0; i < GRID_W; i++)
    {
        if (node->children[i])
        {
            print_tree(node->children[i], depth + 1);
        }
    }
}

void AINode_Print(AINode *node)
{
    if (!node)
        return;
    AINode_Print(node->children[0]);
    printf("  score = %d, move = %d\n", node->score, node->move);
    for (int i = 1; i < GRID_W; i++)
    {
        AINode_Print(node->children[i]);
    }
}

void AITree_Print(AITree *tree)
{
    printf("root = %p\n", tree->root);
    AINode_Print(tree->root);
}

void AINode_PrintTree(AINode *node, int depth, int isLeft)
{
    if (!node)
        return;

    AINode_PrintTree(node->children[GRID_W - 1], depth + 1, false);

    for (int i = 0; i < depth - 1; i++)
        printf("    ");
    if (depth > 0)
    {
        if (isLeft)
            printf("  \\-");
        else
            printf("  /-");
    }
    printf("score = %d, move = %d\n", node->score, node->move);

    for (int i = 0; i < GRID_W - 1; i++)
    {
        AINode_PrintTree(node->children[i], depth + 1, true);
    }
}

void AITree_PrintTree(AITree *tree)
{
    AINode_PrintTree(tree->root, 0, false);
}

// https://web.stanford.edu/class/archive/cs/cs221/cs221.1192/2018/restricted/posters/yuex/poster.pdf
#pragma once

#include "Settings.h"

/// @brief Structure repr�sentant un chronom�tre.
typedef struct Timer_s
{
    /// @brief Temps de d�part.
    float m_startTime;

    /// @brief Temps du dernier appel � Timer_Update().
    float m_currentTime;

    /// @brief Temps de l'avant dernier appel � Timer_Update().
    float m_previousTime;

    /// @brief Ecart entre les deux derniers appels � Timer_Update().
    float m_delta;
    float m_unscaledDelta;

    float m_scale;

    float m_maxDelta;

    float m_elapsed;
    float m_unscaledElapsed;
} Timer;

/// @brief Cr�e un nouveau timer.
/// @return Le timer cr�� ou NULL en cas d'erreur.
Timer* Timer_New();

/// @brief D�truit un timer pr�alablement allou� avec Timer_New().
/// @param[in,out] self le timer � d�truire.
void Timer_Delete(Timer* self);

/// @brief Lance le timer.
/// @param[in,out] self le timer.
void Timer_Start(Timer* self);

/// @brief Met � jour le timer en fonction du temps actuel.
/// Cette fonction est typiquement appel�e � chaque tour de la boucle de rendu.
/// @param[in,out] self le timer.
void Timer_Update(Timer* self);

INLINE void Timer_SetMaximumDeltaTime(Timer *self, float maxDelta)
{
    self->m_maxDelta = maxDelta;
}

INLINE void Timer_SetTimeScale(Timer *self, float scale)
{
    assert(scale >= 0.0f);
    self->m_scale = scale;
}

INLINE float Timer_GetTimeScale(Timer *self)
{
    return self->m_scale;
}

/// @brief Renvoie l'�cart de temps (en secondes) entre les deux derniers appels � la fonction
/// Timer_Update().
/// @param[in] self le timer.
/// @return L'�cart de temps entre les deux derni�res mises � jour.
INLINE float Timer_GetDelta(Timer *self)
{
    return self->m_delta;
}

INLINE float Timer_GetUnscaledDelta(Timer *self)
{
    return self->m_unscaledDelta;
}

/// @brief Renvoie l'�cart de temps (en secondes) entre le lancement du timer avec la fonction
/// Timer_Start() et le dernier appel � la fonction Timer_Update().
/// @param[in] self le timer.
/// @return Le nombre de secondes �coul�es depuis le lancement du timer et la derni�re mise � jour.
INLINE float Timer_GetElapsed(Timer *self)
{
    return self->m_elapsed;
}

INLINE float Timer_GetUnscaledElapsed(Timer *self)
{
    return self->m_unscaledElapsed;
}

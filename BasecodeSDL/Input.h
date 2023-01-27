#pragma once

#include "Settings.h"

typedef struct Input_s
{
    bool quitPressed;
    bool mouseClick;

} Input;

Input *Input_New();
void Input_Delete(Input *self);
void Input_Update(Input *self);

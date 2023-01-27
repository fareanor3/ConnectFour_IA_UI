#include "Input.h"
#include "Math.h"
#include "Common.h"

Input *Input_New()
{
    Input *self = (Input *)calloc(1, sizeof(Input));
    AssertNew(self);

    return self;
}

void Input_Delete(Input *self)
{
    if (!self) return;
    free(self);
}

void Input_Update(Input *self)
{
    self->quitPressed = false;

    SDL_Event evt;
    while (SDL_PollEvent(&evt))
    {
        switch (evt.type)
        {
        case SDL_QUIT:
            self->quitPressed = true;
            break;

        case SDL_KEYDOWN:
            if (evt.key.repeat)
                break;

            switch (evt.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                self->quitPressed = true;
                break;

            default:
                break;
            }
            break;

        case SDL_KEYUP:
            if (evt.key.repeat)
                break;

            switch (evt.key.keysym.scancode)
            {
            default:
                break;
            }
            break;
        
        case SDL_MOUSEBUTTONDOWN:
            self->mouseClick = true;
            break;
        }
    }
}

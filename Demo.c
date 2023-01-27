/***** Demo.c *****/
/* Ken Shoemake, 1993 */

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#endif

#include "BallAux.h"
#include "Body.h"
#include "Ball.h"
#include "BallMath.h"

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <stdbool.h>

typedef struct {long x, y;} Place;


#define RADIUS (0.75)

typedef struct HMatrixRet {
    HMatrix matrix;
} HMatrixRet;

HMatrixRet perspective(
    const float fovy, const float aspect, const float n, const float f) {
    const float e = 1.0f / tanf(fovy * 0.5f);
    return (HMatrixRet){
        .matrix = {
            {e / aspect, 0.0f,  0.0f,                    0.0f},
            {0.0f,       e,     0.0f,                    0.0f},
            {0.0f,       0.0f, (f + n) / (n - f),       -1.0f},
            {0.0f,       0.0f, (2.0f * f * n) / (n - f), 0.0f}}};
}

float radians(const float degrees)
{
    return degrees * g_pi / 180.0f;
}

/* Draw the object being controlled. */
void body_Draw(BallData *ball)
{
    HMatrix mNow;
    Ball_Value(ball, mNow);
    glPushMatrix();
    glMultMatrixf(&mNow[0][0]);
    glScalef(RADIUS, RADIUS, RADIUS);
    drawbody(mNow);
    glPopMatrix();
}

/* Draw whole window, including controller. */
void scene_Draw(BallData *ball)
{
    body_Draw(ball);
    Ball_Draw(ball);
}


int main(int argc, char** argv)
{
    Place mouseNow;
    int keysDown = 0;
    HVect vNow;
    BallData ball;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

    const int width = 600;
    const int height = 600;
    const float aspect = (float)width / (float)height;
    SDL_Window *window = SDL_CreateWindow(
        argv[0], SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    const SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);
    SDL_GL_SetSwapInterval(1); // enable vsync

    // gluPerspective alternative
    HMatrixRet persp = perspective(
        radians(90.0f), (float)width / (float)height, 0.01f, 100000.f);
    glMultMatrixf(&persp.matrix[0][0]);

    glTranslatef(0.0f, 0.0f, -2.0f);

    Ball_Init(&ball);
    Ball_Place(&ball, qOne, RADIUS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    enum Keys {
        Keys_LeftShift = 1 << 0,
        Keys_LeftCtrl = 1 << 1,
        Keys_Caps = 1 << 2
    };

    for (bool quit = false; !quit;) {
        for (SDL_Event current_event; SDL_PollEvent(&current_event) != 0;) {
            if (current_event.type == SDL_QUIT) {
                quit = true;
                break;
            }

            if (current_event.type == SDL_MOUSEMOTION) {
                SDL_MouseMotionEvent *mouse_motion = (SDL_MouseMotionEvent *)&current_event;
                mouseNow.x = mouse_motion->x;
                vNow.x = 2.0f * mouseNow.x / width - 1.0f;
                mouseNow.y = height - mouse_motion->y;
                vNow.y = 2.0f * mouseNow.y / height - 1.0f;
            }

            if (current_event.type == SDL_MOUSEBUTTONDOWN) {
                SDL_MouseButtonEvent *mouse_button = (SDL_MouseButtonEvent *)&current_event;
                if (mouse_button->button == SDL_BUTTON_LEFT) {
                    Ball_BeginDrag(&ball);
                }
            }

            if (current_event.type == SDL_MOUSEBUTTONUP) {
                SDL_MouseButtonEvent *mouse_button = (SDL_MouseButtonEvent *)&current_event;
                if (mouse_button->button == SDL_BUTTON_LEFT) {
                    Ball_EndDrag(&ball);
                }
            }

            if (current_event.type == SDL_KEYDOWN) {
                SDL_KeyboardEvent *keyboard_event = (SDL_KeyboardEvent *)&current_event;
                if (keyboard_event->keysym.sym == SDLK_LCTRL) {
                    keysDown |= Keys_LeftCtrl;
                }
                if (keyboard_event->keysym.sym == SDLK_LSHIFT) {
                    keysDown |= Keys_LeftShift;
                }
                if (keyboard_event->keysym.sym == SDLK_CAPSLOCK) {
                    Ball_ShowResult(&ball);
                }
            }

            if (current_event.type == SDL_KEYUP) {
                SDL_KeyboardEvent *keyboard_event = (SDL_KeyboardEvent *)&current_event;
                if (keyboard_event->keysym.sym == SDLK_LCTRL) {
                    keysDown &= ~Keys_LeftCtrl;
                }
                if (keyboard_event->keysym.sym == SDLK_LSHIFT) {
                    keysDown &= ~Keys_LeftShift;
                }
                if (keyboard_event->keysym.sym == SDLK_CAPSLOCK) {
                    Ball_HideResult(&ball);
                }
            }
        }

        Ball_Mouse(&ball, vNow);

        if (((keysDown & Keys_LeftCtrl) != 0) && (keysDown & Keys_LeftShift) != 0) {
            Ball_UseSet(&ball, OtherAxes);
        }
        else if ((keysDown & Keys_LeftCtrl) != 0) {
            Ball_UseSet(&ball, BodyAxes);
        }
        else if ((keysDown & Keys_LeftShift) != 0) {
            Ball_UseSet(&ball, CameraAxes);
        }
        else {
            Ball_UseSet(&ball, NoAxes);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Ball_Update(&ball);
        scene_Draw(&ball);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

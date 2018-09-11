
/*
This file is part of OGS Mahjong components:
  https://github.com/OGStudio/ogs-mahjong-components

Copyright (C) 2018 Opensource Game Studio

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

// main-web Start
#include "main.h"
#include <emscripten.h>
#include <SDL2/SDL.h>

// main-web End

using namespace omc;

// main-web Start
// We use Example global variable in loop() function.
main::Example *example = 0;

std::string emscriptenResultToString(EMSCRIPTEN_RESULT result)
{
    switch (result)
    {
        case EMSCRIPTEN_RESULT_SUCCESS:
            return "success";
        case EMSCRIPTEN_RESULT_DEFERRED:
            return "deferred";
        case EMSCRIPTEN_RESULT_NOT_SUPPORTED:
            return "not supported";
        case EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED:
            return "failed not deferred";
        case EMSCRIPTEN_RESULT_INVALID_TARGET:
            return "invalid target";
        case EMSCRIPTEN_RESULT_UNKNOWN_TARGET:
            return "unknown target";
        case EMSCRIPTEN_RESULT_INVALID_PARAM:
            return "invalid param";
        case EMSCRIPTEN_RESULT_FAILED:
            return "failed";
        case EMSCRIPTEN_RESULT_NO_DATA:
            return "no data";
    }
    return "UNEXPECTED-RESULT";
}

void getSize(const std::string &name)
{
    double width = 0;
    double height = 0;
    auto result = emscripten_get_element_css_size(name.c_str(), &width, &height);
    auto resultDescription = emscriptenResultToString(result);
    printf("emscripten_get_element_css_size(%s): '%s'\n", name.c_str(), resultDescription.c_str());
    if (result < 0)
    {
        printf("Could not get element css size\n");
        //return;
    }

    printf("'%s' width: '%f' height: '%f'\n", name.c_str(), width, height);

}

void debug()
{
    getSize("canvas");
    getSize("window");
}

// TODO REMOVE
void printEvent(const SDL_Event &e)
{
    printf("Event type: '%d'\n", e.type);
    // Make sure we have window event.
    if (e.type != SDL_WINDOWEVENT)
    {
        return;
    }
    switch (e.window.event)
    {
        case SDL_WINDOWEVENT_SHOWN:
            printf("shown\n");
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            printf("exposed\n");
            break;
        case SDL_WINDOWEVENT_MOVED:
            printf("moved\n");
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            printf("minimized\n");
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            printf("maximized\n");
            break;
        case SDL_WINDOWEVENT_RESIZED:
            printf("resized\n");
            break;
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            printf("size changed\n");
            break;
        case SDL_WINDOWEVENT_ENTER:
            printf("enter\n");
            break;
        case SDL_WINDOWEVENT_LEAVE:
            printf("leave\n");
            break;
        case SDL_WINDOWEVENT_TAKE_FOCUS:
            printf("take focus\n");
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            printf("focus gained\n");
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            printf("focus lost\n");
            break;
        default:
            printf("smth else\n");
    }
}

EM_BOOL resizeCallback(int eventType, const EmscriptenUiEvent *event, void *userData)
{
    printf("resize callback\n");

    return EM_TRUE;
}

// Stand alone function that is called by Emscripten to run the app.
void loop()
{
    if (example)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            printEvent(e);
            debug();
            example->app->handleEvent(e);
        }
        example->app->frame();
    }
}

// main-web End

int main(int argc, char *argv[])
{
 
    // main-web Start
    
        getSize("canvas");
        getSize("window");
    
    // Make sure SDL is working.
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Could not init SDL: '%s'\n", SDL_GetError());
        return 1;
    }
    // Clean SDL up at exit.
    atexit(SDL_Quit);
    // Configure rendering context.
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // Create rendering window.
    int width = 800;
    int height = 600;
    SDL_Window* window =
        SDL_CreateWindow(
            main::EXAMPLE_TITLE,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_OPENGL);
    if (!window)
    {
        printf("Could not create window: '%s'\n", SDL_GetError());
        return 1;
    }
    
    printf("created render window\n");
    
    SDL_GL_CreateContext(window);
    main::Example::Parameters parameters;
    
    emscripten_set_resize_callback(0, 0, EM_FALSE, resizeCallback);
    
    // main-web End
    // main-web Start
    example = new main::Example(parameters);
    example->app->setupWindow(width, height);
    
    // main-web End
    // main-web Start
    // Render asynchronously.
    emscripten_set_main_loop(loop, -1, 0);
    // main-web End

    return 0;
}


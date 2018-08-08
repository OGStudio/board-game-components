
/*
This file is part of Mahjong components:
  https://github.com/OGStudio/mahjong-components

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

// main-desktop Start
#include "main.h"

// main-desktop End

using namespace mc;


int main(int argc, char *argv[])
{
    // main+Arguments-desktop Start
    // Print all arguments.
    for (auto i = 1; i < argc; ++i)
    {
        auto argument = std::string(argv[i]);
        if (format::stringStartsWith(argument, "--"))
        {
            // Skip the dashes.
            auto option = argument.substr(2);
            auto keyAndValue = format::splitString(option, "=");
            if (keyAndValue.size() == 2)
            {
                auto key = keyAndValue[0];
                auto value = keyAndValue[1];
                MC_MAIN_LOG(
                    "Argument key: '%s' value: '%s'",
                    key.c_str(),
                    value.c_str()
                );
            }
    
        }
    }
    // main+Arguments-desktop End
    // main-desktop Start
    auto example = new main::Example();
    example->app->setupWindow(main::EXAMPLE_TITLE, 100, 100, 480, 320);
    
    // main-desktop End
    // main-desktop Start
    example->app->run();
    delete example;
    // main-desktop End
    return 0;
}


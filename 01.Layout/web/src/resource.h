
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

#ifndef OGS_MAHJONG_COMPONENTS_RESOURCE_H
#define OGS_MAHJONG_COMPONENTS_RESOURCE_H

// ResourceStreamBuffer Start
#include <iostream>

// ResourceStreamBuffer End


namespace omc
{
namespace resource
{

// Resource Start
//! Resource container.
struct Resource
{
    Resource(
        const std::string &group,
        const std::string &name,
        unsigned char *contents,
        unsigned int len
    ) :
        group(group),
        name(name),
        contents(contents),
        len(len)
    { }

    std::string group;
    std::string name;
    unsigned char *contents;
    unsigned int len;
};
// Resource End
// ResourceStreamBuffer Start
//! Work with Resource contents as with any stream.
struct ResourceStreamBuffer : std::streambuf
{
    ResourceStreamBuffer(const Resource &resource)
    {
        char *contents = reinterpret_cast<char *>(resource.contents);
        this->setg(contents, contents, contents + resource.len);
    }
    // Implement 'seekoff()' to support 'seekg()' calls.
    // OpenSceneGraph plugins like OSG and ImageIO use 'seekg()'.
    // Topic: How to implement custom std::streambuf's seekoff()?
    // Source: https://stackoverflow.com/a/46068920
    std::streampos seekoff(
        std::streamoff off,
        std::ios_base::seekdir dir,
        std::ios_base::openmode which = std::ios_base::in
    ) {
        if (dir == std::ios_base::cur)
            this->gbump(off);
        else if (dir == std::ios_base::end)
            this->setg(this->eback(), this->egptr() + off, this->egptr());
        else if (dir == std::ios_base::beg)
            this->setg(this->eback(), this->eback() + off, this->egptr());
        return this->gptr() - this->eback();
    }
};
// ResourceStreamBuffer End

// string Start
std::string string(const Resource &resource)
{
    const char *contents = reinterpret_cast<const char *>(resource.contents);
    return std::string(contents, resource.len);
}
// string End



} // namespace resource
} // namespace omc

#endif // OGS_MAHJONG_COMPONENTS_RESOURCE_H


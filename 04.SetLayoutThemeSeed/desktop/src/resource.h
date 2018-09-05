
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
// node Start
#include <osgDB/Registry>

// node End
// setTextureImage Start
#include <osg/Texture2D>
#include <osgDB/Registry>

// setTextureImage End
// createTexture Start
#include <osg/Texture2D>

// createTexture End

// RESOURCE_LOG Start
#include "log.h"
#include "format.h"
#define RESOURCE_LOG_PREFIX "resource %s"
#define RESOURCE_LOG(...) \
    log::logprintf( \
        RESOURCE_LOG_PREFIX, \
        format::printfString(__VA_ARGS__).c_str() \
    )

// RESOURCE_LOG End

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

// extension Start
std::string extension(const Resource &resource)
{
    auto dotPosition = resource.name.rfind(".");
    // Return empty extension if we cannot detect it.
    if (dotPosition == std::string::npos)
    {
        RESOURCE_LOG(
            "ERROR Could not detect file extension for '%s/%s' resource",
            resource.group.c_str(),
            resource.name.c_str()
        );
        return "";
    }
    return resource.name.substr(dotPosition + 1);
}
// extension End
// node Start
osg::ref_ptr<osg::Node> node(Resource &resource, const std::string ext = "")
{
    osg::ref_ptr<osg::Node> node;
    // Get extension from resource's name if extension is not specified.
    std::string ex = ext.empty() ?  extension(resource) : ext;
    // Return empty node if extention is absent.
    if (ex.empty())
    {
        RESOURCE_LOG(
            "ERROR Could not read node of '%s/%s' resource "
            "because extension is absent",
            resource.group.c_str(),
            resource.name.c_str()
        );
        return node.release();
    }
    auto reader = osgDB::Registry::instance()->getReaderWriterForExtension(ex);
    if (reader)
    {
        ResourceStreamBuffer buf(resource);
        std::istream in(&buf);
        auto result = reader->readNode(in, 0);
        if (result.success())
        {
            node = result.getNode();
        }
        else
        {
            RESOURCE_LOG(
                "ERROR Could not read node of '%s/%s' resource from buffer.",
                resource.group.c_str(),
                resource.name.c_str()
            );
        }
    }
    else
    {
        RESOURCE_LOG(
            "ERROR Could not read node of '%s/%s' resource because "
            "node reader for extension '%s' is absent.",
            resource.group.c_str(),
            resource.name.c_str(),
            ex.c_str()
        );
    }
    return node.release();
}
// node End
// string Start
std::string string(const Resource &resource)
{
    const char *contents = reinterpret_cast<const char *>(resource.contents);
    return std::string(contents, resource.len);
}
// string End
// stringToResourceContents Start
unsigned char * stringToResourceContents(const std::string &str)
{
    auto dat = const_cast<char *>(str.data());
    if (!dat)
    {
        RESOURCE_LOG(
            "ERROR Could not convert string to resource contents "
            "at 'const char * -> char *' stage"
        );
        return 0;
    }
    auto contents = reinterpret_cast<unsigned char *>(dat);
    if (!contents)
    {
        RESOURCE_LOG(
            "ERROR Could not convert string to resource contents "
            "at 'char * -> usigned char *' stage"
        );
        return 0;
    }

    return contents;
}
// stringToResourceContents End

// setTextureImage Start
//! Set image for texture.

// \param texture Texture to set image for.
// \param resource Resource to take image from.
// \param ext (optional) If extension is present it won't be autodetected from the resource's name
void setTextureImage(
    osg::Texture2D *texture,
    const Resource &resource,
    const std::string ext = ""
) {
    // Get extension from resource's name if extension is not specified.
    std::string ex = ext.empty() ?  extension(resource) : ext;
    // Do nothing if extension is absent.
    if (ex.empty())
    {
        RESOURCE_LOG(
            "ERROR Could not read image of '%s/%s' resource "
            "because extension is absent",
            resource.group.c_str(),
            resource.name.c_str()
        );
        return;
    }
    auto reader =
        osgDB::Registry::instance()->getReaderWriterForExtension(ex);
    if (reader)
    {
        ResourceStreamBuffer buf(resource);
        std::istream in(&buf);
        auto result = reader->readImage(in, 0);
        if (result.success())
        {
            // NOTE I could not get resulting osg::Image outside the function.
            // NOTE Somehow just returning result.getImage() does not work.
            texture->setImage(result.getImage());
        }
        else
        {
            RESOURCE_LOG(
                "ERROR Could not read image of '%s/%s' resource from buffer.",
                resource.group.c_str(),
                resource.name.c_str()
            );
        }
    }
    else
    {
        RESOURCE_LOG(
            "ERROR Could not read image of '%s/%s' resource because "
            "image reader for extension '%s' is absent.",
            resource.group.c_str(),
            resource.name.c_str(),
            ex.c_str()
        );
    }
}
// setTextureImage End
// createTexture Start
//! Create texture from a resource.
osg::Texture2D *createTexture(const Resource &resource)
{
    osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D;
    setTextureImage(tex, resource);
    tex->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
    tex->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
    tex->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
    tex->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
    return tex.release();
}
// createTexture End

// expandGitHubPath Start
std::string expandGitHubPath(const std::string &path)
{
    const std::string prefix = "github://";

    // Make sure we have expected prefix.
    if (!format::stringStartsWith(path, prefix))
    {
        return path;
    }

    auto location = path.substr(prefix.length());
    auto parts = format::splitString(location, "/");

    // Make sure we have enough location parts to assemble both
    // repository name and file name.
    if (parts.size() < 3)
    {
        return path;
    }

    auto repository =
        format::printfString("%s/%s", parts[0].c_str(), parts[1].c_str());
    auto fileName = location.substr(repository.length() + 1);
    auto githubPrefix = "https://raw.githubusercontent.com";
    auto githubBranch = "master";
    return
        format::printfString(
            "%s/%s/%s/%s",
            githubPrefix,
            repository.c_str(),
            githubBranch,
            fileName.c_str()
        );
}
// expandGitHubPath End
// isPathRemote Start
bool isPathRemote(const std::string &path)
{
    return
        format::stringStartsWith(path, "http://") ||
        format::stringStartsWith(path, "https://")
        ;
}

// isPathRemote End

} // namespace resource
} // namespace omc

#endif // OGS_MAHJONG_COMPONENTS_RESOURCE_H


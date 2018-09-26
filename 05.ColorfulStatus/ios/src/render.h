
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

#ifndef OGS_MAHJONG_COMPONENTS_RENDER_H
#define OGS_MAHJONG_COMPONENTS_RENDER_H

// createGraphicsContext-ios Start
#include <osgViewer/api/IOS/GraphicsWindowIOS>

// createGraphicsContext-ios End
// createShaderProgram Start
#include <osg/Program>

// createShaderProgram End
// setupCamera Start
#include <osg/Camera>

// setupCamera End
// TileTheme Start
#include <osg/Geode>
#include <osg/Geometry>

// TileTheme End
// VBOSetupVisitor Start
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/NodeVisitor>

// VBOSetupVisitor End

// RENDER_TILE_THEME_LOG Start
#include "log.h"
#include "format.h"
#define RENDER_TILE_THEME_LOG_PREFIX "render::TileTheme(%p) %s"
#define RENDER_TILE_THEME_LOG(...) \
    log::logprintf( \
        RENDER_TILE_THEME_LOG_PREFIX, \
        this, \
        format::printfString(__VA_ARGS__).c_str() \
    )
// RENDER_TILE_THEME_LOG End

namespace omc
{
namespace render
{

// createGraphicsContext-ios Start
// Create graphics context for iOS.
osg::GraphicsContext *createGraphicsContext(
    int width,
    int height,
    float scale,
    UIView *parentView
) {
    // Traits is a struct to combine necessary parameters.
    osg::GraphicsContext::Traits *traits =
        new osg::GraphicsContext::Traits;
    // Geometry.
    traits->x = 0;
    traits->y = 0;
    traits->width = width * scale;
    traits->height = height * scale;
    // Double buffer (simply put, it's a flicker fix).
    traits->doubleBuffer = true;
    // Parent view.
    osg::ref_ptr<osgViewer::GraphicsWindowIOS::WindowData> dat =
        new osgViewer::GraphicsWindowIOS::WindowData(
            parentView,
            0,
            osgViewer::GraphicsWindowIOS::WindowData::IGNORE_ORIENTATION
        );
    dat->setViewContentScaleFactor(scale);
    traits->inheritedWindowData = dat;
    // Create GC.
    return osg::GraphicsContext::createGraphicsContext(traits);
}
// createGraphicsContext-ios End
// createShaderProgram Start
osg::Program *createShaderProgram(
    const std::string &vertexShader,
    const std::string &fragmentShader
) {
    // Load shaders.
    osg::Shader *vs = new osg::Shader(osg::Shader::VERTEX, vertexShader);
    osg::Shader *fs = new osg::Shader(osg::Shader::FRAGMENT, fragmentShader);
    // Compile shaders and compose shader program.
    osg::ref_ptr<osg::Program> prog = new osg::Program;
    prog->addShader(vs);
    prog->addShader(fs);
    return prog.release();
}
// createShaderProgram End
// setupCamera Start
// Configure camera with common defaults.
void setupCamera(
    osg::Camera *cam,
    osg::GraphicsContext *gc,
    double fovy,
    int width,
    int height
) {
    // Provide GC.
    cam->setGraphicsContext(gc);
    // Viewport must have the same size.
    cam->setViewport(new osg::Viewport(0, 0, width, height));
    // Clear depth and color buffers each frame.
    cam->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    // Aspect ratio.
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    // Configure projection.
    cam->setProjectionMatrixAsPerspective(fovy, aspect, 1, 1000);
}
// setupCamera End


// TileTheme Start
class TileTheme
{
    public:
        typedef std::vector<int> Indices;

        //! \param faceIndices Face vertex indices; must have four items in (top-left, bottom-left, bottom-right, top-right) order
        TileTheme(
            const osg::Vec2 textureSize,
            const osg::Vec2 faceSize,
            const Indices faceIndices
        ) :
            textureSize(textureSize),
            faceSize(faceSize),
            faceIndices(faceIndices)
        {
            // Calculate face size in texture coordinates.
            this->faceTexSize = {
                this->faceSize.x() / this->textureSize.x(),
                this->faceSize.y() / this->textureSize.y()
            };
            // Find out how many faces fit single texture row.
            this->facesPerRow = this->textureSize.x() / this->faceSize.x();
        }

        void setFaceId(int id, osg::Geode *geode)
        {
            // Make sure inputs are valid.
            std::string errorMessage;
            if (!this->validateInputs(id, geode, errorMessage))
            {
                RENDER_TILE_THEME_LOG(
                    "ERROR Could not set face id for geode: '%s'",
                    errorMessage.c_str()
                );
                return;
            }

            // Find out face location.
            int row = id / this->facesPerRow;
            int column = id - row * this->facesPerRow;

            // Calculate face texture coordinates.
            osg::Vec2 topRight = {
                this->faceTexSize.x() * (column + 1),
                1.0f - this->faceTexSize.y() * row
            };
            osg::Vec2 topLeft = {
                this->faceTexSize.x() * column,
                1.0f - this->faceTexSize.y() * row
            };
            osg::Vec2 bottomLeft = {
                this->faceTexSize.x() * column,
                1.0f - this->faceTexSize.y() * (row + 1)
            };
            osg::Vec2 bottomRight = {
                this->faceTexSize.x() * (column + 1),
                1.0f - this->faceTexSize.y() * (row + 1)
            };

            auto geom =
                reinterpret_cast<osg::Geometry *>(geode->getDrawable(0));
            auto coords =
                reinterpret_cast<osg::Vec2Array *>(geom->getTexCoordArray(0));

            // Update face texture coordinates.
            (*coords)[this->faceIndices[0]] = topLeft;
            (*coords)[this->faceIndices[1]] = bottomLeft;
            (*coords)[this->faceIndices[2]] = bottomRight;
            (*coords)[this->faceIndices[3]] = topRight;
        }

    private:
        bool validateInputs(
            int faceId,
            osg::Geode *geode,
            std::string &errorMessage
        ) {
            // Make sure geode has geometry.
            auto geom =
                reinterpret_cast<osg::Geometry *>(geode->getDrawable(0));
            if (!geom)
            {
                errorMessage = "No geometry found";
                return false;
            }

            // Make sure we have texture coordinates.
            if (!geom->getNumTexCoordArrays())
            {
                errorMessage = "Geode has no texture coordinates";
                return false;
            }

            // Make sure texture coordinates' type is valid.
            auto coords =
                reinterpret_cast<osg::Vec2Array *>(geom->getTexCoordArray(0));
            if (!coords)
            {
                errorMessage = "Geode has invalid texture coordinates' type";
                return false;
            }

            // Make sure there are exactly four indices.
            if (this->faceIndices.size() != 4)
            {
                errorMessage = "Size of face indices is not equal to `4`";
                return false;
            }

            // Make sure face indices are in bounds.
            for (auto faceIndex : this->faceIndices)
            {
                if (faceIndex >= coords->size())
                {
                    errorMessage = "Face index is out of bounds";
                    return false;
                }
            }

            // Everything is fine.
            return true;
        }

        const osg::Vec2 textureSize;
        const osg::Vec2 faceSize;
        const Indices faceIndices;
        osg::Vec2 faceTexSize;
        int facesPerRow;
};
// TileTheme End
// VBOSetupVisitor Start
//! This class forces the use of VBO.
class VBOSetupVisitor : public osg::NodeVisitor
{
    public:
        VBOSetupVisitor() :
            osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN) { }

        virtual void apply(osg::Geode &geode)
        {
            for (auto i = 0; i < geode.getNumDrawables(); ++i)
            {
                osg::Geometry *geom =
                    dynamic_cast<osg::Geometry*>(geode.getDrawable(i));
                if (geom)
                {
                    geom->setUseVertexBufferObjects(true);
                }
            }
            NodeVisitor::apply(geode);
        }
};
// VBOSetupVisitor End

} // namespace render
} // namespace omc

#endif // OGS_MAHJONG_COMPONENTS_RENDER_H


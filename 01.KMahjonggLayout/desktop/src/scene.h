
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

#ifndef MAHJONG_COMPONENTS_SCENE_H
#define MAHJONG_COMPONENTS_SCENE_H

// createShape Start
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>

// createShape End

namespace mc
{
namespace scene
{

// createShape Start
osg::MatrixTransform *createShape(osg::Shape *shape)
{
    auto hints = new osg::TessellationHints;
    hints->setDetailRatio(0.5);
    auto geode = new osg::Geode();
    geode->addDrawable(new osg::ShapeDrawable(shape, hints));
    osg::ref_ptr<osg::MatrixTransform> node = new osg::MatrixTransform;
    node->addChild(geode);
    return node.release();
}
// createShape End
// createSphere Start
osg::MatrixTransform *createSphere(float radius)
{
    auto sphere = new osg::Sphere(osg::Vec3f(0, 0, 0), radius);
    return createShape(sphere);
}
// createSphere End

} // namespace scene
} // namespace mc

#endif // MAHJONG_COMPONENTS_SCENE_H

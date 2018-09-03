
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

#ifndef OGS_MAHJONG_COMPONENTS_SCENE_H
#define OGS_MAHJONG_COMPONENTS_SCENE_H

// setSimplePosition Start
#include <osg/MatrixTransform>

// setSimplePosition End

namespace omc
{
namespace scene
{


// degreesToQuaternion Start
//! Convert from degrees to quaternion.
osg::Quat degreesToQuaternion(const osg::Vec3f &degrees)
{
    osg::Quat q;
    q.makeRotate(
        osg::DegreesToRadians(degrees.x()), osg::Vec3(1, 0, 0),
        osg::DegreesToRadians(degrees.y()), osg::Vec3(0, 1, 0),
        osg::DegreesToRadians(degrees.z()), osg::Vec3(0, 0, 1)
    );
    return q;
}
// degreesToQuaternion End
// setSimplePosition Start
//! Set node position.
//! NOTE Only works for non-rotated/scaled nodes.
void setSimplePosition(osg::MatrixTransform *node, const osg::Vec3f &position)
{
    auto matrix = node->getMatrix();
    node->setMatrix(
        osg::Matrix::scale(matrix.getScale()) *
        osg::Matrix::rotate(matrix.getRotate()) *
        osg::Matrix::translate(position)
    );
}
// setSimplePosition End
// setSimpleRotation Start
//! Set node rotation.
void setSimpleRotation(osg::MatrixTransform *node, const osg::Vec3f &rotation)
{
    osg::Quat quat = degreesToQuaternion(rotation);
    node->setMatrix(
        osg::Matrix::scale(node->getMatrix().getScale()) *
        osg::Matrix::rotate(quat) *
        osg::Matrix::translate(node->getMatrix().getTrans())
    );
}
// setSimpleRotation End

// nodeAtPosition Start
//! Pick node at the specified position using camera's point of view

// \param excludedNodeMask Take the node into consideration if it excludes specified mask.
osg::Node *nodeAtPosition(
    const osg::Vec2f &position,
    osg::Camera *camera,
    unsigned int excludedNodeMask
) {
    // Find intersections.
    osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector =
        new osgUtil::LineSegmentIntersector(
            osgUtil::Intersector::WINDOW,
            position.x(),
            position.y()
        );
    osgUtil::IntersectionVisitor iv(intersector.get());
    camera->accept(iv);

    // No intersections found.
    if (!intersector->containsIntersections())
    {
        return 0;
    }

    // Get closest intersection.
    auto intersection = intersector->getFirstIntersection();
    for (auto node : intersection.nodePath)
    {
        // Make sure node mask is excluded.
        if ((node->getNodeMask() & excludedNodeMask) != excludedNodeMask)
        {
            return node;
        }
    }

    return 0;
}
// nodeAtPosition End

} // namespace scene
} // namespace omc

#endif // OGS_MAHJONG_COMPONENTS_SCENE_H


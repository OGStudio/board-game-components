FEATURE main.h/Include
#include "mahjong.h"
#include "scene.h"
#include <osg/MatrixTransform>

FEATURE main.h/Impl
osg::MatrixTransform* createTiles(const mahjong::Layout &layout)
{
    // Create scene to host tile nodes.
    osg::ref_ptr<osg::MatrixTransform> tileScene = new osg::MatrixTransform;

    // TODO Update this function to take 4 tiles from each group.
    const int FACES_COUNT = 42;
    const float MODEL_HEIGHT_FACTOR = 1.5;
    int faceId = 0;

    // Generate tile nodes.
    for (auto pos : layout.positions)
    {
        // Clone tile.
        auto tile = new osg::Geode(*this->tileModel, osg::CopyOp::DEEP_COPY_ALL);
        // Create node for it.
        auto node = new osg::MatrixTransform;
        node->addChild(tile);
        // Set node's position.
        float x = pos.column;
        float y = pos.row * MODEL_HEIGHT_FACTOR;
        float z = pos.field;
        scene::setSimplePosition(node, {x, y, z});
        // Set tile face id.
        this->theme->setFaceId(faceId, tile);
        // Cycle face id.
        if (++faceId >= FACES_COUNT)
        {
            faceId = 0;
        }

        // Add tile node to the scene.
        tileScene->addChild(node);
    }

    return tileScene.release();
}

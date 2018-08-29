FEATURE main.h/Include
#include "mahjong.h"
#include "scene.h"
#include <osg/MatrixTransform>

FEATURE main.h/Impl
// Make sure positions' count is equal to matchIds' one.
osg::MatrixTransform* createTiles(
    const mahjong::Layout::Positions &positions,
    mahjong::MatchIds &matchIds
) {
    // Create scene to host tile nodes.
    osg::ref_ptr<osg::MatrixTransform> tileScene = new osg::MatrixTransform;
    // Correct the height for `tile-low.osgt` model.
    const float MODEL_HEIGHT_FACTOR = 1.5;

    int id = 0;
    // Generate tile nodes.
    for (auto pos : positions)
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
        // Set match id.
        int matchId = matchIds[id++];
        this->theme->setFaceId(matchId, tile);

        // Add tile node to the scene.
        tileScene->addChild(node);
    }

    return tileScene.release();
}

FEATURE render.h/Include
#include <osg/Geode>
#include <osg/Geometry>

FEATURE render.h/Impl
class TileTheme
{
    public:
        TileTheme(
            const osg::Vec2 textureSize,
            const osg::Vec2 faceSize
        ) :
            textureSize(textureSize),
            faceSize(faceSize)
        {
            // Calculate face size in texture coordinates.
            this->faceTexSize = {
                this->faceSize.x() / this->textureSize.x(),
                this->faceSize.y() / this->textureSize.y()
            };
            // Find out how many faces fit single texture row.
            this->facesPerRow = this->textureSize.x() / this->faceSize.x();
        }

        void setFaceId(int id, osg::Geode *geode, int texCoordStartIndex)
        {
            // Make sure geode has geometry.
            auto geom =
                reinterpret_cast<osg::Geometry *>(geode->getDrawable(0));
            if (!geom)
            {
                MC_RENDER_TILE_THEME_LOG(
                    "ERROR Could not set face id for geode without geometry"
                );
                return;
            }

            // Make sure we have texture coordinates.
            if (!geom->getNumTexCoordArrays())
            {
                MC_RENDER_TILE_THEME_LOG(
                    "ERROR Geode has no texture coordinates"
                );
                return;
            }

            // Make sure texture coordinates are valid.
            auto coords =
                reinterpret_cast<osg::Vec2Array *>(geom->getTexCoordArray(0));
            if (!coords)
            {
                MC_RENDER_TILE_THEME_LOG(
                    "ERROR Geode has invalid texture coordinates"
                );
                return;
            }

            // Make sure provided start index is in bounds.
            auto indexIsInBounds = (coords->size() - texCoordStartIndex >= 4);
            if (!indexIsInBounds)
            {
                MC_RENDER_TILE_THEME_LOG(
                    "ERROR Texture coordinate start index is out of bounds"
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

            // Update face texture coordinates.
            auto i = texCoordStartIndex;
            (*coords)[i++] = topRight;
            (*coords)[i++] = topLeft;
            (*coords)[i++] = bottomLeft;
            (*coords)[i] = bottomRight;
        }

    private:
        const osg::Vec2 textureSize;
        const osg::Vec2 faceSize;
        osg::Vec2 faceTexSize;
        int facesPerRow;
};

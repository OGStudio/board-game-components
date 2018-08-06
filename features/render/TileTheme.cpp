FEATURE render.h/Include
#include <osg/Geode>
#include <osg/Geometry>

FEATURE render.h/Impl
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
                MC_RENDER_TILE_THEME_LOG(
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

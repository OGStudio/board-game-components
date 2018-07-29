FEATURE layout.h/Include
#include <osg/Vec3>

FEATURE layout.h/Impl
//! Layout representation.
struct Layout
{
    // Each osg::Vec3 item contains:
    // * x: field id
    // * y: row
    // * z: column
    typedef std::vector<osg::Vec3i> Positions;

    int width;
    int height;
    int depth;
    Positions positions;
};

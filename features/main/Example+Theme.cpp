FEATURE main.h/Include
#include "render.h"
#include "resource.h"

#include "ppl-theme.frag.h"
#include "ppl-theme.vert.h"
#include "tile-low.osgt.h"
#include "tile-theme.png.h"

FEATURE main.h/Setup
this->setupTheme();

FEATURE main.h/TearDown
this->tearThemeDown();

FEATURE main.h/Impl
private:
    render::TileTheme *theme;
    // Theme texture configuration.
    const osg::Vec2 textureSize = {1024, 2048};
    const osg::Vec2 tileFaceSize = {160, 240};
    // Theme model.
    const render::TileTheme::Indices faceIndices = {15, 23, 16, 17};
    osg::ref_ptr<osg::Geode> tileModel;
    // Materials for normal and selected states of tiles.
    osg::ref_ptr<osg::StateSet> themeMaterial;
    osg::ref_ptr<osg::StateSet> themeMaterialSelected;

    void setupTheme()
    {
        this->theme =
            new render::TileTheme(
                this->textureSize,
                this->tileFaceSize,
                this->faceIndices
            );
        this->setupTileModel();
        this->setupMaterials();
    }
    void tearThemeDown()
    {
        delete this->theme;
    }
    void setupTileModel()
    {
        resource::Resource tileResource(
            "models",
            "tile-low.osgt",
            tile_low_osgt,
            tile_low_osgt_len
        );
        auto node = resource::node(tileResource);
        auto model = reinterpret_cast<osg::Geode *>(node.get());
        // Make sure model is valid.
        if (!model)
        {
            MC_MAIN_EXAMPLE_LOG(
                "ERROR Could not setup tile model '%s/%s'",
                tileResource.group.c_str(),
                tileResource.name.c_str()
            );
            return;
        }
        
        this->tileModel = model;
    }
    void setupMaterials()
    {
        // Create resources.
        resource::Resource shaderFrag(
            "shaders",
            "ppl-theme.frag",
            ppl_theme_frag,
            ppl_theme_frag_len
        );
        resource::Resource shaderVert(
            "shaders",
            "ppl-theme.vert",
            ppl_theme_vert,
            ppl_theme_vert_len
        );
        resource::Resource texRes(
            "textures",
            "tile-theme.png",
            tile_theme_png,
            tile_theme_png_len
        );

        // Create shader program.
        auto prog =
            render::createShaderProgram(
                resource::string(shaderVert),
                resource::string(shaderFrag)
            );
        // Create texture.
        auto texture = resource::createTexture(texRes);

        // Create normal state material.
        auto normal = new osg::StateSet;
        normal->setAttribute(prog);
        normal->setTextureAttributeAndModes(0, texture);
        normal->addUniform(new osg::Uniform("image", 0));
        normal->addUniform(new osg::Uniform("isSelected", false));
        this->themeMaterial = normal;

        // Create selected state material.
        auto selected = new osg::StateSet;
        selected->setAttribute(prog);
        selected->setTextureAttributeAndModes(0, texture);
        selected->addUniform(new osg::Uniform("image", 0));
        selected->addUniform(new osg::Uniform("isSelected", true));
        this->themeMaterialSelected = selected;
    }

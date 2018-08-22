
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

#ifndef OGS_MAHJONG_COMPONENTS_MAIN_H
#define OGS_MAHJONG_COMPONENTS_MAIN_H

// Application+frame+Reporting Start
#include "core.h"

// Application+frame+Reporting End

// Application+Logging Start
#include "log.h"

// Application+Logging End
// Application+Mouse Start
#include "input.h"

// Application+Mouse End
// Application+Rendering Start
#include "render.h"

#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>

// Application+Rendering End

// Example+MatchTilesTest Start
#include "mahjong.h"
#include "scene.h"

#include "resource.h"

#include "cat.layout.h"
#include "short-victory.layout.h"
#include "short-loss.layout.h"

#include "ppl-theme.frag.h"
#include "ppl-theme.vert.h"
#include "tile-low.osgt.h"
#include "tile-theme.png.h"

#include <osg/MatrixTransform>

// Example+MatchTilesTest End
// Example+Scene Start
#include <osg/MatrixTransform>

// Example+Scene End
// Example+TileTheme Start
#include "render.h"

// Example+TileTheme End

// MC_MAIN_LOG Start
#include "log.h"
#include "format.h"
#define MC_MAIN_LOG_PREFIX "main %s"
#define MC_MAIN_LOG(...) \
    log::logprintf( \
        MC_MAIN_LOG_PREFIX, \
        format::printfString(__VA_ARGS__).c_str() \
    )
// MC_MAIN_LOG End
// MC_MAIN_EXAMPLE_LOG Start
#include "log.h"
#include "format.h"
#define MC_MAIN_EXAMPLE_LOG_PREFIX "main::Example(%p) %s"
#define MC_MAIN_EXAMPLE_LOG(...) \
    log::logprintf( \
        MC_MAIN_EXAMPLE_LOG_PREFIX, \
        this, \
        format::printfString(__VA_ARGS__).c_str() \
    )
// MC_MAIN_EXAMPLE_LOG End

// Example+StaticPluginOSG Start
#include <osgDB/Registry>

// Reference (statically) plugins to read `osgt` file.
USE_OSGPLUGIN(osg2)
USE_SERIALIZER_WRAPPER_LIBRARY(osg)
// Example+StaticPluginOSG End
// Example+StaticPluginPNG Start
#include <osgDB/Registry>

// Reference (statically) plugins to read `png` file.
// Apple platforms use ImageIO. All others use libpng.
#ifdef __APPLE__
    USE_OSGPLUGIN(imageio)
#else
    USE_OSGPLUGIN(png)
#endif
// Example+StaticPluginPNG End

namespace mc
{
namespace main
{

// Application Start
class Application
{
    public:
        Application(const std::string &name)
        {

// Application End
            // Application+Logging Start
            this->setupLogging(name);
            
            // Application+Logging End
            // Application+Rendering Start
            this->setupRendering();
            
            // Application+Rendering End
            // Application+Mouse Start
            this->setupMouse();
            
            // Application+Mouse End
// Application Start
        }
        ~Application()
        {

// Application End
            // Application+Mouse Start
            this->tearMouseDown();
            
            // Application+Mouse End
            // Application+Rendering Start
            this->tearRenderingDown();
            
            // Application+Rendering End
            // Application+Logging Start
            this->tearLoggingDown();
            
            // Application+Logging End
// Application Start
        }

// Application End
    // Application+camera Start
    public:
        osg::Camera *camera()
        {
            return this->viewer->getCamera();
        }
    // Application+camera End
    // Application+frame+Reporting Start
    public:
        core::Reporter frameReporter;
        void frame()
        {
            this->viewer->frame();
            this->frameReporter.report();
        }
    // Application+frame+Reporting End
    // Application+run Start
    public:
        void run()
        {
            while (!this->viewer->done())
            {
                this->frame();
            }
        }
    // Application+run End
    // Application+setupWindow-desktop Start
    public:
        void setupWindow(
            const std::string &title,
            int x,
            int y,
            int width,
            int height
        ) {
            osg::GraphicsContext *gc =
                render::createGraphicsContext(title, x, y, width, height);
            // Configure viewer's camera with FOVY and window size.
            osg::Camera *cam = this->viewer->getCamera();
            render::setupCamera(cam, gc, 30, width, height);
        }
    // Application+setupWindow-desktop End

    // Application+Logging Start
    private:
        log::Logger *logger;
        void setupLogging(const std::string &appName)
        {
            // Create custom logger.
            this->logger = new log::Logger(appName);
            // Provide the logger to OpenSceneGraph.
            osg::setNotifyHandler(this->logger);
            // Only accept notifications of Info level or higher
            // like warnings and errors.
            //osg::setNotifyLevel(osg::INFO);
            osg::setNotifyLevel(osg::WARN);
        }
        void tearLoggingDown()
        {
            // Remove the logger from OpenSceneGraph.
            // This also destroys the logger: no need to deallocate it manually.
            osg::setNotifyHandler(0);
        }
    // Application+Logging End
    // Application+Mouse Start
    public:
        osg::ref_ptr<input::Mouse> mouse;
    private:
        void setupMouse()
        {
            // Create mouse events' handler.
            this->mouse = new input::Mouse;
            // Register it.
            this->viewer->addEventHandler(this->mouse);
        }
        void tearMouseDown()
        {
            // This also removes Mouse instance.
            this->viewer->removeEventHandler(this->mouse);
        }
    // Application+Mouse End
    // Application+Rendering Start
    public:
        void setScene(osg::Node *scene)
        {
            // Make sure we reset the scene upon setting the same scene again.
            this->viewer->setSceneData(0);
            this->viewer->setSceneData(scene);
        }
    private:
        osgViewer::Viewer *viewer;
        void setupRendering()
        {
            // Create OpenSceneGraph viewer.
            this->viewer = new osgViewer::Viewer;
            // Use single thread: CRITICAL for mobile and web.
            this->viewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
            // Create manipulator: CRITICAL for mobile and web.
            this->viewer->setCameraManipulator(new osgGA::TrackballManipulator);
        }
        void tearRenderingDown()
        {
            delete this->viewer;
        }
    // Application+Rendering End
// Application Start
};
// Application End

// Example+04 Start
const auto EXAMPLE_TITLE = "Mc04";
// Example+04 End

// Example Start
struct Example
{
    Application *app;

    typedef std::map<std::string, std::string> Parameters;

    Example(const Parameters &parameters)
    {
        this->app = new Application(EXAMPLE_TITLE);

// Example End
        // Example+Scene Start
        this->setupScene();
        
        // Example+Scene End
        // Example+TileTheme Start
        this->setupTileTheme();
        
        // Example+TileTheme End
        // Example+MatchTilesTest Start
        this->setupMatchTilesTest();
        
        // Example+MatchTilesTest End
// Example Start
    }
    ~Example()
    {

// Example End
        // Example+TileTheme Start
        this->tearTileThemeDown();
        
        // Example+TileTheme End
        // Example+MatchTilesTest Start
        this->tearMatchTilesTestDown();
        
        // Example+MatchTilesTest End
// Example Start
        delete this->app;
    }

// Example End
    // Example+Scene Start
    private:
        osg::ref_ptr<osg::MatrixTransform> scene;
        const std::string sceneSetupCallbackName = "SceneSetup";
    
        void setupScene()
        {
            this->scene = new osg::MatrixTransform;
    
            // Provide scene to application after the first frame
            // to let other components configure scene prior that event.
            this->app->frameReporter.addCallback(
                [&] {
                    this->app->setScene(this->scene);
                    // Unsubscribe from the rest of frame reports.
                    this->app->frameReporter.removeCallback(
                        this->sceneSetupCallbackName
                    );
                },
                this->sceneSetupCallbackName
            );
        }
    // Example+Scene End
    // Example+TileTheme Start
    private:
        render::TileTheme *tileTheme;
        const osg::Vec2 textureSize = {1024, 2048};
        const osg::Vec2 tileFaceSize = {160, 240};
        const render::TileTheme::Indices faceIndices = {15, 23, 16, 17};
    
        void setupTileTheme()
        {
            this->tileTheme =
                new render::TileTheme(
                    this->textureSize,
                    this->tileFaceSize,
                    this->faceIndices
                );
        }
        void tearTileThemeDown()
        {
            delete this->tileTheme;
        }
    // Example+TileTheme End
    // Example+MatchTilesTest Start
    private:
        mahjong::Solitaire *game;
        osg::ref_ptr<osg::Geode> tileModel;
        osg::ref_ptr<osg::StateSet> normalMaterial;
        osg::ref_ptr<osg::StateSet> selectedMaterial;
        osg::ref_ptr<osg::MatrixTransform> tileScene;
        mahjong::Layout layout;
        std::map<osg::Node *, mahjong::Tile> tileNodes;
        const std::string selectionCallbackName = "Selection";
        const unsigned int selectionNodeMask = 0x00000004;
        core::Reporter selectedTile;
        osg::Node *selectedTileNode = 0;
        osg::Node *previouslySelectedTileNode = 0;
        core::Reporter tilesMatch;
        std::vector<osg::Node *> nodesToRemove;
        core::Reporter removedTiles;
    
        void setupMatchTilesTest()
        {
            this->game = new mahjong::Solitaire();
            this->setupLayout();
            this->setupMaterials();
            this->setupModel();
            this->setupTileScene();
            this->setupTiles();
            this->setupTileSelection();
            this->setupTileMatching();
            this->setupTileRemoval();
            this->setupGameStateDetection();
        }
        void tearMatchTilesTestDown()
        {
            this->tearTileSelectionDown();
            delete this->game;
        }
    
        void setupLayout()
        {
            resource::Resource res(
                "layouts",
                "cat.layout",
                cat_layout,
                cat_layout_len
            );
            /*
            resource::Resource res(
                "layouts",
                "short-victory.layout",
                short_victory_layout,
                short_victory_layout_len
            );
            */
            /*
            resource::Resource res(
                "layouts",
                "short-loss.layout",
                short_loss_layout,
                short_loss_layout_len
            );
            */
    
            resource::ResourceStreamBuffer buf(res);
            std::istream in(&buf);
            if (!mahjong::parseLayout(in, this->layout))
            {
                MC_MAIN_EXAMPLE_LOG(
                    "ERROR Could not load layout '%s/%s'",
                    res.group.c_str(),
                    res.name.c_str()
                );
            }
        }
        void setupMaterials()
        {
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
    
            // Create shader program.
            auto prog =
                render::createShaderProgram(
                    resource::string(shaderVert),
                    resource::string(shaderFrag)
                );
    
            // Create normal state material.
            this->normalMaterial = new osg::StateSet;
            this->normalMaterial->setAttribute(prog);
            this->normalMaterial->addUniform(new osg::Uniform("image", 0));
            this->normalMaterial->addUniform(new osg::Uniform("isSelected", false));
    
            // Create selected state material.
            this->selectedMaterial = new osg::StateSet;
            this->selectedMaterial->setAttribute(prog);
            this->selectedMaterial->addUniform(new osg::Uniform("image", 0));
            this->selectedMaterial->addUniform(new osg::Uniform("isSelected", true));
    
            // Set texture for both materials.
            resource::Resource theme(
                "themes",
                "tile-theme.png",
                tile_theme_png,
                tile_theme_png_len
            );
            auto texture = resource::createTexture(theme);
            this->normalMaterial->setTextureAttributeAndModes(0, texture);
            this->selectedMaterial->setTextureAttributeAndModes(0, texture);
        }
        void setupModel()
        {
            resource::Resource model(
                "models",
                "tile-low.osgt",
                tile_low_osgt,
                tile_low_osgt_len
            );
            auto node = resource::node(model);
            this->tileModel = reinterpret_cast<osg::Geode *>(node.get());
    
            // Make sure tile is valid.
            if (!this->tileModel.valid())
            {
                MC_MAIN_EXAMPLE_LOG(
                    "ERROR Could not load model '%s/%s'",
                    model.group.c_str(),
                    model.name.c_str()
                );
            }
        }
        void setupTiles()
        {
            std::vector<mahjong::Tile> tiles;
            const int matchIdsCount = 42;
            int id = 0;
    
            // Reorder layout positions with seed.
            unsigned int seed = 1337;
            auto layoutPositions =
                mahjong::orderedLayoutPositions(this->layout.positions, seed);
    
            for (auto pos : layoutPositions)
            {
                // Construct matchId so that two consequent tiles get the same matchId.
                if (id >= matchIdsCount * 2)
                {
                    id = 0;
                }
                int matchId = id++ / 2;
    
                // Create logical tile.
                mahjong::Tile tile;
                tile.position = pos;
                tile.matchId = matchId;
                tiles.push_back(tile);
    
                // Create visual tile.
                auto tileModel = new osg::Geode(*this->tileModel, osg::CopyOp::DEEP_COPY_ALL);
                auto tileNode = new osg::MatrixTransform;
                tileNode->addChild(tileModel);
                // Add it to the scene.
                this->tileScene->addChild(tileNode);
                // Select texture based on matchId value.
                this->tileTheme->setFaceId(matchId, tileModel);
    
                // Map logical tile position to visual one.
                float z = pos.field;
                float y = pos.row * -1.5 /* Factor depends on the model */;
                float x = pos.column;
                // Set visual tile position.
                scene::setSimplePosition(tileNode, {x, y, z});
    
                // Keep correspondence of visual tiles to logical ones.
                this->tileNodes[tileNode] = tile;
            }
    
            // Provide logical tiles to the game.
            this->game->setTiles(tiles);
        }
        void setupTileScene()
        {
            // Create tile scene to host tiles.
            this->tileScene = new osg::MatrixTransform;
            this->scene->addChild(this->tileScene);
            // Rotate it to have a better view.
            scene::setSimpleRotation(this->tileScene, {60, 0, 0});
            // Apply theme.
            this->scene->setStateSet(this->normalMaterial);
        }
        void setupTileSelection()
        {
            // Mark tile nodes as selectable.
            auto tilesCount = this->tileScene->getNumChildren();
            for (int id = 0; id < tilesCount; ++id)
            {
                auto node = this->tileScene->getChild(id);
                node->setNodeMask(node->getNodeMask() & ~this->selectionNodeMask);
            }
    
            // Listen to mouse clicks.
            this->app->mouse->pressedButtonsChanged.addCallback(
                [&] {
                    bool clicked = !this->app->mouse->pressedButtons.empty();
                    if (clicked)
                    {
                        this->selectTile();
                    }
                },
                this->selectionCallbackName
            );
        }
        void tearTileSelectionDown()
        {
            this->app->mouse->pressedButtonsChanged.removeCallback(
                this->selectionCallbackName
            );
        }
        void selectTile()
        {
            this->selectedTileNode =
                scene::nodeAtPosition(
                    this->app->mouse->position,
                    this->app->camera(),
                    this->selectionNodeMask
                );
    
            if (this->selectedTileNode)
            {
                this->selectedTile.report();
            }
        }
        void setupTileMatching()
        {
            this->selectedTile.addCallback(
                [&] {
                    auto nodeNow = this->selectedTileNode;
                    auto tileNow = this->tileNodes[nodeNow];
    
                    // Make sure tile is selectable.
                    if (!this->game->isTileSelectable(tileNow))
                    {
                        return;
                    }
    
                    // Depict selection.
                    this->setNodeSelected(nodeNow, true);
    
                    auto nodeWas = this->previouslySelectedTileNode;
    
                    // Keep previously selected tile node.
                    this->previouslySelectedTileNode = nodeNow;
    
                    // Make sure different nodes were selected.
                    if (!nodeWas || (nodeNow == nodeWas))
                    {
                        return;
                    }
    
                    // Remove selection of previously selected node.
                    this->setNodeSelected(nodeWas, false);
    
                    // Match tiles.
                    auto tileWas = this->tileNodes[nodeWas];
                    bool match = this->game->tilesMatch(tileWas, tileNow);
                    // Report matching.
                    if (match)
                    {
                        this->nodesToRemove.push_back(nodeWas);
                        this->nodesToRemove.push_back(nodeNow);
                        this->tilesMatch.report();
                    }
                }
            );
        }
        void setNodeSelected(osg::Node *node, bool state)
        {
            osg::StateSet *material = 
                state ? 
                this->selectedMaterial :
                0;
            node->setStateSet(material);
        }
        void setupTileRemoval()
        {
            this->tilesMatch.addCallback(
                [&] {
                    this->removeTiles();
                }
            );
        }
        void removeTiles()
        {
            // Find out nodes and tiles to remove.
            auto nodeWas = this->nodesToRemove[0];
            auto nodeNow = this->nodesToRemove[1];
            this->nodesToRemove.clear();
            auto tileWas = this->tileNodes[nodeWas];
            auto tileNow = this->tileNodes[nodeNow];
    
            // Remove tiles from Solitaire (logical) representation.
            this->game->removeTiles(tileWas, tileNow);
            // Remove nodes from tileNodes.
            {
                auto it = this->tileNodes.find(nodeWas);
                this->tileNodes.erase(it);
            }
            {
                auto it = this->tileNodes.find(nodeNow);
                this->tileNodes.erase(it);
            }
            // Deselect both tile nodes.
            this->setNodeSelected(nodeWas, false);
            this->setNodeSelected(nodeNow, false);
            // Remove nodes from tileScene.
            this->tileScene->removeChild(nodeWas);
            this->tileScene->removeChild(nodeNow);
            // Remove selection.
            this->selectedTileNode = 0;
            this->previouslySelectedTileNode = 0;
            
            // Report.
            this->removedTiles.report();
        }
        void setupGameStateDetection()
        {
            this->removedTiles.addCallback(
                [&] {
                    this->detectGameState();
                }
            );
        }
        void detectGameState()
        {
            if (!this->game->hasTurns())
            {
                if (this->game->hasTiles())
                {
                    MC_MAIN_EXAMPLE_LOG("Game over: LOSS");
                }
                else
                {
                    MC_MAIN_EXAMPLE_LOG("Game over: VICTORY");
                }
            }
        }
    // Example+MatchTilesTest End
// Example Start
};
// Example End

} // namespace main
} // namespace mc

#endif // OGS_MAHJONG_COMPONENTS_MAIN_H


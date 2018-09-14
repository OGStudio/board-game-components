
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

// Example+DefaultLayoutTheme Start
#include "mahjong.h"

// Example+DefaultLayoutTheme End
// Example+InternalLayouts Start
#include "X_shaped.layout.h"
#include "short-loss.layout.h"
#include "short-victory.layout.h"

#include "resource.h"

// Example+InternalLayouts End
// Example+InternalThemes Start
#include "numbers-theme.png.h"

#include "resource.h"

// Example+InternalThemes End
// Example+Scene Start
#include <osg/MatrixTransform>

// Example+Scene End
// Example+Theme Start
#include "render.h"
#include "resource.h"

#include "ppl-theme.frag.h"
#include "ppl-theme.vert.h"
#include "tile-low.osgt.h"

// Example+Theme End

// Example+createTiles Start
#include "mahjong.h"
#include "scene.h"
#include <osg/MatrixTransform>

// Example+createTiles End

// MAIN_EXAMPLE_LOG Start
#include "log.h"
#include "format.h"
#define MAIN_EXAMPLE_LOG_PREFIX "main::Example(%p) %s"
#define MAIN_EXAMPLE_LOG(...) \
    log::logprintf( \
        MAIN_EXAMPLE_LOG_PREFIX, \
        this, \
        format::printfString(__VA_ARGS__).c_str() \
    )
// MAIN_EXAMPLE_LOG End

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

namespace omc
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

// Example+03 Start
const auto EXAMPLE_TITLE = "OMC-03: Match tiles";
// Example+03 End

// Example Start
struct Example
{
    Application *app;

    typedef std::map<std::string, std::string> Parameters;

    Example(const Parameters &parameters)
    {
        this->app = new Application(EXAMPLE_TITLE);

// Example End
        // Example+Game Start
        this->setupGame();
        
        // Example+Game End
        // Example+Scene Start
        this->setupScene();
        
        // Example+Scene End
        // Example+Theme Start
        this->setupTheme();
        
        // Example+Theme End
        // Example+MatchTilesTest Start
        this->setupMatchTilesTest();
        
        // Example+MatchTilesTest End
// Example Start
    }
    ~Example()
    {

// Example End
        // Example+MatchTilesTest Start
        this->tearMatchTilesTestDown();
        
        // Example+MatchTilesTest End
        // Example+Theme Start
        this->tearThemeDown();
        
        // Example+Theme End
        // Example+Game Start
        this->tearGameDown();
        
        // Example+Game End
// Example Start
        delete this->app;
    }

// Example End
    // Example+DefaultLayoutTheme Start
    private:
        mahjong::Layout layout;
        void setupDefaultLayoutTheme()
        {
            // Load internal "X_shaped.layout" layout by default.
            auto layoutResource =
                this->internalLayouts->resource("layouts", "X_shaped.layout");
            if (!layoutResource)
            {
                MAIN_EXAMPLE_LOG("ERROR Could not locate internal layout");
                return;
            }
            resource::ResourceStreamBuffer buf(*layoutResource);
            std::istream in(&buf);
            if (!mahjong::parseLayout(in, this->layout))
            {
                MAIN_EXAMPLE_LOG("ERROR Could not parse internal layout");
                return;
            }
    
            // Load internal "numbers-theme.png" theme by default.
            auto themeResource =
                this->internalThemes->resource("themes", "numbers-theme.png");
            if (!themeResource)
            {
                MAIN_EXAMPLE_LOG("ERROR Could not locate internal theme");
                return;
            }
            auto texture = resource::createTexture(*themeResource);
            this->themeMaterial->setTextureAttributeAndModes(0, texture);
            this->themeMaterialSelected->setTextureAttributeAndModes(0, texture);
        }
    // Example+DefaultLayoutTheme End
    // Example+Game Start
    private:
        mahjong::Solitaire *game;
    
        void setupGame()
        {
            this->game = new mahjong::Solitaire();
        }
        void tearGameDown()
        {
            delete this->game;
        }
    // Example+Game End
    // Example+GameState Start
    private:
        bool isGameVictorious = false;
        core::Reporter finishedGame;
        void setupGameState()
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
                this->isGameVictorious = !this->game->hasTiles();
                this->finishedGame.report();
            }
        }
    // Example+GameState End
    // Example+InternalLayouts Start
    private:
        resource::Pool *internalLayouts;
        void setupInternalLayouts()
        {
            // Create pool.
            this->internalLayouts = new resource::Pool;
    
            // Register internal layouts.
            {
                resource::Resource res(
                    "layouts",
                    "X_shaped.layout",
                    X_shaped_layout,
                    X_shaped_layout_len
                );
                this->internalLayouts->addResource(res);
            }
            {
                resource::Resource res(
                    "layouts",
                    "short-loss.layout",
                    short_loss_layout,
                    short_loss_layout_len
                );
                this->internalLayouts->addResource(res);
            }
            {
                resource::Resource res(
                    "layouts",
                    "short-victory.layout",
                    short_victory_layout,
                    short_victory_layout_len
                );
                this->internalLayouts->addResource(res);
            }
        }
        void tearInternalLayoutsDown()
        {
            delete this->internalLayouts;
        }
    // Example+InternalLayouts End
    // Example+InternalThemes Start
    private:
        resource::Pool *internalThemes;
        void setupInternalThemes()
        {
            // Create pool.
            this->internalThemes = new resource::Pool;
    
            // Register internal themes.
            {
                resource::Resource res(
                    "themes",
                    "numbers-theme.png",
                    numbers_theme_png,
                    numbers_theme_png_len
                );
                this->internalThemes->addResource(res);
            }
        }
        void tearInternalThemesDown()
        {
            delete this->internalThemes;
        }
    // Example+InternalThemes End
    // Example+MatchedTilesRemoval Start
    private:
        core::Reporter removedTiles;
    
        void setupMatchedTilesRemoval()
        {
            this->tilesMatchChanged.addCallback(
                [&] {
                    // Make sure there was a match.
                    if (!this->tilesMatch)
                    {
                        return;
                    }
    
                    this->removeMatchedTiles();
                }
            );
        }
        void removeMatchedTiles()
        {
            auto nodeTile1 = this->selectedTiles[0];
            auto nodeTile2 = this->selectedTiles[1];
    
            // Deselect tiles.
            this->selectedTiles.clear();
            this->selectedTilesChanged.report();
    
            // Remove tiles from Solitaire (logical representation).
            this->game->removeTiles(nodeTile1.tile, nodeTile2.tile);
    
            // Remove items from visual-to-logical correspondence.
            {
                auto it = this->tileNodes.find(nodeTile1.node);
                this->tileNodes.erase(it);
            }
            {
                auto it = this->tileNodes.find(nodeTile2.node);
                this->tileNodes.erase(it);
            }
    
            // Remove nodes from the scene.
            this->tileScene->removeChild(nodeTile1.node);
            this->tileScene->removeChild(nodeTile2.node);
            
            // Report.
            this->removedTiles.report();
        }
    // Example+MatchedTilesRemoval End
    // Example+MatchTilesTest Start
    private:
        void setupMatchTilesTest()
        {
            this->setupInternalLayouts();
            this->setupInternalThemes();
            this->setupDefaultLayoutTheme();
            this->setupTiles(time(0));
            this->setupNodeSelection();
            this->setupTileSelection();
            this->setupTileSelectionDepiction();
            this->setupTileMatching();
            this->setupUnmatchedTilesDeselection();
            this->setupMatchedTilesRemoval();
            this->setupGameState();
    
            // Report game result.
            this->finishedGame.addCallback(
                [&] {
                    if (this->isGameVictorious)
                    {
                        MAIN_EXAMPLE_LOG("Game over: VICTORY");
                    }
                    else
                    {
                        MAIN_EXAMPLE_LOG("Game over: LOSS");
                    }
                }
            );
        }
        void tearMatchTilesTestDown()
        {
            this->tearInternalLayoutsDown();
        }
    // Example+MatchTilesTest End
    // Example+NodeSelection Start
    private:
        const std::string nodeSelectionCallbackName = "NodeSelection";
        const unsigned int selectionNodeMask = 0x00000004;
        osg::Node *selectedNode;
        core::Reporter selectedNodeChanged;
    
        void setupNodeSelection()
        {
            // Mark nodes as selectable by excluding specific bitmask.
            auto nodesCount = this->tileScene->getNumChildren();
            for (int id = 0; id < nodesCount; ++id)
            {
                auto node = this->tileScene->getChild(id);
                node->setNodeMask(node->getNodeMask() & ~this->selectionNodeMask);
            }
    
            // Listen to mouse clicks.
            this->app->mouse->pressedButtonsChanged.addCallback(
                [&] {
                    // Try to select a node upon a click or a tap.
                    bool clicked = !this->app->mouse->pressedButtons.empty();
                    if (clicked)
                    {
                        this->selectNode();
                    }
                },
                this->nodeSelectionCallbackName
            );
        }
        void tearNodeSelectionDown()
        {
            this->app->mouse->pressedButtonsChanged.removeCallback(
                this->nodeSelectionCallbackName
            );
        }
        void selectNode()
        {
            auto node =
                scene::nodeAtPosition(
                    this->app->mouse->position,
                    this->app->camera(),
                    this->selectionNodeMask
                );
    
            // Report successful selection.
            if (node)
            {
                this->selectedNode = node;
                this->selectedNodeChanged.report();
            }
        }
    // Example+NodeSelection End
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
    // Example+Theme Start
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
                MAIN_EXAMPLE_LOG(
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
    
            // Create shader program.
            auto prog =
                render::createShaderProgram(
                    resource::string(shaderVert),
                    resource::string(shaderFrag)
                );
    
            // Create normal state material.
            auto normal = new osg::StateSet;
            normal->setAttribute(prog);
            normal->addUniform(new osg::Uniform("image", 0));
            normal->addUniform(new osg::Uniform("isSelected", false));
            this->themeMaterial = normal;
    
            // Create selected state material.
            auto selected = new osg::StateSet;
            selected->setAttribute(prog);
            selected->addUniform(new osg::Uniform("image", 0));
            selected->addUniform(new osg::Uniform("isSelected", true));
            this->themeMaterialSelected = selected;
        }
    // Example+Theme End
    // Example+Tiles Start
    private:
        osg::ref_ptr<osg::MatrixTransform> tileScene;
        std::map<osg::Node *, mahjong::Tile> tileNodes;
    
        void setupTiles(int seed)
        {
            // Order layout positions with seed.
            auto positions = mahjong::orderedLayoutPositions(this->layout.positions, seed);
            auto matchIds = mahjong::matchIds(positions.size());
    
            // Create tile nodes.
            auto tileScene = this->createTiles(positions, matchIds);
            // Set default (non-selected) material.
            tileScene->setStateSet(this->themeMaterial);
            // Rotate the scene to have a better view.
            scene::setSimpleRotation(tileScene, {65, 0, 0});
            // Set the scene.
            this->scene->addChild(tileScene);
            this->app->setScene(this->scene);
    
            // Create logical tiles.
            int tilesCount = positions.size();
            std::vector<mahjong::Tile> tiles;
            for (int i = 0; i < tilesCount; ++i)
            {
                auto position = positions[i];
                auto matchId = matchIds[i];
    
                mahjong::Tile tile;
                tile.position = position;
                tile.matchId = matchId;
                tiles.push_back(tile);
    
                // Keep correspondence of visual tiles to logical ones.
                auto node = tileScene->getChild(i);
                this->tileNodes[node] = tile;
            }
    
            // Provide logical tiles to the game.
            this->game->setTiles(tiles);
            // Keep reference to tile nodes.
            this->tileScene = tileScene;
        }
    // Example+Tiles End
    // Example+TileMatching Start
    private:
        bool tilesMatch = false;
        core::Reporter tilesMatchChanged;
    
        void setupTileMatching()
        {
            this->selectedTilesChanged.addCallback(
                [&] {
                    // Only perform matching once two tiles have been selected.
                    if (this->selectedTiles.size() == 2)
                    {
                        this->matchTiles();
                    }
                }
            );
        }
        void matchTiles()
        {
            auto nodeTile = this->selectedTiles.begin();
            auto tile1 = nodeTile->tile;
            ++nodeTile;
            auto tile2 = nodeTile->tile;
    
            this->tilesMatch = this->game->tilesMatch(tile1, tile2);
            this->tilesMatchChanged.report();
        }
    // Example+TileMatching End
    // Example+TileSelection Start
    private:
        struct NodeTile
        {
            osg::Node *node;
            mahjong::Tile tile;
        };
        std::vector<NodeTile> selectedTiles;
        core::Reporter selectedTilesChanged;
        const std::string tileSelectionCallbackName = "TileSelection";
    
        void setupTileSelection()
        {
            this->selectedNodeChanged.addCallback(
                [&] {
                    this->selectTile();
                },
                this->tileSelectionCallbackName
            );
        }
        void tearTileSelectionDown()
        {
            this->selectedNodeChanged.removeCallback(
                this->tileSelectionCallbackName
            );
        }
        void selectTile()
        {
            auto node = this->selectedNode;
            auto tile = this->tileNodes[node];
    
            // Make sure tile is selectable.
            if (!this->game->isTileSelectable(tile))
            {
                return;
            }
    
            // Make sure the tile has not yet been selected.
            for (auto nodeTile : this->selectedTiles)
            {
                if (nodeTile.node == node)
                {
                    return;
                }
            }
    
            // Select and report.
            this->selectedTiles.push_back({node, tile});
            this->selectedTilesChanged.report();
        }
    // Example+TileSelection End
    // Example+TileSelectionDepiction Start
    private:
        std::vector<NodeTile> depictedTiles;
    
        void setupTileSelectionDepiction()
        {
            this->selectedTilesChanged.addCallback(
                [&] {
                    this->depictSelectedTiles();
                }
            );
        }
        void depictSelectedTiles()
        {
            // Remove depiction of previously selected tiles.
            for (auto nodeTile : this->depictedTiles)
            {
                auto node = nodeTile.node;
                this->depictNodeSelection(node, false);
            }
    
            // Depict currently selected tiles.
            for (auto nodeTile : this->selectedTiles)
            {
                auto node = nodeTile.node;
                this->depictNodeSelection(node, true);
            }
    
            // Keep depicted tiles.
            this->depictedTiles = this->selectedTiles;
        }
        void depictNodeSelection(osg::Node *node, bool isSelected)
        {
            osg::StateSet *material = 
                isSelected ? 
                this->themeMaterialSelected :
                0;
            node->setStateSet(material);
        }
    // Example+TileSelectionDepiction End
    // Example+UnmatchedTilesDeselection Start
    private:
        void setupUnmatchedTilesDeselection()
        {
            this->tilesMatchChanged.addCallback(
                [&] {
                    // Make sure there was no match.
                    if (this->tilesMatch)
                    {
                        return;
                    }
    
                    this->deselectUnmatchedTiles();
                }
            );
        }
        void deselectUnmatchedTiles()
        {
            // Deselect the first unmatched tile only.
            this->selectedTiles.erase(this->selectedTiles.begin());
            this->selectedTilesChanged.report();
        }
    // Example+UnmatchedTilesDeselection End
 
    // Example+createTiles Start
    // Make sure positions' count is equal to matchIds' one.
    osg::MatrixTransform* createTiles(
        const mahjong::Layout::Positions &positions,
        mahjong::MatchIds &matchIds
    ) {
        // Create scene to host tile nodes.
        osg::ref_ptr<osg::MatrixTransform> tileScene = new osg::MatrixTransform;
        // Correct the height for `tile-low.osgt` model.
        const float MODEL_HEIGHT_FACTOR = 1.5;
        // Gap to use among times for enhanced visibility.
        const float GAP = 0.05;
    
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
    
            // Add gap among tiles.
            x += pos.column * GAP;
            y += pos.row * MODEL_HEIGHT_FACTOR * GAP;
            z += pos.field * GAP;
    
            scene::setSimplePosition(node, {x, y, z});
            // Set match id.
            int matchId = matchIds[id++];
            this->theme->setFaceId(matchId, tile);
    
            // Add tile node to the scene.
            tileScene->addChild(node);
        }
    
        return tileScene.release();
    }
    // Example+createTiles End
// Example Start
};
// Example End

} // namespace main
} // namespace omc

#endif // OGS_MAHJONG_COMPONENTS_MAIN_H


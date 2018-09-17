
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

// Application+HTTPClient Start
#include "network.h"

// Application+HTTPClient End
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
            // Application+HTTPClient Start
            this->setupHTTPClient();
            
            // Application+HTTPClient End
            // Application+HTTPClientProcessor Start
            this->setupHTTPClientProcessor();
            
            // Application+HTTPClientProcessor End
// Application Start
        }
        ~Application()
        {

// Application End
            // Application+HTTPClientProcessor Start
            this->tearHTTPClientProcessorDown();
            
            // Application+HTTPClientProcessor End
            // Application+HTTPClient Start
            this->tearHTTPClientDown();
            
            // Application+HTTPClient End
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

    // Application+HTTPClient Start
    public:
        network::HTTPClient *httpClient;
    private:
        void setupHTTPClient()
        {
            this->httpClient = new network::HTTPClient;
        }
        void tearHTTPClientDown()
        {
            delete this->httpClient;
        }
    // Application+HTTPClient End
    // Application+HTTPClientProcessor Start
    public:
        network::HTTPClientProcessor *httpClientProcessor;
    private:
        const std::string httpClientProcessorCallbackName = "HTTPClientProcessor";
    
        void setupHTTPClientProcessor()
        {
            this->httpClientProcessor = new network::HTTPClientProcessor(this->httpClient);
            // Subscribe processor to be processed each frame.
            this->frameReporter.addCallback(
                [&] {
                    this->httpClientProcessor->process();
                },
                this->httpClientProcessorCallbackName
            );
        }
        void tearHTTPClientProcessorDown()
        {
            this->frameReporter.removeCallback(this->httpClientProcessorCallbackName);
            delete this->httpClientProcessor;
        }
    // Application+HTTPClientProcessor End
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

// Example+05 Start
const auto EXAMPLE_TITLE = "OMC-05: Colorful status";
// Example+05 End

// Example Start
struct Example
{
    Application *app;

    typedef std::map<std::string, std::string> Parameters;

    Example(const Parameters &parameters)
    {
        this->app = new Application(EXAMPLE_TITLE);

// Example End
        // Example+Parameters Start
        this->setupParameters(parameters);
        
        // Example+Parameters End
        // Example+Game Start
        this->setupGame();
        
        // Example+Game End
        // Example+Scene Start
        this->setupScene();
        
        // Example+Scene End
        // Example+Theme Start
        this->setupTheme();
        
        // Example+Theme End
        // Example+SetLayoutThemeSeed Start
        this->setupSetLayoutThemeSeed();
        
        // Example+SetLayoutThemeSeed End
        // Example+ColorfulStatus Start
        this->setupColorfulStatus();
        
        // Example+ColorfulStatus End
        // Example+ColorfulStatusTest Start
        this->setupColorfulStatusTest();
        
        // Example+ColorfulStatusTest End
// Example Start
    }
    ~Example()
    {

// Example End
        // Example+SetLayoutThemeSeed Start
        this->tearSetLayoutThemeSeedDown();
        
        // Example+SetLayoutThemeSeed End
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
    // Example+ColorfulStatus Start
    private:
        void setupColorfulStatus()
        {
            this->setupLoadingAnimation();
            //this->setupLoss();
            //this->setupVictory();
        }
    
        // Loading animation.
    
        const std::string loadingAnimationCallbackName = "LoadingAnimation";
        osg::Timer loadingTimer;
        scene::LinearInterpolator interpolator;
        const float loadingAnimationDelay = 0.2;
    
        void setupLoadingAnimation()
        {
            this->setupSequence.insertAction(
                CORE_SEQUENCE_ACTION("startLoading", this->startColorLoading()),
                "loadLayout"
            );
            this->setupSequence.insertAction(
                CORE_SEQUENCE_ACTION("stopLoading", this->stopColorLoading()),
                "finishSetup"
            );
        }
        core::Reporter *startColorLoading()
        {
            this->resetBackground();
            // Configure blinking interpolation.
            this->interpolator.keyValues = {
                {0, 0},
                {0.5, 0.5}, // Change color to 0.5 in 0.5s.
                {1, 0}, // Change color back to 0 in 0.5s.
            };
            this->startColorLoadingAnimation();
    
            return 0;
        }
        core::Reporter *stopColorLoading()
        {
            this->stopColorLoadingAnimation();
            // Make sure we reset background after the last background animation.
            this->app->frameReporter.addOneTimeCallback(
                [&] {
                    this->resetBackground();
                }
            );
            return 0;
        }
        void startColorLoadingAnimation()
        {
            this->loadingTimer.setStartTick();
    
            this->app->frameReporter.addCallback(
                [&] {
                    auto elapsed = this->loadingTimer.time_s();
                    // Make sure delay passed.
                    elapsed -= this->loadingAnimationDelay;
                    if (elapsed < 0)
                    {
                        return;
                    }
                    this->animateColorLoading(elapsed);
                },
                this->loadingAnimationCallbackName
            );
        }
        void stopColorLoadingAnimation()
        {
            this->app->frameReporter.removeCallback(
                this->loadingAnimationCallbackName
            );
        }
        void animateColorLoading(float elapsed)
        {
            // Keep `elapsed` in [0; 1] range by using only fractional part.
            float intpart;
            elapsed = modf(elapsed, &intpart);
    
            auto value = this->interpolator.valueFor(elapsed);
            this->app->camera()->setClearColor({value, value, value, 0});
        }
        void resetBackground()
        {
            this->app->camera()->setClearColor({0, 0, 0, 0});
        }
    // Example+ColorfulStatus End
    // Example+ColorfulStatusTest Start
    private:
        void setupColorfulStatusTest()
        {
            this->setupSequence.setEnabled(true);
        }
    // Example+ColorfulStatusTest End
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
    // Example+Parameters Start
    private:
        Parameters parameters;
        void setupParameters(const Parameters &parameters)
        {
            this->parameters = parameters;
        }
    // Example+Parameters End
    // Example+SetLayoutThemeSeed Start
    private:
        core::Sequence setupSequence;
        void setupSetLayoutThemeSeed()
        {
            this->setupSetup();
            this->setupLoading();
        }
        void tearSetLayoutThemeSeedDown()
        {
            this->tearInternalLayoutsDown();
            this->tearInternalThemesDown();
        }
    
        // Setup.
    
        void setupSetup()
        {
            this->setupSequence.setActions({
                CORE_SEQUENCE_ACTION("startSetup", this->startSetup()),
                CORE_SEQUENCE_ACTION("finishSetup", this->finishSetup()),
            });
        }
        core::Reporter *startSetup()
        {
            this->setupInternalLayouts();
            this->setupInternalThemes();
            this->setupDefaultLayoutTheme();
    
            return 0;
        }
        core::Reporter *finishSetup()
        {
            this->setupTiles();
            this->setupNodeSelection();
            this->setupTileSelection();
            this->setupTileSelectionDepiction();
            this->setupTileMatching();
            this->setupUnmatchedTilesDeselection();
            this->setupMatchedTilesRemoval();
            this->setupGameState();
    
            return 0;
        }
    
        // Layout and theme loading.
    
        void setupLoading()
        {
            this->setupSequence.insertAction(
                CORE_SEQUENCE_ACTION("loadLayout", this->loadLayout()),
                "finishSetup"
            );
            this->setupSequence.insertAction(
                CORE_SEQUENCE_ACTION("loadTheme", this->loadTheme()),
                "finishSetup"
            );
        }
        core::Reporter *loadLayout()
        {
            // Do nothing if `layout` parameter is absent.
            auto it = this->parameters.find("layout");
            if (it == this->parameters.end())
            {
                return 0;
            }
    
            // Load layout otherwise.
            auto value = it->second;
            return this->loadLayout(value);
        }
        core::Reporter *loadTheme()
        {
            // Do nothing if `theme` parameter is absent.
            auto it = this->parameters.find("theme");
            if (it == this->parameters.end())
            {
                return 0;
            }
    
            // Load theme otherwise.
            auto value = it->second;
            return this->loadTheme(value);
        }
        void setupTiles()
        {
            // By default, use seed of the current time.
            int seed = time(0);
    
            // Override it with the seed coming from parameters.
            auto it = this->parameters.find("seed");
            if (it != this->parameters.end())
            {
                seed = atoi(it->second.c_str());
                MAIN_EXAMPLE_LOG("Using seed '%d'", seed);
            }
    
            this->setupTiles(seed);
        }
    // Example+SetLayoutThemeSeed End
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
        typedef std::vector<mahjong::Tile> LogicalTiles;
        osg::ref_ptr<osg::MatrixTransform> tileScene;
        std::map<osg::Node *, mahjong::Tile> tileNodes;
    
        void setupTiles(int seed)
        {
            // Initialize random number generator.
            std::mt19937 random(seed);
    
            mahjong::Layout::Positions positions;
            mahjong::MatchIds matchIds;
            LogicalTiles tiles;
    
            // Keep on generating logical tiles until we have turns.
            while (!this->game->hasTurns())
            {
                MAIN_EXAMPLE_LOG("Generating logical tiles");
                tiles = this->createLogicalTiles(random, positions, matchIds);
                this->game->setTiles(tiles);
            }
    
            this->setupVisualTiles(positions, tiles, matchIds);
        }
        LogicalTiles createLogicalTiles(
            std::mt19937 &randomNumberGenerator,
            mahjong::Layout::Positions &positions,
            mahjong::MatchIds &matchIds
        ) {
            // Order layout positions.
            positions =
                mahjong::orderedLayoutPositions(
                    this->layout.positions,
                    randomNumberGenerator
                );
            // Create match ids.
            matchIds = mahjong::matchIds(positions.size());
    
            // Create logical tiles.
            LogicalTiles tiles;
            for (int i = 0; i < positions.size(); ++i)
            {
                auto position = positions[i];
                auto matchId = matchIds[i];
    
                mahjong::Tile tile;
                tile.position = position;
                tile.matchId = matchId;
                tiles.push_back(tile);
            }
    
            return tiles;
        }
        void setupVisualTiles(
            const mahjong::Layout::Positions &positions,
            const LogicalTiles &tiles,
            const mahjong::MatchIds &matchIds
        ) {
            // Create tile nodes.
            auto tileScene = this->createTiles(positions, matchIds);
            // Set default (non-selected) material.
            tileScene->setStateSet(this->themeMaterial);
            // Rotate the scene to have a better view.
            scene::setSimpleRotation(tileScene, {65, 0, 0});
            // Set the scene.
            this->scene->addChild(tileScene);
            this->app->setScene(this->scene);
    
            for (int i = 0; i < positions.size(); ++i)
            {
                // Keep correspondence of visual tiles to logical ones.
                auto node = tileScene->getChild(i);
                auto tile = tiles[i];
                this->tileNodes[node] = tile;
            }
            // Keep reference to tile scene.
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
        const mahjong::MatchIds &matchIds
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
    // Example+loadLayout Start
    private:
        core::Reporter layoutLoaded;
    
        core::Reporter *loadLayout(const std::string &layout)
        {
            // Try to expand layout in case it's a collapsed remote path.
            auto path = layout;
            path = resource::expandBitBucketPath(path);
            path = resource::expandGitHubPath(path);
    
            // Remote.
            if (resource::isPathRemote(path))
            {
                this->loadRemoteLayout(path);
                return &this->layoutLoaded;
            }
    
            // Local or internal.
            auto internalLayout =
                this->internalLayouts->resource("layouts", layout);
            // Internal.
            if (internalLayout)
            {
                this->loadInternalLayout(*internalLayout);
            }
            // Local.
            else
            {
                this->loadLocalLayout(layout);
            }
    
            // Report finish instantly.
            return 0;
        }
        void loadInternalLayout(resource::Resource &layout)
        {
            resource::ResourceStreamBuffer buf(layout);
            std::istream in(&buf);
            if (!mahjong::parseLayout(in, this->layout))
            {
                MAIN_EXAMPLE_LOG("ERROR Could not parse internal layout");
            }
            else
            {
                MAIN_EXAMPLE_LOG("Successfully loaded and parsed internal layout");
            }
        }
        void loadLocalLayout(const std::string &layoutFileName)
        {
            std::ifstream localLayout(layoutFileName);
            if (localLayout)
            {
                if (!mahjong::parseLayout(localLayout, this->layout))
                {
                    MAIN_EXAMPLE_LOG("ERROR Could not parse local layout");
                }
                else
                {
                    MAIN_EXAMPLE_LOG("Successfully loaded and parsed local layout");
                }
            }
            else
            {
                MAIN_EXAMPLE_LOG("ERROR Could not read local layout");
            }
        }
        void loadRemoteLayout(const std::string &url)
        {
            MAIN_EXAMPLE_LOG("Loading remote layout '%s'", url.c_str());
    
            auto success = [&](std::string response) {
                // Response exists.
                if (response.length())
                {
                    // Parse it.
                    std::istringstream in(response);
                    if (!mahjong::parseLayout(in, this->layout))
                    {
                        MAIN_EXAMPLE_LOG("ERROR Could not parse remote layout");
                    }
                    else
                    {
                        MAIN_EXAMPLE_LOG("Successfully loaded and parsed remote layout");
                    }
                }
                // Response does not exist.
                else
                {
                    MAIN_EXAMPLE_LOG("ERROR Loaded layout is empty");
                }
    
                // Report finish.
                this->layoutLoaded.report();
            };
    
            auto failure = [&](std::string reason) {
                MAIN_EXAMPLE_LOG(
                    "ERROR Could not load remote layout: '%s'",
                    reason.c_str()
                );
    
                // Report finish.
                this->layoutLoaded.report();
            };
    
            // Initiate loading.
            this->app->httpClient->get(url, success, failure);
        }
    // Example+loadLayout End
    // Example+loadTheme Start
    private:
        core::Reporter themeLoaded;
    
        core::Reporter *loadTheme(const std::string &theme)
        {
            // Try to expand theme in case it's a collapsed remote path.
            auto path = theme;
            path = resource::expandBitBucketPath(path);
            path = resource::expandGitHubPath(path);
    
            // Remote.
            if (resource::isPathRemote(path))
            {
                this->loadRemoteTheme(path);
                return &this->themeLoaded;
            }
    
            // Local or internal.
            auto internalTheme =
                this->internalThemes->resource("themes", theme);
            // Internal.
            if (internalTheme)
            {
                this->loadInternalTheme(*internalTheme);
            }
            // Local.
            else
            {
                this->loadLocalTheme(theme);
            }
    
            // Report finish instantly.
            return 0;
        }
        void loadInternalTheme(resource::Resource &theme)
        {
            this->setupThemeMaterials(theme);
            MAIN_EXAMPLE_LOG("Successfully loaded internal theme");
        }
        void loadLocalTheme(const std::string &themeFileName)
        {
            std::ifstream localTheme(themeFileName);
            if (localTheme)
            {
                // Read file contents into string.
                std::string fileContents(
                    (std::istreambuf_iterator<char>(localTheme)),
                    (std::istreambuf_iterator<char>())
                );
                resource::Resource res(
                    "themes",
                    themeFileName,
                    resource::stringToResourceContents(fileContents),
                    fileContents.length()
                );
                this->setupThemeMaterials(res);
                MAIN_EXAMPLE_LOG("Successfully loaded local theme");
            }
            else
            {
                MAIN_EXAMPLE_LOG("ERROR Could not read local theme");
            }
        }
        void loadRemoteTheme(const std::string &url)
        {
            MAIN_EXAMPLE_LOG("Loading remote theme '%s'", url.c_str());
    
            auto success = [=](std::string response) {
                // NOTE Use `=` in lambda capture to capture url copy
                // NOTE Otherwise we have crash when referencing `url`.
                resource::Resource res(
                    "themes",
                    url,
                    resource::stringToResourceContents(response),
                    response.length()
                );
                this->setupThemeMaterials(res);
                MAIN_EXAMPLE_LOG("Successfully loaded remote theme");
    
                // Report finish.
                this->themeLoaded.report();
            };
            auto failure = [&](std::string reason) {
                MAIN_EXAMPLE_LOG(
                    "ERROR Could not load remote theme: '%s'",
                    reason.c_str()
                );
                // Report finish.
                this->themeLoaded.report();
            };
    
            // Initiate loading.
            this->app->httpClient->get(url, success, failure);
        }
        void setupThemeMaterials(resource::Resource &res)
        {
            auto texture = resource::createTexture(res);
            this->themeMaterial->setTextureAttributeAndModes(0, texture);
            this->themeMaterialSelected->setTextureAttributeAndModes(0, texture);
        }
    // Example+loadTheme End
// Example Start
};
// Example End

} // namespace main
} // namespace omc

#endif // OGS_MAHJONG_COMPONENTS_MAIN_H


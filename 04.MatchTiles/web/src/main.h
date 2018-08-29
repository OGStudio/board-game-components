
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
// Application+handleEvent-web Start
#include <SDL2/SDL.h>

// Application+handleEvent-web End

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
#include "scene.h"

#include <osg/MatrixTransform>

// Example+MatchTilesTest End
// Example+Scene Start
#include <osg/MatrixTransform>

// Example+Scene End
// Example+VBO Start
#include "render.h"

// Example+VBO End



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
    // Application+handleEvent-web Start
    private:
        bool fingerEventsDetected = false;
    public:
        bool handleEvent(const SDL_Event &e)
        {
            // Get event queue.
            osgViewer::GraphicsWindow *gw =
                dynamic_cast<osgViewer::GraphicsWindow *>(
                    this->viewer->getCamera()->getGraphicsContext());
            if (!gw)
            {
                return false;
            }
            osgGA::EventQueue &queue = *(gw->getEventQueue());
    
            // Detect finger events.
            if (
                e.type == SDL_FINGERMOTION ||
                e.type == SDL_FINGERDOWN ||
                e.type == SDL_FINGERUP
            ) {
                this->fingerEventsDetected = true;
            }
            // Handle mouse events unless finger events are detected.
            if (!this->fingerEventsDetected)
            {
                return this->handleMouseEvent(e, queue);
            }
            // Handle finger events.
            return this->handleFingerEvent(e, queue);
        }
    
    private:
        bool handleFingerEvent(const SDL_Event &e, osgGA::EventQueue &queue)
        {
            int absX = this->windowWidth * e.tfinger.x;
            int absY = this->windowHeight * e.tfinger.y;
            auto correctedY = -(this->windowHeight - absY);
            switch (e.type)
            {
                case SDL_FINGERMOTION:
                    queue.mouseMotion(absX, correctedY);
                    return true;
                case SDL_FINGERDOWN: 
                    queue.mouseButtonPress(absX, correctedY, e.tfinger.fingerId);
                    return true;
                case SDL_FINGERUP:
                    queue.mouseButtonRelease(absX, correctedY, e.tfinger.fingerId);
                    return true;
                default:
                    break;
            }
            return false;
        }
    
        bool handleMouseEvent(const SDL_Event &e, osgGA::EventQueue &queue)
        {
            switch (e.type)
            {
                case SDL_MOUSEMOTION: {
                    auto correctedY = -(this->windowHeight - e.motion.y);
                    queue.mouseMotion(e.motion.x, correctedY);
                    return true;
                }
                case SDL_MOUSEBUTTONDOWN: {
                    auto correctedY = -(this->windowHeight - e.button.y);
                    queue.mouseButtonPress(e.button.x, correctedY, e.button.button);
                    return true;
                }
                case SDL_MOUSEBUTTONUP: {
                    auto correctedY = -(this->windowHeight - e.button.y);
                    queue.mouseButtonRelease(e.button.x, correctedY, e.button.button);
                    return true;
                }
                default:
                    break;
            }
            return false;
        }
    // Application+handleEvent-web End
    // Application+setupWindow-embedded Start
    private:
        int windowWidth;
        int windowHeight;
    public:
        void setupWindow(int width, int height)
        {
            this->viewer->setUpViewerAsEmbeddedInWindow(0, 0, width, height);
            this->windowWidth = width;
            this->windowHeight = height;
        }
    // Application+setupWindow-embedded End

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
const auto EXAMPLE_TITLE = "OMC-04: Match tiles";
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
        // Example+MatchTilesTest Start
        this->setupMatchTilesTest();
        
        // Example+MatchTilesTest End
        // Example+VBO Start
        this->setupSceneVBO();
        
        // Example+VBO End
// Example Start
    }
    ~Example()
    {

// Example End
        // Example+MatchTilesTest Start
        this->tearMatchTilesTestDown();
        
        // Example+MatchTilesTest End
// Example Start
        delete this->app;
    }

// Example End
    // Example+MatchTilesTest Start
    private:
        mahjong::Solitaire *game;
        osg::ref_ptr<osg::MatrixTransform> tileScene;
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
        void setupTiles()
        {
            // Order layout positions with seed.
            int seed = time(0);
            auto positions = mahjong::orderedLayoutPositions(layout.positions, seed);
    
            auto matchIds = mahjong::matchIds(positions.size());
    
            // Create tile nodes.
            auto tileScene = this->createTiles(positions, matchIds);
            // Set default (non-selected) material.
            tileScene->setStateSet(this->themeMaterial);
            // Rotate the scene to have a better view.
            scene::setSimpleRotation(tileScene, {60, 0, 0});
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
                this->themeMaterialSelected :
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
                    OMC_MAIN_EXAMPLE_LOG("Game over: LOSS");
                }
                else
                {
                    OMC_MAIN_EXAMPLE_LOG("Game over: VICTORY");
                }
            }
        }
    // Example+MatchTilesTest End
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
    // Example+VBO Start
    private:
        void setupSceneVBO()
        {
            // Do nothing for an empty scene.
            if (!this->scene.valid())
            {
                return;
            }
            // Use VBO and EBO instead of display lists.
            // CRITICAL for:
            // * mobile
            // * web (Emscripten) to skip FULL_ES2 emulation flag
            render::VBOSetupVisitor vbo;
            this->scene->accept(vbo);
        }
    // Example+VBO End
 
// Example Start
};
// Example End

} // namespace main
} // namespace omc

#endif // OGS_MAHJONG_COMPONENTS_MAIN_H


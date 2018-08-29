FEATURE main.h/Include
#include "scene.h"

#include <osg/MatrixTransform>

FEATURE main.h/Setup
this->setupMatchTilesTest();

FEATURE main.h/TearDown
this->tearMatchTilesTestDown();

FEATURE main.h/Impl
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

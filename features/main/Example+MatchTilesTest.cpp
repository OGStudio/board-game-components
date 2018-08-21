FEATURE main.h/Include
#include "mahjong.h"
#include "scene.h"

#include "resource.h"

//#include "cat.layout.h"
#include "short-victory.layout.h"
#include "short-loss.layout.h"

#include "ppl-theme.frag.h"
#include "ppl-theme.vert.h"
#include "tile-low.osgt.h"
#include "tile-theme.png.h"

#include <osg/MatrixTransform>

FEATURE main.h/Setup
this->setupMatchTilesTest();

FEATURE main.h/TearDown
this->tearMatchTilesTestDown();

FEATURE main.h/Impl
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
        /*
        resource::Resource res(
            "layouts",
            "cat.layout",
            cat_layout,
            cat_layout_len
        );
        */
        resource::Resource res(
            "layouts",
            "short-victory.layout",
            short_victory_layout,
            short_victory_layout_len
        );
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
        for (auto pos : this->layout.positions)
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

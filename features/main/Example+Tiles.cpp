FEATURE main.h/Impl
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

FEATURE main.h/Impl
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

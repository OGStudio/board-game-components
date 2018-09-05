FEATURE main.h/Setup
this->setupSetLayoutThemeSeedTest(parameters);

FEATURE main.h/Impl
private:
    void setupSetLayoutThemeSeedTest(const Parameters &parameters)
    {
        this->setupDefaultLayoutTheme();
        this->loadLayout(parameters);
        //this->loadTheme(parameters);
        /*
        this->setupTiles();
        this->setupNodeSelection();
        this->setupTileSelection();
        this->setupTileSelectionDepiction();
        this->setupTileMatching();
        this->setupUnmatchedTilesDeselection();
        this->setupMatchedTilesRemoval();
        this->setupGameState();
        */
        /*
        // Load remote layout and/or theme.
        for (auto parameter : parameters)
        {
            auto key = parameter.first;
            auto value = parameter.second;
            if (key == "layout")
            {
                this->loadRemoteLayout(value);
            }
            else if (key == "theme")
            {
                this->loadRemoteTheme(value);
            }
        }
        */
    }
    void tearSetLayoutThemeSeedTestDown()
    {
        this->tearNodeSelectionDown();
    }

    // Layout.

    core::Reporter layoutLoaded;
    void loadLayout(const Parameters &parameters)
    {
        auto it = parameters.find("layout");

        // Skip loading layout if `layout` parameter is absent.
        if (it == parameters.end())
        {
            this->layoutLoaded.report();
            return;
        }

        // Load layout.
        auto layoutValue = it->second;
        
        // Try to expand layout in case it's a collapsed remote path.
        layoutValue = resource::expandBitBucketPath(layoutValue);
        layoutValue = resource::expandGitHubPath(layoutValue);

        if (resource::isPathRemote(layoutValue))
        {
            this->loadRemoteLayout(layoutValue);
        }
        // TODO Check if it's internal layout.
        // TODO Otherwise it's a local layout.
    }
    void loadInternalLayout(const std::string &layoutName)
    {
        MAIN_EXAMPLE_LOG("TODO load internal layout: '%s'", layoutName.c_str());
    }
    void loadLocalLayout(const std::string &layoutFileName)
    {
        MAIN_EXAMPLE_LOG("TODO load local layout: '%s'", layoutFileName.c_str());
    }
    void loadRemoteLayout(const std::string &layoutURL)
    {
        MAIN_EXAMPLE_LOG("TODO load remote layout: '%s'", layoutURL.c_str());
    }
    

    /*
    void loadRemoteLayout(const std::string &url)
    {
        auto success = [&](std::string response) {
            this->parseLayoutResponse(response, url);
        };
        auto failure = [&](std::string reason) {
            MAIN_EXAMPLE_LOG(
                "ERROR Could not load layout: %s",
                reason.c_str()
            );
        };
        MAIN_EXAMPLE_LOG("Loading layout from '%s'", url.c_str());
        this->app->httpClient->get(url, success, failure);
    }
    */
    /*
    void parseLayoutResponse(
        const std::string &response,
        const std::string &url
    ) {
        mahjong::Layout layout;
        std::istringstream in(response);
        if (mahjong::parseLayout(in, layout))
        {
            auto tileScene = this->createTiles(layout.positions);
            // Apply normal state material to the whole scene.
            tileScene->setStateSet(this->themeMaterial);
            // Rotate the tile scene to have a better view.
            scene::setSimpleRotation(tileScene, {60, 0, 0});
            // Add tile scene to the scene.
            this->scene->addChild(tileScene);
            // Reset the scene.
            this->app->setScene(this->scene);
            MAIN_EXAMPLE_LOG("Successfully loaded layout");
        }
        else
        {
            MAIN_EXAMPLE_LOG("ERROR Could not parse loaded layout");
        }
    }
    */

    /*
    void loadRemoteTheme(const std::string &url)
    {
        auto success = [=](std::string response) {
            // NOTE We use `=` in lambda capture to capture url copy
            // NOTE Otherwise we have crash when parsing.
            this->parseThemeResponse(response, url);
        };
        auto failure = [&](std::string reason) {
            MAIN_EXAMPLE_LOG(
                "ERROR Could not load theme: %s",
                reason.c_str()
            );
        };
        MAIN_EXAMPLE_LOG("Loading theme from '%s'", url.c_str());
        this->app->httpClient->get(url, success, failure);
    }
    */
    /*
    void parseThemeResponse(const std::string &response, const std::string &url)
    {
        resource::Resource
            themeRes(
                "theme-remote",
                url,
                resource::stringToResourceContents(response),
                response.length()
            );
        // Set texture to materials.
        auto texture = resource::createTexture(themeRes);
        this->themeMaterial->setTextureAttributeAndModes(0, texture);
        this->themeMaterialSelected->setTextureAttributeAndModes(0, texture);
        MAIN_EXAMPLE_LOG("Successfully loaded theme");
    }
    */

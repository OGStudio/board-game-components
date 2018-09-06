FEATURE main.h/Setup
this->setupSetLayoutThemeSeedTest(parameters);

FEATURE main.h/TearDown
this->tearSetLayoutThemeSeedTestDown();

FEATURE main.h/Impl
private:
    core::Reporter layoutLoaded;

    void setupSetLayoutThemeSeedTest(const Parameters &parameters)
    {
        this->setupInternalLayouts();
        this->setupDefaultLayoutTheme();
        this->setupContinuationAfterLoading();

        this->loadLayout(parameters);
        //this->loadTheme(parameters);
    }
    void tearSetLayoutThemeSeedTestDown()
    {
        this->tearInternalLayoutsDown();
        //this->tearNodeSelectionDown();
    }
    void setupContinuationAfterLoading()
    {
        this->layoutLoaded.addOneTimeCallback(
            [&] {
                this->setupContinuation();
            }
        );

    }
    void setupContinuation()
    {
        this->setupTiles();
        /*
        this->setupNodeSelection();
        this->setupTileSelection();
        this->setupTileSelectionDepiction();
        this->setupTileMatching();
        this->setupUnmatchedTilesDeselection();
        this->setupMatchedTilesRemoval();
        this->setupGameState();
        */
    }

    // Layout.

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

        // Remote.
        if (resource::isPathRemote(layoutValue))
        {
            this->loadRemoteLayout(layoutValue);
        }
        // Local or internal.
        else
        {
            auto internalLayout =
                this->internalLayouts->resource("layouts", layoutValue);
            // Internal.
            if (internalLayout)
            {
                this->loadInternalLayout(*internalLayout);
            }
            // Local.
            else
            {
                this->loadLocalLayout(layoutValue);
            }
        }
    }
    void loadInternalLayout(resource::Resource &layout)
    {
        resource::ResourceStreamBuffer buf(layout);
        std::istream in(&buf);
        if (!mahjong::parseLayout(in, this->layout))
        {
            MAIN_EXAMPLE_LOG("ERROR Could not parse internal layout");
        }

        // Report.
        this->layoutLoaded.report();
    }
    void loadLocalLayout(const std::string &layoutFileName)
    {
        MAIN_EXAMPLE_LOG("TODO load local layout: '%s'", layoutFileName.c_str());
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

            // Report.
            this->layoutLoaded.report();
        };

        auto failure = [&](std::string reason) {
            MAIN_EXAMPLE_LOG(
                "ERROR Could not load remote layout: '%s'",
                reason.c_str()
            );

            // Report.
            this->layoutLoaded.report();
        };

        this->app->httpClient->get(url, success, failure);
    }

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

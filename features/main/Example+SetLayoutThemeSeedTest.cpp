FEATURE main.h/Setup
this->setupSetLayoutThemeSeedTest();

FEATURE main.h/TearDown
this->tearSetLayoutThemeSeedTestDown();

FEATURE main.h/Impl
private:
    core::Sequence setupSequence;
    void setupSetLayoutThemeSeedTest()
    {
        this->setupInternalLayouts();
        this->setupInternalThemes();
        this->setupDefaultLayoutTheme();

        this->setupSequence.setActions({
            "loadLayout",
            //"loadTheme",
            "finishSetup",
        });

        // Register actions.
        CORE_REGISTER_SEQUENCE_ACTION(
            this->setupSequence,
            "loadLayout",
            this->loadLayout()
        );
        CORE_REGISTER_SEQUENCE_ACTION(
            this->setupSequence,
            "finishSetup",
            this->finishSetup()
        );

        // Enable sequence.
        this->setupSequence.setEnabled(true);
    }
    void tearSetLayoutThemeSeedTestDown()
    {
        this->tearInternalLayoutsDown();
        //this->tearNodeSelectionDown();
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
    core::Reporter *finishSetup()
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
        return 0;
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

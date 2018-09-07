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
            "loadTheme",
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
            "loadTheme",
            this->loadTheme()
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

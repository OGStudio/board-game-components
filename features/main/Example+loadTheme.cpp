FEATURE main.h/Impl
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
        }
        else
        {
            MAIN_EXAMPLE_LOG("ERROR Could not read local theme");
        }
    }
    void loadRemoteTheme(const std::string &url)
    {
        MAIN_EXAMPLE_LOG("Loading remote layout '%s'", url.c_str());

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

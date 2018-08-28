FEATURE main.h/Setup
this->setupRemoteLayoutTheme(parameters);

FEATURE main.h/Impl
private:
    void setupRemoteLayoutTheme(const Parameters &parameters)
    {
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
    }

    void loadRemoteLayout(const std::string &url)
    {
        auto success = [&](std::string response) {
            this->parseLayoutResponse(response, url);
        };
        auto failure = [&](std::string reason) {
            OMC_MAIN_EXAMPLE_LOG(
                "ERROR Could not load layout: %s",
                reason.c_str()
            );
        };
        OMC_MAIN_EXAMPLE_LOG("Loading layout from '%s'", url.c_str());
        this->app->httpClient->get(url, success, failure);
    }
    void parseLayoutResponse(
        const std::string &response,
        const std::string &url
    ) {
        mahjong::Layout layout;
        std::istringstream in(response);
        if (mahjong::parseLayout(in, layout))
        {
            auto tileScene = this->createTiles(layout);
            // Apply normal state material to the whole scene.
            tileScene->setStateSet(this->themeMaterial);
            // Rotate the tile scene to have a better view.
            scene::setSimpleRotation(tileScene, {60, 0, 0});
            // Add tile scene to the scene.
            this->scene->addChild(tileScene);
            // Reset the scene.
            this->app->setScene(this->scene);
            OMC_MAIN_EXAMPLE_LOG("Successfully loaded layout");
        }
        else
        {
            OMC_MAIN_EXAMPLE_LOG("ERROR Could not parse loaded layout");
        }
    }

    void loadRemoteTheme(const std::string &url)
    {
        auto success = [=](std::string response) {
            // NOTE We use `=` in lambda capture to capture url copy
            // NOTE Otherwise we have crash when parsing.
            this->parseThemeResponse(response, url);
        };
        auto failure = [&](std::string reason) {
            OMC_MAIN_EXAMPLE_LOG(
                "ERROR Could not load theme: %s",
                reason.c_str()
            );
        };
        OMC_MAIN_EXAMPLE_LOG("Loading theme from '%s'", url.c_str());
        this->app->httpClient->get(url, success, failure);
    }
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
        OMC_MAIN_EXAMPLE_LOG("Successfully loaded theme");
    }

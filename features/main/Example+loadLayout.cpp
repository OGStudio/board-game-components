FEATURE main.h/Impl
private:
    core::Reporter layoutLoaded;

    core::Reporter *loadLayout(const std::string &layout)
    {
        // Try to expand layout in case it's a collapsed remote path.
        auto path = layout;
        path = resource::expandBitBucketPath(path);
        path = resource::expandGitHubPath(path);

        // Remote.
        if (resource::isPathRemote(path))
        {
            this->loadRemoteLayout(path);
            return &this->layoutLoaded;
        }

        // Local or internal.
        auto internalLayout =
            this->internalLayouts->resource("layouts", layout);
        // Internal.
        if (internalLayout)
        {
            this->loadInternalLayout(*internalLayout);
        }
        // Local.
        else
        {
            this->loadLocalLayout(layout);
        }

        // Report finish instantly.
        return 0;
    }
    void loadInternalLayout(resource::Resource &layout)
    {
        resource::ResourceStreamBuffer buf(layout);
        std::istream in(&buf);
        if (!mahjong::parseLayout(in, this->layout))
        {
            MAIN_EXAMPLE_LOG("ERROR Could not parse internal layout");
        }
        else
        {
            MAIN_EXAMPLE_LOG("Successfully loaded and parsed internal layout");
        }
    }
    void loadLocalLayout(const std::string &layoutFileName)
    {
        std::ifstream localLayout(layoutFileName);
        if (localLayout)
        {
            if (!mahjong::parseLayout(localLayout, this->layout))
            {
                MAIN_EXAMPLE_LOG("ERROR Could not parse local layout");
            }
            else
            {
                MAIN_EXAMPLE_LOG("Successfully loaded and parsed local layout");
            }
        }
        else
        {
            MAIN_EXAMPLE_LOG("ERROR Could not read local layout");
        }
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

            // Report finish.
            this->layoutLoaded.report();
        };

        auto failure = [&](std::string reason) {
            MAIN_EXAMPLE_LOG(
                "ERROR Could not load remote layout: '%s'",
                reason.c_str()
            );

            // Report finish.
            this->layoutLoaded.report();
        };

        // Initiate loading.
        this->app->httpClient->get(url, success, failure);
    }

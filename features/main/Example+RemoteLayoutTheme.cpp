FEATURE main.h/Setup
this->setupRemoteLayoutTheme(parameters);

FEATURE main.h/Impl
private:
    void setupRemoteLayoutTheme(const Parameters &parameters)
    {
        for (auto parameter : parameters)
        {
            MC_MAIN_EXAMPLE_LOG(
                "parameter key '%s' value '%s'",
                parameter.first.c_str(),
                parameter.second.c_str()
            );
        }
        // TODO load remote layout
        // TODO load remote theme
        // TODO load local layout/theme when desktop
    }

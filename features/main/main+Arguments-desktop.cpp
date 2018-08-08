FEATURE main.cpp/Impl
// Print all arguments.
for (auto i = 1; i < argc; ++i)
{
    auto argument = std::string(argv[i]);
    if (format::stringStartsWith(argument, "--"))
    {
        // Skip the dashes.
        auto option = argument.substr(2);
        auto keyAndValue = format::splitString(option, "=");
        if (keyAndValue.size() == 2)
        {
            auto key = keyAndValue[0];
            auto value = keyAndValue[1];
            MC_MAIN_LOG(
                "Argument key: '%s' value: '%s'",
                key.c_str(),
                value.c_str()
            );
        }

    }
}

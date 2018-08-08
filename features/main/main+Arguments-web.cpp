FEATURE main.cpp/Impl
// The first argument (if present) under web
// contains all query parameters after `?` sign.
if (argc >= 2)
{
    auto query = argv[1];
    auto items = format::splitString(query, "&");

    for (auto item : items)
    {
        auto keyAndValue = format::splitString(item, "=");
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

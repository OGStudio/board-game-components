FEATURE main.cpp/Impl
// Print all arguments.
for (auto i = 0; i < argc; ++i)
{
    MC_MAIN_LOG("id: '%d' argument: '%s'", i, argv[i]);
}


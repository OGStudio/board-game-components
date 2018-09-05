FEATURE resource.h/Impl
std::string expandGitHubPath(const std::string &path)
{
    const std::string prefix = "github://";

    // Make sure we have expected prefix.
    if (!format::stringStartsWith(path, prefix))
    {
        return path;
    }

    auto location = path.substr(prefix.length());
    auto parts = format::splitString(location, "/");

    // Make sure we have enough location parts to assemble both
    // repository name and file name.
    if (parts.size() < 3)
    {
        return path;
    }

    auto repository =
        format::printfString("%s/%s", parts[0].c_str(), parts[1].c_str());
    auto fileName = location.substr(repository.length() + 1);
    auto githubPrefix = "https://raw.githubusercontent.com";
    auto githubBranch = "master";
    return
        format::printfString(
            "%s/%s/%s/%s",
            githubPrefix,
            repository.c_str(),
            githubBranch,
            fileName.c_str()
        );
}

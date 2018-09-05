FEATURE resource.h/Impl
std::string expandGitHubPath(const std::string &path)
{
    return
        expandRemotePath(
            path,
            "github://",
            "https://raw.githubusercontent.com",
            "master"
        );
}

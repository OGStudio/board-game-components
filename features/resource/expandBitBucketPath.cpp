FEATURE resource.h/Impl
std::string expandBitBucketPath(const std::string &path)
{
    return
        expandRemotePath(
            path,
            "bitbucket://",
            "https://bitbucket.org",
            "raw/default"
        );
}

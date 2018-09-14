FEATURE main.h/Impl
private:
    const std::string loadingAnimationCallbackName = "LoadingAnimation";
    osg::Timer loadingTimer;
    scene::LinearInterpolator interpolator;
    const float loadingAnimationDelay = 0.2;

    core::Reporter *startColorLoading()
    {
        this->resetBackground();
        // Configure blinking interpolation.
        this->interpolator.keyValues = {
            {0, 0},
            {0.5, 0.5}, // Change color to 0.5 in 0.5s.
            {1, 0}, // Change color back to 0 in 0.5s.
        };
        this->startColorLoadingAnimation();

        return 0;
    }
    core::Reporter *stopColorLoading()
    {
        this->stopColorLoadingAnimation();
        // Make sure we reset background after the last background animation.
        this->app->frameReporter.addOneTimeCallback(
            [&] {
                this->resetBackground();
            }
        );
        return 0;
    }
    void startColorLoadingAnimation()
    {
        this->loadingTimer.setStartTick();

        this->app->frameReporter.addCallback(
            [&] {
                auto elapsed = this->loadingTimer.time_s();
                // Make sure delay passed.
                elapsed -= this->loadingAnimationDelay;
                if (elapsed < 0)
                {
                    return;
                }
                this->animateColorLoading(elapsed);
            },
            this->loadingAnimationCallbackName
        );
    }
    void stopColorLoadingAnimation()
    {
        this->app->frameReporter.removeCallback(
            this->loadingAnimationCallbackName
        );
    }
    void animateColorLoading(float elapsed)
    {
        // Keep `elapsed` in [0; 1] range by using only fractional part.
        float intpart;
        elapsed = modf(elapsed, &intpart);

        auto value = this->interpolator.valueFor(elapsed);
        this->app->camera()->setClearColor({value, value, value, 0});
    }
    void resetBackground()
    {
        this->app->camera()->setClearColor({0, 0, 0, 0});
    }

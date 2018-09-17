FEATURE main.h/Setup
this->setupColorfulStatus();

FEATURE main.h/Impl
private:
    void setupColorfulStatus()
    {
        this->setupLoadingAnimation();
        this->setupFinishAnimation();
    }

    // Loading animation.

    const std::string loadingAnimationCallbackName = "LoadingAnimation";
    osg::Timer loadingTimer;
    scene::LinearInterpolator interpolator;
    const float loadingAnimationDelay = 0.2;

    void setupLoadingAnimation()
    {
        this->setupSequence.insertAction(
            CORE_SEQUENCE_ACTION("startLoading", this->startColorLoading()),
            "loadLayout"
        );
        this->setupSequence.insertAction(
            CORE_SEQUENCE_ACTION("stopLoading", this->stopColorLoading()),
            "finishSetup"
        );
    }
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

    // Finish animation.

    void setupFinishAnimation()
    {
        this->finishedGame.addCallback(
            [&] {
                this->animateFinish();
            }
        );
    }
    void animateFinish()
    {
        float r = 0;
        float g = 0;
        // Display green color upon victory.
        if (this->isGameVictorious)
        {
            g = 0.5;
        }
        // Display red color upon loss.
        else
        {
            r = 0.5;
        }
        this->app->camera()->setClearColor({r, g, 0, 0});
    }

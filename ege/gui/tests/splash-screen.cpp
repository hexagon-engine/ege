#include <testsuite/Tests.h>
#include <ege/gui.h>

TESTCASE(loading)
{
    EGE::GUIGameLoop loop;
    loop.openWindow(sf::VideoMode(500, 500), "SplashScreen");

    // We need some 'bootstrap' resource manager which will have only
    // splash screen image. We swap ResourceManagers in splash screen
    // callback.
    auto bootstrap = make<EGE::GUIResourceManager>();
    bootstrap->registerTexture("splash.png");
    loop.setResourceManager(bootstrap);

    auto splashScreen = make<EGE::SplashScreen>(loop);
    splashScreen->setImage("splash.png");
    splashScreen->startLoading([](int& progress) {
        for(int i = 0; i < 1000000000; i++)
        {
            progress = i / 10000000 + 1;
            if(i % 10000000 == 0)
                ege_log.info() << "Loading... " << progress << "%";
        }
    }, [&loop](EGE::AsyncTask::State state) {
        ege_log.notice() << "YAY!! Splash Screen loading finished with rc=" << state.returnCode;
    });
    loop.setCurrentGUIScreen(splashScreen);

    return loop.run();
}

TESTCASE(displaying)
{
    EGE::GUIGameLoop loop;
    loop.openWindow(sf::VideoMode(500, 500), "SplashScreen");

    // We need some 'bootstrap' resource manager which will have only
    // splash screen image. We swap ResourceManagers in splash screen
    // callback.
    auto bootstrap = make<EGE::GUIResourceManager>();
    bootstrap->registerTexture("splash.png");
    loop.setResourceManager(bootstrap);

    auto splashScreen = make<EGE::SplashScreen>(loop);
    splashScreen->setImage("splash.png");
    splashScreen->start(EGE::Time(5), [&loop]() {
        ege_log.notice() << "YAY!! Splash Screen display finished!";
    });
    loop.setCurrentGUIScreen(splashScreen);

    return loop.run();
}

RUN_TESTS(splashScreen)

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

    auto wrapper = splashScreen->addNewWidget<EGE::CompoundWidget>();
    wrapper->layoutDirection = EGE::LayoutElement::Direction::Horizontal;
    wrapper->setSize({"1N", "50px"});
    wrapper->setPadding({"5px", "5px"});

    auto progressBar = wrapper->addNewWidget<EGE::ProgressBar>(splashScreen->createProgress(100));

    splashScreen->startLoading([](EGE::Progress& progress) {
        for(int i = 0; i < 1000000000; i++)
        {
            if(i % 10000000 == 0) {
                ege_log.info() << "Loading... " << progress;
                progress.step();
            }
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
    splashScreen->createProgress(100);
    splashScreen->setImage("splash.png");
    splashScreen->start(EGE::Time(5), [&loop]() {
        ege_log.notice() << "YAY!! Splash Screen display finished!";
    });

    loop.setCurrentGUIScreen(splashScreen);

    return loop.run();
}

RUN_TESTS(splashScreen)

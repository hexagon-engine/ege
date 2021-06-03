#include <testsuite/Tests.h>
#include <ege/sound.h>
#include <ege/debug/Logger.h>

TESTCASE(basic)
{
    EGE::EventLoop loop;

    auto soundManager = make<EGE::SoundManager>();
    // https://soundbible.com/1645-Pling.html
    auto popSound = soundManager->loadSoundFromFile("Pling-KevanGC-1485374730.wav");
    if(!popSound)
        return 1;
    float volume = 0.5f;
    soundManager->setVolume(volume);
    soundManager->addTimer("Sound timer", make<EGE::Timer>(*soundManager, EGE::Timer::Mode::Infinite, EGE::Time(0.5), [&](std::string, EGE::Timer*) {
        ege_log.info() << "Playing pop sound!!! (Vol=" << volume << ")";
        soundManager->playSound(*popSound);
        soundManager->setVolume(volume);
        volume /= 1.1f;
        if(volume <= 0.1f)
            soundManager->exit(0);
    }));

    loop.addSubLoop(soundManager);
    return loop.run();
}

RUN_TESTS(basic)

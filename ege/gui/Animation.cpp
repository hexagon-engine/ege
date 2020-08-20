/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Animation.h"

#include <algorithm>

namespace EGE
{

void Animation::addKeyframe(double time, double value)
{
    m_keyframes.push_back(std::make_pair(time,value));
}

bool Animation::isKeyframe(double time)
{
    for(auto pr: m_keyframes)
        if(pr.first == time)
            return true;

    return false;
}

double Animation::getValue(double time)
{
    ASSERT(!m_keyframes.empty());
    std::pair<double, double> previous;

    if(m_iterations > 1)
         previous = m_keyframes.back();

    // ensure that keyframes are sorted!
    if(!m_sorted)
    {
        std::sort(m_keyframes.begin(), m_keyframes.end(), [](std::pair<double, double> _L, std::pair<double, double> _R)->bool {
                        return _L.first < _R.first;
                  });

        // add "zero frame" and "last frame" if it doesn't exist
        if(!isKeyframe(0.0)) m_keyframes.insert(m_keyframes.begin(), std::make_pair(0.0, m_keyframes.front().second));
        if(!isKeyframe(1.0)) m_keyframes.insert(m_keyframes.end(), std::make_pair(1.0, m_keyframes.back().second));

        m_sorted = true;
    }
    DUMP(ANIMATION_DEBUG, time);

    // clamp animation if it won't be repeated
    if(m_remainingIterations == 1)
        time = std::min(1.0, std::max(0.0, time));

    while(time > 1.0)
        time--;

    while(time < 0.0)
        time++;

    for(auto keyframe: m_keyframes)
    {
        if(keyframe.first >= time)
        {
            //DUMP(ANIMATION_DEBUG, keyframe.first);
            //DUMP(ANIMATION_DEBUG, keyframe.second);
            //DUMP(ANIMATION_DEBUG, previous.first);
            //DUMP(ANIMATION_DEBUG, previous.second);

            double timeDiff = (keyframe.first - previous.first);
            //DUMP(ANIMATION_DEBUG, timeDiff);

            if(timeDiff == 0.0)
                return (keyframe.second + previous.second) / 2;

            double td2 = (time - previous.first);
            //DUMP(ANIMATION_DEBUG, td2);
            double timeFactor = td2 / timeDiff;

            // TODO: here apply easing function f(timeFactor) !
            if(m_ease)
                timeFactor = m_ease(timeFactor);
            //DUMP(ANIMATION_DEBUG, timeFactor);
            return (keyframe.second - previous.second) * timeFactor + previous.second;
        }
        previous = keyframe;
    }
    return 0.0;
}

}

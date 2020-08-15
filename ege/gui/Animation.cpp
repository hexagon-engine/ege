/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Animation.h"

namespace EGE
{

void Animation::addKeyframe(double time, double value)
{
    m_keyframes.push_back(std::make_pair(time,value));
}

double Animation::getValue(double time)
{
    ASSERT(!m_keyframes.empty());
    std::pair<double, double> previous;

    if(time > 1.0)
        return m_keyframes.back().second;

    for(auto keyframe: m_keyframes)
    {
        if(keyframe.first > time)
        {
            DUMP(1, keyframe.first);
            DUMP(1, keyframe.second);
            DUMP(1, previous.first);
            DUMP(1, previous.second);

            double timeDiff = (keyframe.first - previous.first);
            DUMP(1, timeDiff);

            if(timeDiff == 0.0)
                return 0.0;

            double td2 = (time - previous.first);
            DUMP(1, td2);
            double timeFactor = td2 / timeDiff;
            DUMP(1, timeFactor);

            // TODO: here apply easing function f(timeFactor) !
            return (keyframe.second - previous.second) * timeFactor + previous.second;
        }
        previous = keyframe;
    }
    return 0.0;
}

}

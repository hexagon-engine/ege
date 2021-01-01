/*
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*      ,----  ,----  ,----
*      |      |      |
*      |----  | --,  |----
*      |      |   |  |
*      '----  '---'  '----
*
*     Framework Library for Hexagon
*
*    Copyright (c) Sppmacd 2020 - 2021
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
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
            double timeDiff = (keyframe.first - previous.first);

            if(timeDiff == 0.0)
                return (keyframe.second + previous.second) / 2;

            double td2 = (time - previous.first);
            double timeFactor = td2 / timeDiff;

            if(m_ease)
                timeFactor = m_ease(timeFactor);
            return (keyframe.second - previous.second) * timeFactor + previous.second;
        }
        previous = keyframe;
    }
    return 0.0;
}

}

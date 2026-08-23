#include "graphics/Transition.h"
#include <algorithm>

Transition::Transition()
    : type(TransitionType::Fade),
      duration(0.4f),
      timer(0.0f),
      active(false),
      switching(false)
{
}

void Transition::start(TransitionType transitionType, float transitionDuration)
{
    type = transitionType;
    duration = transitionDuration;
    timer = 0.0f;

    active = true;
    switching = false;
}

void Transition::update(float deltaTime)
{
    if (!active)
        return;

    timer += deltaTime;

    if (timer >= duration * 2.0f)
    {
        timer = duration * 2.0f;
        active = false;
    }
}

void Transition::draw() const
{
    if (!active)
        return;

    if (type == TransitionType::Fade)
    {
        float alpha;

        if (timer < duration)
        {
            // Fade out
            alpha = timer / duration;
        }
        else
        {
            // Fade in
            alpha = 1.0f - ((timer - duration) / duration);
        }

        alpha = std::clamp(alpha, 0.0f, 1.0f);

        Color overlay = {
            0,
            0,
            0,
            static_cast<unsigned char>(alpha * 255.0f)};

        DrawRectangle(
            0,
            0,
            GetScreenWidth(),
            GetScreenHeight(),
            overlay);
    }
}

bool Transition::isActive() const
{
    return active;
}

bool Transition::shouldSwitch() const
{
    return active &&
           !switching &&
           timer >= duration;
}

void Transition::finishSwitch()
{
    switching = true;
}

float Transition::getProgress() const
{
    if (duration <= 0.0f)
        return 1.0f;

    return std::clamp(timer / duration, 0.0f, 1.0f);
}
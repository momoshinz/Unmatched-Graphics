#include "graphics/Transition.h"
#include <algorithm>

Transition::Transition()
    : type(TransitionType::Fade),
      duration(0.4f),
      holdDuration(0.0f),
      timer(0.0f),
      active(false),
      switching(false),
      font(),
      text("")
{
}

void Transition::start(TransitionType transitionType, float transitionDuration, float blackHoldDuration)
{
    type = transitionType;

    duration = transitionDuration;
    holdDuration = blackHoldDuration;

    timer = 0.0f;

    active = true;
    switching = false;
}

void Transition::update(float deltaTime)
{
    if (!active)
        return;

    timer += deltaTime;

    float totalDuration = duration + holdDuration + duration;

    if (timer >= totalDuration)
    {
        timer = totalDuration;
        active = false;
    }
}

void Transition::draw() const
{
    if (!active)
        return;

    if (type == TransitionType::Fade)
    {
        float alpha = 0.0f;
        if (timer < duration)
        {
            alpha = timer / duration;
        }

        else if (timer < duration + holdDuration)
        {
            alpha = 1.0f;
        }

        else
        {
            float fadeInTimer = timer - duration - holdDuration;
            alpha = 1.0f - (fadeInTimer / duration);
        }

        alpha = std::clamp(alpha, 0.0f, 1.0f);

        Color overlay = {0, 0, 0, static_cast<unsigned char>(alpha * 255.0f)};

        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), overlay);

        if (timer >= duration && timer < duration + holdDuration && !text.empty())
        {
            float fontSize = 40.0f;
            float spacing = 2.0f;

            Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, spacing);

            float x = (GetScreenWidth() - textSize.x) / 2.0f;
            float y = (GetScreenHeight() - textSize.y) / 2.0f;

            DrawTextEx(font, text.c_str(), {x, y}, fontSize, spacing, WHITE);
        }
    }
}

bool Transition::isActive() const
{
    return active;
}

bool Transition::shouldSwitch() const
{
    return active && !switching && timer >= duration;
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

void Transition::setFont(Font transitionFont)
{
    font = transitionFont;
}

void Transition::setText(const std::string &transitionText)
{
    text = transitionText;
}
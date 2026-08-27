#pragma once

#include "raylib.h"
#include <string>

enum class TransitionType
{
    Fade
};

class Transition
{
private:
    TransitionType type;

    float duration;
    float holdDuration;

    float timer;

    bool active;
    bool switching;

    Font font;
    std::string text;

public:
    Transition();

    void start(TransitionType transitionType,
               float transitionDuration,
               float blackHoldDuration = 0.0f);

    void update(float deltaTime);
    void draw() const;

    bool isActive() const;
    bool shouldSwitch() const;

    void finishSwitch();

    float getProgress() const;

    void setFont(Font transitionFont);
    void setText(const std::string &transitionText);
};
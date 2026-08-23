#pragma once
#include "raylib.h"

enum class TransitionType
{
    Fade
};

class Transition
{
private:
    TransitionType type;

    float duration;
    float timer;

    bool active;
    bool switching;

public:
    Transition();
    void start(TransitionType type, float duration);
    void update(float deltaTime);
    void draw() const;
    bool isActive() const;
    bool shouldSwitch() const;
    void finishSwitch();
    float getProgress() const;
};
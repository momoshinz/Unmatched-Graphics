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
    // =========================================
    // Transition Settings
    // =========================================

    TransitionType type;

    // Duration of fade out
    float duration;

    // How long the screen stays completely black
    float holdDuration;

    // Current transition timer
    float timer;

    // =========================================
    // State
    // =========================================

    bool active;
    bool switching;

    // =========================================
    // Text
    // =========================================

    Font font;
    std::string text;

public:
    // =========================================
    // Constructor
    // =========================================

    Transition();

    // =========================================
    // Start Transition
    // =========================================

    void start(
        TransitionType transitionType,
        float transitionDuration,
        float blackHoldDuration = 0.0f);

    // =========================================
    // Update / Draw
    // =========================================

    void update(float deltaTime);
    void draw() const;

    // =========================================
    // State
    // =========================================

    bool isActive() const;
    bool shouldSwitch() const;

    void finishSwitch();

    // =========================================
    // Progress
    // =========================================

    float getProgress() const;

    // =========================================
    // Font
    // =========================================

    void setFont(Font transitionFont);

    // =========================================
    // Text
    // =========================================

    void setText(const std::string &transitionText);
};
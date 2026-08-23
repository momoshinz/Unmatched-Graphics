#include "graphics/Transition.h"

#include <algorithm>

// =========================================
// Constructor
// =========================================

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

// =========================================
// Start Transition
// =========================================

void Transition::start(
    TransitionType transitionType,
    float transitionDuration,
    float blackHoldDuration)
{
    type = transitionType;

    duration = transitionDuration;
    holdDuration = blackHoldDuration;

    timer = 0.0f;

    active = true;
    switching = false;
}

// =========================================
// Update
// =========================================

void Transition::update(float deltaTime)
{
    if (!active)
        return;

    timer += deltaTime;

    // =========================================
    // Total Transition Time
    //
    // Fade Out
    //      +
    // Black Hold
    //      +
    // Fade In
    // =========================================

    float totalDuration =
        duration +
        holdDuration +
        duration;

    if (timer >= totalDuration)
    {
        timer = totalDuration;
        active = false;
    }
}

// =========================================
// Draw
// =========================================

void Transition::draw() const
{
    if (!active)
        return;

    // =========================================
    // FADE
    // =========================================

    if (type == TransitionType::Fade)
    {
        float alpha = 0.0f;

        // =====================================
        // FADE OUT
        // =====================================

        if (timer < duration)
        {
            alpha = timer / duration;
        }

        // =====================================
        // BLACK HOLD
        // =====================================

        else if (timer < duration + holdDuration)
        {
            alpha = 1.0f;
        }

        // =====================================
        // FADE IN
        // =====================================

        else
        {
            float fadeInTimer =
                timer -
                duration -
                holdDuration;

            alpha =
                1.0f -
                (fadeInTimer / duration);
        }

        // =====================================
        // Clamp Alpha
        // =====================================

        alpha = std::clamp(
            alpha,
            0.0f,
            1.0f);

        // =====================================
        // Black Overlay
        // =====================================

        Color overlay = {
            0,
            0,
            0,
            static_cast<unsigned char>(
                alpha * 255.0f)};

        DrawRectangle(
            0,
            0,
            GetScreenWidth(),
            GetScreenHeight(),
            overlay);

        // =====================================
        // TEXT
        //
        // Only while screen is completely black
        // =====================================

        if (timer >= duration &&
            timer < duration + holdDuration &&
            !text.empty())
        {
            float fontSize = 40.0f;
            float spacing = 2.0f;

            
            Vector2 textSize =
                MeasureTextEx(
                    font,
                    text.c_str(),
                    fontSize,
                    spacing);

            float x =
                (GetScreenWidth() -
                 textSize.x) /
                2.0f;

            float y =
                (GetScreenHeight() -
                 textSize.y) /
                2.0f;

            DrawTextEx(
                font,
                text.c_str(),
                {x, y},
                fontSize,
                spacing,
                WHITE);
        }
    }
}

// =========================================
// Is Active
// =========================================

bool Transition::isActive() const
{
    return active;
}

// =========================================
// Should Switch
// =========================================
//
// The screen changes exactly when the
// Fade Out is finished and the screen
// has reached the black state.
//
// =========================================

bool Transition::shouldSwitch() const
{
    return active &&
           !switching &&
           timer >= duration;
}

// =========================================
// Finish Switch
// =========================================

void Transition::finishSwitch()
{
    switching = true;
}

// =========================================
// Progress
// =========================================

float Transition::getProgress() const
{
    if (duration <= 0.0f)
        return 1.0f;

    return std::clamp(
        timer / duration,
        0.0f,
        1.0f);
}

// =========================================
// Set Font
// =========================================

void Transition::setFont(Font transitionFont)
{
    font = transitionFont;
}

// =========================================
// Set Text
// =========================================

void Transition::setText(
    const std::string &transitionText)
{
    text = transitionText;
}
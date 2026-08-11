#ifndef LOADING_SCREEN_H
#define LOADING_SCREEN_H

#include <raylib.h>

class LoadingScreen
{
    private:
        Texture2D background;
        Font font;

        float elapsedTime;
        float duration;

    public:
        LoadingScreen(float duration = 5.0f);

        bool load();
        void unload();

        void update();
        void draw();

        bool isFinished() const;
        void reset();

        ~LoadingScreen();
};

#endif
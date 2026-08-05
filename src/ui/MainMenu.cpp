#include "ui/MainMenu.h"
#include <iostream>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

int MainMenu::show()
{
    ScreenInteractive screen = ScreenInteractive::TerminalOutput();

    std::vector<std::string> entries = {
        "[o] New Game",
        "[?] Load Game",
        "[>] Exit",
    };

    int selected = 0;

    MenuOption option;
    option.on_enter = screen.ExitLoopClosure();

    auto title =
        window(
            text(" welcome to ") | color(Color::DarkBlue),
            vbox({
                filler(),
                hbox({
                    filler(),
                    text(" <~~ ** U N M A T C H E D ** ~~> ") | bold | color(Color::BlueViolet),
                    filler(),
                }),
                filler(),
            })) |
        size(WIDTH, EQUAL, 35) | size(HEIGHT, EQUAL, 5);

    auto menu = Menu(&entries, &selected, option);

    auto menu_box =
        Renderer(menu, [&]
                 {
    Elements buttons;

   for (size_t i = 0; i < entries.size(); ++i)
    {
        Element button =
            text(entries[i])
            | bold
            | center;

        if (i == selected)
        {
            button = button
                | color(Color::Black)
                | bgcolor(Color::LightSkyBlue3Bis);
        }
        else
        {
            button = button
                | color(Color::NavajoWhite1);
        }

        buttons.push_back(
            window(
                text(""),
                vbox({
                    filler(),
                    button,
                    filler(),
                })
            )
            | size(WIDTH, EQUAL, 15)
            | size(HEIGHT, EQUAL, 3)
            | center
        );
    }

    return window(
        text(" MAIN MENU ") | color(Color::DarkRed),
        vbox(buttons)
    ); });

    auto container =
        Renderer(menu_box, [&]
                 { return vbox({
                              title,
                              filler(),
                              menu_box->Render() | size(WIDTH, EQUAL, 40),
                          }) |
                          center; });

    screen.Loop(container);

    return selected;
}
#include <iostream>
//#include <Winsock2.h> //socket, bind, listen...
#include "GameManager.h"
#include "InputManager.h"
#include "Minesweeper.h"

//Visual studio enters editor mode when left clicking the console
//so the button is not available.
#define MOUSE_L 1

/*
TODO:
    -Online scoreboard.

    -Gfx (OpenGL).
    -Mouse support.
*/

int main()
{
    Console::getInstance().Initialize();
    InputManager::getInstance().Initialize();

    /*TESTS
    bool cont = true;

    while (cont)
    {
        std::cout << GetConsoleWindow() << " : " << GetForegroundWindow() << std::endl;
        std::cin >> cont;
    }
    //TESTS*/

    Game game;

    std::cout << "Enter username: " << std::endl;
    std::cin >> game.username_;
    game.profileManager_.RegisterUser(game.username_);

    while(game.playing_)
    {
        game.StartGame();

#ifndef MOUSE_L
        std::string text = "MODE: CHECK";
        ConsoleButton modeButton(text.size(), 1, game.gameBoard_.boardStartX + game.gameBoard_.GetSizeX() * 4 + 1, game.gameBoard_.boardStartY);
        Console::getInstance().ChangeTextColor(COLORS::GREEN);
        modeButton.UpdateText(text);
        Console::getInstance().ChangeTextColor(COLORS::BLACK);
#endif // MOUSE_L

        //Game loop
        while(true)
        {
            if(InputManager::getInstance().KeyTriggered('W') ||
                    InputManager::getInstance().KeyTriggered(VK_UP))
            {
                if(--InputManager::getInstance().idxY < 0)
                    InputManager::getInstance().idxY = game.gameBoard_.GetSizeY() - 1;
            }

            if(InputManager::getInstance().KeyTriggered('S') ||
                    InputManager::getInstance().KeyTriggered(VK_DOWN))
            {
                if(++InputManager::getInstance().idxY > static_cast<int>(game.gameBoard_.GetSizeY()) - 1)
                    InputManager::getInstance().idxY = 0;
            }

            if(InputManager::getInstance().KeyTriggered('D') ||
                    InputManager::getInstance().KeyTriggered(VK_RIGHT))
            {
                if(++InputManager::getInstance().idxX > static_cast<int>(game.gameBoard_.GetSizeX() - 1))
                    InputManager::getInstance().idxX = 0;
            }

            if(InputManager::getInstance().KeyTriggered('A') ||
                    InputManager::getInstance().KeyTriggered(VK_LEFT))
            {
                if(--InputManager::getInstance().idxX < 0)
                    InputManager::getInstance().idxX = game.gameBoard_.GetSizeX() - 1;
            }

            //game.gameBoard_.UpdateAllBoard();

            if(InputManager::getInstance().KeyTriggered(VK_SPACE))
            {
                game.ended_ = game.gameBoard_.Flag(InputManager::getInstance().idxX, InputManager::getInstance().idxY);

                if(game.ended_)
                {
                    game.EndGame(true);
                    break;
                }
            }

            if(InputManager::getInstance().KeyTriggered(VK_RETURN))
            {
                game.ended_ = game.gameBoard_.CheckTile(InputManager::getInstance().idxX, InputManager::getInstance().idxY);

                if(game.ended_)
                {
                    game.gameBoard_.RevealMines();
                    game.EndGame(false);
                    break;
                }
            }

#ifdef MOUSE_L
            if (InputManager::getInstance().KeyTriggered(MK_LBUTTON))
            {
                float posX = InputManager::getInstance().GetMousePos().x;
                float posY = InputManager::getInstance().GetMousePos().y;
                Console::getInstance().ScreenToConsole(posX, posY);

                bool inside = game.gameBoard_.ConsoleToBoard(posX, posY);

                if (inside)
                {
                    InputManager::getInstance().idxX = posX;
                    InputManager::getInstance().idxY = posY;
                    game.ended_ = game.gameBoard_.CheckTile(posX, posY);

                    if (game.ended_)
                    {
                        game.gameBoard_.RevealMines();
                        game.EndGame(false);
                        break;
                    }
                }
            }

            if (InputManager::getInstance().KeyTriggered(MK_RBUTTON))
            {
                float posX = InputManager::getInstance().GetMousePos().x;
                float posY = InputManager::getInstance().GetMousePos().y;
                Console::getInstance().ScreenToConsole(posX, posY);

                bool inside = game.gameBoard_.ConsoleToBoard(posX, posY);

                if (inside)
                {
                    InputManager::getInstance().idxX = posX;
                    InputManager::getInstance().idxY = posY;
                    game.ended_ = game.gameBoard_.Flag(posX, posY);

                    if (game.ended_)
                    {
                        game.gameBoard_.RevealMines();
                        game.EndGame(false);
                        break;
                    }
                }
            }
#endif // MOUSE_L

#ifndef MOUSE_L
            if (InputManager::getInstance().KeyTriggered('M'))
            {
                game.gameBoard_.ChangeMode();

                if (game.gameBoard_.GetMode())
                {
                    Console::getInstance().ChangeTextColor(COLORS::GREEN);
                    modeButton.UpdateText("MODE: CHECK");
                }
                else
                {
                    Console::getInstance().ChangeTextColor(COLORS::BRIGHT_CYAN);
                    modeButton.UpdateText("MODE: FLAG");
                }
                Console::getInstance().ChangeTextColor(COLORS::BLACK);
            }

            if (InputManager::getInstance().KeyTriggered(MK_RBUTTON))
            {
                float posX = InputManager::getInstance().GetMousePos().x;
                float posY = InputManager::getInstance().GetMousePos().y;
                Console::getInstance().ScreenToConsole(posX, posY);

                if (modeButton.IsClicked(uVec2(posX, posY)))
                {
                    game.gameBoard_.ChangeMode();

                    if (game.gameBoard_.GetMode())
                    {
                        Console::getInstance().ChangeTextColor(COLORS::GREEN);
                        modeButton.UpdateText("MODE: CHECK");
                    }
                    else
                    {
                        Console::getInstance().ChangeTextColor(COLORS::BRIGHT_CYAN);
                        modeButton.UpdateText("MODE: FLAG");
                    }
                    Console::getInstance().ChangeTextColor(COLORS::BLACK);
                }

                bool inside = game.gameBoard_.ConsoleToBoard(posX, posY);

                if (inside)
                {
                    InputManager::getInstance().idxX = posX;
                    InputManager::getInstance().idxY = posY;

                    if(game.gameBoard_.GetMode())
                        game.ended_ = game.gameBoard_.CheckTile(posX, posY);
                    else
                        game.ended_ = game.gameBoard_.Flag(posX, posY);

                    if (game.ended_)
                    {
                        game.gameBoard_.RevealMines();
                        game.EndGame(false);
                        break;
                    }
                }
            }
#endif // MOUSE_L

            if(InputManager::getInstance().KeyTriggered(VK_ESCAPE))
            {
                game.playing_ = false;
                break;
            }
        }
    }
    return 0;
}

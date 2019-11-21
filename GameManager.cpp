#define _WIN32_WINNT   0x0601 //Define windows version to 7 (Needed in order to use GetConsoleWindow)
#include <windows.h>
#include <iostream>
#include <algorithm> //find
#include <fstream> //fstream (fileio)
#include <string>
#include <sstream> //getline
#include <iomanip> //setprecision, std::fixed
#include <limits> //numeric_limits
#include "InputManager.h"
#include "GameManager.h"

void Console::Initialize()
{
    //Change and move the console window
    HWND consoleHWND = GetConsoleWindow();
    handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD consoleSizeCoords = { WINDOW_WIDTH, WINDOW_HEIGHT };
    SetConsoleScreenBufferSize(handle, consoleSizeCoords);
    MoveWindow(consoleHWND, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, TRUE);
    ShowWindow(consoleHWND, SW_SHOWMAXIMIZED);

    //Save rows and column info
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(handle, &csbi);
    charColumns = csbi.srWindow.Right - csbi.srWindow.Left;
    charRows = csbi.srWindow.Bottom - csbi.srWindow.Top;

    Console::ChangeBackgroundColor(COLORS::GREY);
    Console::ChangeTextColor(COLORS::BLACK);
    Console::Clear();
}

void Console::GoToXY(int x, int y)
{
    COORD coords;
    coords.X = x;
    coords.Y = y;

    if (!SetConsoleCursorPosition(handle, coords))
    {
        std::cout << "Console::GoToXY failed.";
    }
}

COORD Console::GetCursorPos()
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(handle, &info);
    return info.dwCursorPosition;
}

void Console::Clear()
{
    GoToXY(0, 0);

    for (int y = 0; y <= charRows + 1; ++y)
    {
        for (int x = 0; x <= charColumns; ++x)
        {
            std::cout << ' ';
        }
        std::cout << std::endl;
    }

    GoToXY(0, 0);
}

void Console::ChangeBackgroundColor(int color)
{
    backgroundColor = 16 * color;
    SetConsoleTextAttribute(Console::handle, backgroundColor);
}

void Console::ChangeTextColor(int color)
{
    SetConsoleTextAttribute(Console::handle, backgroundColor + color);
}

void Console::ScreenToConsole(float& x, float& y)
{
    x /= (WINDOW_WIDTH / charColumns); //8: WINDOW_WIDTH / charColumns
    y /= (WINDOW_HEIGHT / charRows); //16: WINDOW_HEIGHT / charRows

    //A bit of centering
    --x;
    y -= 2;

    x = floor(x);
    y = floor(y);
}

void Game::StartGame()
{
    ended_ = false;

    std::string difficultyS = "--Select difficulty--";
    Console::getInstance().GoToXY((Console::getInstance().charColumns - difficultyS.size()) / 2, Console::getInstance().charRows / 2 - 8);
    std::cout << difficultyS;

    std::string text0 = "  Beginner  ";
    BoxButton begginerB(text0.size(), 1, (Console::getInstance().charColumns - text0.size()) / 2, Console::getInstance().charRows / 2 - 4);
    Console::getInstance().ChangeTextColor(COLORS::GREEN);
    begginerB.UpdateText(text0);

    std::string text1 = "  Intermediate  ";
    BoxButton intermidiateB(text1.size(), 1, (Console::getInstance().charColumns - text1.size()) / 2, Console::getInstance().charRows / 2);
    Console::getInstance().ChangeTextColor(COLORS::BLUE);
    intermidiateB.UpdateText(text1);

    std::string text2 = "  Expert  ";
    BoxButton expertB(text2.size(), 1, (Console::getInstance().charColumns - text2.size()) / 2, Console::getInstance().charRows / 2 + 4);
    Console::getInstance().ChangeTextColor(COLORS::RED);
    expertB.UpdateText(text2);

    std::string text3 = "  Custom  ";
    BoxButton customB(text3.size(), 1, (Console::getInstance().charColumns - text3.size()) / 2, Console::getInstance().charRows / 2 + 8);
    Console::getInstance().ChangeTextColor(COLORS::PURPLE);
    customB.UpdateText(text3);

    Console::getInstance().ChangeTextColor(COLORS::BLACK);

    unsigned int difficulty;
    while (true)
    {
        if (InputManager::getInstance().KeyTriggered(MK_LBUTTON) || InputManager::getInstance().KeyTriggered(MK_RBUTTON))
        {
            float posX = InputManager::getInstance().GetMousePos().x;
            float posY = InputManager::getInstance().GetMousePos().y;
            Console::getInstance().ScreenToConsole(posX, posY);

            if (begginerB.IsClicked(uVec2(posX, posY)))
            {
                difficulty = 0;
                break;
            }
            if (intermidiateB.IsClicked(uVec2(posX, posY)))
            {
                difficulty = 1;
                break;
            }
            if (expertB.IsClicked(uVec2(posX, posY)))
            {
                difficulty = 2;
                break;
            }
            if (customB.IsClicked(uVec2(posX, posY)))
            {
                difficulty = 3;
                break;
            }
        }
    }

    if(difficulty == 0)
        gameBoard_ = Board(9, 9, 10);
    else if(difficulty == 1)
        gameBoard_ = Board(16, 16, 40);
    else if(difficulty == 2)
        gameBoard_ = Board(); //Default constructor is set to max difficulty
    else if(difficulty == 3)
    {
        Console::getInstance().Clear();
        int sizeX = 0;
        int sizeY = 0;
        int mines = 0;

        std::string xS = "Size X: ";
        HSlider sliderX(56, uVec2((Console::getInstance().charColumns - 56) / 2, Console::getInstance().charRows / 2 - 4), 0, 56, xS);
        std::string yS = "Size Y: ";
        HSlider sliderY(32, uVec2((Console::getInstance().charColumns - 32) / 2, Console::getInstance().charRows / 2), 0, 32, yS);
        std::string mS = "Mines: ";
        HSlider sliderM(99, uVec2((Console::getInstance().charColumns - 100) / 2, Console::getInstance().charRows / 2 + 4), 0, 500, mS);

        std::string okS = "  OK  ";
        BoxButton okB(okS.size(), 1, (Console::getInstance().charColumns - okS.size()) / 2, Console::getInstance().charRows / 2 + 10);
        Console::getInstance().ChangeTextColor(COLORS::GREEN);
        okB.UpdateText(okS);
        Console::getInstance().ChangeTextColor(COLORS::BLACK);

        while(true)
        {
            float posX = InputManager::getInstance().GetMousePos().x;
            float posY = InputManager::getInstance().GetMousePos().y;
            Console::getInstance().ScreenToConsole(posX, posY);

            if(InputManager::getInstance().KeyPressed(MK_LBUTTON))
            {
                if(sliderX.IsClicked(uVec2(posX, posY)))
                    sizeX = static_cast<int>(sliderX.GetValue());

                if(sliderY.IsClicked(uVec2(posX, posY)))
                    sizeY = static_cast<int>(sliderY.GetValue());

                if(sliderM.IsClicked(uVec2(posX, posY)))
                    mines = static_cast<int>(sliderM.GetValue());

                if(okB.IsClicked(uVec2(posX, posY)))
                    break;
            }
        }

        /*
        std::cout << "Enter board columns: \n";// << std::endl;
        sizeX = InputManager::getInstance().ReadUnsignedNumber();
        std::cout << "Enter board rows: \n";// << std::endl;
        sizeY = InputManager::getInstance().ReadUnsignedNumber();
        std::cout << "Enter board mines: \n";// << std::endl;
        mines = InputManager::getInstance().ReadUnsignedNumber();
        */

        if(sizeX == 0 && sizeY == 0 && mines == 0)
            gameBoard_ = Board(56, 32, 400); //Giant
        else
        {
            //If the mines are more than the tiles available, clamp to max
            if(mines >= sizeX * sizeY)
                mines = sizeX * sizeY - 1;

            if(sizeX <= 0)
                sizeX = 56;
            if(sizeY <= 0)
                sizeY = 32;
            if (mines <= 0)
                mines = 1;

            gameBoard_ = Board(sizeX, sizeY, mines);
        }
    }

    gameBoard_.CreateBoard();
    gameBoard_.PrintBoard();
    timer_.Start();
}

void Game::EndGame(bool win)
{
    double time = timer_.Stop();
    gameBoard_.UpdateAllBoard();
    Console::getInstance().GoToXY((Console::getInstance().charColumns - gameBoard_.GetSizeX() * 2) / 2,
                    gameBoard_.boardStartY + gameBoard_.GetSizeY() * 2 + 1);

    if(win)
    {
        std::cout << "You win.";// << std::endl;
        Console::getInstance().GoToXY((Console::getInstance().charColumns - gameBoard_.GetSizeX() * 2) / 2, Console::getInstance().GetCursorPos().Y + 1);
        profileManager_.SaveTime(gameBoard_.GenerateFilename(), username_, time);

        std::cout << std::setprecision(2) << std::fixed; //Set precision to only show 2 decimals
        std::cout << "Time: " << time << "s";// << std::endl;
        Console::getInstance().GoToXY((Console::getInstance().charColumns - gameBoard_.GetSizeX() * 2) / 2, Console::getInstance().GetCursorPos().Y + 1);

        double bestTime = 0;
        std::string bestUser = profileManager_.GetBestTime(gameBoard_.GenerateFilename(), bestTime);
        std::cout << "Best time: ";// << std::endl;
        Console::getInstance().GoToXY((Console::getInstance().charColumns - gameBoard_.GetSizeX() * 2) / 2, Console::getInstance().GetCursorPos().Y + 1);

        if(!bestUser.empty())
        {
            std::cout << '\t' << "User: " << bestUser;// << std::endl;
            Console::getInstance().GoToXY((Console::getInstance().charColumns - gameBoard_.GetSizeX() * 2) / 2, Console::getInstance().GetCursorPos().Y + 1);
            std::cout << '\t' << "Time: " << bestTime << "s";// << std::endl;
            Console::getInstance().GoToXY((Console::getInstance().charColumns - gameBoard_.GetSizeX() * 2) / 2, Console::getInstance().GetCursorPos().Y + 1);
        }
        else
        {
            std::cout << '\t' << "New record! ";// << std::endl;
            Console::getInstance().GoToXY((Console::getInstance().charColumns - gameBoard_.GetSizeX() * 2) / 2, Console::getInstance().GetCursorPos().Y + 1);
            std::cout << '\t' << "User: " << username_;// << std::endl;
            Console::getInstance().GoToXY((Console::getInstance().charColumns - gameBoard_.GetSizeX() * 2) / 2, Console::getInstance().GetCursorPos().Y + 1);
            std::cout << '\t' << "Time: " << time << "s";// << std::endl;
            Console::getInstance().GoToXY((Console::getInstance().charColumns - gameBoard_.GetSizeX() * 2) / 2, Console::getInstance().GetCursorPos().Y + 1);
        }

        //Set out stream back to default
        std::cout.unsetf(std::ios_base::fixed);
        std::cout << std::setprecision(6);
    }
    else
    {
        std::cout << "Game over.";// << std::endl;
        Console::getInstance().GoToXY((Console::getInstance().charColumns - gameBoard_.GetSizeX() * 2) / 2, Console::getInstance().GetCursorPos().Y + 1);
        std::cout << "Mines remaining: " << gameBoard_.GetMines() - gameBoard_.GetFoundMines() << "/" << gameBoard_.GetMines();// << std::endl;
        Console::getInstance().GoToXY((Console::getInstance().charColumns - gameBoard_.GetSizeX() * 2) / 2, Console::getInstance().GetCursorPos().Y + 1);
    }

    std::string retryS = "  RETRY  ";
    BoxButton retryB(retryS.size(), 1, Console::getInstance().charColumns / 2 - gameBoard_.GetSizeX() * 2 + 4, gameBoard_.boardStartY + gameBoard_.GetSizeY() * 2 + 1);
    Console::getInstance().ChangeTextColor(COLORS::GREEN);
    retryB.UpdateText(retryS);

    std::string exitS = "  EXIT  ";
    BoxButton exitB(exitS.size(), 1, Console::getInstance().charColumns / 2 + gameBoard_.GetSizeX() * 2 - exitS.size() + 3, gameBoard_.boardStartY + gameBoard_.GetSizeY() * 2 + 1);
    Console::getInstance().ChangeTextColor(COLORS::RED);
    exitB.UpdateText(exitS);
    Console::getInstance().ChangeTextColor(COLORS::BLACK);

    while (true)
    {
        if (InputManager::getInstance().KeyTriggered(MK_LBUTTON))
        {
            float posX = InputManager::getInstance().GetMousePos().x;
            float posY = InputManager::getInstance().GetMousePos().y;
            Console::getInstance().ScreenToConsole(posX, posY);

            if (retryB.IsClicked(uVec2(posX, posY)))
            {
                InputManager::getInstance().idxX = 0;
                InputManager::getInstance().idxY = 0;
                Console::getInstance().Clear();
                break;
            }
            if (exitB.IsClicked(uVec2(posX, posY)))
            {
                playing_ = false;
                Console::getInstance().Clear();
                break;
            }
        }
    }

    /*
    std::cout << "Press SPACE for a new game, ESC to exit" << std::endl;
    while (true)
    {
        if (InputManager::getInstance().KeyTriggered(VK_ESCAPE))
        {
            playing_ = false;
            Console::getInstance().Clear();
            break;
        }
        if (InputManager::getInstance().KeyTriggered(VK_SPACE))
        {
            InputManager::getInstance().idxX = 0;
            InputManager::getInstance().idxY = 0;
            Console::getInstance().Clear();
            break;
        }
    }
    */
}

void ProfileManager::RegisterUser(std::string username)
{
    if(!(std::find(users_.begin(), users_.end(), username) != users_.end()))
        users_.push_back(username);
    else
        std::cout << "User already registered" << std::endl;
}

void ProfileManager::SaveTime(std::string filename, std::string user, double time)
{
    std::fstream fs;
    std::string dir = "./scores/";
    dir.append(filename);

    fs.open(dir.c_str(), std::fstream::app);

    if(fs)
    {
        fs << user << std::endl;
        fs << time << std::endl;
    }
    else
        std::cout << "Could not open file" << std::endl;

    fs.close();
}

std::string ProfileManager::GetBestTime(std::string filename, double& time)
{
    std::string bestUser, tempUser;
    double bestTime, tempTime;

    std::fstream fs;

    std::string dir = "./scores/";
    dir.append(filename);

    fs.open(dir.c_str(), std::fstream::in);

    if(fs)
    {
        fs >> bestUser;
        fs >> bestTime;

        while(!fs.eof())
        {
            fs >> tempUser;
            fs >> tempTime;

            if(tempTime < bestTime)
            {
                bestTime = tempTime;
                bestUser = tempUser;
            }
        }
    }
    else
        std::cout << "Could not open file" << std::endl;

    fs.close();

    time = bestTime;
    return bestUser;
}

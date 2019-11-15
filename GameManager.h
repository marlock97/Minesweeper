#pragma once
#include <windows.h>
#include <fstream> //fstream (fileio)
#include <vector>
#include <map>
#include "Minesweeper.h"
#include "Timer.h"

enum COLORS {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    PURPLE,
    DARK_YELLOW,
    WHITE,
    GREY,
    BRIGHT_BLUE,
    BRIGHT_GREEN,
    BRIGHT_CYAN,
    BRIGHT_RED,
    PINK,
    YELLOW,
    BRIGHT_WHITE
};

class Console{
public:
    //SINGLETON PATTERN
    static Console& getInstance()
    {
        static Console instance; // Instantiated on first use.
        return instance;
    }
    Console(Console const&) = delete;
    void operator=(Console const&) = delete;
    //SINGLETON PATTERN

    void Initialize();
    void GoToXY(int x, int y);
    COORD GetCursorPos();
    void Clear();
    void ChangeBackgroundColor(int color);
    void ChangeTextColor(int color);
    void ScreenToConsole(float& x, float& y);

//private:
    HANDLE handle;
    int backgroundColor;
    int charColumns;
    int charRows;
private:
    //SINGLETON PATTERN
    Console() {}
    //SINGLETON PATTERN
};

//Should probably be a class
class ProfileManager
{
public:
    void RegisterUser(std::string username);
    void SaveTime(std::string filename, std::string user, double time);
    std::string GetBestTime(std::string filename, double& time);

private:
    std::vector<std::string> users_;
    std::vector<std::string> fileNames_;
};

class Game
{
public:
    void StartGame(); //Ask for board specs, create board, print board, start timer
    void EndGame(bool win); //Stop timer, print win/lose message & time, new game?

    bool playing_ = true;
    bool ended_ = false;
    Board gameBoard_;
    Timer timer_;
    ProfileManager profileManager_;
    std::string username_;
};

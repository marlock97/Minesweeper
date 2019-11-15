#pragma once
#include <vector>
#include <iostream>
#include <windows.h>
#include "math.h"

#define INPUT_KEY_NUMBER 256

enum VK_KEYS {
    ANY = -1,
    MOUSELEFT = 0,
    MOUSERIGHT = 1,
    MOUSEMIDDLE = 2,
};

enum VK_NUMBERS {
    ZERO = 0x30,
    ONE = 0x31,
    TWO = 0x32,
    THREE = 0x33,
    FOUR = 0x34,
    FIVE = 0x35,
    SIX = 0x36,
    SEVEN = 0x37,
    EIGHT = 0x38,
    NINE = 0x39
};

class ConsoleButton
{
public:
    ConsoleButton(int sizeX, int sizeY, int posX, int posY);
    ConsoleButton(uVec2 bSize, uVec2 pos);
    bool IsClicked(uVec2 mousePos);
    void UpdateText(std::string newText);

private:
    uVec2 size_;
    uVec2 position_;
    std::string text_;
};

class InputManager
{
public:
    //SINGLETON PATTERN
    static InputManager& getInstance()
    {
        static InputManager instance; // Instantiated on first use.
        return instance;
    }
    InputManager(InputManager const&) = delete;
    void operator=(InputManager const&) = delete;
    //SINGLETON PATTERN

    void Initialize();
    bool WindowIsActive();
    bool KeyTriggered(int key);
    bool KeyPressed(int key);
    POINT GetMousePos() const;
    unsigned int ReadUnsignedNumber();

    int idxX = 0;
    int idxY = 0;

private:
    //SINGLETON PATTERN
    InputManager() {}
    //SINGLETON PATTERN

    std::vector<bool> currState = { false };
    std::vector<bool> prevState = { false };
};


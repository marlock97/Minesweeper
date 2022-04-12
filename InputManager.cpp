#define _WIN32_WINNT   0x0601 //Define windows version to 7 (Needed in order to use GetConsoleWindow)
//#include <windows.h>
#include <string>
#include "GameManager.h"
#include "InputManager.h"

ConsoleButton::ConsoleButton(int sizeX, int sizeY, int posX, int posY) : size_(uVec2(sizeX, sizeY)), position_(uVec2(posX, posY))
{
}

ConsoleButton::ConsoleButton(uVec2 bSize, uVec2 pos) : size_(bSize), position_(pos)
{
}

bool ConsoleButton::IsClicked(uVec2 mousePos)
{
    if (mousePos.x >= position_.x && mousePos.x <= position_.x + size_.x &&
        mousePos.y >= position_.y && mousePos.y <= position_.y + size_.y)
        return true;
    else
        return false;
}

void ConsoleButton::UpdateText(std::string newText)
{
    text_ = newText;

    Console::getInstance().GoToXY(position_.x, position_.y);
    for (unsigned int i = 0; i < size_.x; ++i)
        std::cout << ' ';
    Console::getInstance().GoToXY(position_.x, position_.y);

    std::cout << text_;
}

BoxButton::BoxButton(int sizeX, int sizeY, int posX, int posY) : ConsoleButton(sizeX, sizeY, posX, posY)
{
}

void BoxButton::UpdateText(std::string newText)
{
    text_ = newText;

    Console::getInstance().GoToXY(position_.x - 1, position_.y - 1);
    for (unsigned int i = 0; i < size_.x + 2; ++i)
        std::cout << '-';
    Console::getInstance().GoToXY(position_.x - 1, position_.y);
    std::cout << '|';
    for (unsigned int i = 0; i < size_.x; ++i)
        std::cout << ' ';
    Console::getInstance().GoToXY(position_.x, position_.y);
    std::cout << text_;
    std::cout << '|';
    Console::getInstance().GoToXY(position_.x - 1, position_.y + 1);
    for (unsigned int i = 0; i < size_.x + 2; ++i)
        std::cout << '-';
}

HSlider::HSlider(int sizeX, uVec2 position, int minVal, int maxVal, std::string legend) : sizeX_(sizeX), position_(position), min_(minVal), max_(maxVal), legend_(legend)
{
    currX = sizeX_ / 2;

    Console::getInstance().GoToXY(position_.x - 1, position_.y - 1);
    for (unsigned int i = 0; i < sizeX_ + 2; ++i)
        std::cout << '-';
    Console::getInstance().GoToXY(position_.x - 1, position_.y);
    std::cout << '|';
    for (unsigned int i = 0; i < sizeX_; ++i)
        std::cout << '-';
    std::cout << '|';
    Console::getInstance().GoToXY(position_.x - 1, position_.y + 1);
    for (unsigned int i = 0; i < sizeX_ + 2; ++i)
        std::cout << '-';

    Console::getInstance().GoToXY(position_.x + currX, position_.y);
    std::cout << '\xDB';

    Console::getInstance().GoToXY(position_.x + (sizeX_ - legend_.size()) / 2, position_.y - 2);
    std::cout << legend_;

    int val = static_cast<int>(GetValue());
    Console::getInstance().GoToXY(position_.x + (sizeX_ + legend_.size()) / 2, position_.y - 2);
    std::cout << val;
}

bool HSlider::IsClicked(uVec2 mousePos)
{
    if (mousePos.x >= position_.x && mousePos.x < position_.x + sizeX_ &&
        mousePos.y >= position_.y && mousePos.y <= position_.y )
    {
        Console::getInstance().GoToXY(position_.x + currX, position_.y);
        std::cout << "-";
        currX = mousePos.x - position_.x;
        Console::getInstance().GoToXY(mousePos.x, position_.y);
        std::cout << '\xDB';

        Console::getInstance().GoToXY(position_.x + (sizeX_ + legend_.size()) / 2, position_.y - 2);
        std::cout << "        ";
        Console::getInstance().GoToXY(position_.x + (sizeX_ + legend_.size()) / 2, position_.y - 2);
        std::cout << static_cast<int>(GetValue());

        return true;
    }
    else
        return false;
}

float HSlider::GetValue()
{
    float val = min_ + max_ * ((currX + 1) / sizeX_);
    return val;
}

void InputManager::Initialize()
{
    currState.resize(INPUT_KEY_NUMBER);
    prevState.resize(INPUT_KEY_NUMBER);
}

bool InputManager::WindowIsActive()
{
    if(GetConsoleWindow() == GetForegroundWindow())
        return true;
    return false;
}

bool InputManager::KeyTriggered(int key)
{
    if(!WindowIsActive())
        return false;

    SHORT state = GetAsyncKeyState(key);
    currState[key] = state < 0;

    if(currState[key] && currState[key] != prevState[key])
    {
        prevState[key] = currState[key];
        return true;
    }

    prevState[key] = currState[key];
    return false;
}


bool InputManager::KeyPressed(int key)
{
    if(!WindowIsActive())
        return false;

    if(GetAsyncKeyState(key) < 0)
        return true;
    return false;
}

POINT InputManager::GetMousePos() const
{
    POINT pos;
    GetCursorPos(&pos);
    return pos;
}

unsigned int InputManager::ReadUnsignedNumber()
{
    std::string number;
    while (true)
    {
        if (KeyTriggered(VK_NUMBERS::ZERO))
        {
            std::cout << 0;
            number.push_back('0');
        }
        else if (KeyTriggered(VK_NUMBERS::ONE))
        {
            std::cout << 1;
            number.push_back('1');
        }
        else if (KeyTriggered(VK_NUMBERS::TWO))
        {
            std::cout << 2;
            number.push_back('2');
        }
        else if (KeyTriggered(VK_NUMBERS::THREE))
        {
            std::cout << 3;
            number.push_back('3');
        }
        else if (KeyTriggered(VK_NUMBERS::FOUR))
        {
            std::cout << 4;
            number.push_back('4');
        }
        else if (KeyTriggered(VK_NUMBERS::FIVE))
        {
            std::cout << 5;
            number.push_back('5');
        }
        else if (KeyTriggered(VK_NUMBERS::SIX))
        {
            std::cout << 6;
            number.push_back('6');
        }
        else if (KeyTriggered(VK_NUMBERS::SEVEN))
        {
            std::cout << 7;
            number.push_back('7');
        }
        else if (KeyTriggered(VK_NUMBERS::EIGHT))
        {
            std::cout << 8;
            number.push_back('8');
        }
        else if (KeyTriggered(VK_NUMBERS::NINE))
        {
            std::cout << 9;
            number.push_back('9');
        }
        else if (KeyTriggered(VK_BACK))
        {
            if (!number.empty())
            {
                COORD pos = Console::getInstance().GetCursorPos();
                Console::getInstance().GoToXY(pos.X - 1, pos.Y);
                std::cout << ' ';
                Console::getInstance().GoToXY(pos.X - 1, pos.Y);
                number.pop_back();
            }
        }
        else if (KeyTriggered(VK_RETURN))
        {
            std::cout << std::endl;
            break;
        }
    }

    return std::stoi(number);
}

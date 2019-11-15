#include <iostream> //cout, cin
#include <stdlib.h> //srand, rand
#include <time.h> //time
#include <string>
#include "InputManager.h" //idxX, idxY
#include "GameManager.h" //Console
#include "Minesweeper.h"

//Initializes checked flag of all tiles, 1 to render
#define RENDER_REAL_VALUES 0
#undef DRAW_BOARD_LEGENDS
#define PRINT_IDX_POS 0
#undef CHECK_SURROUNDING

#define MINE 15 //X=88 Cog=15
#define FLAG 159 //F=70 CoolF=159

//NEW_RENDER/OLD_RENDER
#define NEW_RENDER

//Board constructor
Board::Board(unsigned int columns, unsigned int rows, unsigned int mines) : sizeX_(columns), sizeY_(rows), mines_(mines)
{
    //Initialize rand seed with current time
    srand(static_cast<int>(time(0)));

    minesFound_ = 0;
}

//Transforms x, y coordinates to an index in a matrix
unsigned int Board::TransformToIndex(unsigned int column, unsigned int row) const
{
    if (column > sizeX_ || row > sizeY_)
        return 0;

    return column + sizeX_ * row;
}

//Fills the numbers around a mine
void Board::FillMineIndicators(int c, int r)
{
    //Loop around the surrounding tiles
    for(int x = -1; x < 2; ++x)
    {
        for(int y = -1; y < 2; ++y)
        {
            if(y == 0 && x == 0)
                continue;
            else if(r + y >= 0 && r + y < static_cast<int>(sizeY_) &&
                    c + x >= 0 && c + x < static_cast<int>(sizeX_)) //Corner cases
            {
                if(board_[TransformToIndex(c + x, r + y)].data != MINE)
                    board_[TransformToIndex(c + x, r + y)].data += 1;
            }
        }
    }
}

//Creates the board and it fills the mines
void Board::CreateBoard()
{
    //board_ = new Tile[sizeY_ * sizeX_];
    board_.resize(sizeY_ * sizeX_);
    //minePositions_ = new unsigned int[mines_];
    minePositions_.resize(mines_);

    //Initialize tiles
    for(unsigned int c = 0; c < sizeX_; ++c)
    {
        for(unsigned int r = 0; r < sizeY_; ++r)
        {
            board_[TransformToIndex(c, r)].data = '0';
            //Set to false so that the real values don't render
            board_[TransformToIndex(c, r)].checked = RENDER_REAL_VALUES;
            board_[TransformToIndex(c, r)].flag = false;
        }
    }

    unsigned int i = 0;
    while(i < mines_)
    {
        int x = rand()/((RAND_MAX + 1u)/(sizeX_ - 1));
        int y = rand()/((RAND_MAX + 1u)/(sizeY_ - 1));

        unsigned int idx = TransformToIndex(x, y);

        if(board_[idx].data != MINE)
        {
            board_[idx].data = MINE;
            minePositions_[i] = idx;
            FillMineIndicators(x, y);
            ++i;
        }
    }
}

void Board::TestBoard()
{
    minesFound_ = 0;

    sizeX_ = 5;
    sizeY_ = 5;
    mines_ = 16;

    board_.resize(sizeX_ * sizeY_);
    //minePositions_ = new unsigned int[mines_];
    minePositions_.resize(mines_);

    for(unsigned int c = 0; c < sizeX_; ++c)
    {
        for(unsigned int r = 0; r < sizeY_; ++r)
        {
            //Set to false so that the real values don't render
            board_[TransformToIndex(c, r)].checked = RENDER_REAL_VALUES;
            board_[TransformToIndex(c, r)].flag = false;
        }
    }

    board_[TransformToIndex(0, 0)].data = '0';
    board_[TransformToIndex(1, 0)].data = '2';
    board_[TransformToIndex(2, 0)].data = MINE;
    board_[TransformToIndex(3, 0)].data = MINE;
    board_[TransformToIndex(4, 0)].data = MINE;

    board_[TransformToIndex(0, 1)].data = '1';
    board_[TransformToIndex(1, 1)].data = '4';
    board_[TransformToIndex(2, 1)].data = MINE;
    board_[TransformToIndex(3, 1)].data = '8';
    board_[TransformToIndex(4, 1)].data = MINE;

    board_[TransformToIndex(0, 2)].data = MINE;
    board_[TransformToIndex(1, 2)].data = '5';
    board_[TransformToIndex(2, 2)].data = MINE;
    board_[TransformToIndex(3, 2)].data = MINE;
    board_[TransformToIndex(4, 2)].data = MINE;

    board_[TransformToIndex(0, 3)].data = MINE;
    board_[TransformToIndex(1, 3)].data = '6';
    board_[TransformToIndex(2, 3)].data = MINE;
    board_[TransformToIndex(3, 3)].data = '7';
    board_[TransformToIndex(4, 3)].data = MINE;

    board_[TransformToIndex(0, 4)].data = MINE;
    board_[TransformToIndex(1, 4)].data = MINE;
    board_[TransformToIndex(2, 4)].data = '3';
    board_[TransformToIndex(3, 4)].data = MINE;
    board_[TransformToIndex(4, 4)].data = MINE;
}

//When checking a tile that has a no mines around,
//surrounding tiles with numbers are also revealed.
//If a 0 is found in one of those tiles, the function is called recursively.
void Board::Explore0(int c, int r)
{
    //Loop around the surrounding tiles
    for(int x = -1; x < 2; ++x)
    {
        for(int y = -1; y < 2; ++y)
        {
            if(y == 0 && x == 0)
                continue;
            else if(r + y >= 0 && r + y < static_cast<int>(sizeY_) &&
                    c + x >= 0 && c + x < static_cast<int>(sizeX_)) //Corner cases
            {
                if(board_[TransformToIndex(c + x, r + y)].data == '0' &&
                        !board_[TransformToIndex(c + x, r + y)].checked)
                {
                    board_[TransformToIndex(c + x, r + y)].checked = true;
                    UpdateValue(x + c, y + r);
                    Explore0(c + x, r + y);
                }
                else if(board_[TransformToIndex(c + x, r + y)].data != MINE &&
                        !board_[TransformToIndex(c + x, r + y)].checked)
                {
                    board_[TransformToIndex(c + x, r + y)].checked = true;
                    UpdateValue(x + c, y + r);
                }
            }
        }
    }
}

//Checks and reveals a tile. Revealing a mine ends the game. Tiles with flags cannot be revealed.
bool Board::CheckTile(unsigned int x, unsigned int y)
{
    if(board_[TransformToIndex(x, y)].flag)
        return false;

    //Return true if the game has ended
    if(board_[TransformToIndex(x, y)].data == MINE)
    {
        board_[TransformToIndex(x, y)].checked = true;
        UpdateValue(x, y);
        return true;
    }
    else if(board_[TransformToIndex(x, y)].data == '0')
        Explore0(x, y); //Check surrounding tiles
    else if(board_[TransformToIndex(x, y)].checked)
    {
#ifdef CHECK_SURROUNDING
        //Loop around the surrounding tiles
        for(int c = -1; c < 2; ++c)
        {
            for(int r = -1; r < 2; ++r)
            {
                if(r == 0 && c == 0)
                    continue;
                else if(r + static_cast<int>(y) >= 0 && r + static_cast<int>(y) < static_cast<int>(sizeY_) &&
                        c + static_cast<int>(x) >= 0 && c + static_cast<int>(x) < static_cast<int>(sizeX_)) //Corner cases
                {
                    if(board_[TransformToIndex(x + c, y + r)].flag)
                        continue;

                    if(board_[TransformToIndex(x + c, y + r)].data == '0')
                        Explore0(x + c, y + r);

                    board_[TransformToIndex(x + c, y + r)].checked = true;
                    UpdateValue(x + c, y + r);

                    if(board_[TransformToIndex(x + c, y + r)].data == MINE)
                        return true;
                }
            }
        }
#endif // CHECK_SURROUNDING
    }

    board_[TransformToIndex(x, y)].checked = true;
    UpdateValue(x, y);

    return false;
}

//Places a flag in the specified tile.
bool Board::Flag(unsigned int x, unsigned int y)
{
    if(board_[TransformToIndex(x, y)].data == MINE)
    {
        if(!board_[TransformToIndex(x, y)].flag)
            ++minesFound_;
        else
            --minesFound_;
    }

    //Put a flag is there is none, remove it if there is
    board_[TransformToIndex(x, y)].flag = !board_[TransformToIndex(x, y)].flag;
    UpdateValue(x, y);

    //Return true if the game has ended
    if(minesFound_ == mines_)
        return true;

    return false;
}

//Reveals all non flagged mines
void Board::RevealMines()
{
    for(unsigned int i = 0; i < mines_; ++i)
    {
        if(!board_[minePositions_[i]].flag)
            board_[minePositions_[i]].checked = true;
    }
}

bool Board::ConsoleToBoard(float& x, float& y)
{
    int cX = static_cast<int>(x);
    int cY = static_cast<int>(y);

    int trueStartX = boardStartX - 2;
    int trueStartY = boardStartY - 1;
    int trueSizeX = sizeX_ * 4;
    int trueSizeY = sizeY_ * 2;

    if (cX <= trueStartX || cY <= trueStartY || cX >= trueStartX + trueSizeX || cY >= trueStartY + trueSizeY)
        return false;

    x = static_cast<float>((cX - trueStartX) / 4);
    y = static_cast<float>((cY - trueStartY) / 2);

    return true;
}

std::string Board::GenerateFilename() const
{
    std::string filename, tmp;
    filename.append("x");
    filename.append(std::to_string(sizeX_));
    filename.append("y");
    filename.append(std::to_string(sizeY_));
    filename.append("m");
    filename.append(std::to_string(mines_));
    filename.append(".txt");
    return filename;
}

void Board::UpdateValue(int x, int y)
{
    Console::getInstance().GoToXY(boardStartX + x * 4, boardStartY + y * 2);

    if (InputManager::getInstance().idxX == x && InputManager::getInstance().idxY == y && PRINT_IDX_POS)
        Console::getInstance().ChangeBackgroundColor(COLORS::BLUE);

    if(board_[TransformToIndex(x, y)].checked)
    {
        if(board_[TransformToIndex(x, y)].data == '0')
        {
            Console::getInstance().ChangeTextColor(COLORS::WHITE);
        }
        else if(board_[TransformToIndex(x, y)].data == '1')
        {
            Console::getInstance().ChangeTextColor(COLORS::BRIGHT_BLUE);
        }
        else if(board_[TransformToIndex(x, y)].data == '2')
        {
            Console::getInstance().ChangeTextColor(COLORS::GREEN);
        }
        else if(board_[TransformToIndex(x, y)].data == '3')
        {
            Console::getInstance().ChangeTextColor(COLORS::BRIGHT_RED);
        }
        else if(board_[TransformToIndex(x, y)].data == '4')
        {
            Console::getInstance().ChangeTextColor(COLORS::BLUE);
        }
        else if(board_[TransformToIndex(x, y)].data == '5')
        {
            Console::getInstance().ChangeTextColor(COLORS::RED);
        }
        else if(board_[TransformToIndex(x, y)].data == '6')
        {
            Console::getInstance().ChangeTextColor(COLORS::CYAN);
        }
        else if(board_[TransformToIndex(x, y)].data == '7')
        {
            Console::getInstance().ChangeTextColor(COLORS::BLACK);
        }
        else if(board_[TransformToIndex(x, y)].data == '8')
        {
            Console::getInstance().ChangeTextColor(COLORS::PURPLE);
        }
        else if(board_[TransformToIndex(x, y)].data == MINE)
        {
            Console::getInstance().ChangeTextColor(COLORS::BRIGHT_RED);
        }

        std::cout << board_[TransformToIndex(x, y)].data;
    }
    else if(board_[TransformToIndex(x, y)].flag)
    {
        Console::getInstance().ChangeTextColor(COLORS::BRIGHT_CYAN);
        std::cout << static_cast<char>(FLAG);
    }
    else
    {
        std::cout << ' ';
    }

    //Set color back to default
    Console::getInstance().ChangeBackgroundColor(COLORS::GREY);
    Console::getInstance().ChangeTextColor(COLORS::BLACK);
}

void Board::UpdateAllBoard()
{
    for(unsigned int y = 0; y < sizeY_; ++y)
        for(unsigned int x = 0; x < sizeX_; ++x)
            UpdateValue(x, y);
}

//Prints the board.
#ifdef NEW_RENDER
void Board::PrintBoard()
{
#ifdef DRAW_BOARD_LEGENDS
    std::cout.width(4);
    for(unsigned int i = 0; i < sizeX_; ++i)
    {
        std::cout << "|";
        std::cout.width(3);
        std::cout << i;
    }
    std::cout << "|" << std::endl;
#endif //DRAW_BOARD_LEGENDS

    std::string tmpLine((4 * sizeX_) + 1, '-');

    for(unsigned int r = 0; r < sizeY_; ++r)
    {
#ifdef DRAW_BOARD_LEGENDS
        std::cout.width(4 * sizeX_ + 4);
#endif //DRAW_BOARD_LEGENDS

        std::cout << tmpLine << std::endl;

#ifdef DRAW_BOARD_LEGENDS
        std::cout.width(3);
        std::cout << r;
#endif //DRAW_BOARD_LEGENDS

        for(unsigned int c = 0; c < sizeX_; ++c)
        {
            std::cout << "|";

            if(c == 0 && r == 0)
            {
                COORD first = Console::getInstance().GetCursorPos();
                boardStartX = first.X + 1;
                boardStartY = first.Y;
            }

            if(board_[TransformToIndex(c, r)].checked)
            {
                if(board_[TransformToIndex(c, r)].data == '0')
                {
                    Console::getInstance().ChangeTextColor(COLORS::WHITE);
                }
                else if(board_[TransformToIndex(c, r)].data == '1')
                {
                    Console::getInstance().ChangeTextColor(COLORS::BRIGHT_BLUE);
                }
                else if(board_[TransformToIndex(c, r)].data == '2')
                {
                    Console::getInstance().ChangeTextColor(COLORS::GREEN);
                }
                else if(board_[TransformToIndex(c, r)].data == '3')
                {
                    Console::getInstance().ChangeTextColor(COLORS::BRIGHT_RED);
                }
                else if(board_[TransformToIndex(c, r)].data == '4')
                {
                    Console::getInstance().ChangeTextColor(COLORS::BLUE);
                }
                else if(board_[TransformToIndex(c, r)].data == '5')
                {
                    Console::getInstance().ChangeTextColor(COLORS::RED);
                }
                else if(board_[TransformToIndex(c, r)].data == '6')
                {
                    Console::getInstance().ChangeTextColor(COLORS::CYAN);
                }
                else if(board_[TransformToIndex(c, r)].data == '7')
                {
                    Console::getInstance().ChangeTextColor(COLORS::BLACK);
                }
                else if(board_[TransformToIndex(c, r)].data == '8')
                {
                    Console::getInstance().ChangeTextColor(COLORS::PURPLE);
                }
                else if(board_[TransformToIndex(c, r)].data == MINE)
                {
                    Console::getInstance().ChangeTextColor(COLORS::BRIGHT_RED);
                }

                std::cout.width(2);
                std::cout << board_[TransformToIndex(c, r)].data << ' ';
                //Set color back to default
                Console::getInstance().ChangeBackgroundColor(COLORS::GREY);
                Console::getInstance().ChangeTextColor(COLORS::BLACK);
            }
            else if(board_[TransformToIndex(c, r)].flag)
            {
                Console::getInstance().ChangeTextColor(COLORS::BRIGHT_CYAN);
                std::cout.width(2);
                std::cout << static_cast<char>(FLAG) << ' ';
                //Set color back to default
                Console::getInstance().ChangeBackgroundColor(COLORS::GREY);
                Console::getInstance().ChangeTextColor(COLORS::BLACK);
            }
            else
            {
                std::cout.width(3);
                std::cout << ' ';
                //Set color back to default
                Console::getInstance().ChangeBackgroundColor(COLORS::GREY);
                Console::getInstance().ChangeTextColor(COLORS::BLACK);
            }
        }
        std::cout << "|" << std::endl;
    }

#ifdef DRAW_BOARD_LEGENDS
    std::cout.width(4 * sizeX_ + 4);
#endif //DRAW_BOARD_LEGENDS
    std::cout << tmpLine << std::endl;
    std::cout << std::endl;
}
#endif //NEW_RENDER

#ifdef OLD_RENDER
void Board::PrintBoard() const
{
    std::cout << '\t';
    for(unsigned int i = 0; i < sizeX_; ++i)
    {
        std::cout << "|";
        std::cout << i;
    }
    std::cout << "|" << std::endl;

    std::string tmpLine((2 * sizeX_) + 1, '-');
    std::string horizontalLine(1, '\t');
    horizontalLine.append(tmpLine);

    for(unsigned int r = 0; r < sizeY_; ++r)
    {
        std::cout << horizontalLine << std::endl;
        std::cout << r << '\t';

        for(unsigned int c = 0; c < sizeX_; ++c)
        {
            std::cout << "|";

            if(board_[TransformToIndex(c, r)].checked)
            {
                if(board_[TransformToIndex(c, r)].data == '0')
                    SetConsoleTextAttribute(hOutput, 7);    //Default white
                else if(board_[TransformToIndex(c, r)].data == '1')
                    SetConsoleTextAttribute(hOutput, 9);    //Bright Blue
                else if(board_[TransformToIndex(c, r)].data == '2')
                    SetConsoleTextAttribute(hOutput, 2);    //Green
                else if(board_[TransformToIndex(c, r)].data == '3')
                    SetConsoleTextAttribute(hOutput, 3);    //Cyan
                else if(board_[TransformToIndex(c, r)].data == '4')
                    SetConsoleTextAttribute(hOutput, 4);    //Red
                else if(board_[TransformToIndex(c, r)].data == '5')
                    SetConsoleTextAttribute(hOutput, 5);    //Purple
                else if(board_[TransformToIndex(c, r)].data == '6')
                    SetConsoleTextAttribute(hOutput, 6);    //Yellow (Dark)
                else if(board_[TransformToIndex(c, r)].data == '7')
                    SetConsoleTextAttribute(hOutput, 8);    //Grey
                else if(board_[TransformToIndex(c, r)].data == '8')
                    SetConsoleTextAttribute(hOutput, 1);    //Blue
                else if(board_[TransformToIndex(c, r)].data == MINE)
                    SetConsoleTextAttribute(hOutput, 12);    //Bright Red

                std::cout << board_[TransformToIndex(c, r)].data;

                //Set color back to default white
                SetConsoleTextAttribute(hOutput, 7);
            }
            else if(board_[TransformToIndex(c, r)].flag)
            {
                SetConsoleTextAttribute(hOutput, 11);    //Bright Cyan

                std::cout << 'F';

                //Set color back to default white
                SetConsoleTextAttribute(hOutput, 7);
            }
            else
                std::cout << ' ';
        }
        std::cout << "|" << std::endl;
    }
    std::cout << horizontalLine << std::endl;
}
#endif //OLD_RENDER

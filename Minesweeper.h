#pragma once
#include <vector>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1050
//640
//350

struct Tile
{
    bool checked;
    bool flag;
    char data;
};

enum MODE {
    CHECK,
    FLAG
};

class Board
{
public:
    //Windows expert mode c = 30, r = 16, m = 99
    //Nice looking with new render "equivalent" c = 15, r = 15, m = 46 (No longer needed since console resize)
    Board(unsigned int columns = 30, unsigned int rows = 16, unsigned int mines = 99);

    unsigned int TransformToIndex(unsigned int column, unsigned int row) const;
    void FillMineIndicators(int c, int r);
    void CreateBoard();
    void TestBoard();
    bool CheckTile(unsigned int x, unsigned int y);
    void Explore0(int c, int r);
    bool Flag(unsigned int x, unsigned int y);
    void PrintBoard();
    void UpdateValue(int x, int y);
    void UpdateAllBoard();
    void RevealMines();
    bool ConsoleToBoard(float& x, float& y);

    std::string GenerateFilename() const;

    unsigned int GetSizeX() const { return sizeX_; }
    unsigned int GetSizeY() const { return sizeY_; }
    unsigned int GetMines() const { return mines_; }
    bool GetMode() const { return mode; }
    void ChangeMode() { mode = !mode; };
    unsigned int GetFoundMines() const { return minesFound_; }

    unsigned int boardStartX = 0;
    unsigned int boardStartY = 0;

private:
    unsigned int sizeX_;
    unsigned int sizeY_;
    unsigned int mines_;
    unsigned int minesFound_;
    std::vector<unsigned int> minePositions_;
    std::vector<Tile> board_;
    bool mode = true; //True: check, false: flag (only for mouse input)
};

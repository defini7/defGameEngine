// TODO:
/*
* - Capture multiple checkers
* - Add game ending
* - Add game restarting
*/

#define DGE_APPLICATION
#include "../defGameEngine.hpp"

constexpr int nRows = 3;

class Checkers : public def::GameEngine
{
public:
    Checkers()
    {
        SetTitle("Checkers");
    }

    struct Cell
    {
        enum class Colour
        {
            Black,
            White
        };

        bool bOccupied = false;
        Colour colCell;
        Colour colChecker;
    };

    def::vi2d vFieldSize;
    def::vi2d vCellSize;

    std::vector<Cell> vecCells;

    struct Target
    {
        Cell* ptr = nullptr;
        int nIndex = -1;
    };

    Target target;

public:
    bool OnUserCreate() override
    {
        vCellSize = { 32, 32 };
        vFieldSize = GetScreenSize() / vCellSize;

        vecCells.resize(vFieldSize.x * vFieldSize.y);

        for (int x = 0; x < vFieldSize.x; x++)
            for (int y = 0; y < vFieldSize.y; y++)
            {
                int i = x + y * vFieldSize.x;

                vecCells[i].colCell = Cell::Colour((i + y + 1) % 2);
                vecCells[i].bOccupied = false;
            }

        for (int y = 0; y < nRows; y++)
            for (int x = 0; x < vFieldSize.x; x++)
            {
                int t = y * vFieldSize.x + x;
                int b = (vFieldSize.y - y - 1) * vFieldSize.x + x;

                if (vecCells[t].colCell == Cell::Colour::Black)
                {
                    vecCells[t].bOccupied = true;
                    vecCells[t].colChecker = Cell::Colour::White;
                }

                if (vecCells[b].colCell == Cell::Colour::Black)
                {
                    vecCells[b].bOccupied = true;
                    vecCells[b].colChecker = Cell::Colour::Black;
                }
            }

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        if (GetMouse(def::Button::LEFT).pressed)
        {
            def::vi2d vMouse = GetMousePos() / vCellSize;

            if (vMouse.x >= 0 && vMouse.y >= 0 && vMouse.x < vFieldSize.x && vMouse.y < vFieldSize.y)
            {
                int i = vMouse.y * vFieldSize.x + vMouse.x;

                if (target.ptr == nullptr)
                {
                    if (vecCells[i].bOccupied)
                    {
                        target.ptr = &vecCells[i];
                        target.nIndex = i;
                    }
                }
                else
                {
                    if (vecCells[i].bOccupied)
                    {
                        if (target.ptr == &vecCells[i])
                            target.ptr = nullptr;
                        else
                        {
                            if (target.ptr->colChecker == vecCells[i].colChecker)
                            {
                                target.ptr = &vecCells[i];
                                target.nIndex = i;
                            }
                        }
                    }
                    else
                    {
                        int dx = vMouse.x - target.nIndex % vFieldSize.x;
                        int dy = vMouse.y - target.nIndex / vFieldSize.x;

                        bool bAllowMove = false;

                        if (abs(dx) == 1 && abs(dy) == 1)
                            bAllowMove = true;
                        else if (abs(dx) == 2 && abs(dy) == 2)
                        {
                            int bx = vMouse.x - dx / 2;
                            int by = vMouse.y - dy / 2;

                            int j = by * vFieldSize.x + bx;

                            if (vecCells[j].bOccupied && vecCells[j].colChecker != target.ptr->colChecker)
                            {
                                vecCells[j].bOccupied = false;
                                bAllowMove = true;
                            }
                        }

                        if (bAllowMove)
                        {
                            vecCells[i].bOccupied = true;
                            vecCells[i].colChecker = target.ptr->colChecker;
                            target.ptr->bOccupied = false;
                            target.ptr = nullptr;
                        }
                    }
                }
            }

        }





        def::vi2d p;
        for (p.x = 0; p.x < vFieldSize.x; p.x++)
            for (p.y = 0; p.y < vFieldSize.y; p.y++)
            {
                def::vi2d coord = p * vCellSize;
                int i = p.y * vFieldSize.x + p.x;

                switch (vecCells[i].colCell)
                {
                case Cell::Colour::Black:
                    FillRectangle(coord, vCellSize, def::BLACK);
                    break;
                case Cell::Colour::White:
                    FillRectangle(coord, vCellSize, def::WHITE);
                    break;
                }

                if (vecCells[i].bOccupied)
                {
                    int radius = std::min(vCellSize.x, vCellSize.y) * 0.25f;

                    switch (vecCells[i].colChecker)
                    {
                    case Cell::Colour::Black:
                        FillCircle(coord + vCellSize / 2, radius, def::DARK_RED);
                        break;
                    case Cell::Colour::White:
                        FillCircle(coord + vCellSize / 2, radius, def::WHITE);
                        break;
                    }
                }
            }

        return true;
    }
};

int main()
{
    Checkers demo;

    if (demo.Construct(256, 256, 2, 2))
        demo.Run();

    return 0;
}

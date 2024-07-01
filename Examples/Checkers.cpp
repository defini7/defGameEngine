/*
*   BSD 3-Clause License
    Copyright (c) 2024, Alex
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define DGE_APPLICATION
#include "defGameEngine.hpp"

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
        vFieldSize = ScreenSize() / vCellSize;

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
            def::vi2d vMouse = GetMouse() / vCellSize;

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
    demo.Construct(256, 256, 2, 2);
    demo.Run();
    return 0;
}

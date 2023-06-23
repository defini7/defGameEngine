/*
* BSD 3-Clause License
Copyright (c) 2023, Alex
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
#include "../../defGameEngine.h"

#define DGE_PATHFINDER
#include "../../Extensions/DGE_PathFinder.h"

using namespace def;

class AStar : public def::GameEngine
{
public:
	AStar()
	{
		SetTitle("AStar algorithm");
	}

private:
	def::PathFinder pathFinder;

	static float Distance(sNode* n1, sNode* n2)
	{
		return sqrtf((n1->nPosX - n2->nPosX) * (n1->nPosX - n2->nPosX) + (n1->nPosY - n2->nPosY) * (n1->nPosY - n2->nPosY));
	}

	static float Heuristic(sNode* n1, sNode* n2)
	{
		return Distance(n1, n2);
	}

protected:
	bool OnUserCreate() override
	{
		pathFinder.ConstructMap(16, 16);
		pathFinder.SetNodes(1, 8, 14, 8);

		pathFinder.ClearMap();
		pathFinder.FindPath(&Distance, &Heuristic);

		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		int nNodeSize = 9;
		int nNodeBorder = 2;

		int nSelectedNodeX = MouseX() / nNodeSize;
		int nSelectedNodeY = MouseY() / nNodeSize;

		sNode* nodes = pathFinder.GetNodes();

		if (GetMouse(0).bReleased)
		{
			if (nSelectedNodeX >= 0 && nSelectedNodeX < pathFinder.GetMapWidth())
				if (nSelectedNodeY >= 0 && nSelectedNodeY < pathFinder.GetMapHeight())
				{
					int p = nSelectedNodeY * pathFinder.GetMapWidth() + nSelectedNodeX;

					if (GetKey(L'S').bHeld)
						pathFinder.SetNodes(&nodes[p], nullptr);
					else if (GetKey(L'G').bHeld)
						pathFinder.SetNodes(nullptr, &nodes[p]);
					else
					{
						if (&nodes[p] != pathFinder.GetStartNode() && &nodes[p] != pathFinder.GetGoalNode())
							nodes[p].bObstacle = !nodes[p].bObstacle;
					}

					pathFinder.ClearMap();
					pathFinder.FindPath(&Distance, &Heuristic);
				}
		}

		Clear(def::BLACK);

		for (int x = 0; x < pathFinder.GetMapWidth(); x++)
			for (int y = 0; y < pathFinder.GetMapHeight(); y++)
			{
				for (auto n : nodes[y * pathFinder.GetMapWidth() + x].vecNeighbours)
				{
					DrawLine(
						x * nNodeSize + nNodeSize / 2, y * nNodeSize + nNodeSize / 2,
						n->nPosX * nNodeSize + nNodeSize / 2, n->nPosY * nNodeSize + nNodeSize / 2,
						def::DARK_BLUE
					);
				}
			}

		for (int x = 0; x < pathFinder.GetMapWidth(); x++)
			for (int y = 0; y < pathFinder.GetMapHeight(); y++)
			{
				int x1 = x * nNodeSize + nNodeBorder;
				int y1 = y * nNodeSize + nNodeBorder;
				int sx1 = ((x + 1) * nNodeSize - nNodeBorder) - x1;
				int sy1 = ((y + 1) * nNodeSize - nNodeBorder) - y1;

				if (nodes[y * pathFinder.GetMapWidth() + x].bObstacle)
					FillRectangle(x1, y1, sx1, sy1, def::WHITE);
				else if (&nodes[y * pathFinder.GetMapWidth() + x] == pathFinder.GetStartNode())
					FillRectangle(x1, y1, sx1, sy1, def::GREEN);
				else if (&nodes[y * pathFinder.GetMapWidth() + x] == pathFinder.GetGoalNode())
					FillRectangle(x1, y1, sx1, sy1, def::RED);
				else if (nodes[y * pathFinder.GetMapWidth() + x].bVisited)
					FillRectangle(x1, y1, sx1, sy1, def::BLUE);
				else
					FillRectangle(x1, y1, sx1, sy1, def::DARK_BLUE);
			}

		if (pathFinder.GetGoalNode() != nullptr)
		{
			sNode* p = pathFinder.GetGoalNode();
			while (p->pParent != nullptr)
			{
				DrawLine(
					p->nPosX * nNodeSize + nNodeSize / 2, p->nPosY * nNodeSize + nNodeSize / 2,
					p->pParent->nPosX * nNodeSize + nNodeSize / 2, p->pParent->nPosY * nNodeSize + nNodeSize / 2,
					def::YELLOW
				);

				p = p->pParent;
			}
		}

		return true;
	}

};

int main()
{
	AStar demo;

	demo.Construct(256, 240, 4, 4);
	demo.Run();

	return 0;
}

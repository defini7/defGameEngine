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

	static float Distance(Node* n1, Node* n2)
	{
		return sqrtf((n1->posX - n2->posX) * (n1->posX - n2->posX) + (n1->posY - n2->posY) * (n1->posY - n2->posY));
	}

	static float Heuristic(Node* n1, Node* n2)
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
		int nodeSize = 9;
		int nodeBorder = 2;

		int selectedNodeX = MouseX() / nodeSize;
		int selectedNodeY = MouseY() / nodeSize;

		Node* nodes = pathFinder.GetNodes();

		if (GetMouse(0).released)
		{
			if (selectedNodeX >= 0 && selectedNodeX < pathFinder.GetMapWidth())
				if (selectedNodeY >= 0 && selectedNodeY < pathFinder.GetMapHeight())
				{
					int p = selectedNodeY * pathFinder.GetMapWidth() + selectedNodeX;

					if (GetKey(L'S').held)
						pathFinder.SetNodes(&nodes[p], nullptr);
					else if (GetKey(L'G').held)
						pathFinder.SetNodes(nullptr, &nodes[p]);
					else
					{
						if (&nodes[p] != pathFinder.GetStartNode() && &nodes[p] != pathFinder.GetGoalNode())
							nodes[p].isObstacle = !nodes[p].isObstacle;
					}

					pathFinder.ClearMap();
					pathFinder.FindPath(&Distance, &Heuristic);
				}
		}

		Clear(def::BLACK);

		for (int x = 0; x < pathFinder.GetMapWidth(); x++)
			for (int y = 0; y < pathFinder.GetMapHeight(); y++)
			{
				for (auto n : nodes[y * pathFinder.GetMapWidth() + x].neighbours)
				{
					DrawLine(
						x * nodeSize + nodeSize / 2, y * nodeSize + nodeSize / 2,
						n->posX * nodeSize + nodeSize / 2, n->posY * nodeSize + nodeSize / 2,
						def::DARK_BLUE
					);
				}
			}

		for (int x = 0; x < pathFinder.GetMapWidth(); x++)
			for (int y = 0; y < pathFinder.GetMapHeight(); y++)
			{
				int x1 = x * nodeSize + nodeBorder;
				int y1 = y * nodeSize + nodeBorder;
				int sx1 = ((x + 1) * nodeSize - nodeBorder) - x1;
				int sy1 = ((y + 1) * nodeSize - nodeBorder) - y1;

				if (nodes[y * pathFinder.GetMapWidth() + x].isObstacle)
					FillRectangle(x1, y1, sx1, sy1, def::WHITE);
				else if (&nodes[y * pathFinder.GetMapWidth() + x] == pathFinder.GetStartNode())
					FillRectangle(x1, y1, sx1, sy1, def::GREEN);
				else if (&nodes[y * pathFinder.GetMapWidth() + x] == pathFinder.GetGoalNode())
					FillRectangle(x1, y1, sx1, sy1, def::RED);
				else if (nodes[y * pathFinder.GetMapWidth() + x].isVisited)
					FillRectangle(x1, y1, sx1, sy1, def::BLUE);
				else
					FillRectangle(x1, y1, sx1, sy1, def::DARK_BLUE);
			}

		if (pathFinder.GetGoalNode() != nullptr)
		{
			Node* p = pathFinder.GetGoalNode();
			while (p->parent != nullptr)
			{
				DrawLine(
					p->posX * nodeSize + nodeSize / 2, p->posY * nodeSize + nodeSize / 2,
					p->parent->posX * nodeSize + nodeSize / 2, p->parent->posY * nodeSize + nodeSize / 2,
					def::YELLOW
				);

				p = p->parent;
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

#include "defGameEngine.h"

#define DGE_PATHFINDER
#include "DGE_PathFinder.h"

using namespace def;

class AStar : public def::GameEngine
{
public:
	AStar()
	{
		SetTitle("AStar algorithm");
	}

private:
	DGE_PathFinder pathFinder;

protected:
	bool OnUserCreate() override
	{
		pathFinder.ConstructMap(16, 16);
		pathFinder.SetNodes(1, 8, 14, 8);

		auto distance = [&](sNode* n1, sNode* n2)
		{
			return sqrtf((n1->nPosX - n2->nPosX) * (n1->nPosX - n2->nPosX) + (n1->nPosY - n2->nPosY) * (n1->nPosY - n2->nPosY));
		};

		auto heuristic = [distance](sNode* n1, sNode* n2)
		{
			return distance(n1, n2);
		};

		pathFinder.ClearMap();
		pathFinder.FindPath(distance, heuristic);

		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		int nNodeSize = 9;
		int nNodeBorder = 2;

		int nSelectedNodeX = GetMouseX() / nNodeSize;
		int nSelectedNodeY = GetMouseY() / nNodeSize;

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

					auto distance = [&](sNode* n1, sNode* n2)
					{
						return sqrtf((n1->nPosX - n2->nPosX) * (n1->nPosX - n2->nPosX) + (n1->nPosY - n2->nPosY) * (n1->nPosY - n2->nPosY));
					};

					auto heuristic = [distance](sNode* n1, sNode* n2)
					{
						return distance(n1, n2);
					};

					pathFinder.ClearMap();
					pathFinder.FindPath(distance, heuristic);
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
	def::rcode err = demo.Construct(256, 240, 4, 4);

	if (err.ok)
		demo.Run();
	else
		std::cerr << err.info << "\n";

	return 0;
}

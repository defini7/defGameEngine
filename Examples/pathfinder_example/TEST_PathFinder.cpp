/*
*	BSD 3-Clause License
	Copyright (c) 2022 - 2024, Alex
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

#define DGE_PATHFINDER
#include "DGE_PathFinder.hpp"

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
		return (n1->pos - n2->pos).length();
	}

	static float Heuristic(Node* n1, Node* n2)
	{
		return Distance(n1, n2);
	}

protected:
	bool OnUserCreate() override
	{
		pathFinder.ConstructMap({ 16, 16 });
		pathFinder.SetNodes({ 1, 8 }, { 14, 8 });

		pathFinder.ClearMap();
		pathFinder.FindPath(&Distance, &Heuristic);

		return true;
	}

	bool OnUserUpdate(float fDeltaTime) override
	{
		int nodeSize = 9;
		int nodeBorder = 2;

		def::vi2d selectedNode = GetMousePos() / nodeSize;

		Node* nodes = pathFinder.GetNodes();

		if (GetMouse(def::Button::LEFT).released)
		{
			if (selectedNode >= def::vi2d(0, 0) && selectedNode < pathFinder.GetMapSize())
			{
				int p = selectedNode.y * pathFinder.GetMapWidth() + selectedNode.x;

				if (GetKey(def::Key::S).held)
					pathFinder.SetNodes(&nodes[p], nullptr);
				else if (GetKey(def::Key::G).held)
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

		def::vi2d i;
		for (i.x = 0; i.x < pathFinder.GetMapWidth(); i.x++)
			for (i.y = 0; i.y < pathFinder.GetMapHeight(); i.y++)
			{
				for (auto n : nodes[i.y * pathFinder.GetMapWidth() + i.x].neighbours)
				{
					DrawLine(
						i * nodeSize + nodeSize / 2,
						n->pos * nodeSize + nodeSize / 2,
						def::DARK_BLUE);
				}
			}

		for (i.x = 0; i.x < pathFinder.GetMapWidth(); i.x++)
			for (i.y = 0; i.y < pathFinder.GetMapHeight(); i.y++)
			{
				def::Node& n = nodes[i.y * pathFinder.GetMapWidth() + i.x];
				def::Pixel col;

				if (n.isObstacle)
					col = def::WHITE;
				else if (&n == pathFinder.GetStartNode())
					col = def::GREEN;
				else if (&n == pathFinder.GetGoalNode())
					col = def::RED;
				else if (n.isVisited)
					col = def::BLUE;
				else
					col = def::DARK_BLUE;

				int s = nodeSize - 2 * nodeBorder;
				FillRectangle(i * nodeSize + nodeBorder, { s, s }, col);
			}

		if (pathFinder.GetGoalNode() != nullptr)
		{
			Node* n = pathFinder.GetGoalNode();
			while (n->parent != nullptr)
			{
				DrawLine(
					n->pos * nodeSize + nodeSize / 2,
					n->parent->pos * nodeSize + nodeSize / 2,
					def::YELLOW);

				n = n->parent;
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

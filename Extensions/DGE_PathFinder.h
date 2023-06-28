#pragma once

#pragma region license
/***
*	BSD 3-Clause License
	Copyright (c) 2021, 2022 Alex
	All rights reserved.
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
***/
#pragma endregion

#pragma region includes

#include <list>
#include <cmath>
#include <functional>
#include <vector>

#pragma endregion

namespace def
{
	struct Node
	{
		bool isObstacle = false;
		bool isVisited = false;
		float globalGoal;
		float localGoal;
		int posX;
		int posY;
		std::vector<Node*> neighbours;
		Node* parent;
	};

	class PathFinder
	{
	public:
		PathFinder() = default;

		PathFinder(int mapWidth, int mapHeight)
		{
			m_MapWidth = mapWidth;
			m_MapHeight = mapHeight;

			m_Nodes = new Node[m_MapWidth * m_MapHeight];
		}

		~PathFinder()
		{
			FreeMap();
		}

	private:
		int m_MapWidth;
		int m_MapHeight;

		Node* m_Nodes;

		Node* m_Start;
		Node* m_Goal;

		bool m_IsMapFreed = false;

	public:
		void ClearMap();
		bool FreeMap();
		bool ConstructMap(int mapWidth, int mapHeight);

		bool SetNodes(int startX, int startY, int goalX, int goalY);
		void SetNodes(Node* start = nullptr, Node* goal = nullptr);

		Node* GetStartNode();
		Node* GetGoalNode();
		Node* GetNodes();

		void ResetNodes();
		int GetMapWidth();
		int GetMapHeight();

		void FindPath(float (*dist)(Node*, Node*), float (*heuristic)(Node*, Node*));

	};

#ifdef DGE_PATHFINDER
#undef DGE_PATHFINDER

	void PathFinder::ClearMap()
	{
		for (int x = 0; x < m_MapWidth; x++)
			for (int y = 0; y < m_MapHeight; y++)
			{
				int p = y * m_MapWidth + x;
				m_Nodes[p].isVisited = false;
				m_Nodes[p].globalGoal = INFINITY;
				m_Nodes[p].localGoal = INFINITY;
				m_Nodes[p].parent = nullptr;
			}
	}

	bool PathFinder::FreeMap()
	{
		if (!m_IsMapFreed)
		{
			delete[] m_Nodes;
			m_IsMapFreed = true;
		}

		return m_IsMapFreed;
	}

	bool PathFinder::ConstructMap(int mapWidth, int mapHeight)
	{
		if (mapWidth <= 0 || mapHeight <= 0)
			return false;

		FreeMap();

		m_MapWidth = mapWidth;
		m_MapHeight = mapHeight;

		m_Nodes = new Node[mapWidth * mapHeight];

		for (int x = 0; x < mapWidth; x++)
			for (int y = 0; y < mapHeight; y++)
			{
				int p = y * mapWidth + x;
				m_Nodes[p].isObstacle = false;
				m_Nodes[p].isVisited = false;
				m_Nodes[p].posX = x;
				m_Nodes[p].posY = y;
				m_Nodes[p].parent = nullptr;
			}

		for (int x = 0; x < mapWidth; x++)
			for (int y = 0; y < mapHeight; y++)
			{
				if (y > 0) m_Nodes[y * mapWidth + x].neighbours.push_back(&m_Nodes[(y - 1) * mapWidth + x]);
				if (y < mapHeight - 1) m_Nodes[y * mapWidth + x].neighbours.push_back(&m_Nodes[(y + 1) * mapWidth + x]);
				if (x > 0) m_Nodes[y * mapWidth + x].neighbours.push_back(&m_Nodes[y * mapWidth + x - 1]);
				if (x < mapWidth - 1) m_Nodes[y * mapWidth + x].neighbours.push_back(&m_Nodes[y * mapWidth + x + 1]);
			}

		m_IsMapFreed = false;

		return true;
	}

	bool PathFinder::SetNodes(int nStartX, int nStartY, int nGoalX, int nGoalY)
	{
		if (nStartX < 0 || nStartX >= m_MapWidth || nGoalX < 0 || nGoalX >= m_MapWidth ||
			nStartY < 0 || nStartY >= m_MapHeight || nGoalY < 0 || nGoalY >= m_MapHeight)
			return false;

		m_Start = &m_Nodes[nStartY * m_MapWidth + nStartX];
		m_Goal = &m_Nodes[nGoalY * m_MapWidth + nGoalX];

		return true;
	}

	void PathFinder::SetNodes(Node* start, Node* goal)
	{
		if (start) m_Start = start;
		if (goal) m_Goal = goal;
	}

	Node* PathFinder::GetStartNode()
	{
		return m_Start;
	}

	Node* PathFinder::GetGoalNode()
	{
		return m_Goal;
	}

	void PathFinder::ResetNodes()
	{
		m_Start = nullptr;
		m_Goal = nullptr;
	}

	Node* PathFinder::GetNodes()
	{
		return m_Nodes;
	}

	int PathFinder::GetMapWidth()
	{
		return m_MapWidth;
	}

	int PathFinder::GetMapHeight()
	{
		return m_MapHeight;
	}

	void PathFinder::FindPath(float (*dist)(Node*, Node*), float (*heuristic)(Node*, Node*))
	{
		Node* current = m_Start;
		m_Start->localGoal = 0.0f;
		m_Start->globalGoal = heuristic(m_Start, m_Goal);

		std::list<Node*> nodesToTest;
		nodesToTest.push_back(m_Start);

		while (!nodesToTest.empty() && current != m_Goal)
		{
			nodesToTest.sort(
				[](const Node* lhs, const Node* rhs)
				{
					return lhs->globalGoal < rhs->globalGoal;
				}
			);

			while (!nodesToTest.empty() && nodesToTest.front()->isVisited)
				nodesToTest.pop_front();

			if (nodesToTest.empty()) break;

			current = nodesToTest.front();
			current->isVisited = true;

			for (auto& n : current->neighbours)
			{
				if (!n->isVisited && !n->isObstacle)
					nodesToTest.push_back(n);

				float possiblyLowerGoal = current->localGoal + dist(current, n);

				if (possiblyLowerGoal < n->localGoal)
				{
					n->parent = current;
					n->localGoal = possiblyLowerGoal;
					n->globalGoal = n->localGoal + heuristic(n, m_Goal);
				}
			}
		}
	}
	
#endif
}

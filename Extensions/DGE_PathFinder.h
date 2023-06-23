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
	struct sNode
	{
		bool bObstacle = false;
		bool bVisited = false;
		float fGlobalGoal;
		float fLocalGoal;
		int nPosX;
		int nPosY;
		std::vector<sNode*> vecNeighbours;
		sNode* pParent;
	};

	class PathFinder
	{
	public:
		PathFinder() = default;

		PathFinder(int nMapWidth, int nMapHeight)
		{
			m_nMapWidth = nMapWidth;
			m_nMapHeight = nMapHeight;

			m_nodeNodes = new sNode[m_nMapWidth * m_nMapHeight];
		}

		~PathFinder()
		{
			FreeMap();
		}

	private:
		int m_nMapWidth;
		int m_nMapHeight;

		sNode* m_nodeNodes;

		sNode* m_nodeStart;
		sNode* m_nodeGoal;

		bool m_bMapFreed = false;

	public:
		void ClearMap();
		bool FreeMap();
		bool ConstructMap(int nMapWidth, int nMapHeight);

		bool SetNodes(int nStartX, int nStartY, int nGoalX, int nGoalY);
		void SetNodes(sNode* start = nullptr, sNode* goal = nullptr);

		sNode* GetStartNode();
		sNode* GetGoalNode();
		sNode* GetNodes();

		void ResetNodes();

		int GetMapWidth();

		int GetMapHeight();

		void FindPath(float (*dist)(sNode*, sNode*), float (*heuristic)(sNode*, sNode*));

	};

#ifdef DGE_PATHFINDER
#undef DGE_PATHFINDER

	void PathFinder::ClearMap()
	{
		for (int x = 0; x < m_nMapWidth; x++)
			for (int y = 0; y < m_nMapHeight; y++)
			{
				int p = y * m_nMapWidth + x;
				m_nodeNodes[p].bVisited = false;
				m_nodeNodes[p].fGlobalGoal = INFINITY;
				m_nodeNodes[p].fLocalGoal = INFINITY;
				m_nodeNodes[p].pParent = nullptr;
			}
	}

	bool PathFinder::FreeMap()
	{
		if (!m_bMapFreed)
		{
			delete[] m_nodeNodes;
			m_bMapFreed = true;
		}

		return m_bMapFreed;
	}

	bool PathFinder::ConstructMap(int nMapWidth, int nMapHeight)
	{
		if (nMapWidth <= 0 || nMapHeight <= 0)
			return false;

		FreeMap();

		m_nMapWidth = nMapWidth;
		m_nMapHeight = nMapHeight;

		m_nodeNodes = new sNode[m_nMapWidth * m_nMapHeight];

		for (int x = 0; x < nMapWidth; x++)
			for (int y = 0; y < nMapHeight; y++)
			{
				int p = y * nMapWidth + x;
				m_nodeNodes[p].bObstacle = false;
				m_nodeNodes[p].bVisited = false;
				m_nodeNodes[p].nPosX = x;
				m_nodeNodes[p].nPosY = y;
				m_nodeNodes[p].pParent = nullptr;
			}

		for (int x = 0; x < nMapWidth; x++)
			for (int y = 0; y < nMapHeight; y++)
			{
				if (y > 0)
					m_nodeNodes[y * nMapWidth + x].vecNeighbours.push_back(&m_nodeNodes[(y - 1) * nMapWidth + x]);

				if (y < nMapHeight - 1)
					m_nodeNodes[y * nMapWidth + x].vecNeighbours.push_back(&m_nodeNodes[(y + 1) * nMapWidth + x]);

				if (x > 0)
					m_nodeNodes[y * nMapWidth + x].vecNeighbours.push_back(&m_nodeNodes[y * nMapWidth + x - 1]);

				if (x < nMapWidth - 1)
					m_nodeNodes[y * nMapWidth + x].vecNeighbours.push_back(&m_nodeNodes[y * nMapWidth + x + 1]);
			}

		m_bMapFreed = false;

		return true;
	}

	bool PathFinder::SetNodes(int nStartX, int nStartY, int nGoalX, int nGoalY)
	{
		if (nStartX < 0 || nStartX >= m_nMapWidth || nGoalX < 0 || nGoalX >= m_nMapWidth ||
			nStartY < 0 || nStartY >= m_nMapHeight || nGoalY < 0 || nGoalY >= m_nMapHeight)
			return false;

		m_nodeStart = &m_nodeNodes[nStartY * m_nMapWidth + nStartX];
		m_nodeGoal = &m_nodeNodes[nGoalY * m_nMapWidth + nGoalX];

		return true;
	}

	void PathFinder::SetNodes(sNode* start, sNode* goal)
	{
		if (start)
			m_nodeStart = start;

		if (goal)
			m_nodeGoal = goal;
	}

	sNode* PathFinder::GetStartNode()
	{
		return m_nodeStart;
	}

	sNode* PathFinder::GetGoalNode()
	{
		return m_nodeGoal;
	}

	void PathFinder::ResetNodes()
	{
		m_nodeStart = nullptr;
		m_nodeGoal = nullptr;
	}

	sNode* PathFinder::GetNodes()
	{
		return m_nodeNodes;
	}

	int PathFinder::GetMapWidth()
	{
		return m_nMapWidth;
	}

	int PathFinder::GetMapHeight()
	{
		return m_nMapHeight;
	}

	void PathFinder::FindPath(float (*dist)(sNode*, sNode*), float (*heuristic)(sNode*, sNode*))
	{
		sNode* current = m_nodeStart;
		m_nodeStart->fLocalGoal = 0.0f;
		m_nodeStart->fGlobalGoal = heuristic(m_nodeStart, m_nodeGoal);

		std::list<sNode*> listNodesToTest;
		listNodesToTest.push_back(m_nodeStart);

		while (!listNodesToTest.empty() && current != m_nodeGoal)
		{
			listNodesToTest.sort(
				[](const sNode* lhs, const sNode* rhs)
				{
					return lhs->fGlobalGoal < rhs->fGlobalGoal;
				}
			);

			while (!listNodesToTest.empty() && listNodesToTest.front()->bVisited)
				listNodesToTest.pop_front();

			if (listNodesToTest.empty())
				break;

			current = listNodesToTest.front();
			current->bVisited = true;

			for (auto n : current->vecNeighbours)
			{
				if (!n->bVisited && !n->bObstacle)
					listNodesToTest.push_back(n);

				float fPossiblyLowerGoal = current->fLocalGoal + dist(current, n);

				if (fPossiblyLowerGoal < n->fLocalGoal)
				{
					n->pParent = current;
					n->fLocalGoal = fPossiblyLowerGoal;
					n->fGlobalGoal = n->fLocalGoal + heuristic(n, m_nodeGoal);
				}
			}
		}
	}
	
#endif
}

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

#include <string>
#include <vector>

#ifdef DGE_RAYCASTING

namespace def
{
	struct sObject
	{
		float x;
		float y;

		float vx;
		float vy;

		float speed;

		uint32_t id;

		bool bRemove = false;
	};

	enum Side
	{
		CEILING,
		WALL,
		FLOOR
	};

	enum State
	{
		HELD,
		PRESSED,
		RELEASED
	};

	class DGE_RayCasting
	{
	public:
		DGE_RayCasting() = default;

		virtual ~DGE_RayCasting()
		{
			delete[] m_fDepthBuffer;
		}

	private:
		int m_nScreenWidth = 0;
		int m_nScreenHeight = 0;

		float m_fPlayerX = 7.0f;
		float m_fPlayerY = 3.0f;

		float m_fMoveSpeed = 5.0f;
		float m_fRotSpeed = 3.0f;

		float m_fDirX = -1.0f;
		float m_fDirY = 0.0f;

		float m_fPlaneX = 0.0f;
		float m_fPlaneY = 0.66f; // FOV in degrees

		int m_nMapWidth = 0;
		int m_nMapHeight = 0;

		float m_fDepth = 16.0f;

		float m_fDeltaTime = 0.0f;

		int m_nTexWidth = 0;
		int m_nTexHeight = 0;

		std::string m_sMap;

		std::vector<sObject> m_vecObjects;

		float* m_fDepthBuffer = nullptr;

		KeyCode m_kcForward;
		KeyCode m_kcBack;
		
		KeyCode m_kcRotLeft;
		KeyCode m_kcRotRight;

	public:

		void SetDeltaTime(float fDeltaTime)
		{
			m_fDeltaTime = fDeltaTime;
		}

		void SetTexWidth(int nTexWidth)
		{
			m_nTexWidth = nTexWidth;
		}

		void SetTexHeight(int nTexHeight)
		{
			m_nTexHeight = nTexHeight;
		}

		void SetMap(std::string sMap, int nMapWidth, int nMapHeight)
		{
			m_sMap = sMap;

			m_nMapWidth = nMapWidth;
			m_nMapHeight = nMapHeight;
		}

		void AddObject(sObject o)
		{
			m_vecObjects.push_back(o);
		}

		float GetPlayerX()
		{
			return m_fPlayerX;
		}

		float GetPlayerY()
		{
			return m_fPlayerY;
		}

		float GetDirX()
		{
			return m_fDirX;
		}

		float GetDirY()
		{
			return m_fDirY;
		}

		void SetKeys(KeyCode kcForward, KeyCode kcBack, KeyCode kcRotLeft, KeyCode kcRotRight)
		{
			m_kcForward = kcForward;
			m_kcBack = kcBack;

			m_kcRotLeft = kcRotLeft;
			m_kcRotRight = kcRotRight;
		}

		void Setup(int nScreenWidth, int nScreenHeight)
		{
			m_nScreenWidth = nScreenWidth;
			m_nScreenHeight = nScreenHeight;

			m_fDepthBuffer = new float[m_nScreenWidth];
		}

		virtual def::Pixel ApplyWorldPixel(Side side, int tex_x, int tex_y, int map_x, int map_y, bool is_wall = false) = 0;
		virtual def::Pixel ApplyObjectPixel(uint32_t id, int tex_x, int tex_y) = 0;

		virtual void DrawPixel(int x, int y, def::Pixel p) = 0;
		virtual bool GetKeyState(KeyCode kc, State s) = 0;

		void DoRayCasting()
		{
			for (int x = 0; x < m_nScreenWidth; x++)
			{
				float fCameraX = 2.0f * (float)x / (float)m_nScreenWidth - 1.0f;

				float fRayDirX = m_fDirX + m_fPlaneX * fCameraX;
				float fRayDirY = m_fDirY + m_fPlaneY * fCameraX;

				float fDistanceX = fabs(1.0f / fRayDirX);
				float fDistanceY = fabs(1.0f / fRayDirY);

				int nStepX = 0;
				int nStepY = 0;

				bool bHitWall = false;
				bool bNoWall = false;
				int nSide = 0;

				float fFromCurrentDistX;
				float fFromCurrentDistY;

				int nMapX = (int)m_fPlayerX;
				int nMapY = (int)m_fPlayerY;

				float fDistanceToWall;

				if (fRayDirX < 0.0f)
				{
					nStepX = -1;
					fFromCurrentDistX = (m_fPlayerX - (float)nMapX) * fDistanceX;
				}
				else
				{
					nStepX = 1;
					fFromCurrentDistX = ((float)nMapX + 1.0f - m_fPlayerX) * fDistanceX;
				}

				if (fRayDirY < 0.0f)
				{
					nStepY = -1;
					fFromCurrentDistY = (m_fPlayerY - (float)nMapY) * fDistanceY;
				}
				else
				{
					nStepY = 1;
					fFromCurrentDistY = ((float)nMapY + 1.0f - m_fPlayerY) * fDistanceY;
				}

				while (!bHitWall && !bNoWall)
				{
					if (fFromCurrentDistX < fFromCurrentDistY)
					{
						fFromCurrentDistX += fDistanceX;
						nMapX += nStepX;
						nSide = 0;
					}
					else
					{
						fFromCurrentDistY += fDistanceY;
						nMapY += nStepY;
						nSide = 1;
					}

					if (nMapY < 0 || nMapY >= m_nMapHeight || nMapX < 0 || nMapX >= m_nMapWidth)
					{
						fDistanceToWall = m_fDepth;
						bNoWall = true;
					}

					if (!bNoWall && std::isdigit(m_sMap[nMapY * m_nMapWidth + nMapX]))
						bHitWall = true;
				}

				if (nSide == 0)
					fDistanceToWall = fFromCurrentDistX - fDistanceX;
				else
					fDistanceToWall = fFromCurrentDistY - fDistanceY;

				int nLineHeight = int((float)m_nScreenHeight / fDistanceToWall) * 2;

				int nCeiling = -nLineHeight / 2 + m_nScreenHeight / 2;
				int nFloor = nLineHeight / 2 + m_nScreenHeight / 2;

				if (nCeiling < 0)
					nCeiling = 0;

				if (nFloor >= m_nScreenHeight)
					nFloor = m_nScreenHeight;

				float fTestPoint;
				float fTexStep;
				float fTexPos;

				int nTexX;

				if (nSide == 0)
					fTestPoint = m_fPlayerY + fRayDirY * fDistanceToWall;
				else
					fTestPoint = m_fPlayerX + fRayDirX * fDistanceToWall;

				fTestPoint -= floorf(fTestPoint);

				nTexX = int(fTestPoint * (float)m_nTexWidth);

				if ((nSide == 0 && fRayDirX > 0.0f) || (nSide == 1 && fRayDirY < 0.0f))
					nTexX = m_nTexWidth - nTexX - 1;

				fTexStep = (float)m_nTexHeight / (float)nLineHeight;
				fTexPos = float(nCeiling - m_nScreenHeight / 2 + nLineHeight / 2) * fTexStep;

				for (int y = 0; y <= nFloor; y++)
				{
					if (y <= nCeiling) // sky and floor
					{
						float fPlaneZ = float(m_nScreenHeight / 2) / float(m_nScreenHeight / 2 - y);

						float fPlanePointX = m_fPlayerX + 2.0f * fRayDirX * fPlaneZ;
						float fPlanePointY = m_fPlayerY + 2.0f * fRayDirY * fPlaneZ;

						float fPlaneSampleX = fPlanePointX - (float)(int)fPlanePointX;
						float fPlaneSampleY = fPlanePointY - (float)(int)fPlanePointY;

						int nTexX = std::min(int(fPlaneSampleX * (float)m_nTexWidth), m_nTexWidth);
						int nTexY = std::min(int(fPlaneSampleY * (float)m_nTexHeight), m_nTexHeight);

						def::Pixel pCeilingPixel = ApplyWorldPixel(CEILING, nTexX, nTexY, nMapX, nMapY);
						def::Pixel pFloorPixel = ApplyWorldPixel(FLOOR, nTexX, nTexY, nMapX, nMapY);

						if (fDistanceToWall > 2.0f)
						{
							pCeilingPixel.r = uint8_t((float)pCeilingPixel.r / fDistanceToWall * 2.0f);
							pCeilingPixel.g = uint8_t((float)pCeilingPixel.g / fDistanceToWall * 2.0f);
							pCeilingPixel.b = uint8_t((float)pCeilingPixel.b / fDistanceToWall * 2.0f);

							pFloorPixel.r = uint8_t((float)pFloorPixel.r / fDistanceToWall * 2.0f);
							pFloorPixel.g = uint8_t((float)pFloorPixel.g / fDistanceToWall * 2.0f);
							pFloorPixel.b = uint8_t((float)pFloorPixel.b / fDistanceToWall * 2.0f);
						}

						DrawPixel(x, y, pCeilingPixel); // sky
						DrawPixel(x, m_nScreenHeight - y, pFloorPixel); // ceiling
					}
					else if (y > nCeiling && !bNoWall) // wall
					{
						if (fDistanceToWall < m_fDepth)
						{
							int nTexY = (int)fTexPos & (m_nTexHeight - 1);
							fTexPos += fTexStep;

							def::Pixel pWallPixel = ApplyWorldPixel(WALL, nTexX, nTexY, nMapX, nMapY, true);

							if (fDistanceToWall > 2.0f)
							{
								pWallPixel.r = uint8_t((float)pWallPixel.r / fDistanceToWall * 2.0f);
								pWallPixel.g = uint8_t((float)pWallPixel.g / fDistanceToWall * 2.0f);
								pWallPixel.b = uint8_t((float)pWallPixel.b / fDistanceToWall * 2.0f);
							}

							DrawPixel(x, y, pWallPixel);
						}
					}
				}

				m_fDepthBuffer[x] = fDistanceToWall;
			}

			for (auto& o : m_vecObjects)
			{
				o.x += o.speed * o.vx * m_fDeltaTime;
				o.y += o.speed * o.vy * m_fDeltaTime;

				if ((int)o.x < 0 || (int)o.y < 0 || (int)o.x >= m_nMapWidth || (int)o.y >= m_nMapHeight || std::isdigit(m_sMap[(int)o.y * m_nMapWidth + (int)o.x]))
				{
					o.bRemove = true;
					continue;
				}

				float fObjectX = o.x - m_fPlayerX;
				float fObjectY = o.y - m_fPlayerY;

				float fInvDet = 1.0f / (m_fPlaneX * m_fDirY - m_fDirX * m_fPlaneY);

				float fTransformX = fInvDet * (m_fDirY * fObjectX - m_fDirX * fObjectY);
				float fTransformY = fInvDet * (-m_fPlaneY * fObjectX + m_fPlaneX * fObjectY);

				float fAspectRatio = fTransformX / fTransformY;

				int nObjectScreenX = int(float(m_nScreenWidth / 2) * (1.0f + fAspectRatio));

				int nObjectHeight = abs(int((float)m_nScreenHeight / fTransformY));

				int nCeilingY = -nObjectHeight / 2 + m_nScreenHeight / 2;
				int nFloorY = nObjectHeight / 2 + m_nScreenHeight / 2;

				if (nCeilingY < 0)
					nCeilingY = 0;

				if (nFloorY >= m_nScreenHeight)
					nFloorY = m_nScreenHeight - 1;

				int nObjectWidth = abs(int((float)m_nScreenHeight / fTransformY));

				int nCeilingX = -nObjectWidth / 2 + nObjectScreenX;
				int nFloorX = nObjectWidth / 2 + nObjectScreenX;

				if (nCeilingX < 0)
					nCeilingX = 0;

				if (nFloorX >= m_nScreenWidth)
					nFloorX = m_nScreenWidth - 1;

				for (int x = nCeilingX; x < nFloorX; x++)
				{
					int nTexX = (256 * (x - (-nObjectWidth / 2 + nObjectScreenX)) * m_nTexWidth / nObjectWidth) / 256;

					if (fTransformY > 0 && x > 0 && x < m_nScreenWidth && fTransformY < m_fDepthBuffer[x])
					{
						for (int y = nCeilingY; y < nFloorY; y++)
						{
							int d = y * 256 - m_nScreenHeight * 128 + nObjectHeight * 128;
							int nTexY = (d * m_nTexHeight / nObjectHeight) / 256;

							def::Pixel pObjectPixel = ApplyObjectPixel(o.id, nTexX, nTexY);

							if (pObjectPixel.a == 255)
								DrawPixel(x, y, pObjectPixel);

							m_fDepthBuffer[x] = fTransformY;
						}
					}
				}
			}

			auto itRemove = std::remove_if(m_vecObjects.begin(), m_vecObjects.end(), [](sObject& o) { return o.bRemove; });

			if (itRemove != m_vecObjects.end())
				m_vecObjects.erase(itRemove);

			if (GetKeyState(m_kcForward, HELD))
			{
				float fNewX = m_fPlayerX + m_fDirX * m_fMoveSpeed * m_fDeltaTime;
				float fNewY = m_fPlayerY + m_fDirY * m_fMoveSpeed * m_fDeltaTime;

				if ((int)fNewX < m_nMapWidth && (int)fNewX >= 0 && (int)fNewY < m_nMapHeight && (int)fNewY >= 0)
				{
					if (m_sMap[(int)fNewY * m_nMapWidth + (int)fNewX] == '.') m_fPlayerX = fNewX;
					if (m_sMap[(int)fNewY * m_nMapWidth + (int)fNewX] == '.') m_fPlayerY = fNewY;
				}
			}

			if (GetKeyState(m_kcBack, HELD))
			{
				float fNewX = m_fPlayerX - m_fDirX * m_fMoveSpeed * m_fDeltaTime;
				float fNewY = m_fPlayerY - m_fDirY * m_fMoveSpeed * m_fDeltaTime;

				if ((int)fNewX < m_nMapWidth && (int)fNewX >= 0 && (int)fNewY < m_nMapHeight && (int)fNewY >= 0)
				{
					if (m_sMap[(int)fNewY * m_nMapWidth + (int)fNewX] == '.') m_fPlayerX = fNewX;
					if (m_sMap[(int)fNewY * m_nMapWidth + (int)fNewX] == '.') m_fPlayerY = fNewY;
				}
			}

			if (GetKeyState(m_kcRotLeft, HELD))
			{
				float fOldDirX = m_fDirX;
				float fOldPlaneX = m_fPlaneX;

				m_fDirX = m_fDirX * cosf(m_fRotSpeed * m_fDeltaTime) - m_fDirY * sinf(m_fRotSpeed * m_fDeltaTime);
				m_fDirY = fOldDirX * sinf(m_fRotSpeed * m_fDeltaTime) + m_fDirY * cosf(m_fRotSpeed * m_fDeltaTime);

				m_fPlaneX = m_fPlaneX * cosf(m_fRotSpeed * m_fDeltaTime) - m_fPlaneY * sinf(m_fRotSpeed * m_fDeltaTime);
				m_fPlaneY = fOldPlaneX * sinf(m_fRotSpeed * m_fDeltaTime) + m_fPlaneY * cosf(m_fRotSpeed * m_fDeltaTime);
			}

			if (GetKeyState(m_kcRotRight, HELD))
			{
				float fOldDirX = m_fDirX;
				float fOldPlaneX = m_fPlaneX;

				m_fDirX = m_fDirX * cosf(-m_fRotSpeed * m_fDeltaTime) - m_fDirY * sinf(-m_fRotSpeed * m_fDeltaTime);
				m_fDirY = fOldDirX * sinf(-m_fRotSpeed * m_fDeltaTime) + m_fDirY * cosf(-m_fRotSpeed * m_fDeltaTime);

				m_fPlaneX = m_fPlaneX * cosf(-m_fRotSpeed * m_fDeltaTime) - m_fPlaneY * sinf(-m_fRotSpeed * m_fDeltaTime);
				m_fPlaneY = fOldPlaneX * sinf(-m_fRotSpeed * m_fDeltaTime) + m_fPlaneY * cosf(-m_fRotSpeed * m_fDeltaTime);
			}
		}

	};
}

#endif

#pragma once

#pragma region license
/***
*	BSD 3-Clause License
	Copyright (c) 2021, Alex
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

#ifdef DGE_ANIMATED

namespace def
{
	struct sAnimation
	{
	public:
		def::vi2d vPos;
		def::vi2d vSize;

		uint32_t nFrames;

		def::vi2d vLastPos; // do not modify that
		uint32_t nCompletedFrames;
		float fTicks;
	};

	class Animated
	{
	public:
		Animated(def::GFX* gfx)
		{
			m_gfxData = gfx;
		}

	private:
		std::vector<sAnimation> m_vecAnimations;

		def::GFX* m_gfxData;

	public:
		uint32_t AddAnimation(def::vi2d& vPos, def::vi2d& vSize, uint32_t nFrames)
		{
			sAnimation a;
			a.vPos = vPos;
			a.vSize = vSize;
			a.nFrames = nFrames;

			a.vLastPos = def::vi2d(-vSize.x, 0);
			a.nCompletedFrames = 0;
			a.fTicks = 0.0f;

			m_vecAnimations.push_back(a);

			return m_vecAnimations.size() - 1;
		}

		/* returns true if animation is done */
		bool Animate(uint32_t id, def::vi2d& vFilePos, def::vi2d& vFileSize, float fDeltaTime) 
		{
			sAnimation& a = m_vecAnimations[id];

			if (a.nCompletedFrames < a.nFrames)
			{
				if (a.fTicks > 1.0f)
				{
					vFileSize = a.vSize;
					vFilePos = a.vLastPos + def::vi2d(a.vSize.x, 0);

					a.vLastPos = vFilePos;

					a.nCompletedFrames++;

					a.fTicks = 0.0f;
				}

				a.fTicks += fDeltaTime;

				return false; // animation in progress
			}
			else
			{
				a.nCompletedFrames = 0;
				a.fTicks = 0.0f;
				a.vLastPos = def::vi2d(0, 0);

				return true; // animation done
			}
		}

	};
}

#endif

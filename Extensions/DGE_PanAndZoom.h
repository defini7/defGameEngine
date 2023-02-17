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

#ifdef DGE_PANANDZOOM

namespace def
{
	class PanAndZoom
	{
	public:
		PanAndZoom() = default;
		~PanAndZoom() = default;

	private:
		def::vf2d m_vScale;
		def::vf2d m_vOffset;

		def::vf2d m_vStartPan;

		def::GameEngine* pDge = nullptr;

	public:
		def::vi2d WorldToScreen(const def::vf2d& vWorld)
		{
			return ((def::vf2d)vWorld - m_vOffset) * m_vScale;
		}

		def::vf2d ScreenToWorld(const def::vi2d& vScreen)
		{
			return (def::vf2d)vScreen / m_vScale + m_vOffset;
		}

		void Initialize(const def::vf2d& vScale = { 1.0f, 1.0f }, const def::vf2d& vOffset = { 0.0f, 0.0f })
		{
			m_vScale = vScale;
			m_vOffset = vOffset;
		}

		def::vf2d GetScale() const
		{
			return m_vScale;
		}

		def::vf2d GetOffset() const
		{
			return m_vOffset;
		}

		void SetScale(const def::vf2d& vScale)
		{
			m_vScale = vScale;
		}

		void SetOffset(const def::vf2d& vOffset)
		{
			m_vOffset = vOffset;
		}

	    void Handle(def::GameEngine* dge)
		{
			pDge = dge;

			def::vf2d vMouse = pDge->GetMouse();

			if (pDge->GetMouse(2).bPressed)
				m_vStartPan = vMouse;

			if (pDge->GetMouse(2).bHeld)
			{
				m_vOffset -= (vMouse - m_vStartPan) / m_vScale;
				m_vStartPan = vMouse;
			}

			def::vf2d vBeforeZoom = ScreenToWorld(vMouse);

			if (pDge->GetKey(def::Key::EQUAL).bHeld)
				m_vScale *= 1.01f;

			if (pDge->GetKey(def::Key::MINUS).bHeld)
				m_vScale *= 0.99f;

			m_vScale.x = std::max(m_vScale.x, 0.0f);
			m_vScale.y = std::max(m_vScale.y, 0.0f);

			def::vf2d vAfterZoom = ScreenToWorld(vMouse);
			m_vOffset += vBeforeZoom - vAfterZoom;
		}

		void Draw(int32_t x, int32_t y, const def::Pixel& p)
		{
			def::vi2d pos = WorldToScreen(def::vf2d(x, y));

			for (int i = 0; i < m_vScale.x; i++)
				for (int j = 0; j < m_vScale.y; j++)
					pDge->Draw(pos.x + i, pos.y + j, p);
		}

		void DrawSprite(int32_t x, int32_t y, Sprite* sprite)
		{
			if (sprite == nullptr)
				return;

			if (!sprite->IsOk())
				return;

			for (int i = 0; i < sprite->GetWidth(); i++)
				for (int j = 0; j < sprite->GetHeight(); j++)
					Draw(x + i, y + j, sprite->GetPixel(i, j));
		}

		void DrawCircle(int32_t x, int32_t y, int32_t r, const def::Pixel& p)
		{
			def::vi2d pos = WorldToScreen(def::vf2d(x, y));
			pDge->DrawCircle(pos.x, pos.y, r * m_vScale.x, p);
		}

		void FillCircle(int32_t x, int32_t y, int32_t r, const def::Pixel& p)
		{
			def::vi2d pos = WorldToScreen(def::vf2d(x, y));
			pDge->FillCircle(pos.x, pos.y, r * m_vScale.x, p);
		}

		void DrawRectangle(int32_t x, int32_t y, int32_t sx, int32_t sy, const def::Pixel& p)
		{
			def::vi2d pos = WorldToScreen(def::vf2d(x, y));
			pDge->DrawRectangle(pos.x, pos.y, sx * m_vScale.x, sy * m_vScale.y, p);
		}

		void FillRectangle(int32_t x, int32_t y, int32_t sx, int32_t sy, const def::Pixel& p)
		{
			def::vi2d pos = WorldToScreen(def::vf2d(x, y));
			pDge->FillRectangle(pos.x, pos.y, sx * m_vScale.x, sy * m_vScale.y, p);
		}

	};
}

#endif

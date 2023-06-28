#pragma once

#pragma region license
/***
*	BSD 3-Clause License
	Copyright (c) 2021, 2022, 2023 Alex
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

namespace def
{
	class PanAndZoom
	{
	private:
		def::vf2d m_Scale;
		def::vf2d m_Offset;

		def::vf2d m_StartPan;

		def::GameEngine* m_Dge = nullptr;

	public:
		def::vi2d WorldToScreen(const def::vf2d& vWorld);
		def::vf2d ScreenToWorld(const def::vi2d& vScreen);

		void Initialize(def::GameEngine* dge, const def::vf2d& scale = { 1.0f, 1.0f }, const def::vf2d& offset = { 0.0f, 0.0f });

		def::vf2d GetScale() const;
		def::vf2d GetOffset() const;

		void SetScale(const def::vf2d& scale);
		void SetOffset(const def::vf2d& offset);
		void Handle();
		void Draw(int32_t x, int32_t y, const def::Pixel& p);
		void DrawSprite(int32_t x, int32_t y, Sprite* sprite);
		void DrawCircle(float x, float y, float r, const def::Pixel& p);
		void FillCircle(float x, float y, float r, const def::Pixel& p);
		void DrawRectangle(float x, float y, float sx, float sy, const def::Pixel& p);
		void FillRectangle(float x, float y, float sx, float sy, const def::Pixel& p);

	};

#ifdef DGE_PANANDZOOM
#undef DGE_PANANDZOOM

	def::vi2d PanAndZoom::WorldToScreen(const def::vf2d& world)
	{
		return ((def::vf2d)world - m_Offset) * m_Scale;
	}

	def::vf2d PanAndZoom::ScreenToWorld(const def::vi2d& screen)
	{
		return (def::vf2d)screen / m_Scale + m_Offset;
	}

	void PanAndZoom::Initialize(def::GameEngine* dge, const def::vf2d& scale, const def::vf2d& offset)
	{
		dge = dge;

		m_Scale = scale;
		m_Offset = offset;
	}

	def::vf2d PanAndZoom::GetScale() const
	{
		return m_Scale;
	}

	def::vf2d PanAndZoom::GetOffset() const
	{
		return m_Offset;
	}

	void PanAndZoom::SetScale(const def::vf2d& scale)
	{
		m_Scale = scale;
	}

	void PanAndZoom::SetOffset(const def::vf2d& offset)
	{
		m_Offset = offset;
	}

	void PanAndZoom::Handle()
	{
		def::vf2d mouse = m_Dge->GetMouse();

		if (m_Dge->GetMouse(2).pressed)
			m_StartPan = mouse;

		if (m_Dge->GetMouse(2).held)
		{
			m_Offset -= (mouse - m_StartPan) / m_Scale;
			m_StartPan = mouse;
		}

		def::vf2d beforeZoom = ScreenToWorld(mouse);

		if (m_Dge->GetKey(def::Key::EQUAL).held)
			m_Scale *= 1.01f;

		if (m_Dge->GetKey(def::Key::MINUS).held)
			m_Scale *= 0.99f;

		m_Scale.x = std::max(m_Scale.x, 0.0f);
		m_Scale.y = std::max(m_Scale.y, 0.0f);

		def::vf2d afterZoom = ScreenToWorld(mouse);
		m_Offset += beforeZoom - afterZoom;
	}

	void PanAndZoom::Draw(int32_t x, int32_t y, const def::Pixel& p)
	{
		def::vi2d pos = WorldToScreen(def::vf2d(x, y));

		for (int i = 0; i < m_Scale.x; i++)
			for (int j = 0; j < m_Scale.y; j++)
				m_Dge->Draw(pos.x + i, pos.y + j, p);
	}

	void PanAndZoom::DrawSprite(int32_t x, int32_t y, Sprite* sprite)
	{
		if (sprite == nullptr)
			return;

		for (int i = 0; i < sprite->width; i++)
			for (int j = 0; j < sprite->height; j++)
				Draw(x + i, y + j, sprite->GetPixel(i, j));
	}

	void PanAndZoom::DrawCircle(float x, float y, float r, const def::Pixel& p)
	{
		def::vi2d pos = WorldToScreen(def::vf2d(x, y));
		m_Dge->DrawCircle(pos, r * m_Scale.x, p);
	}

	void PanAndZoom::FillCircle(float x, float y, float r, const def::Pixel& p)
	{
		def::vi2d pos = WorldToScreen(def::vf2d(x, y));
		m_Dge->FillCircle(pos, r * m_Scale.x, p);
	}

	void PanAndZoom::DrawRectangle(float x, float y, float sx, float sy, const def::Pixel& p)
	{
		def::vi2d pos = WorldToScreen(def::vf2d(x, y));
		m_Dge->DrawRectangle(pos, def::vi2d(sx * m_Scale.x, sy * m_Scale.y), p);
	}

	void PanAndZoom::FillRectangle(float x, float y, float sx, float sy, const def::Pixel& p)
	{
		def::vi2d pos = WorldToScreen(def::vf2d(x, y));
		m_Dge->FillRectangle(pos, def::vi2d(sx * m_Scale.x, sy * m_Scale.y), p);
	}

#endif
}


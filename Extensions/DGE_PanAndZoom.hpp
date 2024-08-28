#ifndef DGE_PAN_AND_ZOOM_HPP
#define DGE_PAN_AND_ZOOM_HPP

#pragma region License

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

#pragma endregion

#pragma region Includes

#include "defGameEngine.hpp"

#pragma endregion

namespace def
{
	class PanAndZoom
	{
	public:
		PanAndZoom();
		~PanAndZoom() = default;

	public:
		vf2d ScreenToWorld(const vf2d& pos) const;
		vf2d WorldToScreen(const vf2d& pos) const;

		vf2d GetScale() const;
		vf2d GetOffset() const;

		float GetScaleX() const;
		float GetScaleY() const;

		float GetOffsetX() const;
		float GetOffsetY() const;

		void SetScale(float x, float y);
		void SetScale(const vf2d& scale);

		void SetOffset(float x, float y);
		void SetOffset(const vf2d& offset);

		void Zoom(float factor, float x, float y);
		void Zoom(float factor, const vf2d& pos);

		void StartPan(float x, float y);
		void StartPan(const vf2d& pos);

		void UpdatePan(float x, float y);
		void UpdatePan(const vf2d& pos);

		bool IsVisible(const vf2d& point);

	public:
		void DrawTexture(const vf2d& pos, const Texture* tex, const vf2d& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);
		void DrawTexture(float x, float y, const Texture* tex, float scaleX = 1.0f, float scaleY = 1.0f, const Pixel& tint = WHITE);

		void DrawPartialTexture(const vf2d& pos, const vf2d& filePos, const vf2d& fileSize, const Texture* tex, const vf2d& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);
		void DrawPartialTexture(float x, float y, float filePosX, float filePosY, float fileSizeX, float fileSizeY, const Texture* tex, float scaleX = 1.0f, float scaleY = 1.0f, const Pixel& tint = WHITE);

		void DrawWarpedTexture(const std::vector<vf2d>& points, const Texture* tex, const Pixel& tint = WHITE);

		void DrawRotatedTexture(const vf2d& pos, float rot, const Texture* tex, const vf2d& center = { 0.0f, 0.0f }, const vf2d& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);
		void DrawRotatedTexture(float x, float y, float rot, const Texture* tex, float centerX = 0.0f, float centerY = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f, const Pixel& tint = WHITE);

		void DrawPartialRotatedTexture(const vf2d& pos, const vf2d& filePos, const vf2d& fileSize, float r, const Texture* tex, const vf2d& center = { 0.0f, 0.0f }, const vf2d& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);
		void DrawPartialRotatedTexture(float x, float y, float filePosX, float filePosY, float fileWidth, float fileHeight, float r, const Texture* tex, float centerX = 0.0f, float centerY = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f, const Pixel& tint = WHITE);

		void DrawTexturePolygon(const std::vector<vf2d>& verts, const std::vector<Pixel>& cols, Texture::Structure structure);

		void DrawTextureLine(const vi2d& pos1, const vi2d& pos2, const Pixel& col = WHITE);

		void DrawTextureTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col = WHITE);
		void DrawTextureRectangle(const vi2d& pos, const vi2d& size, const Pixel& col = WHITE);
		void DrawTextureCircle(const vi2d& pos, int radius, const Pixel& col = WHITE);

		void FillTextureTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col = WHITE);
		void FillTextureRectangle(const vi2d& pos, const vi2d& size, const Pixel& col = WHITE);
		void FillTextureCircle(const vi2d& pos, int radius, const Pixel& col = WHITE);

		void GradientTextureTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col1 = WHITE, const Pixel& col2 = WHITE, const Pixel& col3 = WHITE);
		void GradientTextureRectangle(const vi2d& pos, const vi2d& size, const Pixel& colTL = WHITE, const Pixel& colTR = WHITE, const Pixel& colBR = WHITE, const Pixel& colBL = WHITE);

		void DrawTextureString(const vi2d& pos, std::string_view text, const Pixel& col = def::WHITE, const vf2d& scale = { 1.0f, 1.0f });
	
	private:
		vf2d m_Offset;
		vf2d m_Scale;
		vf2d m_PanPrev;

		GameEngine* m_Engine;

	};

#ifdef DGE_PAN_AND_ZOOM
#undef DGE_PAN_AND_ZOOM

	PanAndZoom::PanAndZoom()
	{
		m_Scale = { 1.0f, 1.0f };
		m_Engine = GameEngine::s_Engine;
	}

	vf2d PanAndZoom::GetScale() const
	{
		return m_Scale;
	}

	vf2d PanAndZoom::GetOffset() const
	{
		return m_Offset;
	}

	float PanAndZoom::GetScaleX() const
	{
		return m_Scale.x;
	}

	float PanAndZoom::GetScaleY() const
	{
		return m_Scale.y;
	}

	float PanAndZoom::GetOffsetX() const
	{
		return m_Offset.x;
	}

	float PanAndZoom::GetOffsetY() const
	{
		return m_Offset.y;
	}

	void PanAndZoom::SetScale(float x, float y)
	{
		m_Scale.x = x;
		m_Scale.y = y;
	}

	void PanAndZoom::SetScale(const vf2d& scale)
	{
		m_Scale = scale;
	}

	void PanAndZoom::SetOffset(float x, float y)
	{
		m_Offset.x = x;
		m_Offset.y = y;
	}

	void PanAndZoom::SetOffset(const vf2d& offset)
	{
		m_Offset = offset;
	}

	void PanAndZoom::Zoom(float factor, float x, float y)
	{
		Zoom(factor, { x, y });
	}

	void PanAndZoom::Zoom(float factor, const vf2d& pos)
	{
		m_Offset += (factor - 1.0f) * pos;
		m_Scale *= factor;
	}

	void PanAndZoom::StartPan(float x, float y)
	{
		m_PanPrev.x = x;
		m_PanPrev.y = y;
	}

	void PanAndZoom::StartPan(const vf2d& pos)
	{
		m_PanPrev = pos;
	}

	void PanAndZoom::UpdatePan(float x, float y)
	{
		StartPan({ x, y });
	}

	void PanAndZoom::UpdatePan(const vf2d& pos)
	{
		m_Offset -= (pos - m_PanPrev) / m_Scale;
		StartPan(pos);
	}

	bool PanAndZoom::IsVisible(const vf2d& point)
	{
		vf2d transformed = WorldToScreen(point);

		return transformed >= vf2d(0.0f, 0.0f) && transformed < m_Engine->GetScreenSize();
	}

	void PanAndZoom::DrawTexture(const vf2d& pos, const Texture* tex, const vf2d& scale, const Pixel& tint)
	{
		m_Engine->DrawTexture(WorldToScreen(pos), tex, scale * m_Scale, tint);
	}

	void PanAndZoom::DrawTexture(float x, float y, const Texture* tex, float scaleX, float scaleY, const Pixel& tint)
	{
		DrawTexture({ x, y }, tex, { scaleX, scaleY }, tint);
	}

	void PanAndZoom::DrawPartialTexture(const vf2d& pos, const vf2d& filePos, const vf2d& fileSize, const Texture* tex, const vf2d& scale, const Pixel& tint)
	{
		m_Engine->DrawPartialTexture(WorldToScreen(pos), filePos, fileSize, tex, scale * m_Scale, tint);
	}

	void PanAndZoom::DrawPartialTexture(float x, float y, float filePosX, float filePosY, float fileSizeX, float fileSizeY, const Texture* tex, float scaleX, float scaleY, const Pixel& tint)
	{
		DrawPartialTexture({ x, y }, { filePosX, filePosY }, { fileSizeX, fileSizeY }, tex, { scaleX, scaleY }, tint);
	}

	void PanAndZoom::DrawWarpedTexture(const std::vector<vf2d>& points, const Texture* tex, const Pixel& tint)
	{
		std::vector<vf2d> transformed(points.size());

		std::transform(points.begin(), points.end(), transformed.begin(),
			[&](const vf2d& p) { return WorldToScreen(p); });

		m_Engine->DrawWarpedTexture(transformed, tex, tint);
	}

	void PanAndZoom::DrawRotatedTexture(const vf2d& pos, float rot, const Texture* tex, const vf2d& center, const vf2d& scale, const Pixel& tint)
	{
		m_Engine->DrawRotatedTexture(WorldToScreen(pos), rot, tex, center, scale * m_Scale, tint);
	}

	void PanAndZoom::DrawRotatedTexture(float x, float y, float rot, const Texture* tex, float centerX, float centerY, float scaleX, float scaleY, const Pixel& tint)
	{
		DrawRotatedTexture({ x, y }, rot, tex, { centerX, centerY }, { scaleX, scaleY }, tint);
	}

	void PanAndZoom::DrawPartialRotatedTexture(const vf2d& pos, const vf2d& filePos, const vf2d& fileSize, float rot, const Texture* tex, const vf2d& center, const vf2d& scale, const Pixel& tint)
	{
		m_Engine->DrawPartialRotatedTexture(WorldToScreen(pos), filePos, fileSize, rot, tex, center, scale * m_Scale, tint);
	}

	void PanAndZoom::DrawPartialRotatedTexture(float x, float y, float filePosX, float filePosY, float fileWidth, float fileHeight, float rot, const Texture* tex, float centerX, float centerY, float scaleX, float scaleY, const Pixel& tint)
	{
		m_Engine->DrawPartialRotatedTexture({ x, y }, { filePosX, filePosY }, { fileWidth, fileHeight }, rot, tex, { centerX, centerY }, { scaleX, scaleY }, tint);
	}

	void PanAndZoom::DrawTexturePolygon(const std::vector<vf2d>& verts, const std::vector<Pixel>& cols, Texture::Structure structure)
	{
		std::vector<vf2d> transformed(verts.size());

		std::transform(verts.begin(), verts.end(), transformed.begin(),
			[&](const vf2d& p) { return WorldToScreen(p); });

		m_Engine->DrawTexturePolygon(transformed, cols, structure);
	}

	void PanAndZoom::DrawTextureLine(const vi2d& pos1, const vi2d& pos2, const Pixel& col)
	{
		DrawTexturePolygon({ pos1, pos2 }, { col, col }, Texture::Structure::WIREFRAME);
	}

	void PanAndZoom::DrawTextureTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col)
	{
		DrawTexturePolygon({ pos1, pos2, pos3 }, { col, col, col }, Texture::Structure::WIREFRAME);
	}

	void PanAndZoom::FillTextureTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col)
	{
		DrawTexturePolygon({ pos1, pos2, pos3 }, { col, col, col }, Texture::Structure::FAN);
	}

	void PanAndZoom::DrawTextureRectangle(const vi2d& pos, const vi2d& size, const Pixel& col)
	{
		DrawTexturePolygon({ pos, { float(pos.x + size.x), (float)pos.y }, pos + size, { (float)pos.x, float(pos.y + size.y) } }, { col, col, col, col }, Texture::Structure::WIREFRAME);
	}

	void PanAndZoom::FillTextureRectangle(const vi2d& pos, const vi2d& size, const Pixel& col)
	{
		DrawTexturePolygon({ pos, { float(pos.x + size.x), (float)pos.y }, pos + size, { (float)pos.x, float(pos.y + size.y) } }, { col, col, col, col }, Texture::Structure::FAN);
	}

	void PanAndZoom::DrawTextureCircle(const vi2d& pos, int radius, const Pixel& col)
	{
		m_Engine->DrawTextureCircle(WorldToScreen(pos), radius * m_Scale.x, col);
	}

	void PanAndZoom::FillTextureCircle(const vi2d& pos, int radius, const Pixel& col)
	{
		m_Engine->FillTextureCircle(WorldToScreen(pos), radius * m_Scale.x, col);
	}

	void PanAndZoom::GradientTextureTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col1, const Pixel& col2, const Pixel& col3)
	{
		m_Engine->GradientTextureTriangle(
			WorldToScreen(pos1), WorldToScreen(pos2), WorldToScreen(pos3),
			col1, col2, col3
		);
	}

	void PanAndZoom::GradientTextureRectangle(const vi2d& pos, const vi2d& size, const Pixel& colTL, const Pixel& colTR, const Pixel& colBR, const Pixel& colBL)
	{
		m_Engine->GradientTextureRectangle(WorldToScreen(pos), size * m_Scale, colTL, colTR, colBR, colBL);
	}

	void PanAndZoom::DrawTextureString(const vi2d& pos, std::string_view text, const Pixel& col, const vf2d& scale)
	{
		m_Engine->DrawTextureString(WorldToScreen(pos), text, col, scale * m_Scale);
	}

	vf2d PanAndZoom::ScreenToWorld(const vf2d& pos) const
	{
		return pos / m_Scale + m_Offset;
	}

	vf2d PanAndZoom::WorldToScreen(const vf2d& pos) const
	{
		return (pos - m_Offset) * m_Scale;
	}

#endif

}

#endif

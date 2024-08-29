#ifndef DGE_AFFINE_TRANSFORMS_HPP
#define DGE_AFFINE_TRANSFORMS_HPP

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
	class AffineTransforms
	{
	public:
		AffineTransforms();
		~AffineTransforms() = default;

	public:
		vf2d ScreenToWorld(const vf2d& pos) const;
		vf2d WorldToScreen(const vf2d& pos) const;

		vf2d GetScale() const;
		vf2d GetOffset() const;

		vf2d GetOrigin();
		vf2d GetEnd();

		void SetScale(const vf2d& scale);
		void SetOffset(const vf2d& offset);

		void Zoom(float factor, const vf2d& pos);

		void StartPan(const vf2d& pos);
		void UpdatePan(const vf2d& pos);

		bool IsVisible(const vf2d& point);

	public:
		void DrawTexture(const vf2d& pos, const Texture* tex, const vf2d& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);
		void DrawPartialTexture(const vf2d& pos, const Texture* tex, const vf2d& filePos, const vf2d& fileSize, const vf2d& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);

		void DrawWarpedTexture(const std::vector<vf2d>& points, const Texture* tex, const Pixel& tint = WHITE);

		void DrawRotatedTexture(const vf2d& pos, const Texture* tex, float rotation, const vf2d& center = { 0.0f, 0.0f }, const vf2d& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);
		void DrawPartialRotatedTexture(const vf2d& pos, const Texture* tex, const vf2d& filePos, const vf2d& fileSize, float rotation, const vf2d& center = { 0.0f, 0.0f }, const vf2d& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);

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

#ifdef DGE_AFFINE_TRANSFORMS
#undef DGE_AFFINE_TRANSFORMS

	AffineTransforms::AffineTransforms()
	{
		m_Scale = { 1.0f, 1.0f };
		m_Engine = GameEngine::s_Engine;
	}

	vf2d AffineTransforms::GetScale() const
	{
		return m_Scale;
	}

	vf2d AffineTransforms::GetOffset() const
	{
		return m_Offset;
	}

	vf2d AffineTransforms::GetOrigin()
	{
		return ScreenToWorld({ 0, 0 });
	}

	vf2d AffineTransforms::GetEnd()
	{
		return ScreenToWorld(m_Engine->GetScreenSize());
	}

	void AffineTransforms::SetScale(const vf2d& scale)
	{
		m_Scale = scale;
	}

	void AffineTransforms::SetOffset(const vf2d& offset)
	{
		m_Offset = offset;
	}

	void AffineTransforms::Zoom(float factor, const vf2d& pos)
	{
		vf2d before = ScreenToWorld(pos);
		m_Scale *= factor;
		vf2d after = ScreenToWorld(pos);

		m_Offset += before - after;
	}

	void AffineTransforms::StartPan(const vf2d& pos)
	{
		m_PanPrev = pos;
	}

	void AffineTransforms::UpdatePan(const vf2d& pos)
	{
		m_Offset -= (pos - m_PanPrev) / m_Scale;
		StartPan(pos);
	}

	bool AffineTransforms::IsVisible(const vf2d& point)
	{
		vf2d p = WorldToScreen(point);

		return p.x >= 0.0f && p.y >= 0.0f && p.x < m_Engine->ScreenWidth() && p.y < m_Engine->ScreenHeight();
	}

	void AffineTransforms::DrawTexture(const vf2d& pos, const Texture* tex, const vf2d& scale, const Pixel& tint)
	{
		m_Engine->DrawTexture(WorldToScreen(pos), tex, scale * m_Scale, tint);
	}

	void AffineTransforms::DrawPartialTexture(const vf2d& pos, const Texture* tex, const vf2d& filePos, const vf2d& fileSize, const vf2d& scale, const Pixel& tint)
	{
		m_Engine->DrawPartialTexture(WorldToScreen(pos), tex, filePos, fileSize, scale * m_Scale, tint);
	}

	void AffineTransforms::DrawWarpedTexture(const std::vector<vf2d>& points, const Texture* tex, const Pixel& tint)
	{
		std::vector<vf2d> transformed(points.size());

		std::transform(points.begin(), points.end(), transformed.begin(),
			[&](const vf2d& p) { return WorldToScreen(p); });

		m_Engine->DrawWarpedTexture(transformed, tex, tint);
	}

	void AffineTransforms::DrawRotatedTexture(const vf2d& pos, const Texture* tex, float rotation, const vf2d& center, const vf2d& scale, const Pixel& tint)
	{
		m_Engine->DrawRotatedTexture(WorldToScreen(pos), tex, rotation, center, scale * m_Scale, tint);
	}

	void AffineTransforms::DrawPartialRotatedTexture(const vf2d& pos, const Texture* tex, const vf2d& filePos, const vf2d& fileSize, float rotation, const vf2d& center, const vf2d& scale, const Pixel& tint)
	{
		m_Engine->DrawPartialRotatedTexture(WorldToScreen(pos), tex, filePos, fileSize, rotation, center, scale * m_Scale, tint);
	}

	void AffineTransforms::DrawTexturePolygon(const std::vector<vf2d>& verts, const std::vector<Pixel>& cols, Texture::Structure structure)
	{
		std::vector<vf2d> transformed(verts.size());

		std::transform(verts.begin(), verts.end(), transformed.begin(),
			[&](const vf2d& p) { return WorldToScreen(p); });

		m_Engine->DrawTexturePolygon(transformed, cols, structure);
	}

	void AffineTransforms::DrawTextureLine(const vi2d& pos1, const vi2d& pos2, const Pixel& col)
	{
		DrawTexturePolygon({ pos1, pos2 }, { col, col }, Texture::Structure::WIREFRAME);
	}

	void AffineTransforms::DrawTextureTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col)
	{
		DrawTexturePolygon({ pos1, pos2, pos3 }, { col, col, col }, Texture::Structure::WIREFRAME);
	}

	void AffineTransforms::FillTextureTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col)
	{
		DrawTexturePolygon({ pos1, pos2, pos3 }, { col, col, col }, Texture::Structure::FAN);
	}

	void AffineTransforms::DrawTextureRectangle(const vi2d& pos, const vi2d& size, const Pixel& col)
	{
		DrawTexturePolygon({ pos, { float(pos.x + size.x), (float)pos.y }, pos + size, { (float)pos.x, float(pos.y + size.y) } }, { col, col, col, col }, Texture::Structure::WIREFRAME);
	}

	void AffineTransforms::FillTextureRectangle(const vi2d& pos, const vi2d& size, const Pixel& col)
	{
		DrawTexturePolygon({ pos, { float(pos.x + size.x), (float)pos.y }, pos + size, { (float)pos.x, float(pos.y + size.y) } }, { col, col, col, col }, Texture::Structure::FAN);
	}

	void AffineTransforms::DrawTextureCircle(const vi2d& pos, int radius, const Pixel& col)
	{
		m_Engine->DrawTextureCircle(WorldToScreen(pos), int((float)radius * m_Scale.x), col);
	}

	void AffineTransforms::FillTextureCircle(const vi2d& pos, int radius, const Pixel& col)
	{
		m_Engine->FillTextureCircle(WorldToScreen(pos), int((float)radius * m_Scale.x), col);
	}

	void AffineTransforms::GradientTextureTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col1, const Pixel& col2, const Pixel& col3)
	{
		m_Engine->GradientTextureTriangle(
			WorldToScreen(pos1), WorldToScreen(pos2), WorldToScreen(pos3),
			col1, col2, col3
		);
	}

	void AffineTransforms::GradientTextureRectangle(const vi2d& pos, const vi2d& size, const Pixel& colTL, const Pixel& colTR, const Pixel& colBR, const Pixel& colBL)
	{
		m_Engine->GradientTextureRectangle(WorldToScreen(pos), size * m_Scale, colTL, colTR, colBR, colBL);
	}

	void AffineTransforms::DrawTextureString(const vi2d& pos, std::string_view text, const Pixel& col, const vf2d& scale)
	{
		m_Engine->DrawTextureString(WorldToScreen(pos), text, col, scale * m_Scale);
	}

	vf2d AffineTransforms::ScreenToWorld(const vf2d& pos) const
	{
		return pos / m_Scale + m_Offset;
	}

	vf2d AffineTransforms::WorldToScreen(const vf2d& pos) const
	{
		return (pos - m_Offset) * m_Scale;
	}

#endif

}

#endif

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

		bool IsPointVisible(const vf2d& point);
		bool IsRectVisible(const vf2d& pos, const vf2d& size);

	public:
		bool Draw(const vi2d& pos, Pixel col = WHITE);
		virtual bool Draw(int x, int y, Pixel col = WHITE);

		void DrawLine(const vi2d& pos1, const vi2d& pos2, const Pixel& col = WHITE);
		virtual void DrawLine(int x1, int y1, int x2, int y2, const Pixel& col = WHITE);

		void DrawTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col = WHITE);
		virtual void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Pixel& col = WHITE);

		void FillTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col = WHITE);
		virtual void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Pixel& col = WHITE);

		void DrawRectangle(const vi2d& pos, const vi2d& size, const Pixel& col = WHITE);
		virtual void DrawRectangle(int x, int y, int sizeX, int sizeY, const Pixel& col = WHITE);

		void FillRectangle(const vi2d& pos, const vi2d& size, const Pixel& col = WHITE);
		virtual void FillRectangle(int x, int y, int sizeX, int sizeY, const Pixel& col = WHITE);

		void DrawCircle(const vi2d& pos, int radius, const Pixel& col = WHITE);
		virtual void DrawCircle(int x, int y, int radius, const Pixel& col = WHITE);

		void FillCircle(const vi2d& pos, int radius, const Pixel& col = WHITE);
		virtual void FillCircle(int x, int y, int radius, const Pixel& col = WHITE);

		void DrawEllipse(const vi2d& pos, const vi2d& size, const Pixel& col = WHITE);
		virtual void DrawEllipse(int x, int y, int sizeX, int sizeY, const Pixel& col = WHITE);

		void FillEllipse(const vi2d& pos, const vi2d& size, const Pixel& col = WHITE);
		virtual void FillEllipse(int x, int y, int sizeX, int sizeY, const Pixel& col = WHITE);

		void DrawSprite(const vi2d& pos, const Sprite* sprite);
		virtual void DrawSprite(int x, int y, const Sprite* sprite);

		void DrawPartialSprite(const vi2d& pos, const vi2d& filePos, const vi2d& fileSize, const Sprite* sprite);
		virtual void DrawPartialSprite(int x, int y, int fileX, int fileY, int fileSizeX, int fileSizeY, const Sprite* sprite);

		void DrawWireFrameModel(const std::vector<vf2d>& modelCoordinates, const vf2d& pos, float rotation = 0.0f, float scale = 1.0f, const Pixel& col = WHITE);
		virtual void DrawWireFrameModel(const std::vector<vf2d>& modelCoordinates, float x, float y, float rotation = 0.0f, float scale = 1.0f, const Pixel& col = WHITE);

		void FillWireFrameModel(const std::vector<vf2d>& modelCoordinates, const vf2d& pos, float rotation = 0.0f, float scale = 1.0f, const Pixel& col = WHITE);
		virtual void FillWireFrameModel(const std::vector<vf2d>& modelCoordinates, float x, float y, float rotation = 0.0f, float scale = 1.0f, const Pixel& col = WHITE);

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

	bool AffineTransforms::IsPointVisible(const vf2d& point)
	{
		vf2d p = WorldToScreen(point);

		return p.x >= 0.0f && p.y >= 0.0f && p.x < m_Engine->ScreenWidth() && p.y < m_Engine->ScreenHeight();
	}

	bool AffineTransforms::IsRectVisible(const vf2d& pos, const vf2d& size)
	{
		vf2d p = WorldToScreen(pos);
		vf2d s = size * m_Scale;

		return p.x + s.x >= 0.0f && p.y + s.y >= 0.0f && p.x < m_Engine->ScreenWidth() && p.y < m_Engine->ScreenHeight();
	}

	bool AffineTransforms::Draw(const vi2d& pos, Pixel col)
	{
		return m_Engine->Draw(WorldToScreen(pos), col);
	}

	bool AffineTransforms::Draw(int x, int y, Pixel col)
	{
		return Draw({ x, y }, col);
	}

	void AffineTransforms::DrawLine(const vi2d& pos1, const vi2d& pos2, const Pixel& col)
	{
		m_Engine->DrawLine(WorldToScreen(pos1), WorldToScreen(pos2), col);
	}

	void AffineTransforms::DrawLine(int x1, int y1, int x2, int y2, const Pixel& col)
	{
		DrawLine({ x1, y1 }, { x2, y2 }, col);
	}

	void AffineTransforms::DrawTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col)
	{
		m_Engine->DrawTriangle(WorldToScreen(pos1), WorldToScreen(pos2), WorldToScreen(pos3), col);
	}

	void AffineTransforms::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Pixel& col)
	{
		DrawTriangle({ x1, y1 }, { x2, y2 }, { x3, y3 }, col);
	}

	void AffineTransforms::FillTriangle(const vi2d& pos1, const vi2d& pos2, const vi2d& pos3, const Pixel& col)
	{
		m_Engine->FillTriangle(WorldToScreen(pos1), WorldToScreen(pos2), WorldToScreen(pos3), col);
	}

	void AffineTransforms::FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Pixel& col)
	{
		FillTriangle({ x1, y1 }, { x2, y2 }, { x3, y3 }, col);
	}

	void AffineTransforms::DrawRectangle(const vi2d& pos, const vi2d& size, const Pixel& col)
	{
		m_Engine->DrawRectangle(WorldToScreen(pos), size * m_Scale, col);
	}

	void AffineTransforms::DrawRectangle(int x, int y, int sizeX, int sizeY, const Pixel& col)
	{
		DrawRectangle({ x, y }, { sizeX, sizeY }, col);
	}

	void AffineTransforms::FillRectangle(const vi2d& pos, const vi2d& size, const Pixel& col)
	{
		m_Engine->FillRectangle(WorldToScreen(pos), size * m_Scale, col);
	}

	void AffineTransforms::FillRectangle(int x, int y, int sizeX, int sizeY, const Pixel& col)
	{
		FillRectangle({ x, y }, { sizeX, sizeY }, col);
	}

	void AffineTransforms::DrawCircle(const vi2d& pos, int radius, const Pixel& col)
	{
		m_Engine->DrawCircle(WorldToScreen(pos), (float)radius * m_Scale.x, col);
	}

	void AffineTransforms::DrawCircle(int x, int y, int radius, const Pixel& col)
	{
		DrawCircle({ x, y }, radius, col);
	}

	void AffineTransforms::FillCircle(const vi2d& pos, int radius, const Pixel& col)
	{
		m_Engine->FillCircle(WorldToScreen(pos), (float)radius * m_Scale.x, col);
	}

	void AffineTransforms::FillCircle(int x, int y, int radius, const Pixel& col)
	{
		FillCircle({ x, y }, radius, col);
	}

	void AffineTransforms::DrawEllipse(const vi2d& pos, const vi2d& size, const Pixel& col)
	{
		m_Engine->DrawEllipse(WorldToScreen(pos), size * m_Scale, col);
	}

	void AffineTransforms::DrawEllipse(int x, int y, int sizeX, int sizeY, const Pixel& col)
	{
		DrawEllipse({ x, y }, { sizeX, sizeY }, col);
	}

	void AffineTransforms::FillEllipse(const vi2d& pos, const vi2d& size, const Pixel& col)
	{
		m_Engine->FillEllipse(WorldToScreen(pos), size * m_Scale, col);
	}

	void AffineTransforms::FillEllipse(int x, int y, int sizeX, int sizeY, const Pixel& col)
	{
		FillEllipse({ x, y }, { sizeX, sizeY }, col);
	}

	void AffineTransforms::DrawSprite(const vi2d& pos, const Sprite* sprite)
	{
		vf2d size = vf2d(sprite->size) * m_Scale;

		if (IsRectVisible(pos, sprite->size))
		{
			vi2d spriteStart = WorldToScreen(pos);
			vi2d spriteEnd = WorldToScreen(pos + sprite->size);

			vi2d screenStart = spriteStart.max({ 0, 0 });
			vi2d screenEnd = spriteEnd.min(m_Engine->GetScreenSize());

			vf2d step = 1.0f / size;

			vi2d pixel;
			for (pixel.y = screenStart.y; pixel.y < screenEnd.y; pixel.y++)
				for (pixel.x = screenStart.x; pixel.x < screenEnd.x; pixel.x++)
				{
					vf2d sampled = vf2d(pixel - spriteStart) * step;
					m_Engine->Draw(pixel, sprite->Sample(sampled, Sprite::SampleMethod::LINEAR, Sprite::WrapMethod::NONE));
				}
		}
	}

	void AffineTransforms::DrawSprite(int x, int y, const Sprite* sprite)
	{
		DrawSprite({ x, y }, sprite);
	}

	void AffineTransforms::DrawPartialSprite(const vi2d& pos, const vi2d& filePos, const vi2d& fileSize, const Sprite* sprite)
	{
		vf2d size = sprite->size * m_Scale;

		if (IsRectVisible(pos, size))
		{
			vf2d spriteStep = 1.0f / sprite->size;
			vf2d screenStep = 1.0f / size;

			vi2d start = WorldToScreen(pos);
			vi2d end = start + size;

			vi2d pixel;
			for (pixel.y = start.y; pixel.y < end.y; pixel.y++)
				for (pixel.x = start.x; pixel.x < end.x; pixel.x++)
				{
					vf2d sampled = (vf2d(pixel - start) * screenStep * vf2d(size) + vf2d(filePos)) * spriteStep;
					m_Engine->Draw(pixel, sprite->Sample(sampled, Sprite::SampleMethod::LINEAR, Sprite::WrapMethod::NONE));
				}
		}
	}

	void AffineTransforms::DrawPartialSprite(int x, int y, int fileX, int fileY, int fileSizeX, int fileSizeY, const Sprite* sprite)
	{
		DrawPartialSprite({ x, y }, { fileX, fileY }, { fileSizeX, fileSizeY }, sprite);
	}

	void AffineTransforms::DrawWireFrameModel(const std::vector<vf2d>& modelCoordinates, const vf2d& pos, float rotation, float scale, const Pixel& col)
	{
		std::vector<vf2d> transformed(modelCoordinates.size());

		std::transform(modelCoordinates.begin(), modelCoordinates.end(), transformed.begin(),
			[&](const vf2d& p) { return p * m_Scale; });

		m_Engine->DrawWireFrameModel(transformed, WorldToScreen(pos), rotation, scale, col);
	}

	void AffineTransforms::DrawWireFrameModel(const std::vector<vf2d>& modelCoordinates, float x, float y, float rotation, float scale, const Pixel& col)
	{
		DrawWireFrameModel(modelCoordinates, { x, y }, rotation, scale, col);
	}

	void AffineTransforms::FillWireFrameModel(const std::vector<vf2d>& modelCoordinates, const vf2d& pos, float rotation, float scale, const Pixel& col)
	{
		std::vector<vf2d> transformed(modelCoordinates.size());

		std::transform(modelCoordinates.begin(), modelCoordinates.end(), transformed.begin(),
			[&](const vf2d& p) { return p * m_Scale; });

		m_Engine->FillWireFrameModel(transformed, WorldToScreen(pos), rotation, scale, col);
	}

	void AffineTransforms::FillWireFrameModel(const std::vector<vf2d>& modelCoordinates, float x, float y, float rotation, float scale, const Pixel& col)
	{
		FillWireFrameModel(modelCoordinates, { x, y }, rotation, scale, col);
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

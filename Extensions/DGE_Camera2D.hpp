#ifndef DGE_CAMERA2D_HPP
#define DGE_CAMERA2D_HPP

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
	class Camera2D
	{
	public:
		enum class Mode
		{
			None,
			Lock,
			LazyLock,
			BorderLock
		};

	public:
		Camera2D() = default;
		Camera2D(const vf2d& pos, const vf2d& viewArea, const Mode mode = Mode::None);

		void Initialise(const vf2d& pos, const vf2d& viewArea, const Mode mode = Mode::None);
		vf2d Update(vf2d& target, float deltaTime);

		void SetMode(const Mode mode);
		void SetPosition(const vf2d& pos);

		Mode GetMode() const;
		vf2d GetPosition() const;

	private:
		Mode m_Mode;
		vf2d m_ViewArea;
		vf2d m_Position;

	};
}

#ifdef DGE_CAMERA2D
#undef DGE_CAMERA2D

namespace def
{
	Camera2D::Camera2D(const vf2d& pos, const vf2d& viewArea, const Mode mode)
	{
		Initialise(pos, viewArea, mode);
	}

	void Camera2D::Initialise(const vf2d& pos, const vf2d& viewArea, const Mode mode)
	{
		m_Mode = mode;
		m_ViewArea = viewArea;
		m_Position = pos;
	}

	vf2d Camera2D::Update(vf2d& target, float deltaTime)
	{
		switch (m_Mode)
		{
		case Camera2D::Mode::Lock:
			m_Position = target;
			break;

		case Camera2D::Mode::LazyLock:
			m_Position += (target - m_Position) * deltaTime;
			break;

		case Camera2D::Mode::BorderLock:
			m_Position = (target / m_ViewArea).floor() * m_ViewArea + m_ViewArea * 0.5f;
			break;

		}

		return m_Position - m_ViewArea * 0.5f;
	}

	void Camera2D::SetMode(const Camera2D::Mode mode)
	{
		m_Mode = mode;
	}

	void Camera2D::SetPosition(const vf2d& pos)
	{
		m_Position = pos;
	}

	Camera2D::Mode Camera2D::GetMode() const
	{
		return m_Mode;
	}

	vf2d Camera2D::GetPosition() const
	{
		return m_Position;
	}
}

#endif

#endif

#ifndef DGE_CAMERA2D_HPP
#define DGE_CAMERA2D_HPP

#include "../defGameEngine.hpp"

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

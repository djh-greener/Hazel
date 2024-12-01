#pragma once


namespace Hazel
{

	class Timestep {
	public:
		Timestep(float time = 0.0f) :
			m_time(time)
		{

		}
		operator float()const { return m_time; }
		float const GetMilliseconds()const { return m_time*1000; }

	private:
		float m_time = 0.0f;
	};
}
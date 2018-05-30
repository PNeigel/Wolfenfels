#pragma once

#include <vector>

template<typename T>
class Animation
{
	public:
		Animation(bool interpolated, bool repeat);
		~Animation();

		void tick(float time);
		void addKeyframe(T keyframe, float timestamp);
		void setTimeToStart();
		void setTimeToEnd();
		void reverse() { m_reverse = !m_reverse; }
		bool atEnd() { return m_atEnd; }
		bool atStart() { return m_atStart; }
		T getCurrentKeyframe();

		bool m_interpolated = false;
		bool m_playing = false;
		bool m_repeat = false;
		bool m_reverse = false;
		float m_currentTime = 0.0;
		std::vector<T> m_keyframes;
		std::vector<float> m_timestamps;

	private:
		bool m_atEnd = false;
		bool m_atStart = true;
};


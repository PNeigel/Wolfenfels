#include "Animation.h"

#include <numeric>
#include "glm.hpp"

template<typename T>
Animation<T>::Animation(bool interpolated, bool repeat) :
	m_interpolated(interpolated), m_repeat(repeat)
{
}

template<typename T>
Animation<T>::~Animation()
{
}

template<typename T>
void Animation<T>::tick(float delta_time)
{
	if (m_currentTime > 0.0 && m_currentTime < m_timestamps.back()) {
		m_atStart = false;
		m_atEnd = false;
	}
	if (m_playing && !m_reverse) {
		m_currentTime += delta_time;
		if (m_currentTime > m_timestamps.back()) {
			if (!m_repeat) {
				m_playing = false;
				setTimeToEnd();
			}
			else
				setTimeToStart();
		}
	}
	else if (m_playing && m_reverse) {
		m_currentTime -= delta_time;
		if (m_currentTime <= 0) {
			if (!m_repeat) {
				m_playing = false;
				setTimeToStart();
			}
			else
				setTimeToEnd();
		}
	}
}

template<typename T>
void Animation<T>::addKeyframe(T keyframe, float timestamp)
{
	m_keyframes.push_back(keyframe);
	m_timestamps.push_back(timestamp);
}

template<typename T>
void Animation<T>::setTimeToStart()
{
	m_currentTime = 0.0f;
	m_atStart = true;
	m_atEnd = false;
}

template<typename T>
inline void Animation<T>::setTimeToEnd()
{
	m_currentTime = m_timestamps.back();
	m_atEnd = true;
	m_atStart = false;
}

template<typename T>
T Animation<T>::getCurrentKeyframe()
{
	int lastIdx, nextIdx; 
	float intoNext;
	
	if (!m_reverse) {

		lastIdx = m_keyframes.size() - 2;
		nextIdx = m_keyframes.size() - 1;
		intoNext = 1.0;

		for (int i = 0; i < m_keyframes.size() - 1; i++) {
			if (m_currentTime >= m_timestamps[i] && m_currentTime < m_timestamps[i+1]) {
				lastIdx = i;
				nextIdx = i + 1;
				intoNext = (m_currentTime - m_timestamps[i]) / (m_timestamps[i + 1] - m_timestamps[i]);
				break;
			}
		}
	}
	else {

		lastIdx = 1;
		nextIdx = 0;
		intoNext = 1.0;

		for (int i = m_keyframes.size()-1; i > 0; i--) {
			if (m_currentTime <= m_timestamps[i] && m_currentTime > m_timestamps[i-1]) {
				lastIdx = i;
				nextIdx = i - 1;
				intoNext = (m_timestamps[i] - m_currentTime) / (m_timestamps[i] - m_timestamps[i - 1]);
				break;
			}
		}
	}

	if (!m_interpolated) {
		return m_keyframes[lastIdx];
	}
	else {
		return m_keyframes[lastIdx] * (1 - intoNext) + m_keyframes[nextIdx] * intoNext;
	}

}

template class Animation<glm::vec2>;
template class Animation<glm::mat4>;

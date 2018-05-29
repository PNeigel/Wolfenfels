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
	T getCurrentKeyframe();

	bool m_interpolated = false;
	bool m_playing = false;
	bool m_repeat = false;
	bool m_reverse = false;
	float m_currentTime = 0.0;
	std::vector<T> m_keyframes;
	std::vector<float> m_timestamps;
};


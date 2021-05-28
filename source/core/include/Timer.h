/**
 * ============================================================================
 *  Name        : Timer.h
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : basic high-precision timer
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#pragma once

#include <cstdint>

class Timer
{
public:
	Timer();
	~Timer();

	/**
	 * Create
	 * initialize the timer
	 */
	void Create();

	/**
	 * BeginTimer
	 * start calculating the time
	 */
	void BeginTimer();

	/**
	 * EndTimer
	 * stop calculatig the time
	 */
	void EndTimer();

	/**
	 * GetElapsedSeconds
	 * @return seconds between last BeginTimer - EndTimer calls.
	 */
	float GetElapsedSeconds() const;

	/**
	 * GetTicks
	 * @return system timer ticks
	 */
	static uint64_t GetTicks();

private:
	double		m_dRateToSeconds;
	uint64_t	m_uStartClock;
	uint64_t	m_uTickFrequency;

	float		m_fElapsedSeconds;
};


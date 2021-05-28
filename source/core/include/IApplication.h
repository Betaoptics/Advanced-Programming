/**
 * ============================================================================
 *  Name        : IApplication.h
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : Abstract application layer
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#pragma once

#if defined (_WINDOWS)
// include minimum set of win32 stuff
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined (_LINUX)
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#endif


// we use std::string and std::vector
#include <string>
#include <vector>

// include timer component and renderer interface
#include "Timer.h"
#include "IRenderer.h"

// SAFE_DELETE macro, helpful when deleting object instances
#define SAFE_DELETE(x) if (x) { delete (x); (x) = nullptr; }

// define some common keycodes
#if defined (_WINDOWS)
#define KEY_ESC		VK_ESCAPE
#define KEY_LEFT	VK_LEFT
#define KEY_RIGHT	VK_RIGHT
#define KEY_UP		VK_UP
#define KEY_DOWN	VK_DOWN
#define KEY_SPACE	VK_SPACE

#elif defined (_LINUX)
#define KEY_ESC		XK_Escape
#define KEY_LEFT	XK_Left
#define KEY_RIGHT	XK_Right
#define KEY_UP		XK_Up
#define KEY_DOWN	XK_Down
#define KEY_SPACE	XK_Space

#endif


class IApplication
{
public:
	IApplication();
	~IApplication();

	/**
	 * Create
	 * initialize the application layer
	 * @param iResX horizontal resolution of the screen in pixels
	 * @param iResY vertical resolution of the screen in pixels
	 * @return true if successful, false otherwise
	 */
	bool Create(int32_t iResX, int32_t iResY, const std::string& strTitle);

	/**
	 * Run
	 * enter into app main loop. This function will not return until app has been terminated.
	 */
	void Run();

	/**
	 * OnCreate
	 * pure virtual app initializer
	 * @return true if successful, false otherwise. If implementation returns false, app startup is canceled and app is closed.
	 */
	virtual bool OnCreate() = 0;

	/**
	 * OnDestroy
	 * pure virtual app destroyer, called by the application when app is about to get destoyed
	 */
	virtual void OnDestroy() = 0;

	/**
	 * OnUpdate
	 * pure virtual app update loop
	 * @param fFrametime time since previous update, in seconds
	 */
	virtual void OnUpdate(float fFrametime) = 0;

	/**
	 * OnDraw
	 * pure virtual app drawing method
	 * @param pRenderer pointer to renderer object
	 */
	virtual void OnDraw(IRenderer* pRenderer) = 0;

	/**
	 * IsActive
	 * @return true if app is active, false otherwise
	 */
	inline bool IsActive() const { return m_bActive; }

	/**
	 * SetActive
	 * @param true to set app active, false otherwise
	 */
	void SetActive(bool bSet);

	/**
	 * Close
	 * close the app
	 */
	void Close();

	/**
	 * GetFrameTime
	 * @return time since previous update, in seconds, this value is essentially the same as
	 * fFrametime parameter for OnUpdate
	 */
	inline float GetFrameTime() const { return m_Timer.GetElapsedSeconds(); }

	/**
	 * GetRenderer
	 * @return pointer to current renderer
	 */
	inline IRenderer* GetRenderer() { return m_pRenderer; }

	/**
	 * RandSeed
	 * initialize random seed with tick count
	 */
	void RandSeed();

	/**
	 * GetWidth
	 * @return screen width in pixels
	 */
	inline int32_t GetWidth() const { return m_iWidth; }

	/**
	 * GetHeight
	 * @return screen height in pixels
	 */
	inline int32_t GetHeight() const { return m_iHeight; }

	/**
	 * GetAspect
	 * @return screen aspect ratio
	 */
	inline float GetAspect() const { return (float)m_iWidth / (float)m_iHeight; }

	/**
	 * GetWindow
	 * @return window handle
	 */
#if defined (_WINDOWS)
	inline HWND GetWindow() { return m_Window; }
#endif
#if defined (_LINUX)
	inline Display* GetDisplay() { return m_pDisplay; }
	inline Window GetWindow() { return m_Window; }
#endif


	/**
	 * GetApp
	 * static app pointer accessor
	 * @return pointer to active app, or nullptr if app has been terminated
	 */
	static IApplication* GetApp() { return m_pApp; }

	/**
	 * Debug
	 * @param msg message to print to debug stream
	 */
	static void Debug(const wchar_t* msg);
	static void Debug(const std::string& msg);
	static void Debug(const char* msg);

protected:
	/**
	 * OnScreenSizeChanged
	 * event when screen size changes
	 * @param uWidthPixels, uHeightPixels new pixel size of the screen
	 */
	virtual void OnScreenSizeChanged(uint32_t uWidthPixels, uint32_t uHeightPixels) {}

	/**
	 * OnMouseBegin
	 * event when mouse button is pressed down
	 * @param iButtonIndex index of the mouse button (0 based)
	 * @param vPoint event coordinate in pixels
	 * @return true if event was handled by the receiver
	 */
	virtual bool OnMouseBegin(int32_t iButtonIndex, const glm::vec2& vPoint) { return false; }

	/**
	 * OnMouseDrag
	 * event when mouse is dragged
	 * @param iButtonIndex index of the mouse button (0 based)
	 * @param vPoint event coordinate in pixels
	 * @return true if event was handled by the receiver
	 */
	virtual bool OnMouseDrag(int32_t iButtonIndex, const glm::vec2& vPoint) { return false; }

	/**
	 * OnMouseEnd
	 * event when mouse button is lifted up
	 * @param iButtonIndex index of the mouse button (0 based)
	 * @param vPoint event coordinate in pixels
	 * @return true if event was handled by the receiver
	 */
	virtual bool OnMouseEnd(int32_t iButtonIndex, const glm::vec2& vPoint) { return false; }

	/**
	 * OnKeyDown
	 * key down event
	 * @param uKeyCode key code
	 * @return true if event was handled by the receiver
	 */
	virtual bool OnKeyDown(uint32_t uKeyCode) { return false; }

#if defined (_WINDOWS)
	/**
	 * OnEvent
	 * Allows capturing all win32 events before they are handled by default handler.
	 * @param uMessage message id number
	 * @param wParam win32 wparam
	 * @param lParam win32 lparam
	 * @return true if event was handled by the receiver, if receiver returns false, event is passed to default handler.
	 */
	virtual bool OnEvent(UINT uMessage, WPARAM wParam, LPARAM lParam);
#endif

private:
#if defined (_WINDOWS)
	static HWND MakeWindow(int32_t iWidth, int32_t iHeight, const std::string& strTitle);
	static long WINAPI WndProc(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

	HWND							m_Window;
#endif

#if defined (_LINUX)
	static Window MakeWindow(int32_t iWidth, int32_t iHeight, const char* pTitle);
    static bool ProcessEvents(XEvent* pEvent);

    static Display*					m_pDisplay;
    static int32_t					m_iScreen;
    static Window					m_Window;
    static XSetWindowAttributes		m_WindowAttributes;
    static Atom						m_CloseAtom;
#endif

	static IApplication*			m_pApp;
	bool							m_bActive;
	Timer							m_Timer;

	int32_t							m_iWidth;
	int32_t							m_iHeight;

	IRenderer*						m_pRenderer;
};


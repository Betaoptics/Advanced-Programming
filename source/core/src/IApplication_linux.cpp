/**
 * ============================================================================
 *  Name        : IApplication_linux.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : Abstract application layer (linux x11)
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 *
 * ============================================================================
**/

#include "../include/IApplication.h"

// include all your renderers
#include "../include/OpenGLRenderer.h"

#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>

//#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>


#if defined (_LINUX)

IApplication*           IApplication::m_pApp = nullptr;

Display*                 IApplication::m_pDisplay;
int                      IApplication::m_iScreen;
Window                   IApplication::m_Window;
XSetWindowAttributes     IApplication::m_WindowAttributes;
Atom                     IApplication::m_CloseAtom;


IApplication::IApplication()
{
	m_pApp = this;

    m_pDisplay = XOpenDisplay(NULL);
    m_iScreen = DefaultScreen(m_pDisplay);

	m_Window = 0;
	m_bActive = false;
	m_Timer.Create();

	m_pRenderer = nullptr;
}


IApplication::~IApplication()
{
	m_pApp = nullptr;
}


bool IApplication::Create(int iResX, int iResY, const std::string& strTitle)
{
	m_Window = MakeWindow(iResX, iResY, strTitle.c_str());
	if (m_Window)
	{
		m_iWidth = iResX;
		m_iHeight = iResY;

		// create the renderer
		m_pRenderer = new COpenGLRenderer;
		if (!m_pRenderer->Create())
		{
			return false;
		}

		// call pure virtual OnCreate
		if (OnCreate())
		{
			SetActive(true);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


bool IApplication::ProcessEvents(XEvent* pEvent)
{
    IApplication* pApp = IApplication::GetApp();
    if (!m_Window || !pApp)
    {
        return false;
    }

    switch (pEvent->type)
    {
    case ClientMessage:
        if (m_CloseAtom && (long unsigned int)pEvent->xclient.data.l[0] == m_CloseAtom)
        {
            m_Window = 0;
        }
        break;

    case ButtonPress:
        if (pEvent->xbutton.button == 4 || pEvent->xbutton.button == 5)
        {
            // mouse wheel event
        }
        else
        {
            // mouse button event
            const int touchIndex = pEvent->xbutton.button - 1;
       		pApp->OnMouseBegin(touchIndex, glm::vec2(pEvent->xbutton.x, pEvent->xbutton.y));
        }
        break;

    case ButtonRelease:
        if (pEvent->xbutton.button < 4)
        {
            const int touchIndex = pEvent->xbutton.button - 1;
       		pApp->OnMouseEnd(touchIndex, glm::vec2(pEvent->xbutton.x, pEvent->xbutton.y));
        }
        break;

    case MotionNotify:
        // first mouse button that is held down
        pApp->OnMouseDrag(0, glm::vec2(pEvent->xbutton.x, pEvent->xbutton.y));
        break;

    case KeyPress:
        {
            KeySym xkey = XLookupKeysym(&pEvent->xkey, 0);

            // doh, in Maemo, hardware keyboard enter is XK_KP_Enter, not XK_Return
            // for now, lets map XK_KP_Enter to XK_Return
            if (xkey == XK_KP_Enter)
            {
                xkey = XK_Return;
            }

            pApp->OnKeyDown((uint32_t)xkey);
        }
        break;

    case KeyRelease:
        break;

    case FocusIn:
        pApp->SetActive(true);
        break;

    case FocusOut:
        pApp->SetActive(false);
        break;

    case Expose:
        break;

    case ResizeRequest:
        break;

    case VisibilityNotify:
    {
        XVisibilityEvent* visibility = (XVisibilityEvent*)pEvent;
        switch (visibility->state)
        {
        case VisibilityFullyObscured:
            break;

        case VisibilityPartiallyObscured:
            break;

        case VisibilityUnobscured:
            break;

        default:
            break;
        }
    }
    break;

    case ConfigureNotify:
        if (pEvent->xconfigure.width != pApp->m_iWidth || pEvent->xconfigure.height != pApp->m_iHeight)
        {
            pApp->m_iWidth = pEvent->xconfigure.width;
            pApp->m_iHeight = pEvent->xconfigure.height;
            pApp->m_pRenderer->SetViewport(0, 0, pApp->m_iWidth, pApp->m_iHeight);
            pApp->OnScreenSizeChanged(pApp->m_iWidth, pApp->m_iHeight);
        }
        break;

    case GraphicsExpose:
        break;

    case NoExpose:
        break;

    default:
        break;
    }

    return true;
}


void IApplication::SetActive(bool bSet)
{
	m_bActive = bSet;
	m_Timer.BeginTimer();
}


void IApplication::Close()
{
	m_Window = 0;
}


void IApplication::Run()
{
    // run the application
    XEvent event;
    bool bGotMsg = false;

    while (m_Window)
    {
        bGotMsg = false;
        if (XPending(m_pDisplay))
        {
            XNextEvent(m_pDisplay, &event);
            bGotMsg = true;
        }

        if (bGotMsg)
        {
            ProcessEvents(&event);
        }
        else
        {
			m_Timer.EndTimer();
			m_Timer.BeginTimer();

			OnUpdate(m_Timer.GetElapsedSeconds());
			OnDraw(m_pRenderer);
			m_pRenderer->Flip();
        }
    }
	OnDestroy();
	SAFE_DELETE(m_pRenderer);
}


Window IApplication::MakeWindow(int iWidth, int iHeight, const char* pTitle)
{
    Window wnd = 0;
    Window rootWindow = DefaultRootWindow(m_pDisplay);
    //const char strWMClassName[] = "GraphicsProgrammingApp";

    int i32Depth = XDefaultDepth(m_pDisplay, m_iScreen);
    XVisualInfo* x11Visual = (XVisualInfo*)alloca(sizeof(XVisualInfo));
    Colormap x11Colormap;

    XMatchVisualInfo(m_pDisplay, m_iScreen, i32Depth, TrueColor, x11Visual);
    if (!x11Visual)
    {
        return 0;
    }

    // Colormap of the specified visual type for the display.
    x11Colormap = XCreateColormap(m_pDisplay, rootWindow, x11Visual->visual, AllocNone);
    m_WindowAttributes.colormap = x11Colormap;

    m_WindowAttributes.event_mask =
        ExposureMask | VisibilityChangeMask | KeyPressMask | KeyReleaseMask |
        ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
        StructureNotifyMask | SubstructureNotifyMask | FocusChangeMask |
        PropertyChangeMask | 0;

    wnd = XCreateWindow(m_pDisplay, rootWindow, 0, 0, iWidth, iHeight, 0,
                         CopyFromParent, InputOutput, CopyFromParent,
                         CWBackPixel | CWBorderPixel | CWEventMask |
                             CWColormap, // windowMask,
                         &m_WindowAttributes);

    /*
    if (bFullscreen)
    {
        // Force X11 to take full size by setting min_size to required
        XSizeHints sh;
        sh.width = sh.min_width = iWidth;
        sh.height = sh.min_height = iHeight;
        sh.flags = PSize | PMinSize | PPosition;
        XSetWMNormalHints(m_pDisplay, wnd, &sh);

        XMapWindow(m_pDisplay, wnd);
        XFlush(m_pDisplay);

        XEvent x11_event;
        Atom x11_state_atom;
        Atom x11_fs_atom;

        x11_state_atom = XInternAtom(m_pDisplay, "_NET_WM_STATE", False);
        x11_fs_atom = XInternAtom(m_pDisplay, "_NET_WM_STATE_FULLSCREEN", False);

        x11_event.xclient.type = ClientMessage;
        x11_event.xclient.serial = 0;
        x11_event.xclient.send_event = True;
        x11_event.xclient.window = wnd;
        x11_event.xclient.message_type = x11_state_atom;
        x11_event.xclient.format = 32;
        x11_event.xclient.data.l[0] = 1;
        x11_event.xclient.data.l[1] = x11_fs_atom;
        x11_event.xclient.data.l[2] = 0;

        XSendEvent(m_pDisplay, rootWindow, False,
                   SubstructureRedirectMask | SubstructureNotifyMask,
                   &x11_event);
    }
    else*/
    {
        // create atom for window close button
        m_CloseAtom = XInternAtom(m_pDisplay, "WM_DELETE_WINDOW", True);
        XSetWMProtocols(m_pDisplay, wnd, &m_CloseAtom, 1);

        XSetStandardProperties(m_pDisplay, wnd, pTitle, pTitle, None, NULL, 0, NULL);

        XMapRaised(m_pDisplay, wnd);
        XFlush(m_pDisplay);
    }

    XWMHints* hints = NULL;
    hints = XAllocWMHints();
    hints->input = True;
    hints->flags = InputHint;
    XSetWMHints(m_pDisplay, wnd, hints);
    XFree(hints);

    // Find out what size window we actually got.
    XWindowAttributes xWindowAttributes;
    XGetWindowAttributes(m_pDisplay, wnd, &xWindowAttributes);

    return wnd;
}


void IApplication::Debug(const wchar_t* msg)
{
    fwprintf(stderr, L"%s", msg);
}


void IApplication::Debug(const std::string& msg)
{
    fprintf(stderr, "%s", msg.c_str());
}


void IApplication::Debug(const char* msg)
{
    fprintf(stderr, "%s", msg);
}


void IApplication::RandSeed()
{
	struct timespec ts;
	srand(clock_gettime(CLOCK_MONOTONIC, &ts));
}


#endif	// #if defined (_LINUX)


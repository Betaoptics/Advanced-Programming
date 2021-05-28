/**
 * ============================================================================
 *  Name        : TheApp.h
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : the concrete application layer
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#pragma once

#include "../core/include/IApplication.h"
#include "../core/include/OpenGLRenderer.h"
#include "../core/include/Geometry.h"
#include "../core/include/Material.h"
#include "../core/include/IGraphNode.h"


// forward declare game object
class CGameObject;

class CTheApp : public IApplication
{
public:
	CTheApp();
	~CTheApp();

	/**
	 * OnCreate
	 * app initializer, called by abstraction layer when engine initialization is complete
	 * @return true if successful, false otherwise.
	 */
	bool OnCreate() override;

	/**
	 * OnDestroy
	 * app destroyer, called by the abstraction layer when app is about to get destoyed
	 */
	void OnDestroy() override;

	/**
	 * OnUpdate
	 * app update loop
	 * @param fFrametime time since previous update, in seconds
	 */
	void OnUpdate(float fFrametime) override;

	/**
	 * OnDraw
	 * app drawing method
	 * @param pRenderer pointer to renderer object
	 */
	void OnDraw(IRenderer* pRenderer) override;

protected:
	/**
	 * OnScreenSizeChanged
	 * event when screen size changes
	 * @param uWidthPixels, uHeightPixels new pixel size of the screen
	 */
	void OnScreenSizeChanged(uint32_t uWidthPixels, uint32_t uHeightPixels) override;

	/**
	 * OnMouseBegin
	 * event when mouse button is pressed down
	 * @param iButtonIndex index of the mouse button (0 based)
	 * @param vPoint event coordinate in pixels
	 * @return true if event was handled by the receiver
	 */
	bool OnMouseBegin(int iButtonIndex, const glm::vec2& vPoint) override;

	/**
	 * OnMouseDrag
	 * event when mouse is dragged
	 * @param iButtonIndex index of the mouse button (0 based)
	 * @param vPoint event coordinate in pixels
	 * @return true if event was handled by the receiver
	 */
	bool OnMouseDrag(int iButtonIndex, const glm::vec2& vPoint) override;

	/**
	 * OnMouseEnd
	 * event when mouse button is lifted up
	 * @param iButtonIndex index of the mouse button (0 based)
	 * @param vPoint event coordinate in pixels
	 * @return true if event was handled by the receiver
	 */
	bool OnMouseEnd(int iButtonIndex, const glm::vec2& vPoint) override;

	/**
	 * OnKeyDown
	 * key down event
	 * @param uKeyCode key code
	 * @return true if event was handled by the receiver
	 */
	bool OnKeyDown(uint32_t uKeyCode) override;

private:
	/**
	 * GetOpenGLRenderer
	 * helper method to access the openGL renderer layer
	 * @return pointer to OpenGL renderer
	 */
	COpenGLRenderer* GetOpenGLRenderer() { return static_cast<COpenGLRenderer*>(GetRenderer()); }

private:
	void CheckSphereToSphereCollisions();

	GLuint						m_uVertexShader;
	GLuint						m_uFragmentShader;
	GLuint						m_uProgram;

	GLuint						m_uTexture;

	Geometry					m_Sphere;
	Material					m_Material;

	IGraphNode*					m_pSceneRoot;
};


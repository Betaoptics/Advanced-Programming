/**
 * ============================================================================
 *  Name        : OpenGLRenderer.h
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : concrete OpenGL renderer layer
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#pragma once

#include "IRenderer.h"
#include "IApplication.h"


#include <GL/gl.h>
#if defined (_WINDOWS)
#include "./GL/glext.h"
#endif
#include "./GL/myGL.h"


class COpenGLRenderer : public IRenderer
{
public:
	COpenGLRenderer();
	~COpenGLRenderer();

	/**
	 * Create (from IRenderer)
	 * @return true if succesful, or false to cancel app startup
	 */
	bool Create() override;

	/**
	 * Flip (from IRenderer)
	 * transfer graphics to screen
	 */
	void Flip() override;

	/**
	 * Clear
	 * clear the color buffer, depth buffer and stencil
	 */
	void Clear(float r, float g, float b, float a, float fDepth = 1.0f, int32_t iStencil = 0) override;

	/**
	 * SetViewport
	 * set viewport area of the screen
	 * @param iX, iY, iWidth, iHeight viewport area
	 */
	void SetViewport(int32_t iX, int32_t iY, int32_t iWidth, int32_t iHeight) override;

	/**
	 * CreateTexture
	 * create opengl texture handle from image file
	 * @param strFilename file to load
	 * @return opengl texture handle, or 0 if failed
	 */
	GLuint CreateTexture(const std::string& strFilename);

	/**
	 * CreateVertexShader
	 * create opengl vertex shader from text
	 * @param pVertexShader shader source code
	 * @return opengl shader handle, or 0 if failed
	 */
	GLuint CreateVertexShader(const char* pVertexShader);

	/**
	 * CreateVertexShaderFromFile
	 * create opengl vertex shader from text file
	 * @param strFilename text file to load
	 * @return opengl shader handle, or 0 if failed
	 */
	GLuint CreateVertexShaderFromFile(const std::string& strFilename);

	/**
	 * CreateFragmentShader
	 * create opengl fragment shader from text
	 * @param pFragmentShader shader source code
	 * @return opengl shader handle, or 0 if failed
	 */
	GLuint CreateFragmentShader(const char* pFragmentShader);

	/**
	 * CreateFragmentShaderFromFile
	 * create opengl fragment shader from text file
	 * @param strFilename text file to load
	 * @return opengl shader handle, or 0 if failed
	 */
	GLuint CreateFragmentShaderFromFile(const std::string& strFilename);

	/**
	 * CreateProgram
	 * Link opengl program from vertex and fragment shader
	 * @param uVertexShader
	 * @param uFragmentShader
	 * @return opengl program handle, or 0 if failed
	 */
	GLuint CreateProgram(GLuint uVertexShader, GLuint uFragmentShader);

	/**
	 * PrintShaderError/PrintProgramError
	 * helpers to print detailed information of shader and program errors
	 * @param uShader
	 * @param uProgram
	 */
	void PrintShaderError(GLuint uShader);
	void PrintProgramError(GLuint uProgram);

private:
	bool InitFunctions();
	bool SetDefaultSettings();

#if defined (_WINDOWS)
	HDC				m_Context;
	HGLRC			m_hRC;
#endif

#if defined (_LINUX)
	void*			m_Context;
#endif
};


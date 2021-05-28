/**
 * ============================================================================
 *  Name        : TheApp.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : LAB3
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#include "TheApp.h"


CTheApp::CTheApp()
{
	// constructor, init members
	m_uVertexShader = 0;
	m_uFragmentShader = 0;
	m_uProgram = 0;
	m_uTexture = 0;

	flip = false;

	// seed the random number generator
	RandSeed();
}


CTheApp::~CTheApp()
{
}


bool CTheApp::OnCreate()
{
	// OnCreate is called by the application when window and graphics initialization is complete
	m_uVertexShader = GetOpenGLRenderer()->CreateVertexShaderFromFile("triangleshader.vs");
	m_uFragmentShader = GetOpenGLRenderer()->CreateFragmentShaderFromFile("triangleshader.fs");
	m_uProgram = GetOpenGLRenderer()->CreateProgram(m_uVertexShader, m_uFragmentShader);
	m_uTexture = GetOpenGLRenderer()->CreateTexture("leaf.png");

	if (!m_uVertexShader || !m_uFragmentShader || !m_uProgram || !m_uTexture)
	{
		return false;
	}

	m_Triangle[0] = TRIANGLEVERTEX(glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 1.0f));
	m_Triangle[1] = TRIANGLEVERTEX(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 1.0f));
	m_Triangle[2] = TRIANGLEVERTEX(glm::vec3( 0.0f,  0.5f, 0.0f), glm::vec2(0.5f, 0.0f));

	return true;
}


void CTheApp::OnDestroy()
{
	// app is about to close, clear all resources
	glDeleteProgram(m_uProgram);
	glDeleteShader(m_uFragmentShader);
	glDeleteShader(m_uVertexShader);

	// TODO: delete loaded texture

	m_uTexture = 0;
	m_uVertexShader = 0;
	m_uFragmentShader = 0;
	m_uProgram = 0;
}


void CTheApp::OnUpdate(float fFrametime)
{
	if (flip)
	{ 
		m_Triangle[0] = TRIANGLEVERTEX(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f));
		m_Triangle[1] = TRIANGLEVERTEX(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f));
		m_Triangle[2] = TRIANGLEVERTEX(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec2(0.5f, 1.0f));
	}
	else
	{
		m_Triangle[0] = TRIANGLEVERTEX(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 1.0f));
		m_Triangle[1] = TRIANGLEVERTEX(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 1.0f));
		m_Triangle[2] = TRIANGLEVERTEX(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec2(0.5f, 0.0f));
	}
}


void CTheApp::OnDraw(IRenderer* pRenderer)
{
	// clear color, depth and stencil buffers
	pRenderer->Clear(0.2f, 0.2f, 0.2f, 1.0f);

	// setup the rendering program
	glUseProgram(m_uProgram);

	// get the vertex attribute locations
	GLint position = glGetAttribLocation(m_uProgram, "position");

	// set the vertex position
	glEnableVertexAttribArray(position);
	glVertexAttribPointer(
		position,
		3,
		GL_FLOAT,
		GL_FALSE,
		TRIANGLEVERTEX::GetStride(),
		(void*)&m_Triangle[0].x);

	GLint uv = glGetAttribLocation(m_uProgram, "uv");

	glEnableVertexAttribArray(uv);
	glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, TRIANGLEVERTEX::GetStride(), (float*)m_Triangle + 3);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uTexture);
	
	GLint texture01 = glGetUniformLocation(m_uProgram, "texture01");
	glUniform1i(texture01, 0);

	// set the model matrix
	glm::mat4 model = glm::mat4(1.0f);
	GLint location = glGetUniformLocation(m_uProgram, "modelMatrix");
	glUniformMatrix4fv(location, 1, GL_FALSE, &model[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}


void CTheApp::OnScreenSizeChanged(uint32_t uWidthPixels, uint32_t uHeightPixels)
{
}


bool CTheApp::OnMouseBegin(int iButtonIndex, const glm::vec2& vPoint)
{
	//IApplication::Debug("OnMouseBegin " + std::to_string(iButtonIndex) + ": " + std::to_string((int)vPoint.x) + "x" + std::to_string((int)vPoint.y) + "\r\n");
	return true;
}


bool CTheApp::OnMouseDrag(int iButtonIndex, const glm::vec2& vPoint)
{
	//IApplication::Debug("OnMouseDrag " + std::to_string(iButtonIndex) + ": " + std::to_string((int)vPoint.x) + "x" + std::to_string((int)vPoint.y) + "\r\n");
	return true;
}


bool CTheApp::OnMouseEnd(int iButtonIndex, const glm::vec2& vPoint)
{
	//IApplication::Debug("OnMouseEnd " + std::to_string(iButtonIndex) + ": " + std::to_string((int)vPoint.x) + "x" + std::to_string((int)vPoint.y) + "\r\n");
	return true;
}


bool CTheApp::OnKeyDown(uint32_t uKeyCode)
{
	// TODO: modify texture coordinates when user presses space
	if (uKeyCode == KEY_ESC)
	{
		Close();
		return true;
	}

	if (uKeyCode == KEY_SPACE)
	{
		if (flip == true)
		{
			flip = false;
		}
		else
		{
			flip = true;
		}
		return true;
	}

	return false;
}


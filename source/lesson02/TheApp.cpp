/**
 * ============================================================================
 *  Name        : TheApp.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : the concrete application layer
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

	m_fQuadX = 0.0f;
	m_fAngle = 0.0f;

	// seed the random number generator
	RandSeed();
}


CTheApp::~CTheApp()
{
}


bool CTheApp::OnCreate()
{
	// OnCreate is called by the application when window and graphics initialization is complete
	m_uVertexShader = GetOpenGLRenderer()->CreateVertexShaderFromFile("simpleshader.vs");
	m_uFragmentShader = GetOpenGLRenderer()->CreateFragmentShaderFromFile("simpleshader.fs");
	m_uProgram = GetOpenGLRenderer()->CreateProgram(m_uVertexShader, m_uFragmentShader);
	m_uTexture = GetOpenGLRenderer()->CreateTexture("dice.png");
	if (!m_uVertexShader || !m_uFragmentShader || !m_uProgram || !m_uTexture)
	{
		return false;
	}

	// declare the vertices for one quad
	const float hw = 0.55f;
	m_Quad[0] = VERTEX(-hw, hw, 0.0f,		0.0f, 0.0f);
	m_Quad[1] = VERTEX(hw, hw, 0.0f,		1.0f, 0.0f);
	m_Quad[2] = VERTEX(hw, -hw, 0.0f,		1.0f, 1.0f);

	m_Quad[3] = VERTEX(-hw, hw, 0.0f,		0.0f, 0.0f);
	m_Quad[4] = VERTEX(hw, -hw, 0.0f,		1.0f, 1.0f);
	m_Quad[5] = VERTEX(-hw, -hw, 0.0f,		0.0f, 1.0f);

	// setup our view and projection matrices
	m_mView = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 5.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	m_mProjection = glm::perspective(1.51f, GetAspect(), 0.1f, 500.0f);

	return true;
}


void CTheApp::OnDestroy()
{
	// app is about to close, clear all resources

	glDeleteTextures(1, &m_uTexture);
	glDeleteProgram(m_uProgram);
	glDeleteShader(m_uFragmentShader);
	glDeleteShader(m_uVertexShader);

	m_uTexture = 0;
	m_uVertexShader = 0;
	m_uFragmentShader = 0;
	m_uProgram = 0;
}


void CTheApp::OnUpdate(float fFrametime)
{
	// the main loop

	// generate rotation and translate matrices for quad
	const glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), m_fAngle, glm::vec3(0.0f, 0.0f, -1.0f));
	glm::mat4 translation(1.0f);
	//translation[3][0] = m_fQuadX;

	// combine the transformations into the m_mModel matrix
	m_mModel = translation * rotation;

	// move camera back and forth
	m_mView[3][2] = -5.0f + sinf(m_fAngle) * 4.0f;

	// update quad x-position and rotation angle
	m_fQuadX += fFrametime * 2.0f;
	if (m_fQuadX > 7.5f)
	{
		m_fQuadX = -7.5f;
	}
	m_fAngle += fFrametime;

}


void CTheApp::OnDraw(IRenderer* pRenderer)
{
	// clear color, depth and stencil buffers
	pRenderer->Clear(0.2f, 0.2f, 0.2f, 1.0f);

	// setup the rendering of our quad
	glUseProgram(m_uProgram);

	// find the locations of position and uv attributes in shader program
	GLint position = glGetAttribLocation(m_uProgram, "position");
	GLint uv = glGetAttribLocation(m_uProgram, "uv");

	// enable the position and uv attributes
	glEnableVertexAttribArray(position);
	glEnableVertexAttribArray(uv);

	// set the data source for position and uv attributes
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, VERTEX::GetStride(), m_Quad);
	glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, VERTEX::GetStride(), (float*)m_Quad + 3);

	// set the texture for the quad (slot 0)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uTexture);

	// the textures are bind as uniforms into the shared program
	GLint location = glGetUniformLocation(m_uProgram, "texture01");
	glUniform1i(location, 0);

	// set model-view-projection matrix to shader uniform
	glm::mat4 modelViewProjectionMatrix(m_mProjection * m_mView * m_mModel);
	GLint mvp = glGetUniformLocation(m_uProgram, "mvpMatrix");
	glUniformMatrix4fv(mvp, 1, GL_FALSE, &modelViewProjectionMatrix[0][0]);

	// set the alpha blending for the texture
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	// draw the quad
	glDrawArrays(GL_TRIANGLES, 0, 6);
}


void CTheApp::OnScreenSizeChanged(uint32_t uWidthPixels, uint32_t uHeightPixels)
{
	m_mProjection = glm::perspective(1.51f, GetAspect(), 0.1f, 500.0f);
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
	if (uKeyCode == KEY_ESC)
	{
		Close();
		return true;
	}

	return false;
}


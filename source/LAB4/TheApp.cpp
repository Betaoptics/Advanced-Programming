/**
 * ============================================================================
 *  Name        : TheApp.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : LAB4
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
	m_uTexture[0] = 0;
	m_uTexture[1] = 0;
	m_fBlendAngle = 0;

	m_pSceneRoot = nullptr;

	// seed the random number generator
	RandSeed();
}


CTheApp::~CTheApp()
{
}


bool CTheApp::OnCreate()
{
	// OnCreate is called by the application when window and graphics initialization is complete
	m_uVertexShader = GetOpenGLRenderer()->CreateVertexShaderFromFile("multitexturing.vs");
	m_uFragmentShader = GetOpenGLRenderer()->CreateFragmentShaderFromFile("multitexturing.fs");
	m_uProgram = GetOpenGLRenderer()->CreateProgram(m_uVertexShader, m_uFragmentShader);
	m_uTexture[0] = GetOpenGLRenderer()->CreateTexture("box.png");
	m_uTexture[1] = GetOpenGLRenderer()->CreateTexture("box_specular.png");
	if (!m_uVertexShader || !m_uFragmentShader || !m_uProgram || !m_uTexture[0] || !m_uTexture[1])
	{
		return false;
	}

	// setup our view and projection matrices
	m_mView = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 12.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	m_mProjection = glm::perspective(0.7f, GetAspect(), 0.1f, 500.0f);

	m_Cube.GenCube(glm::vec3(4.0f, 4.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	m_Material.m_cDiffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	m_Material.m_cSpecular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	m_Material.m_cAmbient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_Material.m_cEmissive = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_Material.m_fSpecularPower = 30.0f;

	// build the scenegraph
	m_pSceneRoot = new CBox();
	
	CBox* pObject = new CBox();
	pObject->SetPos(0.0f, 0.0f, -5.0f);
	pObject->SetGeometry(&m_Cube);
	pObject->SetMaterial(&m_Material);
	pObject->SetRotationSpeed(0.3);
	pObject->SetRotationAxis(glm::vec3(0.3, 0.3, 0.3));

	m_pSceneRoot->AddChild(pObject);

	return true;
}


void CTheApp::OnDestroy()
{
	// app is about to close, clear all resources
	if (m_pSceneRoot)
	{
		delete m_pSceneRoot;
		m_pSceneRoot = nullptr;
	}

	glDeleteProgram(m_uProgram);
	glDeleteShader(m_uFragmentShader);
	glDeleteShader(m_uVertexShader);

	glDeleteTextures(2, m_uTexture);

	m_uVertexShader = 0;
	m_uFragmentShader = 0;
	m_uProgram = 0;
}


void CTheApp::OnUpdate(float fFrametime)
{
	// update scenegraph
	if (m_pSceneRoot)
	{
		m_pSceneRoot->Update(fFrametime);
	}
}


void CTheApp::OnDraw(IRenderer* pRenderer)
{
	// clear color, depth and stencil buffers
	pRenderer->Clear(0.2f, 0.2f, 0.2f, 1.0f);

	// set view and projection into the renderer
	GetRenderer()->SetViewMatrix(m_mView);
	GetRenderer()->SetProjectionMatrix(m_mProjection);

	// setup the rendering program
	glUseProgram(m_uProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uTexture[0]);
	GLint location = glGetUniformLocation(m_uProgram, "texture01");
	glUniform1i(location, 0);

	SetTexturingParams();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_uTexture[1]);
	location = glGetUniformLocation(m_uProgram, "texture02");
	glUniform1i(location, 1);

	SetTexturingParams();

	// setup the light position (to camera position)
	const glm::vec3 campos(-pRenderer->GetViewMatrix()[3]);
	location = glGetUniformLocation(m_uProgram, "lightPosition");
	glUniform3f(location, campos.x, campos.y, campos.z);

	// setup the camera position uniform
	location = glGetUniformLocation(m_uProgram, "cameraPosition");
	glUniform3f(location, campos.x, campos.y, campos.z);

	// draw the scenegraph
	if (m_pSceneRoot)
	{
		m_pSceneRoot->Render(pRenderer, m_uProgram);
	}
}

void CTheApp::SetTexturingParams()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}


void CTheApp::OnScreenSizeChanged(uint32_t uWidthPixels, uint32_t uHeightPixels)
{
	m_mProjection = glm::perspective(0.7f, GetAspect(), 0.1f, 500.0f);
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


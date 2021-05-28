/**
 * ============================================================================
 *  Name        : TheApp.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : LAB5
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
	m_uVertexShader = GetOpenGLRenderer()->CreateVertexShaderFromFile("phongshader.vs");
	m_uFragmentShader = GetOpenGLRenderer()->CreateFragmentShaderFromFile("phongshader.fs");
	m_uProgram = GetOpenGLRenderer()->CreateProgram(m_uVertexShader, m_uFragmentShader);
	m_uTexture[0] = GetOpenGLRenderer()->CreateTexture("terrain.jpg");
	m_uTexture[1] = GetOpenGLRenderer()->CreateTexture("grass.png");

	if (!m_uVertexShader || !m_uFragmentShader || !m_uProgram)
	{
		return false;
	}

	// setup our view and projection matrices
	m_mView = glm::lookAt(
		glm::vec3(0.0f, 3.0f, 30.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	m_mProjection = glm::perspective(0.7f, GetAspect(), 0.1f, 500.0f);

	// TODO: generate terrain & grass geometry
	m_Quad[0].GenQuad(glm::vec2(300.0f, 200.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	m_Quad[1].GenQuad(glm::vec2(10.0f, 30.0f), glm::vec3(0.0f, 0.0f, 4.0f));

	m_Material.m_cDiffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	m_Material.m_cSpecular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	m_Material.m_cAmbient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_Material.m_cEmissive = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_Material.m_fSpecularPower = 30.0f;

	// build the scenegraph
	m_pSceneRoot = new CTerrain();
	
	// Set attributes to the terrain plane
	CTerrain* pObject1 = new CTerrain();
	pObject1->SetPos(0.0f, 0.0f, -50.0f);
	pObject1->SetGeometry(&m_Quad[0]);
	pObject1->SetMaterial(&m_Material);
	pObject1->SetTexture(m_uTexture[0]);
	pObject1->SetRotation(-85.0f, (glm::vec3(1.0f, 0.0f, 0.0f)));

	// Set attributes for grass
	/*int i = 0;
	for (i = 0; i < 100; i++)
	{
		CTerrain* pObject2 = new CTerrain();
		pObject2->SetPos(0.0f, 0.0f, 0.0f);
		pObject2->SetGeometry(&m_Quad[1]);
		pObject2->SetMaterial(&m_Material);
		pObject2->SetTexture(m_uTexture[1]);
		pObject2->SetRotation(15.0f, (glm::vec3(1.0f, 0.0f, 0.0f)));
	}*/

	CTerrain* pObject2 = new CTerrain();
	pObject2->SetPos(0.0f, 0.0f, 0.0f);
	pObject2->SetGeometry(&m_Quad[1]);
	pObject2->SetMaterial(&m_Material);
	pObject2->SetTexture(m_uTexture[1]);
	pObject2->SetRotation(15.0f, (glm::vec3(1.0f, 0.0f, 0.0f)));

	// add children to the scene
	pObject1->AddChild(pObject2);
	m_pSceneRoot->AddChild(pObject1);

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
	pRenderer->Clear(0.6f, 0.6f, 1.0f, 1.0f);

	// set view and projection into the renderer
	GetRenderer()->SetViewMatrix(m_mView);
	GetRenderer()->SetProjectionMatrix(m_mProjection);

	// setup the rendering program
	glUseProgram(m_uProgram);

	// TODO: set material uniforms to program

	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uTexture);
	GLint location = glGetUniformLocation(m_uProgram, "texture01");
	glUniform1i(location, 0);

	SetTexturingParams();*/

	// setup the light position (above scene)
	const glm::vec3 campos(-pRenderer->GetViewMatrix()[3]);
	const glm::vec3 lightpos(0.0f, 10.0f, campos.z);
	GLint location = glGetUniformLocation(m_uProgram, "lightPosition");
	glUniform3f(location, lightpos.x, lightpos.y, lightpos.z);

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


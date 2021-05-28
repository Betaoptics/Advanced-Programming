/**
 * ============================================================================
 *  Name        : TheApp.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : LAB1
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#include "TheApp.h"
#include "TankPiece.h"


CTheApp::CTheApp()
{
	// constructor, init members
	m_uVertexShader = 0;
	m_uFragmentShader = 0;
	m_uProgram = 0;
	m_uTexture = 0;

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
	m_uTexture = GetOpenGLRenderer()->CreateTexture("USArmy.png");
	if (!m_uVertexShader || !m_uFragmentShader || !m_uProgram || !m_uTexture)
	{
		return false;
	}

	// setup our view and projection matrices
	m_mView = glm::lookAt(
		glm::vec3(0.0f, 7.0f, 25.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	m_mProjection = glm::perspective(0.7f, GetAspect(), 0.1f, 500.0f);

	// generate cube vertices for tank base
	m_CubeBase.GenCube(
		glm::vec3(9.0f, 1.5f, 15.0f),
		glm::vec3(0.0f, 0.0f, 0.0f));

	// generate cube vertices for tank tower
	m_CubeTower.GenCube(
		glm::vec3(4.5f, 4.5f, 4.5f),
		glm::vec3(0.0f, 0.0f, 0.0f));

	// generate cube vertices for tank cannon
	m_CubeCannon.GenCube(
		glm::vec3(1.0f, 1.0f, 8.0f),
		glm::vec3(0.0f, 0.0f, 4.0f));

	// set the material properties
	m_Material.m_cAmbient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	m_Material.m_cDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Material.m_cSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Material.m_cEmissive = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_Material.m_fSpecularPower = 10.0f;

	// build the scenegraph
	m_pSceneRoot = new CTankPiece();
	//m_pSceneRoot->SetRotationAxis(glm::vec3(0.0f, 1.0f, 0.0f));
	//m_pSceneRoot->SetRotationSpeed(float(0.1f));

	// add a tank base object
	CTankPiece* tankBaseObject = new CTankPiece;
	tankBaseObject->SetGeometry(&m_CubeBase);
	tankBaseObject->SetPos(glm::vec3(0.0f, 0.0f, 0.0f));

	// add a tank tower object
	CTankPiece* tankTowerObject = new CTankPiece;
	tankTowerObject->SetGeometry(&m_CubeTower);
	tankTowerObject->SetPos(glm::vec3(0.0f, 3.0f, 0.0f));

	// add a tank cannon object
	CTankPiece* tankCannonObject = new CTankPiece;
	tankCannonObject->SetGeometry(&m_CubeCannon);
	tankCannonObject->SetPos(glm::vec3(0.0f, 1.5f, 1.5f));


	tankTowerObject->AddChild(tankCannonObject);
	tankBaseObject->AddChild(tankTowerObject);
	m_pSceneRoot->AddChild(tankBaseObject);

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
	// update scenegraph
	if (m_pSceneRoot)
	{
		m_pSceneRoot->Update(fFrametime);
		// TODO list: Add a collision check 
	}

	// Set x, y and z-axis movement for tank base
	IGraphNode * base = m_pSceneRoot->GetChildren()[0];


	if (GetAsyncKeyState(VK_UP))
	{
		glm::mat4 baseForward = base->GetMatrix();
		baseForward = glm::translate(baseForward,
			glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));

		base->SetMatrix(baseForward);
	}

	if (GetAsyncKeyState(VK_DOWN))
	{
		glm::mat4 baseReverse = base->GetMatrix();
		baseReverse = glm::translate(baseReverse,
			glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f)));

		base->SetMatrix(baseReverse);
	}

	if (GetAsyncKeyState(VK_LEFT))
	{
		glm::mat4 baseLeft = base->GetMatrix();
		baseLeft = glm::rotate(baseLeft, fFrametime,
			glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
	
		base->SetMatrix(baseLeft);
	}

	if (GetAsyncKeyState(VK_RIGHT))
	{
		glm::mat4 baseRight = base->GetMatrix();
		baseRight = glm::rotate(baseRight, fFrametime,
			glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f)));

		base->SetMatrix(baseRight);
	}

	// Set x-axis rotation for tank tower
	IGraphNode* tower = base->GetChildren()[0];

	if (GetAsyncKeyState('A'))
	{
		glm::mat4 towerLeft = tower->GetMatrix();
		towerLeft = glm::rotate(towerLeft, fFrametime,
			glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f)));

		tower->SetMatrix(towerLeft);
	}

	if (GetAsyncKeyState('D'))
	{
		glm::mat4 towerRight = tower->GetMatrix();
		towerRight = glm::rotate(towerRight, fFrametime,
			glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));

		tower->SetMatrix(towerRight);
	}

	// Set y-axis rotation for tank cannon
	IGraphNode* cannon = tower->GetChildren()[0];

	if (GetAsyncKeyState('W'))
	{
		glm::mat4 cannonUp = cannon->GetMatrix();
		cannonUp = glm::rotate(cannonUp, fFrametime,
			glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f)));

		cannon->SetMatrix(cannonUp);
		
	}

	if (GetAsyncKeyState('S'))
	{
		glm::mat4 cannonDown = cannon->GetMatrix();
		cannonDown = glm::rotate(cannonDown, fFrametime,
			glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));

		cannon->SetMatrix(cannonDown);
	}
}

//TODO: Add the whole code of void:CTheApp::CheckCubeCollusions() et. al., for reference check lesson03 code.


void CTheApp::OnDraw(IRenderer* pRenderer)
{
	// clear color, depth and stencil buffers
	pRenderer->Clear(0.2f, 0.2f, 0.2f, 1.0f);

	// set view and projection into the renderer
	GetRenderer()->SetViewMatrix(m_mView);
	GetRenderer()->SetProjectionMatrix(m_mProjection);

	// setup the rendering of our quad
	glUseProgram(m_uProgram);

	// set the texture for the quad (slot 0)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uTexture);

	// the textures are bind as uniforms into the program
	GLint location = glGetUniformLocation(m_uProgram, "texture01");
	glUniform1i(location, 0);

	// setup the light position (to camera position)
	const glm::vec3 campos(-pRenderer->GetViewMatrix()[3]);
	location = glGetUniformLocation(m_uProgram, "lightPosition");
	glUniform3f(location, campos.x, campos.y, campos.z);

	// setup the camera position uniform
	location = glGetUniformLocation(m_uProgram, "cameraPosition");
	glUniform3f(location, campos.x, campos.y, campos.z);

	// set material to program
	m_Material.SetToProgram(m_uProgram);

	// set the geometry attributes for tank base
	m_CubeBase.SetAttribs(m_uProgram);

	// set the geometry attributes for tank tower
	m_CubeTower.SetAttribs(m_uProgram);

	// set the geometry attributes for tank cannon
	m_CubeCannon.SetAttribs(m_uProgram);

	// draw the scenegraph
	if (m_pSceneRoot)
	{
		m_pSceneRoot->Render(pRenderer, m_uProgram);
	}
}


void CTheApp::OnScreenSizeChanged(uint32_t uWidthPixels, uint32_t uHeightPixels)
{
	glm::mat4 m_mProjection = glm::perspective(0.7f, GetAspect(), 0.1f, 500.0f);
	GetRenderer()->SetProjectionMatrix(m_mProjection);
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


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
	GetRenderer()->SetViewMatrix(glm::lookAt(
		glm::vec3(0.0f, 0.0f, 15.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)));
	GetRenderer()->SetProjectionMatrix(glm::perspective(1.51f, GetAspect(), 0.1f, 500.0f));

	// build the scenegraph
	m_pSceneRoot = new CQuadNode();

	// set some velocity and rotation to entire scene
	m_pSceneRoot->SetVelocity(glm::vec3(0.0f, 0.0f, 1.0f));
	m_pSceneRoot->SetRotationSpeed(0.1f);

	// create some child nodes to the scene root
	const int32_t count = 1500;
	for (int32_t i=0; i<count; i++)
	{
		CQuadNode* pNode = new CQuadNode();

		// set a random position for the nodes
		pNode->SetPos(glm::linearRand(-10.0f, 10.0f),
			glm::linearRand(-10.0f, 10.0f),
			(i * 0.01f));

		// make some of the nodes rotate
		if (glm::linearRand(0, 3) == 1)
		{
			pNode->SetRotationSpeed(glm::linearRand(-1.0f, 1.0f));
		}

		// add new node to the scene root.
		// note that ownership of the node is transfered and node is deleted by its owner
		m_pSceneRoot->AddChild(pNode);
	}

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
	// the main loop
	if (m_pSceneRoot)
	{
		m_pSceneRoot->Update(fFrametime);

		// flip the velocity of the scene root if it moves too far from the camera
		glm::vec3 pos = m_pSceneRoot->GetPos();
		if (pos.z > 15.0f)
		{
			// clamp the position and flip the velocity
			pos.z = 15.0f;
			m_pSceneRoot->SetPos(pos);
			m_pSceneRoot->GetVelocity() = -m_pSceneRoot->GetVelocity();
		}
		else if (pos.z < -15.0f)
		{
			pos.z = -15.0f;
			m_pSceneRoot->SetPos(pos);
			m_pSceneRoot->GetVelocity() = -m_pSceneRoot->GetVelocity();
		}
	}
}


void CTheApp::OnDraw(IRenderer* pRenderer)
{
	// clear color, depth and stencil buffers
	pRenderer->Clear(0.2f, 0.2f, 0.2f, 1.0f);

	CQuadNode::Setup(m_uProgram, m_uTexture, (float*)m_Quad, VERTEX::GetStride());

	// set the alpha blending for the texture
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	if (m_pSceneRoot)
	{
		m_pSceneRoot->Render(pRenderer, m_uProgram);
	}
}


void CTheApp::OnScreenSizeChanged(uint32_t uWidthPixels, uint32_t uHeightPixels)
{
	GetRenderer()->SetProjectionMatrix(glm::perspective(1.51f, GetAspect(), 0.1f, 500.0f));
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


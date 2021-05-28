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
#include "GameObject.h"


CTheApp::CTheApp() : m_iShadowMapSize(2048)
{
	m_uShadowMap = 0;
	m_uFrameBuffer = 0;

	m_iOriginalFrameBuffer = -1;
	m_fLightAngle = 0.0f;

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
	if (!CreatePrograms())
	{
		return false;
	}

	if (!CreateShadowMap(m_iShadowMapSize, m_iShadowMapSize))
	{
		return false;
	}

	// setup our view and projection matrices
	glm::mat4 view = glm::lookAt(
		glm::vec3(4.0f, 5.0f, 14.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 projection = glm::perspective(0.61f, GetAspect(), 1.0f, 500.0f);

	// set them into the renderer
	GetRenderer()->SetViewMatrix(view);
	GetRenderer()->SetProjectionMatrix(projection);
	GetRenderer()->SetLightPos(0.0f, 2.0f, 0.0f);

	// generate geometrys
	m_Geometries[0].GenCube(glm::vec3(15.0f, 1.0f, 15.0f), glm::vec3(0.0f, -2.5f, 0.0f));
	m_Geometries[1].GenCube(glm::vec3(5.0f, 5.0f, 0.1f), glm::vec3(0.0f, 0.0f, -4.5f));
	m_Geometries[2].GenSphere(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 32, 32);
	m_Geometries[3].GenSphere(glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.0f, 0.0f, 0.0f), 16, 16);

	// init materials
	m_Materials[0].m_cDiffuse = glm::vec4(0.3f, 1.0f, 1.0f, 1.0f);
	m_Materials[0].m_cAmbient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_Materials[0].m_fSpecularPower = 0.0f;

	m_Materials[1].m_cDiffuse = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	m_Materials[1].m_cAmbient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_Materials[1].m_fSpecularPower = 0.0f;

	m_Materials[2].m_cDiffuse = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	m_Materials[2].m_cAmbient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	m_Materials[2].m_cSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Materials[2].m_fSpecularPower = 45.0f;

	m_Materials[3].m_cDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Materials[3].m_cAmbient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Materials[3].m_fSpecularPower = 0.0f;

	m_pSceneRoot = new CGameObject();

	// create walls
	CGameObject* pWall = new CGameObject;
	pWall->SetGeometry(&m_Geometries[0]);
	pWall->SetPos(0.0f, 0.0f, 0.0f);
	pWall->SetShadowReceiver(true);
	m_pSceneRoot->AddChild(pWall);

	pWall = new CGameObject;
	pWall->SetGeometry(&m_Geometries[1]);
	pWall->SetPos(0.0f, 0.0f, 0.0f);
	pWall->SetShadowReceiver(true);
	//pWall->SetShadowCaster(true);
	m_pSceneRoot->AddChild(pWall);

	// spawn a sphere
	CGameObject* pSphere = new CGameObject;
	pSphere->SetGeometry(&m_Geometries[2]);
	pSphere->SetPos(0.0f, -1.0f, 0.0f);
	pSphere->SetShadowCaster(true);
	m_pSceneRoot->AddChild(pSphere);

	// light position indicator
	pSphere = new CGameObject;
	pSphere->SetGeometry(&m_Geometries[3]);
	pSphere->SetPos(GetRenderer()->GetLightPos());
	m_pSceneRoot->AddChild(pSphere);

	std::vector<IGraphNode*>& arrNodes = m_pSceneRoot->GetChildren();
	for (int i = 0; i < arrNodes.size(); i++)
	{
		CGameObject* pGameObject = static_cast<CGameObject*>(arrNodes.at(i));
		pGameObject->SetMaterial(&m_Materials[i]);
	}

	return true;
}


bool CTheApp::CreatePrograms()
{
	GLuint vertexShader = GetOpenGLRenderer()->CreateVertexShaderFromFile("shadowmapping.vs");
	GLuint fragmentShader = GetOpenGLRenderer()->CreateFragmentShaderFromFile("shadowmapping.fs");
	GLuint program = GetOpenGLRenderer()->CreateProgram(vertexShader, fragmentShader);
	if (!vertexShader || !fragmentShader || !program)
	{
		return false;
	}

	// store shaders and program
	m_arrShaders.push_back(vertexShader);
	m_arrShaders.push_back(fragmentShader);
	m_arrPrograms.push_back(program);

	// program for rendering the shadow only
	vertexShader = GetOpenGLRenderer()->CreateVertexShaderFromFile("shadowonly.vs");
	fragmentShader = GetOpenGLRenderer()->CreateFragmentShaderFromFile("shadowonly.fs");
	program = GetOpenGLRenderer()->CreateProgram(vertexShader, fragmentShader);
	if (!vertexShader || !fragmentShader || !program)
	{
		return false;
	}

	// store
	m_arrShaders.push_back(vertexShader);
	m_arrShaders.push_back(fragmentShader);
	m_arrPrograms.push_back(program);
	return true;
}


bool CTheApp::CreateShadowMap(int iWidth, int iHeight)
{
	// store original frame buffer handle
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_iOriginalFrameBuffer);

	glGenTextures(1, &m_uShadowMap);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uShadowMap);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_DEPTH_COMPONENT,
		iWidth,
		iHeight,
		0,
		GL_DEPTH_COMPONENT,
		GL_UNSIGNED_INT,
		nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	
	glGenFramebuffers(1, &m_uFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_uFrameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_uShadowMap, 0);

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

	int32_t i;
	for (i = 0; i < m_arrPrograms.size(); i++)
	{
		glDeleteProgram(m_arrPrograms[i]);
	}
	m_arrPrograms.clear();

	for (i = 0; i < m_arrShaders.size(); i++)
	{
		glDeleteShader(m_arrShaders[i]);
	}
	m_arrShaders.clear();

	if (m_uShadowMap)
	{
		glDeleteTextures(1, &m_uShadowMap);
		m_uShadowMap = 0;
	}
	if (m_uFrameBuffer)
	{
		glDeleteFramebuffers(1, &m_uFrameBuffer);
		m_uFrameBuffer = 0;
	}
}


void CTheApp::OnUpdate(float fFrametime)
{
	if (m_pSceneRoot)
	{
		m_pSceneRoot->Update(fFrametime);

		constexpr float pi2 = glm::pi<float>() * 2.0f;
		m_fLightAngle += fFrametime;
		if (m_fLightAngle > pi2)
		{
			m_fLightAngle -= pi2;
		}

		glm::vec3 lightPos;
		lightPos.x = cosf(m_fLightAngle);
		lightPos.y = 0.9f;
		lightPos.z = sinf(m_fLightAngle) * 6.0f;

		GetRenderer()->SetLightPos(lightPos);

		m_pSceneRoot->GetChildren().at(3)->SetPos(lightPos);
	}
}


void CTheApp::OnDraw(IRenderer* pRenderer)
{
	// clear depth and stencil buffers
	glDisable(GL_BLEND);

	DrawShadowPass(pRenderer);

	DrawNormalPass(pRenderer);
}


void CTheApp::DrawShadowPass(IRenderer* pRenderer)
{
	SetShadowMapAsRenderTarget();

	GLuint program = m_arrPrograms.at(1);
	glUseProgram(program);

	if (m_pSceneRoot)
	{
		std::vector<IGraphNode*>& arrNodes = m_pSceneRoot->GetChildren();
		for (int i = 0; i < arrNodes.size(); i++)
		{
			CGameObject* pObject = static_cast<CGameObject*>(arrNodes.at(i));
			if (pObject->IsShadowCaster())
			{
				pObject->DrawShadowOnly(pRenderer, program);
			}
		}
	}
}


void CTheApp::SetShadowMapAsRenderTarget()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_uFrameBuffer);
	glViewport(0, 0, m_iShadowMapSize, m_iShadowMapSize);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glPolygonOffset(1.1f, 4.0f);
	glDisable(GL_POLYGON_OFFSET_FILL);
}


void CTheApp::DrawNormalPass(IRenderer* pRenderer)
{
	SetFramebufferAsRenderTarget();
	GLuint program = m_arrPrograms.at(0);
	glUseProgram(program);

	// setup the camera position
	const glm::vec3 campos(-pRenderer->GetViewMatrix()[3]);
	GLint location = glGetUniformLocation(program, "cameraPosition");
	glUniform3f(location, campos.x, campos.y, campos.z);

	// setup the light position
	const glm::vec3& lightPosition(pRenderer->GetLightPos());
	location = glGetUniformLocation(program, "lightPosition");
	glUniform3f(location, lightPosition.x, lightPosition.y, lightPosition.z);

	// set the shadow map into program
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uShadowMap);
	location = glGetUniformLocation(program, "shadowMap");
	glUniform1i(location, 0);

	if (m_pSceneRoot)
	{
		m_pSceneRoot->Render(pRenderer, program);
	}
}


void CTheApp::SetFramebufferAsRenderTarget()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_iOriginalFrameBuffer);
	glViewport(0, 0, GetWidth(), GetHeight());

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonOffset(0.0f, 0.0f);
	glDisable(GL_POLYGON_OFFSET_FILL);
}


void CTheApp::SetTexturingParams()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}


void CTheApp::OnScreenSizeChanged(uint32_t uWidthPixels, uint32_t uHeightPixels)
{
	glm::mat4 projection = glm::perspective(0.61f, GetAspect(), 1.0f, 500.0f);
	GetRenderer()->SetProjectionMatrix(projection);
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


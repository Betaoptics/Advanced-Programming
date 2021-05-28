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


CTheApp::CTheApp()
{
	// constructor, init members
	m_uVertexShader = 0;
	m_uFragmentShader = 0;
	m_uProgram = 0;
	m_uTextures[0] = 0;
	m_uTextures[1] = 0;
	m_uTextures[2] = 0;
	m_uTextures[3] = 0;

	m_fUvOffset = 0.0f;

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
	m_uTextures[0] = GetOpenGLRenderer()->CreateTexture("earth.jpg");
	m_uTextures[1] = GetOpenGLRenderer()->CreateTexture("hex.jpg");
	m_uTextures[2] = GetOpenGLRenderer()->CreateTexture("clouds.jpg");
	m_uTextures[3] = GetOpenGLRenderer()->CreateTexture("earth_specular.jpg");
	if (!m_uVertexShader || !m_uFragmentShader || !m_uProgram)
	{
		return false;
	}
	if (!m_uTextures[0] || !m_uTextures[1] || !m_uTextures[2] || !m_uTextures[3])
	{
		return false;
	}

	// setup our view and projection matrices
	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 9.5f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 projection = glm::perspective(0.61f, GetAspect(), 1.0f, 500.0f);

	// set them into the renderer
	GetRenderer()->SetViewMatrix(view);
	GetRenderer()->SetProjectionMatrix(projection);
	GetRenderer()->SetLightPos(0.0f, 0.0f, 15.0f);

	// generate geometrys
	m_Cube.GenCube(glm::vec3(4.0f, 4.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	m_Sphere.GenSphere(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), 24, 24);

	m_pSceneRoot = new CGameObject();

	// spawn a cube
	CGameObject* pCube = new CGameObject;
	pCube->SetPos(-2.5f, 0.0f, 0.0f);
	pCube->SetGeometry(&m_Cube);
	m_pSceneRoot->AddChild(pCube);

	// spawn a sphere
	CGameObject* pSphere = new CGameObject;
	pSphere->SetPos(2.5f, 0.0f, 0.0f);
	pSphere->SetGeometry(&m_Sphere);
	m_pSceneRoot->AddChild(pSphere);


	for (int32_t i = 0; i < m_pSceneRoot->GetChildren().size(); i++)
	{
		CGameObject* pGameObject = static_cast<CGameObject*>(m_pSceneRoot->GetChildren().at(i));
		pGameObject->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
		pGameObject->SetRotationSpeed(glm::linearRand(-1.0f, 1.0f));
		pGameObject->SetGravity(0.0f);
		pGameObject->SetMaterial(&m_Material);
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

	glDeleteTextures(4, m_uTextures);
	glDeleteProgram(m_uProgram);
	glDeleteShader(m_uFragmentShader);
	glDeleteShader(m_uVertexShader);

	m_uVertexShader = 0;
	m_uFragmentShader = 0;
	m_uProgram = 0;
}


void CTheApp::OnUpdate(float fFrametime)
{
	if (m_pSceneRoot)
	{
		m_pSceneRoot->Update(fFrametime);
	}

	// animate our texture uv2 offset
	m_fUvOffset += 0.1f * fFrametime;
	if (m_fUvOffset > 1.0f)
	{
		m_fUvOffset -= 1.0f;
	}
}


void CTheApp::OnDraw(IRenderer* pRenderer)
{
	// clear depth and stencil buffers
	pRenderer->Clear(0.2f, 0.2f, 0.2f, 1.0f);

	// render the sphere
	glUseProgram(m_uProgram);

	// setup the light position
	const glm::vec3& lightPosition = pRenderer->GetLightPos();
	GLint location = glGetUniformLocation(m_uProgram, "lightPosition");
	glUniform3f(location, lightPosition.x, lightPosition.y, lightPosition.z);

	// setup the camera position
	glm::vec3 campos(-pRenderer->GetViewMatrix()[3]);
	location = glGetUniformLocation(m_uProgram, "cameraPosition");
	glUniform3f(location, campos.x, campos.y, campos.z);

	// set the uv2 offset uniform
	location = glGetUniformLocation(m_uProgram, "uvOffset");
	glUniform1f(location, m_fUvOffset);

	glDisable(GL_BLEND);

	if (m_pSceneRoot)
	{
		int32_t i;
		std::vector<IGraphNode*>& arrChildren = m_pSceneRoot->GetChildren();
		for (i = 0; i < arrChildren.size(); i++)
		{
			// setup the texture for rendering
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_uTextures[0]);
			location = glGetUniformLocation(m_uProgram, "texture01");
			glUniform1i(location, 0);
			SetTexturingParams();

			if (i == 0)
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, m_uTextures[1]);
				location = glGetUniformLocation(m_uProgram, "texture02");
				glUniform1i(location, 1);
				SetTexturingParams();
			}
			else
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, m_uTextures[2]);
				location = glGetUniformLocation(m_uProgram, "texture02");
				glUniform1i(location, 1);
				SetTexturingParams();
			}

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, m_uTextures[3]);
			location = glGetUniformLocation(m_uProgram, "texture03");
			glUniform1i(location, 2);
			SetTexturingParams();

			arrChildren.at(i)->Render(pRenderer, m_uProgram);

			/**
			 * Your turn to code:
			 * - Add 3rd texture to program, make it a grayscale version of earth texture
			 * - Apply the 3rd texture in a shader as specular map
			 * - Specular map determines which parts of the object are shine and which are not
			 *
			 */
		}
	}
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


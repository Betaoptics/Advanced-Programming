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
	m_uTexture = GetOpenGLRenderer()->CreateTexture("earth.jpg");
	if (!m_uVertexShader || !m_uFragmentShader || !m_uProgram || !m_uTexture)
	{
		return false;
	}

	// setup our view and projection matrices
	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 32.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 projection = glm::perspective(0.61f, GetAspect(), 1.0f, 500.0f);

	// set them into the renderer
	GetRenderer()->SetViewMatrix(view);
	GetRenderer()->SetProjectionMatrix(projection);

	// set the material
	m_Material.m_cAmbient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	m_Material.m_cDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Material.m_cSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Material.m_cEmissive = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_Material.m_fSpecularPower = 25.0f;

	// generate sphere geometry
	constexpr float radius = 2.0f;
	m_Sphere.GenSphere(
		glm::vec3(radius, radius, radius),
		glm::vec3(0.0f, 0.0f, 0.0f),
		24,
		24);

	// build scenegraph
	m_pSceneRoot = new CGameObject();

	int32_t i;
	for (i = 0; i < 25; i++)
	{
		CGameObject* pObject = new CGameObject;
		pObject->SetGeometry(&m_Sphere);
		pObject->SetRadius(radius);
		pObject->SetPos(glm::vec3(glm::linearRand(-10.0f, 10.0f),
			glm::linearRand(-10.0f, 10.0f),
			glm::linearRand(-10.0f, 10.0f)));

		pObject->SetRandomRotationAxis();
		pObject->SetRotationAngle(glm::linearRand(0.0f, 6.0f));
		pObject->SetRotationSpeed(glm::linearRand(-10.0f, 10.0f));
		pObject->SetVelocity(glm::vec3(
			glm::linearRand(-10.0f, 10.0f),
			glm::linearRand(0.0f, 10.0f),
			glm::linearRand(-10.0f, 10.0f)));

		m_pSceneRoot->AddChild(pObject);
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
	if (m_pSceneRoot)
	{
		m_pSceneRoot->Update(fFrametime);
		CheckSphereToSphereCollisions();
	}
}


void CTheApp::CheckSphereToSphereCollisions()
{
	std::vector<IGraphNode*>& arrChildren = m_pSceneRoot->GetChildren();

	int32_t i;
	for (i = 0; i < arrChildren.size(); i++)
	{
		CGameObject* pObject1 = static_cast<CGameObject*>(arrChildren.at(i));

		int32_t j;
		for (j = 0; j < arrChildren.size(); j++)
		{
			CGameObject* pObject2 = static_cast<CGameObject*>(arrChildren.at(j));
			if (pObject1 != pObject2)
			{
				// calculate vector from object1 to object2
				glm::vec3 pos1 = pObject1->GetPos();
				glm::vec3 pos2 = pObject2->GetPos();
				glm::vec3 d(pos2 - pos1);

				const float len = glm::length(d);
				const float r = pObject1->GetRadius() + pObject2->GetRadius();
				if (len < r)
				{
					// objects intersect

					// calculate how much objects intersect
					const float inside = (r - len) * 1.001f;

					// normalize the d to use it as direction to move away from intersection
					d = glm::normalize(d);

					// move object positions away from intersection by half of the
					// intersection distance
					pos1 += d * -inside * 0.5f;
					pos2 += d * inside * 0.5f;

					pObject1->SetPos(pos1);
					pObject2->SetPos(pos2);


					// rest of the collision response from gamasutra article:
					// http://www.gamasutra.com/view/feature/131424/pool_hall_lessons_fast_accurate.php

					float mass1 = 1.0f;
					float mass2 = 1.0f;

					// Find the length of the component of each of the movement
					// vectors along n. 
					glm::vec3 v1 = pObject1->GetVelocity();
					glm::vec3 v2 = pObject2->GetVelocity();
					float a1 = glm::dot(v1, d);
					float a2 = glm::dot(v2, d);

					// Using the optimized version, 
					// optimizedP =  2(a1 - a2)
					//              -----------
					//                m1 + m2
					float optimizedP = (2.0f * (a1 - a2)) / (mass1 + mass2);

					// Calculate v1', the new movement vector of circle1
					v1 = v1 - optimizedP * mass2 * d;

					// Calculate v2', the new movement vector of circle2
					v2 = v2 + optimizedP * mass1 * d;

					pObject1->SetVelocity(v1);
					pObject2->SetVelocity(v2);

					pObject1->SetRotationSpeed(glm::linearRand(-10.0f, 10.0f));
					pObject2->SetRotationSpeed(glm::linearRand(-10.0f, 10.0f));
				}
			}
		}
	}
}


void CTheApp::OnDraw(IRenderer* pRenderer)
{
	// clear depth and stencil buffers
	pRenderer->Clear(0.2f, 0.2f, 0.2f, 1.0f);

	// render the sphere
	glUseProgram(m_uProgram);

	// get the vertex attribute locations
	GLint position = glGetAttribLocation(m_uProgram, "position");
	GLint normal = glGetAttribLocation(m_uProgram, "normal");
	GLint uv = glGetAttribLocation(m_uProgram, "uv");

	// set the vertex position
	glEnableVertexAttribArray(position);
	glVertexAttribPointer(
		position,
		3,
		GL_FLOAT,
		GL_FALSE,
		VERTEX::GetStride(),
		m_Sphere.GetData());

	// vertex normal
	glEnableVertexAttribArray(normal);
	glVertexAttribPointer(
		normal,
		3,
		GL_FLOAT,
		GL_FALSE,
		VERTEX::GetStride(),
		(char*)((float*)m_Sphere.GetData() + 3));

	// vertex uv
	glEnableVertexAttribArray(uv);
	glVertexAttribPointer(
		uv,
		2,
		GL_FLOAT,
		GL_FALSE,
		VERTEX::GetStride(),
		(char*)((float*)m_Sphere.GetData() + 6));

	// setup the texture for rendering
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uTexture);
	GLint location = glGetUniformLocation(m_uProgram, "texture01");
	glUniform1i(location, 0);

	// setup the light position
	glm::vec3 lightPos(0.0f, 0.0f, 15.0f);
	location = glGetUniformLocation(m_uProgram, "lightPosition");
	glUniform3f(location, lightPos.x, lightPos.y, lightPos.z);

	// setup the camera position
	glm::vec3 campos(-pRenderer->GetViewMatrix()[3]);
	location = glGetUniformLocation(m_uProgram, "cameraPosition");
	glUniform3f(location, campos.x, campos.y, campos.z);

	// set material uniforms to program
	m_Material.SetToProgram(m_uProgram);

	if (m_pSceneRoot)
	{
		m_pSceneRoot->Render(pRenderer, m_uProgram);
	}
}


void CTheApp::OnScreenSizeChanged(uint32_t uWidthPixels, uint32_t uHeightPixels)
{
	const glm::mat4 projection = glm::perspective(0.61f, GetAspect(), 1.0f, 500.0f);
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


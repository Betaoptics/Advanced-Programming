/**
 * ============================================================================
 *  Name        : Geometry.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : 3d geometry generator and storage
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#include "../include/Geometry.h"


Geometry::Geometry()
{
	m_IndexBuffer = 0;
	m_uIndexCount = 0;
	m_eDrawMode = GL_TRIANGLES;
}


Geometry::~Geometry()
{
	Clear();
}


void Geometry::Clear()
{
	m_arrVertices.clear();
	if (m_IndexBuffer)
	{
		glDeleteBuffers(1, &m_IndexBuffer);
		m_IndexBuffer = 0;
	}
	m_uIndexCount = 0;
}


void Geometry::GenSphere(const glm::vec3& vRadius, const glm::vec3& vOffset, uint32_t uRings, uint32_t uSegments)
{
	Clear();
	GenSphere(vRadius, vOffset, uRings, uSegments, m_arrVertices);
	m_eDrawMode = GL_TRIANGLE_STRIP;
}


void Geometry::GenCube(const glm::vec3& vSize, const glm::vec3& vOffset)
{
	Clear();
	GenCube(vSize, vOffset, m_arrVertices, m_IndexBuffer, m_uIndexCount);
	m_eDrawMode = GL_TRIANGLES;
}


void Geometry::GenQuad(const glm::vec2& vSize, const glm::vec3& vOffset)
{
	Clear();
	GenQuad(vSize, vOffset, m_arrVertices);
	m_eDrawMode = GL_TRIANGLES;
}


void Geometry::SetAttribs(GLuint uProgram) const
{
	// get the vertex attribute locations
	GLint position = glGetAttribLocation(uProgram, "position");
	GLint normal = glGetAttribLocation(uProgram, "normal");
	GLint uv = glGetAttribLocation(uProgram, "uv");

	// set the vertex position
	glEnableVertexAttribArray(position);
	glVertexAttribPointer(
		position,
		3,
		GL_FLOAT,
		GL_FALSE,
		VERTEX::GetStride(),
		GetData());

	// vertex normal
	glEnableVertexAttribArray(normal);
	glVertexAttribPointer(
		normal,
		3,
		GL_FLOAT,
		GL_FALSE,
		VERTEX::GetStride(),
		(char*)((float*)GetData() + 3));

	// vertex uv
	glEnableVertexAttribArray(uv);
	glVertexAttribPointer(
		uv,
		2,
		GL_FLOAT,
		GL_FALSE,
		VERTEX::GetStride(),
		(char*)((float*)GetData() + 6));
}


void Geometry::DisableAttribs(GLuint uProgram) const
{
	// get the vertex attribute locations
	GLint position = glGetAttribLocation(uProgram, "position");
	GLint normal = glGetAttribLocation(uProgram, "normal");
	GLint uv = glGetAttribLocation(uProgram, "uv");

	glDisableVertexAttribArray(position);
	glDisableVertexAttribArray(normal);
	glDisableVertexAttribArray(uv);
}


void Geometry::Draw(IRenderer* pRenderer) const
{
	if (m_IndexBuffer && m_uIndexCount)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		glDrawElements(m_eDrawMode, (GLsizei)m_uIndexCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDrawArrays(m_eDrawMode, 0, (GLsizei)GetVertexCount());
	}
}


void Geometry::GenSphere(const glm::vec3& vRadius, const glm::vec3& vOffset, uint32_t uRings, uint32_t uSegments, std::vector<VERTEX>& arrVertices)
{
	const float deltaRingAngle = (glm::pi<float>() / uRings);
	const float deltaSegAngle = (glm::two_pi<float>() / uSegments);

	for (uint32_t ring = 0; ring < uRings; ring++)
	{
		const float r0 = sinf((ring + 0) * deltaRingAngle);
		const float r1 = sinf((ring + 1) * deltaRingAngle);
		const float y0 = cosf((ring + 0) * deltaRingAngle);
		const float y1 = cosf((ring + 1) * deltaRingAngle);

		for (uint32_t seg = 0; seg < (uSegments + 1); seg++)
		{
			const float angle = seg * deltaSegAngle;
			const float x0 = r0 * sinf(angle);
			const float z0 = r0 * cosf(angle);
			const float x1 = r1 * sinf(angle);
			const float z1 = r1 * cosf(angle);

			arrVertices.push_back(VERTEX(
				vRadius.x * x1 + vOffset.x, vRadius.y * y1 + vOffset.y, vRadius.z * z1 + vOffset.z,
				x1, y1, z1,
				((float)seg) / uSegments,
				(ring + 1) / (float)uRings));

			arrVertices.push_back(VERTEX(
				vRadius.x * x0 + vOffset.x, vRadius.y * y0 + vOffset.y, vRadius.z * z0 + vOffset.z,
				x0, y0, z0,
				((float)seg) / uSegments,
				(ring / (float)uRings)));
		}
	}
}


void Geometry::GenCube(const glm::vec3& vSize, const glm::vec3& vOffset, std::vector<VERTEX>& arrVertices, GLuint& uIndexbuffer, size_t& uIndexCount)
{
	// cube normals
	const glm::vec3 nor1(1.0f, 0.0f, 0.0f);
	const glm::vec3 nor2(0.0f, 1.0f, 0.0f);
	const glm::vec3 nor3(0.0f, 0.0f, 1.0f);

	// cube size
	const float fW = vSize.x * 0.5f;
	const float fH = vSize.y * 0.5f;
	const float fD = vSize.z * 0.5f;

	// cube corners
	glm::vec3 p[8];
	p[0] = glm::vec3(-fW,  fH,  fD);
	p[1] = glm::vec3( fW,  fH,  fD);
	p[2] = glm::vec3( fW, -fH,  fD);
	p[3] = glm::vec3(-fW, -fH,  fD);
	p[4] = glm::vec3(-fW, -fH, -fD);
	p[5] = glm::vec3( fW, -fH, -fD);
	p[6] = glm::vec3( fW,  fH, -fD);
	p[7] = glm::vec3(-fW,  fH, -fD);

	// apply offset
	for (int32_t i=0; i<8; i++)
	{
		p[i] += vOffset;
	}

	// front face
	arrVertices.push_back(VERTEX(p[0], nor3, 0.0f, 0.0f));
	arrVertices.push_back(VERTEX(p[1], nor3, 1.0f, 0.0f));
	arrVertices.push_back(VERTEX(p[2], nor3, 1.0f, 1.0f));
	arrVertices.push_back(VERTEX(p[3], nor3, 0.0f, 1.0f));
	// right face
	arrVertices.push_back(VERTEX(p[1], nor1, 0.0f, 0.0f));
	arrVertices.push_back(VERTEX(p[6], nor1, 1.0f, 0.0f));
	arrVertices.push_back(VERTEX(p[5], nor1, 1.0f, 1.0f));
	arrVertices.push_back(VERTEX(p[2], nor1, 0.0f, 1.0f));
	// back face
	arrVertices.push_back(VERTEX(p[6], -nor3, 0.0f, 0.0f));
	arrVertices.push_back(VERTEX(p[7], -nor3, 1.0f, 0.0f));
	arrVertices.push_back(VERTEX(p[4], -nor3, 1.0f, 1.0f));
	arrVertices.push_back(VERTEX(p[5], -nor3, 0.0f, 1.0f));
	// left face
	arrVertices.push_back(VERTEX(p[7], -nor1, 0.0f, 0.0f));
	arrVertices.push_back(VERTEX(p[0], -nor1, 1.0f, 0.0f));
	arrVertices.push_back(VERTEX(p[3], -nor1, 1.0f, 1.0f));
	arrVertices.push_back(VERTEX(p[4], -nor1, 0.0f, 1.0f));
	// top face
	arrVertices.push_back(VERTEX(p[7], nor2, 0.0f, 0.0f));
	arrVertices.push_back(VERTEX(p[6], nor2, 1.0f, 0.0f));
	arrVertices.push_back(VERTEX(p[1], nor2, 1.0f, 1.0f));
	arrVertices.push_back(VERTEX(p[0], nor2, 0.0f, 1.0f));
	// bottom face
	arrVertices.push_back(VERTEX(p[3], -nor2, 0.0f, 0.0f));
	arrVertices.push_back(VERTEX(p[2], -nor2, 1.0f, 0.0f));
	arrVertices.push_back(VERTEX(p[5], -nor2, 1.0f, 1.0f));
	arrVertices.push_back(VERTEX(p[4], -nor2, 0.0f, 1.0f));


	// create indices to cube object
	uint32_t indices[36];

	for (int32_t i=0, j=0; i<21; i+=4, j+=6)
	{
		indices[j] = (i + 0);
		indices[j + 1] = (i + 1);
		indices[j + 2] = (i + 2);
		indices[j + 3] = (i + 0);
		indices[j + 4] = (i + 2);
		indices[j + 5] = (i + 3);
	}

	glGenBuffers(1, &uIndexbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uIndexbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	uIndexCount = 36;
}


void Geometry::GenQuad(const glm::vec2& vSize, const glm::vec3& vOffset, std::vector<VERTEX>& arrVertices)
{
	// quad size
	const float fW = vSize.x * 0.5f;
	const float fH = vSize.y * 0.5f;

	// quad normal
	const glm::vec3 normal(0.0f, 0.0f, 1.0f);

	// quad corners
	glm::vec3 p[4];
	p[0] = glm::vec3( fW, -fH,  0.0f);
	p[1] = glm::vec3(-fW, -fH,  0.0f);
	p[2] = glm::vec3(-fW,  fH,  0.0f);
	p[3] = glm::vec3( fW,  fH,  0.0f);

	// apply offset
	for (int32_t i=0; i<4; i++)
	{
		p[i] += vOffset;
	}

	arrVertices.push_back(VERTEX(p[0], normal, 1.0f, 1.0f));
	arrVertices.push_back(VERTEX(p[1], normal, 0.0f, 1.0f));
	arrVertices.push_back(VERTEX(p[2], normal, 0.0f, 0.0f));

	arrVertices.push_back(VERTEX(p[2], normal, 0.0f, 0.0f));
	arrVertices.push_back(VERTEX(p[3], normal, 1.0f, 0.0f));
	arrVertices.push_back(VERTEX(p[0], normal, 1.0f, 1.0f));
}



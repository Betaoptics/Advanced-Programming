/**
 * ============================================================================
 *  Name        : Geometry.h
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : 3d geometry generator and storage
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#pragma once

#include <vector>
#include "../include/OpenGLRenderer.h"

struct VERTEX
{
	VERTEX()
	{
		x = 0.0f; y = 0.0f; z = 0.0f;
		nx = 0.0f; ny = 0.0f; nz = 0.0f;
		tu = 0.0f; tv = 0.0f;
	}
	VERTEX(float _x, float _y, float _z,
		float _nx, float _ny, float _nz,
		float _tu, float _tv)
	{
		x = _x; y = _y; z = _z;
		nx = _nx; ny = _ny; nz = _nz;
		tu = _tu; tv = _tv;
	}
	VERTEX(const glm::vec3 pos, const glm::vec3& normal, float _tu, float _tv)
	{
		x = pos.x; y = pos.y; z = pos.z;
		nx = normal.x; ny = normal.y; nz = normal.z;
		tu = _tu; tv = _tv;
	}

	/**
	* GetStride
	* @return number of bytes allocated by one vertex
	*/
	static int GetStride() { return 32; }

	float	x, y, z;
	float	nx, ny, nz;
	float	tu, tv;
};


class Geometry
{
public:
	Geometry();
	~Geometry();

	void Clear();

	/**
	 * GenSphere
	 * generate sphere. Sphere is single triangle strip, no indexing
	 * @param vRadius radius
	 * @param vOffset offset
	 * @param uRings number of rings in geometry
	 * @param uSegments number of segments in geometry
	 */
	void GenSphere(const glm::vec3& vRadius, const glm::vec3& vOffset, uint32_t uRings, uint32_t uSegments);

	/**
	 * GenCube
	 * generate cube. Cube is a triangle list with index buffer
	 * @param vSize size
	 * @param vOffset offset
	 */
	void GenCube(const glm::vec3& vSize, const glm::vec3& vOffset);

	/**
	 * GenQuad
	 * generate a front facing quad. Quad is a single triangle list, no indexing
	 * @param vSize size
	 * @param vOffset offset
	 */
	void GenQuad(const glm::vec2& vSize, const glm::vec3& vOffset);

	void SetAttribs(GLuint uProgram) const;
	void DisableAttribs(GLuint uProgram) const;

	void Draw(IRenderer* pRenderer) const;

	static void GenSphere(const glm::vec3& vRadius, const glm::vec3& vOffset, uint32_t uRings, uint32_t uSegments, std::vector<VERTEX>& arrVertices);
	static void GenCube(const glm::vec3& vSize, const glm::vec3& vOffset, std::vector<VERTEX>& arrVertices, GLuint& uIndexbuffer, size_t& uIndexCount);
	static void GenQuad(const glm::vec2& vSize, const glm::vec3& vOffset, std::vector<VERTEX>& arrVertices);

	inline VERTEX* GetData() { return m_arrVertices.data(); }
	inline const VERTEX* GetData() const { return m_arrVertices.data(); }
	inline size_t GetVertexCount() const { return m_arrVertices.size(); }
	inline GLuint GetIndexBuffer() const { return m_IndexBuffer; }
	inline size_t GetIndexCount() const { return m_uIndexCount; }

private:
	std::vector<VERTEX>			m_arrVertices;
	GLenum						m_eDrawMode;
	GLuint						m_IndexBuffer;
	size_t						m_uIndexCount;
};


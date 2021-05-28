/**
 * ============================================================================
 *  Name        : Material.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : simple material
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#include "../include/Material.h"


Material::Material()
{
	m_cAmbient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	m_cDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_cSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_cEmissive = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_fSpecularPower = 50.0f;
}


Material::~Material()
{
}


void Material::SetToProgram(GLuint uProgram)
{
	GLint location = glGetUniformLocation(uProgram, "materialAmbient");
	if (location != -1)
	{
		glUniform4f(location, m_cAmbient.r, m_cAmbient.g, m_cAmbient.b, m_cAmbient.a);
	}

	location = glGetUniformLocation(uProgram, "materialDiffuse");
	if (location != -1)
	{
		glUniform4f(location, m_cDiffuse.r, m_cDiffuse.g, m_cDiffuse.b, m_cDiffuse.a);
	}

	location = glGetUniformLocation(uProgram, "materialSpecular");
	if (location != -1)
	{
		glUniform4f(location, m_cSpecular.r, m_cSpecular.g, m_cSpecular.b, m_cSpecular.a);
	}

	location = glGetUniformLocation(uProgram, "materialEmissive");
	if (location != -1)
	{
		glUniform4f(location, m_cEmissive.r, m_cEmissive.g, m_cEmissive.b, m_cEmissive.a);
	}

	location = glGetUniformLocation(uProgram, "specularPower");
	if (location != -1)
	{
		glUniform1f(location, m_fSpecularPower);
	}
}



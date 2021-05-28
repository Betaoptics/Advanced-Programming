/**
 * ============================================================================
 *  Name        : Grass.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : LAB5
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/

#include "Grass.h"


CGrass::CGrass()
{
}


void CGrass::Update(float fFrametime, glm::vec3(axis))
{
    // TODO: grass swing animation
	SetMatrix(glm::rotate(GetMatrix(), fFrametime, glm::normalize(axis)));

    CTerrain::Update(fFrametime);
}




void CGrass::Render(IRenderer* pRenderer, GLuint uProgram)
{
	// TODO: set the alpha blending for the grass texture

	CTerrain::Render(pRenderer, uProgram);
}

//void CTerrain::SetSwing(float angle, glm::vec3 axis) {
//	SetMatrix(glm::rotate(GetMatrix(), fFrametime, glm::normalize(axis)));
//}

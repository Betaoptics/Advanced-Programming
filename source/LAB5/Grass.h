/**
 * ============================================================================
 *  Name        : Grass.h
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : LAB5
 *  Version     : 1.00
 *	Author		: Jani Immonen, <realdashdev@gmail.com>
 * ============================================================================
**/


#pragma once
#include "Terrain.h"
#include "TheApp.h"

class CGrass : public CTerrain
{
public:
    CGrass();

	/*
	 * Update
	 * update node and its children
	 * @param fFrametime frame delta time
	 */
	void Update(float fFrametime, glm::vec3 axis) override;

	/**
	 * Draw
	 * virtual drawing method of the game object
	 * @param pRenderer pointer to renderer
	 */
	void Render(IRenderer* pRenderer, GLuint uProgram) override;
	void SetSwing(float fFrametime, glm::vec3 axis);

private:
	// TODO: member for the swing animation
	float		swing;
};


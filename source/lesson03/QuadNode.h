#pragma once
#include "../core/include/IGraphNode.h"

class CQuadNode : public IGraphNode
{
public:
	CQuadNode();
	~CQuadNode();

	static void Setup(GLuint uProgram, GLuint uTexture, float* pVertexData, int32_t iVertexStride);

	void Render(IRenderer* pRenderer, GLuint uProgram) override;
};


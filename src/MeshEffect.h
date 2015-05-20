#pragma once

#include "Common.h"
#include "Effect.h"
#include "MeshRenderer.h"
#include <glm/glm.hpp>



//Effects for drawing meshes will implement this.
class MeshEffect: public Effect
{
public:
	virtual void Draw(MeshPtr mesh, MaterialPtr mat, MeshRenderingData & data) = 0;
};
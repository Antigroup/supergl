#pragma once

#include "Common.h"
#include "MeshEffect.h"
#include "ShaderProgram.h"
#include "Shaders.h"

class BasicEffect:public MeshEffect
{
	ShaderProgramPtr _directional;
	ShaderProgramPtr _point;
	ShaderProgramPtr _spot;
public:
	BasicEffect();

	bool Initialize();
	void Draw(MeshPtr mesh, MaterialPtr mat, MeshRenderingData & data);
	void FillParameterMaps(Material * mat);
};
//Contains source code and important definition for writing shaders.
#pragma once

#include "Common.h"
#include <glm/glm.hpp>

//C types to pass to shaders
struct DirectionalLight
{
	glm::vec4 Direction_Intensity;
	glm::vec4 Color_AmbientIntensity;
};

struct PointLight
{
	glm::vec4 Position_Intensity;
	glm::vec4 Color_AmbientIntensity;
	glm::vec4 Attenuation_Pad;
};

struct SpotLight
{
	glm::vec4 Position_Intensity;
	glm::vec4 Color_AmbientIntensity;
	glm::vec4 Attenuation_InnerAngle;
	glm::vec4 Direction_OuterAngle;
};

void FillUniform(const DirectionalLight & light, ShaderProgramPtr program, const std::string & varName);
void FillUniform(const PointLight & light, ShaderProgramPtr program, const std::string & varName);
void FillUniform(const SpotLight & light, ShaderProgramPtr program, const std::string & varName);

//Shader code
extern const char * LIGHT_STRUCTS;
extern const char * LIGHT_CALCULATIONS;
extern const char * MESH_UNIFORMS;
extern const char * MESH_ATTRIBUTES;
extern const char * MESH_VARYINGS;
extern const char * BASIC_VERTEX_SHADER;
extern const char * BASICEFFECT_FRAG_DIRECTIONAL;
extern const char * BASICEFFECT_FRAG_POINT;
extern const char * BASICEFFECT_FRAG_SPOT;
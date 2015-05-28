#include "Common.h"
#include "Shaders.h"
#include "ShaderProgram.h"

void FillUniform(const DirectionalLight & light, ShaderProgramPtr program, const std::string & varName)
{
	program->SetUniform(varName + ".Direction_Intensity", light.Direction_Intensity);
	program->SetUniform(varName + ".Color_AmbientIntensity", light.Color_AmbientIntensity);
}

void FillUniform(const PointLight & light, ShaderProgramPtr program, const std::string & varName)
{
	program->SetUniform(varName + ".Position_Intensity", light.Position_Intensity);
	program->SetUniform(varName + ".Color_AmbientIntensity", light.Color_AmbientIntensity);
	program->SetUniform(varName + ".Attenuation_Pad", light.Attenuation_Pad);
}

void FillUniform(const SpotLight & light, ShaderProgramPtr program, const std::string & varName)
{
	program->SetUniform(varName + ".Position_Intensity", light.Position_Intensity);
	program->SetUniform(varName + ".Color_AmbientIntensity", light.Color_AmbientIntensity);
	program->SetUniform(varName + ".Attenuation_InnerAngle", light.Attenuation_InnerAngle);
	program->SetUniform(varName + ".Direction_OuterAngle", light.Direction_OuterAngle);
}

const char * LIGHT_STRUCTS =
"struct DirectionalLight"
"{"
"	vec4 Direction_Intensity;"
"	vec4 Color_AmbientIntensity;"
"}; "
"struct PointLight"
"{"
"	vec4 Position_Intensity;"
"	vec4 Color_AmbientIntensity;"
"	vec4 Attenuation_Pad;"
"};"
"struct SpotLight"
"{"
"	vec4 Position_Intensity;"
"	vec4 Color_AmbientIntensity;"
"	vec4 Attenuation_InnerAngle;"
"	vec4 Direction_OuterAngle;"
"};";

const char * LIGHT_CALCULATIONS =
"float LambertDiffuse(vec3 normal, vec3 lightVector)"
"{"
"   return clamp(dot(normal, lightVector), 0.0, 1.0);"
"}"

"float BlinnPhongSpecular(vec3 normal, vec3 lightVector, vec3 viewVector, float power)"
"{"
"   vec3 halfVector = normalize(lightVector + viewVector);"
"   return pow(clamp(dot(normal, halfVector), 0.0, 1.0), power);"
"}"

"vec3 ComputeDirectionalLightLBP(vec3 normal, vec3 viewVector, float power, DirectionalLight light, vec3 diffuse, vec3 specular)"
"{"
"   float diff = LambertDiffuse(normal, light.Direction_Intensity.xyz);"
"   float spec = BlinnPhongSpecular(normal, light.Direction_Intensity.xyz, viewVector, power);"
"   return (diff * diffuse + spec * specular) * light.Direction_Intensity.w * light.Color_AmbientIntensity.xyz +"
"      light.Color_AmbientIntensity.xyz * light.Color_AmbientIntensity.w * diffuse;"
"}"

"vec3 ComputePointLightLBP(vec3 normal, vec3 viewVector, vec3 position, float power, PointLight light, vec3 diffuse, vec3 specular)"
"{"
"   vec3 lightDirection = light.Position_Intensity.xyz - position;"
"   float distance = length(lightDirection);"
"   lightDirection = lightDirection / distance;"
"   float diff = LambertDiffuse(normal, lightDirection);"
"   float spec = BlinnPhongSpecular(normal, lightDirection, viewVector, power);"
"   float att = 1.0 / (light.Attenuation_Pad.x * (distance * distance) + light.Attenuation_Pad.y * "
"      distance + light.Attenuation_Pad.z);"
"   return ((diff * diffuse + spec * specular) * light.Position_Intensity.w * light.Color_AmbientIntensity.xyz +"
"      light.Color_AmbientIntensity.xyz * light.Color_AmbientIntensity.w * diffuse) * att;"
"}"

"vec3 ComputeSpotLightLBP(vec3 normal, vec3 viewVector, vec3 position, float power, SpotLight light, vec3 diffuse, vec3 specular)"
"{"
"   vec3 lightDirection = light.Position_Intensity.xyz - position;"
"   float distance = length(lightDirection);"
"   lightDirection = lightDirection / distance;"
"   float diff = LambertDiffuse(normal, lightDirection);"
"   float spec = BlinnPhongSpecular(normal, lightDirection, viewVector, power);"
"   float att = 1.0 / (light.Attenuation_InnerAngle.x * distance * distance + light.Attenuation_InnerAngle.y * "
"      distance + light.Attenuation_InnerAngle.z);"
"   float spot = dot(-lightDirection, light.Direction_OuterAngle.xyz);"
//Spot function y = (1/(InnerCos - OuterCos)x - ((OuterCos)/(InnerCos - OuterCos))
"   float innerCos = light.Attenuation_InnerAngle.w;"
"   float outerCos = light.Direction_OuterAngle.w;"
"   spot = clamp(1.0/(innerCos - outerCos)*spot - ((outerCos)/(innerCos - outerCos)), 0.0, 1.0);"
"   return ((diff * diffuse + spec * specular) * light.Position_Intensity.w * light.Color_AmbientIntensity.xyz +"
"      light.Color_AmbientIntensity.xyz * light.Color_AmbientIntensity.w * diffuse) * att * spot;"
"}";

const char * MESH_UNIFORMS =
"uniform mat4 WVP;"
"uniform mat4 WORLD;"
"uniform mat4 WORLD_N;";

const char * MESH_ATTRIBUTES =
"attribute vec4 POSITION;"
"attribute vec3 NORMAL;"
"attribute vec2 TEXCOORD;";

const char * MESH_VARYINGS =
"varying vec3 V_POSITION;"
"varying vec3 V_NORMAL;"
"varying vec2 V_TEXCOORD;";

const char * BASIC_VERTEX_SHADER =
"void main()"
"{"
"   gl_Position = WVP * POSITION;"
"   V_POSITION = (WORLD * POSITION).xyz;"
"   V_NORMAL = (WORLD_N * vec4(NORMAL, 0.0)).xyz;"
"   V_TEXCOORD = TEXCOORD;"
"}";

const char * BASICEFFECT_FRAG_DIRECTIONAL =
"uniform DirectionalLight light;"
"uniform vec3 EyePosition;"
"uniform float Shine;"
"uniform vec3 DiffuseColor;"
"uniform vec3 SpecularColor;"
"uniform sampler2D DiffuseTex;"

"void main()"
"{"
"   vec3 viewVector = normalize(EyePosition - V_POSITION);"
"   vec3 diffuse = texture2D(DiffuseTex, V_TEXCOORD).xyz * DiffuseColor;"
"   gl_FragColor = vec4(ComputeDirectionalLightLBP(normalize(V_NORMAL), viewVector, Shine, light, diffuse, SpecularColor), 1.0);"
"}";

const char * BASICEFFECT_FRAG_POINT =
"uniform PointLight light;"
"uniform vec3 EyePosition;"
"uniform float Shine;"
"uniform vec3 DiffuseColor;"
"uniform vec3 SpecularColor;"
"uniform sampler2D DiffuseTex;"

"void main()"
"{"
"   vec3 viewVector = normalize(EyePosition - V_POSITION);"
"   vec3 diffuse = texture2D(DiffuseTex, V_TEXCOORD).xyz * DiffuseColor;"
"   gl_FragColor = vec4(ComputePointLightLBP(normalize(V_NORMAL), viewVector, V_POSITION, Shine, light, diffuse, SpecularColor), 1.0);"
"}";

const char * BASICEFFECT_FRAG_SPOT =
"uniform SpotLight light;"
"uniform vec3 EyePosition;"
"uniform float Shine;"
"uniform vec3 DiffuseColor;"
"uniform vec3 SpecularColor;"
"uniform sampler2D DiffuseTex;"

"void main()"
"{"
"   vec3 viewVector = normalize(EyePosition - V_POSITION);"
"   vec3 diffuse = texture2D(DiffuseTex, V_TEXCOORD).xyz * DiffuseColor;"
"   gl_FragColor = vec4(ComputeSpotLightLBP(normalize(V_NORMAL), viewVector, V_POSITION, Shine, light, diffuse, SpecularColor), 1.0);"
"}";
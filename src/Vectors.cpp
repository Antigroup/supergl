#include "Common.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

void supergl_WrapVectorsCommon()
{
	using namespace boost::python;

	glm::vec2(*normalize2)(const glm::vec2 &) = &glm::normalize;
	glm::vec3(*normalize3)(const glm::vec3 &) = &glm::normalize;
	glm::vec4(*normalize4)(const glm::vec4 &) = &glm::normalize;
	def("normalize", normalize2);
	def("normalize", normalize3);
	def("normalize", normalize4);

	float(*length2)(const glm::vec2 &) = &glm::length;
	float(*length3)(const glm::vec3 &) = &glm::length;
	float(*length4)(const glm::vec4 &) = &glm::length;
	def("length", length2);
	def("length", length3);
	def("length", length4);

	float(*dot2)(const glm::vec2 &, const glm::vec2 &) = &glm::dot;
	float(*dot3)(const glm::vec3 &, const glm::vec3 &) = &glm::dot;
	float(*dot4)(const glm::vec4 &, const glm::vec4 &) = &glm::dot;
	def("dot", dot2);
	def("dot", dot3);
	def("dot", dot4);

	glm::vec3(*cross)(const glm::vec3 &, const glm::vec3 &) = &glm::cross;
	def("cross", cross);

	float(*distance2)(const glm::vec2 &, const glm::vec2 &) = &glm::distance;
	float(*distance3)(const glm::vec3 &, const glm::vec3 &) = &glm::distance;
	float(*distance4)(const glm::vec4 &, const glm::vec4 &) = &glm::distance;
	def("distance", distance2);
	def("distance", distance3);
	def("distance", distance4);

	glm::vec2(*reflect2)(const glm::vec2 &, const glm::vec2 &) = &glm::reflect;
	glm::vec3(*reflect3)(const glm::vec3 &, const glm::vec3 &) = &glm::reflect;
	glm::vec4(*reflect4)(const glm::vec4 &, const glm::vec4 &) = &glm::reflect;
	def("reflect", reflect2);
	def("reflect", reflect3);
	def("reflect", reflect4);

	glm::vec2(*refract2)(const glm::vec2 &, const glm::vec2 &, float) = &glm::refract;
	glm::vec3(*refract3)(const glm::vec3 &, const glm::vec3 &, float) = &glm::refract;
	glm::vec4(*refract4)(const glm::vec4 &, const glm::vec4 &, float) = &glm::refract;
	def("refract", refract2);
	def("refract", refract3);
	def("refract", refract4);
}
#include "Common.h"
#include "Vector4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vectors.h"

void supergl_WrapVec4()
{
	using namespace boost::python;

	class_<glm::vec4>("vec4").
		def(init<float>()).
		def(init<float, float, float, float>()).
		def(init<glm::vec4>()).
		def(init<glm::vec3, float>()).
		def(init<float, glm::vec3>()).
		def(init<glm::vec2, float, float>()).
		def(init<float, glm::vec2, float>()).
		def(init<float, float, glm::vec2>()).
		def_readwrite("x", &glm::vec4::x).
		def_readwrite("y", &glm::vec4::y).
		def_readwrite("z", &glm::vec4::z).
		def_readwrite("w", &glm::vec4::w).
		def(self + self).
		def(self - self).
		def(self * self).
		def(self / self).
		def(self + float()).
		def(float() + self).
		def(float() - self).
		def(self - float()).
		def(self * float()).
		def(float() * self).
		def(self / float()).
		def(float() / self).
		def(self += self).
		def(self -= self).
		def(self *= self).
		def(self /= self).
		def(self += float()).
		def(self -= float()).
		def(self *= float()).
		def(self /= float());
}
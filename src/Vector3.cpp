#include "Common.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"
#include "Vectors.h"

void supergl_WrapVec3()
{
	using namespace boost::python;

	class_<glm::vec3>("vec3").
		def(init<float>()).
		def(init<float, float, float>()).
		def(init<glm::vec3>()).
		def(init<glm::vec2, float>()).
		def(init<float, glm::vec2>()).
		def_readwrite("x", &glm::vec3::x).
		def_readwrite("y", &glm::vec3::y).
		def_readwrite("z", &glm::vec3::z).
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
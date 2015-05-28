#include "Common.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Vectors.h"

void supergl_WrapVec2()
{
	using namespace boost::python;

	class_<glm::vec2>("vec2").
		def(init<float>()).
		def(init<float, float>()).
		def(init<glm::vec2>()).
		def_readwrite("x", &glm::vec2::x).
		def_readwrite("y", &glm::vec2::y).
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
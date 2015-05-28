#include "Common.h"
#include "Vectors.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Material.h"
#include "Transform.h"
#include "Camera.h"
#include "Engine.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Input.h"
#include "Light.h"
#include "Texture2D.h"
#include "GameTime.h"


BOOST_PYTHON_MODULE(supergl)
{
	using namespace boost::python;

	supergl_WrapVec2();
	supergl_WrapVec3();
	supergl_WrapVec4();
	supergl_WrapVectorsCommon();
	supergl_WrapTransform();
	supergl_WrapCamera();
	supergl_WrapGameTime();
	supergl_WrapKeyboardAndMouse();
	supergl_WrapLight();
	supergl_WrapMaterial();
	supergl_WrapMesh();
	supergl_WrapMeshRenderer();
	supergl_WrapTexture2D();
	supergl_WrapEngine();
}
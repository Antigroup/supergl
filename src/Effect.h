//Represents a technique for drawing something.
//Meant to be inherited by more specific classes. Like "MeshEffect"
#pragma once

#include "Common.h"

class Effect
{
public:
	//Fills a Material object's parameter maps will default values. These will be used by the material to set the effects parameters later.
	virtual void FillParameterMaps(Material * mat) = 0;
};
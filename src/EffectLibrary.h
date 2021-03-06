#pragma once

#include "Common.h"


class EffectLibrary
{
private:
	std::map<std::string, EffectPtr> _effects;
public:
	EffectLibrary();

	bool Initialize();

	EffectPtr GetEffect(const std::string & name);
};

extern EffectLibrary * g_EffectLibrary;
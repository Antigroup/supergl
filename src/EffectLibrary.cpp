#include "EffectLibrary.h"
#include "BasicEffect.h"

EffectLibrary::EffectLibrary()
{

}

bool EffectLibrary::Initialize()
{
	std::shared_ptr<BasicEffect> basicEffect = std::make_shared<BasicEffect>();
	basicEffect->Initialize();
	_effects["BasicEffect"] = basicEffect;

	return true;
}

EffectPtr EffectLibrary::GetEffect(const std::string & name)
{
	return _effects[name];
}
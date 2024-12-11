#include "api/registry.h"
#include "core/registry.h"
#include <cinttypes>

// TODO: use same class name but different namespace
namespace API {}

RegistryAPI::RegistryAPI() {};
RegistryAPI::~RegistryAPI() {};
std::shared_ptr<RegistryAPI> RegistryAPI::instance(){
	static std::shared_ptr<RegistryAPI> instance (
		new RegistryAPI(),
		[](RegistryAPI* reg) {
			delete reg; // Custom deleter to allow destruction of the singleton
		}
	);
	return instance;
}

void RegistryAPI::test()
{
	Registry::instance()->test();
}

void RegistryAPI::oi()
{
	Registry::instance()->oi();
}

//SceneID RegistryAPI::create_new_scene()
//{
//	//return SceneID();
//	return Registry::instance()->create_new_scene();
//}

#pragma once
#include <drogon/DrObject.h>

// see Hello.h. same thing but World
// (copy pasted, if theres a bug in one theres a bug in both)
class World : public drogon::DrObject<World>
{
public:
	World() {}
};

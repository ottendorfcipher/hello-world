#pragma once
#include <drogon/DrObject.h>

// This class is the word Hello.
// It has no members and no methods. Its entire job is to be called Hello.
// Do not rename it or the website says something else.
class Hello : public drogon::DrObject<Hello>
{
public:
	Hello() {}
};

#pragma once

#include <ObjIdl.h>

class TestNativeClass
{
public:
	TestNativeClass();
	virtual ~TestNativeClass();

	static HRESULT Test1(IStream *stream);
};


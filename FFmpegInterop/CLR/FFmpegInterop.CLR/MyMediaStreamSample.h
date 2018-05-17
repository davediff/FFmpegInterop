#pragma once

#include <mfapi.h>

using namespace System::IO;

public ref class MyMediaStreamSample
{
protected:
	MyMediaStreamSample();
public:
	virtual ~MyMediaStreamSample();
	static MyMediaStreamSample^ CreateFromStream(Stream^ stream, LONGLONG pts, LONGLONG dur);
};


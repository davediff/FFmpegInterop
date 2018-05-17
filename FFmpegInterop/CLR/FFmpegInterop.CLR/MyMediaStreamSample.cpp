#include "stdafx.h"
#include "MyMediaStreamSample.h"


MyMediaStreamSample::MyMediaStreamSample()
{
}


MyMediaStreamSample::~MyMediaStreamSample()
{
}

MyMediaStreamSample ^ MyMediaStreamSample::CreateFromStream(Stream^ stream, LONGLONG pts, LONGLONG dur)
{
	throw gcnew System::NotImplementedException();
	// TODO: insert return statement here
}

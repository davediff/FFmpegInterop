//*****************************************************************************
//
//	Copyright 2015 Microsoft Corporation
//
//	Licensed under the Apache License, Version 2.0 (the "License");
//	you may not use this file except in compliance with the License.
//	You may obtain a copy of the License at
//
//	http ://www.apache.org/licenses/LICENSE-2.0
//
//	Unless required by applicable law or agreed to in writing, software
//	distributed under the License is distributed on an "AS IS" BASIS,
//	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//	See the License for the specific language governing permissions and
//	limitations under the License.
//
//*****************************************************************************

#pragma once
#include <queue>
//#include <memory>
//#include <cliext/vector>
//#include <winnt.h>
#include <mfapi.h>
#include "MyMediaStreamSample.h"

extern "C"
{
#include <libavformat/avformat.h>
}

//using namespace Windows::Storage::Streams;
//using namespace Windows::Media::Core;
using namespace System;
//using namespace cliext;


namespace FFmpegInterop
{
	ref class FFmpegInteropMSS;
	ref class FFmpegReader;

	ref class MediaSampleProvider
	{
	public:
		MediaSampleProvider();
		virtual ~MediaSampleProvider();
		//virtual MediaStreamSample^ GetNextSample();
		virtual MyMediaStreamSample^ GetNextSample();
		virtual void Flush();
		virtual void SetCurrentStreamIndex(int streamIndex);

	internal:
		void QueuePacket(AVPacket packet);
		AVPacket PopPacket();

	private:
		std::vector<AVPacket> *m_packetQueue;
		//cliext::vector<AVPacket^> m_packetQueue;
		int m_streamIndex;
		System::Int64 m_startOffset = 0;

	internal:
		// The FFmpeg context. Because they are complex types
		// we declare them as internal so they don't get exposed
		// externally
		FFmpegReader^ m_pReader;
		AVFormatContext* m_pAvFormatCtx;
		AVCodecContext* m_pAvCodecCtx;

	internal:
		MediaSampleProvider(
			FFmpegReader^ reader,
			AVFormatContext* avFormatCtx,
			AVCodecContext* avCodecCtx);
		virtual HRESULT AllocateResources();
		//virtual HRESULT WriteAVPacketToStream(DataWriter^ writer, AVPacket* avPacket);
		//virtual HRESULT DecodeAVPacket(DataWriter^ dataWriter, AVPacket* avPacket, int64_t& framePts, int64_t& frameDuration);
		//virtual HRESULT GetNextPacket(DataWriter^ writer, LONGLONG& pts, LONGLONG& dur);
		virtual HRESULT WriteAVPacketToStream(BinaryWriter^ writer, AVPacket* avPacket);
		virtual HRESULT DecodeAVPacket(BinaryWriter^ writer, AVPacket* avPacket, int64_t& framePts, int64_t& frameDuration);
		virtual HRESULT GetNextPacket(BinaryWriter^ writer, LONGLONG& pts, LONGLONG& dur);
	};
}
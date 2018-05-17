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
#include "RawSampleProvider.h"

namespace FFmpegInterop
{
	ref class RawVideoSampleProvider : RawSampleProvider
	{
	public:
		virtual ~RawVideoSampleProvider();
		//virtual void GetNextSample() override;
	internal:
		RawVideoSampleProvider(
			FFmpegReader^ reader,
			AVFormatContext* avFormatCtx,
			AVCodecContext* avCodecCtx);
		virtual HRESULT WriteAVPacketToStream(BinaryWriter^ writer, AVPacket* avPacket) override;
		HRESULT RawVideoSampleProvider::WriteAVPacketToStream2(BinaryWriter^ binaryWriter, AVPacket* avPacket);
		virtual HRESULT DecodeAVPacket(BinaryWriter^ dataWriter, AVPacket* avPacket, int64_t& framePts, int64_t& frameDuration) override;
		virtual HRESULT AllocateResources() override;

	private:
		//SwsContext* m_pSwsCtx;
		array<int> ^m_rgVideoBufferLineSize;
		//int m_rgVideoBufferLineSize[4
        array<uint8_t*>  ^m_rgVideoBufferData;;
		//uint8_t* m_rgVideoBufferData[4];
		int m_rgVideoBufferDestinationSize;
		bool m_interlaced_frame;
		bool m_top_field_first;
	};
}


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

#include "stdafx.h"
#include "RawVideoSampleProvider.h"
#include <mfapi.h>

extern "C"
{
#include <libavutil/imgutils.h>
}


using namespace FFmpegInterop;

RawVideoSampleProvider::RawVideoSampleProvider(
	FFmpegReader^ reader,
	AVFormatContext* avFormatCtx,
	AVCodecContext* avCodecCtx)
	: RawSampleProvider(reader, avFormatCtx, avCodecCtx)
{
	m_rgVideoBufferLineSize = gcnew array<int>(4);
	m_rgVideoBufferData = gcnew array<uint8_t*>(4);
	for (int i = 0; i < 4; i++)
	{
		m_rgVideoBufferLineSize[i] = 0;
		m_rgVideoBufferData[i] = nullptr;
	}
}

HRESULT RawVideoSampleProvider::AllocateResources()
{
	HRESULT hr = S_OK;
	hr = RawSampleProvider::AllocateResources();
	
	/*
	if (SUCCEEDED(hr))
	{
		// Setup software scaler to convert any decoder pixel format (e.g. YUV420P) to NV12 that is supported in Windows & Windows Phone MediaElement
		m_pSwsCtx = sws_getContext(
			m_pAvCodecCtx->width,
			m_pAvCodecCtx->height,
			m_pAvCodecCtx->pix_fmt,
			m_pAvCodecCtx->width,
			m_pAvCodecCtx->height,
			AV_PIX_FMT_NV12,
			SWS_BICUBIC,
			NULL,
			NULL,
			NULL);

		if (m_pSwsCtx == nullptr)
		{
			hr = E_OUTOFMEMORY;
		}
	}
	*/

	if (SUCCEEDED(hr))
	{
		m_pAvFrame = av_frame_alloc();
		if (m_pAvFrame == nullptr)
		{
			hr = E_OUTOFMEMORY;
		}
	}

	if (SUCCEEDED(hr))
	{
		cli::pin_ptr<int> pArrayElement = &m_rgVideoBufferLineSize[0];
		cli::pin_ptr<uint8_t*> ppArrayElement = &m_rgVideoBufferData[0];
		
		m_rgVideoBufferDestinationSize = av_image_alloc(ppArrayElement, pArrayElement, m_pAvCodecCtx->width, m_pAvCodecCtx->height, m_pAvCodecCtx->pix_fmt, 1);
		if (m_rgVideoBufferDestinationSize < 0)
		{
			hr = E_FAIL;
		}
	}

	return hr;
}

RawVideoSampleProvider::~RawVideoSampleProvider()
{
	if (m_pAvFrame)
	{
		cli::pin_ptr<AVFrame*> pAVFrame = &m_pAvFrame;
		av_frame_free(pAVFrame);
	}

	if (m_rgVideoBufferData)
	{
		cli::pin_ptr<uint8_t*> ppArrayElement = &m_rgVideoBufferData[0];
		av_freep(ppArrayElement);
	}
}

HRESULT RawVideoSampleProvider::DecodeAVPacket(BinaryWriter^ binaryWriter, AVPacket* avPacket, int64_t& framePts, int64_t& frameDuration)
{
	HRESULT hr = S_OK;
	hr = RawSampleProvider::DecodeAVPacket(binaryWriter, avPacket, framePts, frameDuration);

	// Don't set a timestamp on S_FALSE
	if (hr == S_OK)
	{
		// Try to get the best effort timestamp for the frame.
		framePts = av_frame_get_best_effort_timestamp(m_pAvFrame);
		m_interlaced_frame = m_pAvFrame->interlaced_frame == 1;
		m_top_field_first = m_pAvFrame->top_field_first == 1;
	}

	return hr;
}

/*
void RawVideoSampleProvider::GetNextSample()
{
	MediaSampleProvider::GetNextSample();
	
	if (sample != nullptr)
	{
		if (m_interlaced_frame)
		{
			sample->ExtendedProperties->Insert(MFSampleExtension_Interlaced, TRUE);
			sample->ExtendedProperties->Insert(MFSampleExtension_BottomFieldFirst, m_top_field_first ? safe_cast<Platform::Object^>(FALSE) : TRUE);
			sample->ExtendedProperties->Insert(MFSampleExtension_RepeatFirstField, safe_cast<Platform::Object^>(FALSE));
		}
		else
		{
			sample->ExtendedProperties->Insert(MFSampleExtension_Interlaced, safe_cast<Platform::Object^>(FALSE));
		}
	}
	
}
*/

HRESULT RawVideoSampleProvider::WriteAVPacketToStream(BinaryWriter^ binaryWriter, AVPacket* avPacket)
{
	/*
	// Convert decoded video pixel format to NV12 using FFmpeg software scaler
	if (sws_scale(m_pSwsCtx, (const uint8_t **)(m_pAvFrame->data), m_pAvFrame->linesize, 0, m_pAvCodecCtx->height, m_rgVideoBufferData, m_rgVideoBufferLineSize) < 0)
	{
		return E_FAIL;
	}

	auto YBuffer = ref new Platform::Array<uint8_t>(m_rgVideoBufferData[0], m_rgVideoBufferLineSize[0] * m_pAvCodecCtx->height);
	auto UVBuffer = ref new Platform::Array<uint8_t>(m_rgVideoBufferData[1], m_rgVideoBufferLineSize[1] * m_pAvCodecCtx->height / 2);
	binaryWriter->Write(YBuffer);
	binaryWriter->Write(UVBuffer);
	av_frame_unref(m_pAvFrame);
	av_frame_free(&m_pAvFrame);
	*/
		// This is the simplest form of transfer. Copy the packet directly to the stream
		// This works for most compressed formats
		// TODO: looks like impossible without copying??
		// TODO: fix this to use array directly instead of BinaryWriter and stream, so that copying occurs only once
		//auto copy = gcnew array<uint8_t>(avPacket->size);
		//System::Runtime::InteropServices::Marshal::Copy(IntPtr((void*)avPacket->data), copy, 0, avPacket->size);
		//auto aBuffer = ref new Platform::Array<uint8_t>(avPacket->data, avPacket->size);
		//binaryWriter->Write(copy);

	   cli::pin_ptr<uint8_t*> pointer = &m_rgVideoBufferData[0];
	   cli::pin_ptr<int> pointer2 = &m_rgVideoBufferLineSize[0];
	   av_image_copy(pointer, pointer2, (const uint8_t **)m_pAvFrame->data, m_pAvFrame->linesize, m_pAvCodecCtx->pix_fmt, m_pAvCodecCtx->width, m_pAvCodecCtx->height);

		auto copy = gcnew array<uint8_t>(m_rgVideoBufferDestinationSize);

		System::Runtime::InteropServices::Marshal::Copy(IntPtr((void*)m_rgVideoBufferData[0]), copy, 0, m_rgVideoBufferDestinationSize);
		/*
		for (int i = 0; i < 4; i++)
		{
			if (m_rgVideoBufferLineSize[i] > 0)
			{
				System::Runtime::InteropServices::Marshal::Copy(IntPtr((void*)m_rgVideoBufferData[i]), copy, 0, m_rgVideoBufferLineSize[i]);
			}
		}
		*/

		binaryWriter->Write(copy, 0, m_rgVideoBufferDestinationSize);
		
		AVFrame *pFrame = m_pAvFrame;
		av_frame_unref(pFrame);
		av_frame_free(&pFrame);

		return S_OK;
}

HRESULT RawVideoSampleProvider::WriteAVPacketToStream2(BinaryWriter^ binaryWriter, AVPacket* avPacket)
{
	int xsize = m_pAvFrame->width;
	for (int i = 0; i < m_pAvFrame->height; i++)
	{
		auto buf = gcnew array<uint8_t>(xsize);
		System::Runtime::InteropServices::Marshal::Copy(IntPtr((void*)(m_pAvFrame->data[0] + i * m_pAvFrame->linesize[0])), buf, 0, xsize);
		binaryWriter->Write(buf, 0, xsize);
	}

	AVFrame *pFrame = m_pAvFrame;
	av_frame_unref(pFrame);
	av_frame_free(&pFrame);

	return S_OK;
}
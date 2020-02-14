#include "../Graphics.hpp"

#include "../D3D/D3D.hpp"

#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avcodec.lib")

extern "C"
{
#pragma warning(disable : 4635)
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/time.h>
#pragma warning(default : 4635)
}

#include <thread>
#include <string>

namespace Kodo
{
	namespace Graphics
	{
		using namespace System::Security;
		using namespace System::Runtime::InteropServices;
		using namespace System::Threading;
		using namespace System::Threading::Tasks;

		public enum class FFmpegEngineError
		{
			None,
			CantOpen,
			NotSupported,
			BadOptions,

		};

		public enum class FFmpegEngineEvent
		{
			Started,
			Stopped,

			Error,
		};

		public delegate void FFmpegEngineEventHandler(FFmpegEngineEvent engineEvent);

		public value struct FFmpegStreamDesc
		{
			initonly bool IsHardwareDecoded;
			initonly int Width;
			initonly int Height;
			initonly Double FramesPerSecond;
			initonly Int64 Bitrate;

		internal:

			FFmpegStreamDesc(bool hwDecoded, int w, int h, Double fps, Int64 bits)
			{
				IsHardwareDecoded = hwDecoded;
				Width = w;
				Height = h;
				FramesPerSecond = fps;
				Bitrate = bits;
			}
		};

		value class FFmpegFrame
		{
		public:


		};

		ref class FFmpegFrameMemory
		{
			initonly Object^ frameLock;

			uint32_t framesDropped;

			uint32_t maxFrames;
			uint32_t frameCount;
			uint32_t framePosition;

			array<FFmpegFrame>^ frames;

		public:

			FFmpegFrameMemory(uint32_t sizeInFrames)
			{
				frameLock = gcnew Object();

				maxFrames = sizeInFrames;
				framePosition = 0;
				frameCount = 0;
				framesDropped = 0;
				frames = gcnew array<FFmpegFrame>(maxFrames);
			}

			Boolean Peek(FFmpegFrame% frame)
			{
				Monitor::Enter(frameLock);
				Boolean ret = false;

				if (frameCount > 0)
				{
					frame = frames[framePosition];
					ret = true;
				}

				Monitor::Exit(frameLock);
				return ret;
			}

			void Enqueue(FFmpegFrame frame)
			{
				Monitor::Enter(frameLock);

				if (frameCount == maxFrames)
					framesDropped++;

				framePosition = (framePosition + 1) % maxFrames;
				frames[framePosition] = frame;
				frameCount++;

				Monitor::Exit(frameLock);
			}

			Boolean Dequeue(FFmpegFrame% frame)
			{
				Monitor::Enter(frameLock);
				Boolean ret = false;

				if (frameCount > 0)
				{
					frame = frames[framePosition];
					frameCount--;
					ret = true;
				}

				Monitor::Exit(frameLock);
				return ret;
			}
		};

		public ref class FFmpegEngine
		{
			[UnmanagedFunctionPointer(CallingConvention::Cdecl), SuppressUnmanagedCodeSecurity]
			delegate enum AVPixelFormat get_hw_format_callback(AVCodecContext* ctx, const enum AVPixelFormat* pix_fmts);

			const int32_t StreamNotFound = AVERROR_STREAM_NOT_FOUND;
			const int32_t ThreadFrame = FF_THREAD_FRAME;
			const int32_t ThreadSlice = FF_THREAD_SLICE;

			bool useDirectShow = false;
			bool useHWDecoding = false;

			int32_t retOK = false;
			int32_t retHWOK = false;

			int32_t streamVideoIndex = StreamNotFound;
			AVStream* streamVideo = nullptr;
			AVFormatContext* streamFormatContext = nullptr;
			AVCodec* streamVideoCodec = nullptr;
			AVCodecContext* streamVideoCodecContext = nullptr;
			AVBufferRef* hardwareDeviceContext = nullptr;

			uint32_t hardwareDeviceType = 0;
			uint32_t hardwareDevicePixelFormat = -1;
			const AVCodecHWConfig* hardwareDeviceConfig = nullptr;

			bool isPlaying;

			HandleRef _eventCallbackRef;
			get_hw_format_callback^ _eventCallback;

			Task^ streamTask;

			bool waitForFrameTransfer = false;
			AVFrame* streamCurrentFrame = nullptr;

			String^ streamSourceUrl;
			AVDictionary* streamSourceOptions = nullptr;

			FFmpegEngineError streamError = FFmpegEngineError::None;
			FFmpegEngineEventHandler^ streamEvent;

			double lastFrameTransferTime = 0;
			double lastFramePts = 0;
			double currentTime = 0;

		public:

			FFmpegEngine(FFmpegEngineEventHandler^ engineEventHandler)
			{
				ThrowIfNull(engineEventHandler);
				streamEvent = engineEventHandler;

				streamFormatContext = avformat_alloc_context();
				avformat_free_context(streamFormatContext);
			}

			~FFmpegEngine()
			{
			}

			FFmpegEngineError GetError()
			{
				return streamError;
			}

			void Stop()
			{

			}

			FFmpegStreamDesc GetSourceDescription()
			{
				ThrowIf(!isPlaying);
				return FFmpegStreamDesc(retHWOK, streamVideoCodecContext->width, streamVideoCodecContext->height, av_q2d(av_guess_frame_rate(streamFormatContext, streamVideo, nullptr)), streamVideoCodecContext->bit_rate);
			}

			void SetSourceOption(String^ key) { SetSourceOption(key, String::Empty); }
			void SetSourceOption(String^ key, String^ value)
			{
				ThrowIf(isPlaying);

				int32_t dictRet = 0;
				AVDictionary* dict = streamSourceOptions;

				if (key == "hw")
				{
					useHWDecoding = value == "true" ? true : false;
				}
				else
				{
					// convert managed strings to native
					const char* keyStr = (char*)(void*)Marshal::StringToHGlobalAnsi(key);
					const char* valueStr = (char*)(void*)Marshal::StringToHGlobalAnsi(value);

					// set option
					dictRet = av_dict_set(&dict, keyStr, valueStr, 0);

					// free native strings
					Marshal::FreeHGlobal(IntPtr((void*)keyStr));
					Marshal::FreeHGlobal(IntPtr((void*)valueStr));
				}

				streamSourceOptions = dict;
				ThrowIf(dictRet < 0);
			}

			void SetSource(String^ sourceUrl)
			{
				ThrowIf(isPlaying);

				streamSourceUrl = sourceUrl;
			}

			Boolean HasFrame([Out] Double% presentationTime, [Out] Double% presentationDuration)
			{
				if (waitForFrameTransfer)
				{
					presentationTime = av_q2d(streamVideo->time_base) * 1 * streamCurrentFrame->pts;
					presentationDuration = av_q2d(streamVideo->time_base) * 1 * streamCurrentFrame->pkt_duration;
				}
				else
				{
					presentationTime = 0;
					presentationDuration = 0;
				}

				return waitForFrameTransfer;
			}

			Boolean TransferFrame(D3DTexture2D^ destinationTexture)
			{
				if (retHWOK)
				{
					// get ffmpeg resources
					ID3D11Texture2D* ffmpegTexture = nullptr;
					ID3D11Device* ffmpegDevice = nullptr;
					ID3D11DeviceContext* ffmpegDeviceContext = nullptr;
					ffmpegTexture = reinterpret_cast<ID3D11Texture2D*>(streamCurrentFrame->data[0]);
					ffmpegTexture->GetDevice(&ffmpegDevice);
					ffmpegDevice->GetImmediateContext(&ffmpegDeviceContext);

					com_ptr<IDXGIResource> destinationTextureDXGI;
					ThrowIfFailed(destinationTexture->Native->QueryInterface(IID_PPV_ARGS(&destinationTextureDXGI)));

					HANDLE destinationTextureSharedHandle;
					ThrowIfFailed(destinationTextureDXGI->GetSharedHandle(&destinationTextureSharedHandle));

					com_ptr<ID3D11Texture2D> destinationTextureAsShared;
					ThrowIfFailed(ffmpegDevice->OpenSharedResource(destinationTextureSharedHandle, IID_PPV_ARGS(&destinationTextureAsShared)));

					ffmpegDeviceContext->CopySubresourceRegion(destinationTextureAsShared, 0, 0, 0, 0, ffmpegTexture, (UINT)streamCurrentFrame->data[1], nullptr);
				}
				else
				{
					// get destination resources
					ID3D11Texture2D* destTexture = nullptr;
					ID3D11Device* destDevice = nullptr;
					ID3D11DeviceContext* destDeviceContext = nullptr;
					destTexture = destinationTexture->Native;
					destTexture->GetDevice(&destDevice);
					destDevice->GetImmediateContext(&destDeviceContext);

					D3D11_MAPPED_SUBRESOURCE resource;
					destDeviceContext->Map(destTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

					uint8_t* destData = reinterpret_cast<uint8_t*>(resource.pData);
					uint32_t destLineSize = resource.RowPitch;

					uint32_t frameHeight = streamCurrentFrame->height;

					uint32_t uvLineCount = frameHeight / 2;
					uint32_t singleChannelLineSize = streamCurrentFrame->linesize[1];

					uint8_t* channelY = streamCurrentFrame->data[0];
					uint8_t* channelU = streamCurrentFrame->data[1];
					uint8_t* channelV = streamCurrentFrame->data[2];

					// copy the Y channel
					for (uint32_t i = 0; i < frameHeight; i++)
					{
						memcpy(destData + destLineSize * i, channelY + i * streamCurrentFrame->linesize[0], streamCurrentFrame->linesize[0]);
					}

					// copy the U and V channels
					for (uint32_t i = 0; i < uvLineCount; i++)
					{
						uint32_t byteI = 0;
						uint32_t uvLineI = i * singleChannelLineSize;
						uint32_t destUVLine = ((frameHeight + i) * destLineSize);

						for (uint32_t ii = 0; ii < singleChannelLineSize; ii++)
						{
							destData[destUVLine + byteI++] = channelU[uvLineI + ii];
							destData[destUVLine + byteI++] = channelV[uvLineI + ii];
						}
					}

					destDeviceContext->Unmap(destTexture, 0);
				}

				waitForFrameTransfer = false;
				return true;
			}

			void Play()
			{
				ThrowIf(isPlaying);
				ThrowIf(String::IsNullOrEmpty(streamSourceUrl));

				streamError = FFmpegEngineError::None;
				retOK = true;
				retHWOK = false;

				// avformat_open_input
				if (retOK)
				{
					const char* streamUrl = (char*)(void*)Marshal::StringToHGlobalAnsi(streamSourceUrl);

					AVDictionary* avd = this->streamSourceOptions;
					AVFormatContext* avfc = avformat_alloc_context();
					AVInputFormat* avif = nullptr;

					if (avformat_open_input(&avfc, streamUrl, avif, &avd) < 0)
					{
						Console::WriteLine("failed: avformat_open_input");
						retOK = false;
						streamError = FFmpegEngineError::CantOpen;
					}

					Marshal::FreeHGlobal(IntPtr((void*)streamUrl));

					int32_t count = av_dict_count(avd);

					if (count > 0)
					{
						avformat_close_input(&avfc);
						retOK = false;
						streamError = FFmpegEngineError::BadOptions;
					}

					av_dict_free(&avd);

					streamFormatContext = avfc;
				}
				// avformat_find_stream_info
				if (retOK)
				{
					if (avformat_find_stream_info(streamFormatContext, NULL) < 0)
					{
						Console::WriteLine("failed: avformat_find_stream_info");
						retOK = false;
						streamError = FFmpegEngineError::NotSupported;
					}
				}
				// av_find_best_stream
				if (retOK)
				{
					AVCodec* streamVideoCodec = nullptr;
					if ((streamVideoIndex = av_find_best_stream(streamFormatContext, AVMEDIA_TYPE_VIDEO, -1, -1, &streamVideoCodec, 0)) == StreamNotFound || streamVideoCodec == nullptr)
					{
						Console::WriteLine("failed: av_find_best_stream");
						retOK = false;
						streamError = FFmpegEngineError::NotSupported;
					}

					this->streamVideoCodec = streamVideoCodec;
					this->streamVideo = streamFormatContext->streams[streamVideoIndex];
				}
				// av_hwdevice_ctx_create
				if (retOK && useHWDecoding)
				{
					if ((hardwareDeviceType = (uint32_t)av_hwdevice_find_type_by_name("d3d11va")) == AV_HWDEVICE_TYPE_NONE)
					{
						Console::WriteLine("failed: av_hwdevice_find_type_by_name [d3d11va]");
					}
					else
					{
						for (uint32_t i = 0;; i++)
						{
							if (!(hardwareDeviceConfig = avcodec_get_hw_config(streamVideoCodec, i)))
							{
								Console::WriteLine("failed: avcodec_get_hw_config [d3d11va]");
								break;
							}
							if (hardwareDeviceConfig->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX && hardwareDeviceConfig->device_type == (AVHWDeviceType)hardwareDeviceType)
							{
								hardwareDevicePixelFormat = hardwareDeviceConfig->pix_fmt;
								retHWOK = true;
								break;
							}
						}
					}

					if (retHWOK)
					{
						AVBufferRef* hardwareDeviceContext = nullptr;
						if (av_hwdevice_ctx_create(&hardwareDeviceContext, (AVHWDeviceType)hardwareDeviceType, NULL, NULL, 0) < 0)
						{
							Console::WriteLine("failed: av_hwdevice_ctx_create [d3d11va]");
							retHWOK = false;
						}

						this->hardwareDeviceContext = hardwareDeviceContext;
					}
				}
				// avcodec_alloc_context3
				if (retOK)
				{
					if (!(streamVideoCodecContext = avcodec_alloc_context3(streamVideoCodec)))
					{
						Console::WriteLine("failed: avcodec_alloc_context3");
						retOK = false;
					}
				}
				// avcodec_parameters_to_context
				if (retOK)
				{
					if (avcodec_parameters_to_context(streamVideoCodecContext, streamVideo->codecpar) < 0)
					{
						Console::WriteLine("failed: avcodec_parameters_to_context");
						retOK = false;
					}
				}
				// hardware settings
				if (retOK)
				{
					if (std::thread::hardware_concurrency())
					{
						streamVideoCodecContext->thread_count = std::thread::hardware_concurrency();
						streamVideoCodecContext->thread_type = ThreadFrame | ThreadSlice;
					}
					if (retHWOK)
					{
						_eventCallback = gcnew get_hw_format_callback(this, &FFmpegEngine::get_hw_format);
						_eventCallbackRef = HandleRef(_eventCallback, Marshal::GetFunctionPointerForDelegate(_eventCallback));

						streamVideoCodecContext->get_format = reinterpret_cast<enum AVPixelFormat(*)(struct AVCodecContext* s, const enum AVPixelFormat* fmt)>(_eventCallbackRef.Handle.ToPointer());
						streamVideoCodecContext->hw_device_ctx = av_buffer_ref(hardwareDeviceContext);
					}
				}
				// avcodec_open2
				if (retOK)
				{
					if (avcodec_open2(streamVideoCodecContext, streamVideoCodec, NULL) < 0)
					{
						Console::WriteLine("failed: avcodec_open2");
						retOK = false;
					}
				}

				// all is prepared - start stream task
				if (retOK)
				{
					isPlaying = true;
					streamTask = gcnew Task(gcnew Action(this, &FFmpegEngine::StreamTask), TaskCreationOptions::LongRunning);
					streamTask->Start();

					streamEvent(FFmpegEngineEvent::Started);
				}
				// error somewhere - cleanup
				else
				{
					isPlaying = false;

					AVCodecContext* avcc = streamVideoCodecContext;
					avcodec_free_context(&avcc);
					streamVideoCodecContext = avcc;

					AVFormatContext* avfc = streamFormatContext;
					avformat_close_input(&avfc);
					streamFormatContext = avfc;

					streamEvent(FFmpegEngineEvent::Error);
				}
			}

		private:

			static int32_t is_error_again(int32_t error)
			{
				return error == AVERROR(EAGAIN);
			}

			enum AVPixelFormat get_hw_format(AVCodecContext* ctx, const enum AVPixelFormat* pix_fmts)
			{
				const enum AVPixelFormat* p;

				for (p = pix_fmts; *p != -1; p++)
				{
					if (*p == (AVPixelFormat)hardwareDevicePixelFormat)
					{
						return *p;
					}
				}
				return AV_PIX_FMT_NONE;
			}

			void StreamTask()
			{
				int32_t frameOK = true;
				int32_t frameCounter = 0;
				int32_t frameRet = 0;
				int32_t sendAgain = 0;
				int32_t recvAgain = 0;

				AVRational streamTimeBase = streamVideo->time_base;
				AVPacket avPacket;
				av_init_packet(&avPacket);
				avPacket.data = NULL;
				avPacket.size = 0;

				while (retOK)
				{
					if (waitForFrameTransfer)
					{
						Sleep(0);
						continue;
					}
					else
					{
						lastFrameTransferTime = av_gettime_relative() / 1000000.0;

						AVFrame* frame = streamCurrentFrame;
						av_frame_free(&frame);
						streamCurrentFrame = nullptr;
						av_packet_unref(&avPacket);
					}

					if (av_read_frame(streamFormatContext, &avPacket) < 0)
					{
						Console::WriteLine("failed: av_read_frame");
						retOK = false;
						break;
					}

					frameOK = streamVideoIndex == avPacket.stream_index;

					if (retOK && frameOK)
					{
						frameCounter++;

						while (frameOK)
						{
							if (sendAgain)
							{
								Console::WriteLine("again: avcodec_send_packet");
								sendAgain = false;
							}

							if ((frameRet = avcodec_send_packet(streamVideoCodecContext, &avPacket)) < 0)
							{
								if (is_error_again(frameRet))
								{
									recvAgain = true;
									Console::WriteLine("eagain: avcodec_send_packet");
								}
								else
								{
									Console::WriteLine("failed: avcodec_send_packet");
									frameOK = false;
								}
							}

							if (frameOK)
							{
								if (!(streamCurrentFrame = av_frame_alloc()))
								{
									Console::WriteLine("failed: av_frame_alloc");
									frameOK = false;
									break;
								}

								if (recvAgain)
								{
									Console::WriteLine("again: avcodec_receive_frame");
									recvAgain = false;
								}

								if ((frameRet = avcodec_receive_frame(streamVideoCodecContext, streamCurrentFrame)) < 0)
								{
									if (is_error_again(frameRet))
									{
										sendAgain = true;
										Console::WriteLine("eagain: avcodec_receive_frame");

										AVFrame* frame = streamCurrentFrame;
										av_frame_free(&frame);
										streamCurrentFrame = nullptr;
										continue;
									}
									else
									{
										Console::WriteLine("failed: avcodec_receive_frame");
										frameOK = false;
									}
								}
							}

							if (frameOK)
							{
								auto pts = LONGLONG(av_q2d(streamTimeBase) * 10000000 * streamCurrentFrame->pts);

								//if (pts != AV_NOPTS_VALUE && streamCurrentFrame->pkt_duration != 0)
								{
									waitForFrameTransfer = true;
								}
								break;
							}
						}
					}
					else
					{
						//Console::WriteLine("frame skipped (not video)");
					}
				}

				// cleanup
				avformat_free_context(streamFormatContext);
				streamFormatContext = nullptr;
				AVCodecContext* codecContext = this->streamVideoCodecContext;
				avcodec_free_context(&codecContext);
				streamVideoCodecContext = nullptr;
				Console::WriteLine("stopped");
				isPlaying = false;

				streamEvent(FFmpegEngineEvent::Stopped);
			}
		};
	}
}
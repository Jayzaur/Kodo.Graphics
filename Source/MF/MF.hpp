#pragma once

#include "../Graphics.hpp"

#include "../D3D/D3D.hpp"

#ifdef SUPPORT_MF

namespace Kodo
{
	namespace Graphics
	{
		using namespace System::Security;
		using namespace System::Runtime::InteropServices;
		using namespace System::Threading::Tasks;

		public enum class MFMediaEngineError
		{
			NoError = MF_MEDIA_ENGINE_ERR_NOERROR,
			Aborted = MF_MEDIA_ENGINE_ERR_ABORTED,
			Network = MF_MEDIA_ENGINE_ERR_NETWORK,
			Decode = MF_MEDIA_ENGINE_ERR_DECODE,
			NotSupported = MF_MEDIA_ENGINE_ERR_SRC_NOT_SUPPORTED,
			Encrypted = MF_MEDIA_ENGINE_ERR_ENCRYPTED
		};

		public enum class MFMediaEngineEvent
		{
			LoadStart = 1,
			Progress = 2,
			Suspend = 3,
			Abort = 4,
			Error = 5,
			Emptied = 6,
			Stalled = 7,
			Play = 8,
			Pause = 9,
			LoadedMetaData = 10,
			LoadedData = 11,
			Waiting = 12,
			Playing = 13,
			CanPlay = 14,
			CanPlayThrough = 15,
			Seeking = 16,
			Seeked = 17,
			TimeUpdate = 18,
			Ended = 19,
			RateChange = 20,
			DurationChange = 21,
			VolumeChange = 22,
			FormatChange = 1000,
			PurgeQueuedEvents = 1001,
			TimelineMarker = 1002,
			BalanceChange = 1003,
			DownloadComplete = 1004,
			BufferingStarted = 1005,
			BufferingEnded = 1006,
			FramestepCompleted = 1007,
			NotifyStableState = 1008,
			FirstFrameReady = 1009,
			TracksChange = 1010,
			OPMInfo = 1011,
			ResourceLost = 1012,
			DelayLoadEventChanged = 1013,
			StreamRenderingError = 1014,
			SupportedRatesChanged = 1015,
			AudioEndpointChange = 1016
		};

		class MediaEngineNotify : public IMFMediaEngineNotify
		{
			long m_cRef;
			void(*m_pCB) (DWORD);

		public:
			MediaEngineNotify() noexcept :
				m_cRef(1),
				m_pCB(nullptr)
			{
			}

			STDMETHODIMP QueryInterface(REFIID riid, void** ppv)
			{
				if (__uuidof(IMFMediaEngineNotify) == riid)
				{
					*ppv = static_cast<IMFMediaEngineNotify*>(this);
				}
				else
				{
					*ppv = nullptr;
					return E_NOINTERFACE;
				}

				AddRef();

				return S_OK;
			}

			STDMETHODIMP_(ULONG) AddRef()
			{
				return InterlockedIncrement(&m_cRef);
			}

			STDMETHODIMP_(ULONG) Release()
			{
				LONG cRef = InterlockedDecrement(&m_cRef);
				if (cRef == 0)
				{
					delete this;
				}
				return cRef;
			}

			void SetCallback(void(*pCB) (DWORD))
			{
				m_pCB = pCB;
			}

			// EventNotify is called when the Media Engine sends an event.
			STDMETHODIMP EventNotify(DWORD meEvent, DWORD_PTR param1, DWORD)
			{
				if (meEvent == MF_MEDIA_ENGINE_EVENT_NOTIFYSTABLESTATE)
				{
					SetEvent(reinterpret_cast<HANDLE>(param1));
				}
				else
				{
					m_pCB(meEvent);
				}

				return S_OK;
			}
		};

		[UnmanagedFunctionPointer(CallingConvention::Cdecl), SuppressUnmanagedCodeSecurity]
		public delegate void MFMediaEngineCallback(MFMediaEngineEvent meEvent);

		public ref class MFMediaEngine : GraphicsObject
		{
			GRAPHICS_OBJECT(MFMediaEngine, IMFMediaEngine);

			HandleRef _eventCallbackRef;
			MFMediaEngineCallback^ _eventCallback;

			MediaEngineNotify* spNotify;

		public:

			static D3DDevice^ CreateVideoDevice(D3DDevice^ renderDevice)
			{
				ThrowIfFailed(MFStartup(MF_SDK_VERSION));

				// Create our own device to avoid threading issues
				com_ptr<IDXGIDevice> dxgiDevice;
				ThrowIfFailed(renderDevice->Native->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));

				com_ptr<IDXGIAdapter> adapter;
				ThrowIfFailed(dxgiDevice->GetAdapter(&adapter));

				D3D_FEATURE_LEVEL fLevel = renderDevice->Native->GetFeatureLevel();

				ID3D11Device* videoDevice = nullptr;

				ThrowIfFailed(
					D3D11CreateDevice(
						adapter,
						D3D_DRIVER_TYPE_UNKNOWN,
						nullptr,
						D3D11_CREATE_DEVICE_VIDEO_SUPPORT | D3D11_CREATE_DEVICE_BGRA_SUPPORT,
						&fLevel,
						1,
						D3D11_SDK_VERSION,
						&videoDevice,
						nullptr,
						nullptr
					));

				com_ptr<ID3D10Multithread> multithreaded;
				ThrowIfFailed(videoDevice->QueryInterface(IID_PPV_ARGS(&multithreaded)));
				multithreaded->SetMultithreadProtected(TRUE);

				return gcnew D3DDevice(videoDevice);
			}

			MFMediaEngine(D3DDevice^ videoDevice, DxgiFormat videoFormat, MFMediaEngineCallback^ eventCallback)
			{
				// Setup Media Engine
				com_ptr<IMFDXGIDeviceManager> dxgiManager;
				UINT resetToken;
				ThrowIfFailed(MFCreateDXGIDeviceManager(&resetToken, &dxgiManager));
				ThrowIfFailed(dxgiManager->ResetDevice(videoDevice->Native, resetToken));

				// Create our event callback object.
				spNotify = new MediaEngineNotify();

				if (eventCallback != nullptr)
				{
					_eventCallback = eventCallback;
					_eventCallbackRef = HandleRef(_eventCallback, Marshal::GetFunctionPointerForDelegate(_eventCallback));

					spNotify->SetCallback(reinterpret_cast<void(*)(DWORD)>(_eventCallbackRef.Handle.ToPointer()));
				}

				// Set configuration attributes.

				IMFDXGIDeviceManager* idxgiManager = dxgiManager;
				MediaEngineNotify* ispNotify = spNotify;

				com_ptr<IMFAttributes> attributes;
				ThrowIfFailed(MFCreateAttributes(&attributes, 1));
				ThrowIfFailed(attributes->SetUnknown(MF_MEDIA_ENGINE_DXGI_MANAGER, reinterpret_cast<IUnknown*>(idxgiManager)));
				ThrowIfFailed(attributes->SetUnknown(MF_MEDIA_ENGINE_CALLBACK, reinterpret_cast<IUnknown*>(ispNotify)));
				ThrowIfFailed(attributes->SetUINT32(MF_MEDIA_ENGINE_VIDEO_OUTPUT_FORMAT, static_cast<DXGI_FORMAT>(videoFormat)));

				// Create MediaEngine.
				com_ptr<IMFMediaEngineClassFactory> mfFactory;
				ThrowIfFailed(CoCreateInstance(CLSID_MFMediaEngineClassFactory, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&mfFactory)));

				IMFMediaEngine* mediaEngine = nullptr;
				ThrowIfFailed(mfFactory->CreateInstance(0, attributes, &mediaEngine));

				Assing(mediaEngine);
			}

			void GetErrorInformation([Out] MFMediaEngineError% engineError)
			{
				com_ptr<IMFMediaError> error;
				Native->GetError(&error);
				engineError = static_cast<MFMediaEngineError>(error->GetErrorCode());
			}

			Vec2 GetNativeVideoSize()
			{
				DWORD x, y;
				ThrowIfFailed(Native->GetNativeVideoSize(&x, &y));
				return Vec2(static_cast<int>(x), static_cast<int>(y));
			}

			Double GetVolume()
			{
				return Native->GetVolume();
			}

			void SetVolume(Double volume)
			{
				Native->SetVolume(volume);
			}

			Double GetCurrentTime()
			{
				return Native->GetCurrentTime();
			}

			void Play()
			{
				ThrowIfFailed(Native->Play());
			}

			void SetMuted(Boolean muted)
			{
				ThrowIfFailed(Native->SetMuted(muted));
			}

			void SetSource(String^ sourceUrl)
			{
				static BSTR bstrURL = nullptr;

				if (bstrURL != nullptr)
				{
					CoTaskMemFree(bstrURL);
					bstrURL = nullptr;
				}

				pin_ptr<const wchar_t> sourceUrlPinned = PtrToStringChars(sourceUrl);

				size_t cchAllocationSize = 1 + wcslen(sourceUrlPinned);
				bstrURL = reinterpret_cast<BSTR>(CoTaskMemAlloc(sizeof(wchar_t) * (cchAllocationSize)));

				if (bstrURL == 0)
				{
					ThrowIfFailed(E_OUTOFMEMORY);
				}

				wcscpy_s(bstrURL, cchAllocationSize, sourceUrlPinned);

				ThrowIfFailed(Native->SetSource(bstrURL));
			}

			Boolean HasFrame([Out] Int64% presentationTime)
			{
				LONGLONG retPts;
				HRESULT ret = Native->OnVideoStreamTick(&retPts);

				presentationTime = static_cast<Int64>(retPts);
				return ret == S_OK;
			}

			Boolean TransferFrame(D3DDevice^ videoDevice, D3DTexture2D^ destinationTexture, Vec4 destinationRect, Color backgroundColor) { return TransferFrame(videoDevice, destinationTexture, Vec4(0, 0, 1, 1), destinationRect, backgroundColor); }
			Boolean TransferFrame(D3DDevice^ videoDevice, D3DTexture2D^ destinationTexture, Vec4 sourceRect, Vec4 destinationRect, Color backgroundColor)
			{
				// The texture must have been created with D3D11_RESOURCE_MISC_SHARED
				com_ptr<IDXGIResource> destinationTextureDXGI;
				ThrowIfFailed(destinationTexture->Native->QueryInterface(IID_PPV_ARGS(&destinationTextureDXGI)));

				HANDLE destinationTextureSharedHandle;
				ThrowIfFailed(destinationTextureDXGI->GetSharedHandle(&destinationTextureSharedHandle));

				com_ptr<ID3D11Texture2D> destinationTextureAsShared;
				if (SUCCEEDED(videoDevice->Native->OpenSharedResource(destinationTextureSharedHandle, IID_PPV_ARGS(&destinationTextureAsShared))))
				{
					RECT destinationRectAsRECT;
					destinationRectAsRECT.left = static_cast<LONG>(destinationRect.Left);
					destinationRectAsRECT.top = static_cast<LONG>(destinationRect.Top);
					destinationRectAsRECT.right = static_cast<LONG>(destinationRect.Right);
					destinationRectAsRECT.bottom = static_cast<LONG>(destinationRect.Bottom);

					MFARGB backgroundColorAsMFARGB;
					backgroundColorAsMFARGB.rgbRed = static_cast<BYTE>(255 * backgroundColor.R);
					backgroundColorAsMFARGB.rgbGreen = static_cast<BYTE>(255 * backgroundColor.G);
					backgroundColorAsMFARGB.rgbBlue = static_cast<BYTE>(255 * backgroundColor.B);
					backgroundColorAsMFARGB.rgbAlpha = static_cast<BYTE>(255 * backgroundColor.A);

					if (Native->TransferVideoFrame(destinationTextureAsShared, (MFVideoNormalizedRect*)&sourceRect, &destinationRectAsRECT, &backgroundColorAsMFARGB) == S_OK)
						return true;
				}

				return false;
			}
		};
	}
}

#endif 
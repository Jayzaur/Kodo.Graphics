
#include "../Graphics.hpp"
#include "WIC.hpp"

namespace Kodo
{
	namespace Graphics
	{
		namespace Imaging
		{
			//--------------------------------------------------------------------------------------
			// WicBitmapSource

			void WicBitmapSource::GetResolution([OutAttribute()]Double% dpiX, [OutAttribute()]Double% dpiY)
			{
				double x, y;
				ThrowIfFailed<GraphicsException>(Native->GetResolution((double*)& x, (double*)& y));
				dpiX = x;
				dpiY = y;
			}

			void WicBitmapSource::CopyPalette(WicPalette^ palette)
			{
				ThrowIfFailed<GraphicsException>(Native->CopyPalette(palette->Native));
			}

			void WicBitmapSource::CopyPixels(WicRect rect, Int32 stride, array<Byte>^ buffer)
			{
				pin_ptr<Byte> pBuffer = &buffer[0];
				ThrowIfFailed<GraphicsException>(Native->CopyPixels((WICRect*)& rect, (UINT)stride, buffer->Length, (BYTE*)pBuffer));
			}

			//--------------------------------------------------------------------------------------
			// WicPalette

			array<WicColor>^ WicPalette::GetColors()
			{
				IWICPalette* palette = Native;
				UINT count;
				ThrowIfFailed<GraphicsException>(palette->GetColorCount(&count));

				array<WicColor>^ colors = gcnew array<WicColor>(count);
				pin_ptr<WicColor> pColors = &colors[0];
				UINT actualColors;
				ThrowIfFailed<GraphicsException>(palette->GetColors(count, (WICColor*)pColors, &actualColors));
				return colors;
			}

			void WicPalette::Initialize(array<WicColor>^ colors)
			{
				if (colors == nullptr)
					throw gcnew ArgumentNullException("colors");

				pin_ptr<WicColor> pColors = &colors[0];
				ThrowIfFailed<GraphicsException>(Native->InitializeCustom((WICColor*)pColors, colors->Length));
			}

			void WicPalette::Initialize(WicBitmapSource^ surface, Int32 colorCount, Boolean addTransparentColor)
			{
				ThrowIfFailed<GraphicsException>(Native->InitializeFromBitmap(surface->Native, (UINT)colorCount, (BOOL)addTransparentColor));
			}

			void WicPalette::Initialize(WicPalette^ source)
			{
				ThrowIfFailed<GraphicsException>(Native->InitializeFromPalette(source->Native));
			}

			void WicPalette::Initialize(WICBitmapPaletteType paletteType, Boolean addTransparentColor)
			{
				ThrowIfFailed<GraphicsException>(Native->InitializePredefined((WICBitmapPaletteType)paletteType, (BOOL)addTransparentColor));
			}

			//--------------------------------------------------------------------------------------
			// WicFactory

			WicFactory::WicFactory()
			{
				IWICImagingFactory* factory;

				ThrowIfFailed<GraphicsException>(CoCreateInstance(
					CLSID_WICImagingFactory,
					nullptr,
					CLSCTX_INPROC_SERVER,
					IID_IWICImagingFactory,
					(LPVOID*)& factory));

				Assing(factory);
			}

			//--------------------------------------------------------------------------------------
			// WicBitmap

			WicBitmap^ WicFactory::CreateBitmap(Int32 width, Int32 height, Guid pixelFormat, WicBitmapCreateCacheOption option)
			{
				IWICBitmap* bitmap;
				ThrowIfFailed<GraphicsException>(Native->CreateBitmap(width, height, *(GUID*)& pixelFormat, (WICBitmapCreateCacheOption)option, &bitmap));
				return gcnew WicBitmap(bitmap);
			}

			void WicBitmap::SetPalette(WicPalette^ palette)
			{
				if (palette == nullptr)
					throw gcnew ArgumentNullException("palette");
				ThrowIfFailed<GraphicsException>(Native->SetPalette(palette->Native));
			}

			void WicBitmap::SetResolution(Double dpiX, Double dpiY)
			{
				ThrowIfFailed<GraphicsException>(Native->SetResolution(dpiX, dpiY));
			}

			WicBitmapLock^ WicBitmap::Lock(Kodo::Graphics::Rectangle lockRect, WICBitmapLockFlags flags)
			{
				WICRect rect = { static_cast<Int32>(lockRect.Left), static_cast<Int32>(lockRect.Top), static_cast<Int32>(lockRect.Width), static_cast<Int32>(lockRect.Height) };
				IWICBitmapLock* pLock;
				ThrowIfFailed<GraphicsException>(Native->Lock(&rect, (DWORD)flags, &pLock));
				return gcnew WicBitmapLock(pLock);
			}

			//--------------------------------------------------------------------------------------
			// WicDecoder

			WicDecoder::WicDecoder(WicStream^ stream, Guid vendor, WicDecodeOptions options)
			{
				IWICBitmapDecoder* decoder;

				ThrowIfFailed<GraphicsException>(GlobalAssets::FactoryWic->Native->CreateDecoderFromStream(
					(IStream*)stream->Native,
					(vendor == Guid::Empty ? nullptr : (GUID*)& vendor),
					(WICDecodeOptions)options,
					&decoder)
					);

				Assing(decoder);
			}

			WicDecoder::WicDecoder(String^ fileName, Guid vendor, WicDesiredAccess desiredAccess, WicDecodeOptions options)
			{
				pin_ptr<const System::Char> pFileName = PtrToStringChars(fileName);

				IWICBitmapDecoder* decoder;

				ThrowIfFailed<GraphicsException>(GlobalAssets::FactoryWic->Native->CreateDecoderFromFilename(
					pFileName,
					(vendor == Guid::Empty ? nullptr : (GUID*)& vendor),
					(DWORD)desiredAccess,
					(WICDecodeOptions)options,
					&decoder)
					);

				Assing(decoder);
			}

			//--------------------------------------------------------------------------------------
			// WicFormatConverter

			WicFormatConverter::WicFormatConverter()
			{
				IWICFormatConverter* converter;
				ThrowIfFailed<GraphicsException>(GlobalAssets::FactoryWic->Native->CreateFormatConverter(&converter));
				Assing(converter);
			}

			void WicFormatConverter::Convert(WicBitmapSource^ source, Guid dstPixelFormat, WicDitherType dither, WicPalette^ palette, Double alphaThresholdPercent, WicPaletteType paletteType)
			{
				IWICPalette* p = (palette == nullptr ? nullptr : palette->Native);

				ThrowIfFailed<GraphicsException>(Native->Initialize(
					source->Native,
					*(GUID*)& dstPixelFormat,
					(WICBitmapDitherType)dither,
					p,
					alphaThresholdPercent,
					(WICBitmapPaletteType)paletteType));
			}

			//--------------------------------------------------------------------------------------
			// WicScaler

			WicScaler::WicScaler()
			{
				IWICBitmapScaler* scaler;
				ThrowIfFailed<GraphicsException>(GlobalAssets::FactoryWic->Native->CreateBitmapScaler(&scaler));
				Assing(scaler);
			}

			void WicScaler::Scale(WicBitmapSource^ source, Int32 width, Int32 height, WicInterpolationMode mode)
			{
				ThrowIfFailed<GraphicsException>(Native->Initialize(source->Native, width, height, (WICBitmapInterpolationMode)mode));
			}

			//--------------------------------------------------------------------------------------
			// WicStream

			ref class StreamWrapper : public System::Runtime::InteropServices::ComTypes::IStream
			{
				System::IO::Stream^ _stream;

			public:

				StreamWrapper(System::IO::Stream^ stream)
				{
					if (stream == nullptr)
						throw gcnew ArgumentNullException("stream");
					_stream = stream;
				}

			protected:

				virtual void Clone(System::Runtime::InteropServices::ComTypes::IStream^% ppstm) = IStream::Clone
				{
					ppstm = nullptr;
				}

					virtual void Commit(Int32 grfCommitFlags) = IStream::Commit
				{
				}

				virtual void CopyTo(System::Runtime::InteropServices::ComTypes::IStream ^ pstm, __int64 cb, IntPtr pcbRead, IntPtr pcbWritten) = IStream::CopyTo
				{
				}

				virtual void LockRegion(__int64 libOffset, __int64 cb, Int32 dwLockType) = IStream::LockRegion
				{
				}

				virtual void Read(cli::array<unsigned char> ^ pv, Int32 cb, System::IntPtr pcbRead) = IStream::Read
				{
					if (pv == nullptr)
						throw gcnew ArgumentNullException();
					if (pcbRead == IntPtr::Zero)
						throw gcnew ArgumentNullException();

					try
					{
						auto value = (System::Int32)_stream->Read(pv, 0, cb);
						//if ( value > 0 )
						{

							Marshal::WriteInt32(pcbRead, value);
						}
					}
					catch (AccessViolationException^ e)
					{
						UNREFERENCED_PARAMETER(e);
						throw;
					}
				}

					virtual void Revert() = IStream::Revert
				{
				}

				virtual void Seek(__int64 dlibMove, Int32 dwOrigin, IntPtr plibNewPosition) = IStream::Seek
				{
					__int64* pNewPosition = (__int64*)plibNewPosition.ToPointer();
					__int64 newPosition = _stream->Seek(dlibMove, (System::IO::SeekOrigin)dwOrigin);

					if (pNewPosition != nullptr)
						* (pNewPosition) = newPosition;
				}

					virtual void SetSize(__int64 libNewSize) = IStream::SetSize
				{
				}

				virtual void Stat(System::Runtime::InteropServices::ComTypes::STATSTG % pstatstg, Int32 grfStatFlag) = IStream::Stat
				{
					if ((grfStatFlag & STATFLAG_NONAME) == 0)
					{
						pstatstg.pwcsName = gcnew String(L"no name");
					}

					pstatstg.type = STGTY_STREAM;
					pstatstg.cbSize = _stream->Length;
					pstatstg.mtime.dwHighDateTime = 0;
					pstatstg.mtime.dwLowDateTime = 0;
					pstatstg.atime.dwHighDateTime = 0;
					pstatstg.atime.dwLowDateTime = 0;
					pstatstg.grfMode = 0;
					pstatstg.grfLocksSupported = 0;
					pstatstg.clsid = Guid(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
					pstatstg.grfStateBits = 0;
				}

					virtual void UnlockRegion(__int64 libOffset, __int64 cb, Int32 dwLockType) = IStream::UnlockRegion
				{
				}

				virtual void Write(cli::array<unsigned char> ^ pv, Int32 cb, IntPtr pcbWritten) = IStream::Write
				{
				}
			};

			void WicStream::Create()
			{
				IWICStream* stream;

				ThrowIfFailed<GraphicsException>(GlobalAssets::WIC->Native->CreateStream(&stream));

				Assing(stream);
			}

			void WicStream::Initialize(Type^ type, String^ resourceName)
			{
				System::IO::Stream^ stream = type->Assembly->GetManifestResourceStream(type, resourceName);
				Initialize(stream);
			}

			void WicStream::Initialize(String^ fileName, WicDesiredAccess desiredAccess)
			{
				pin_ptr<const System::Char> pFileName = PtrToStringChars(fileName);
				ThrowIfFailed<GraphicsException>(Native->InitializeFromFilename(pFileName, (DWORD)desiredAccess));
			}

			void WicStream::Initialize(System::IO::Stream^ stream)
			{
				auto wrapper = gcnew StreamWrapper(stream);
				auto pStream = Marshal::GetComInterfaceForObject(wrapper, System::Runtime::InteropServices::ComTypes::IStream::typeid);

				try
				{
					ThrowIfFailed<GraphicsException>(Native->InitializeFromIStream((IStream*)pStream.ToPointer()));
				}
				finally
				{
					Marshal::Release(pStream);
				}
			}

			void WicStream::Initialize(System::IO::Stream^ stream, UInt64 offset, UInt64 maxSize)
			{
				StreamWrapper^ wrapper = gcnew StreamWrapper(stream);
				IntPtr pStream = Marshal::GetComInterfaceForObject(wrapper, System::Runtime::InteropServices::ComTypes::IStream::typeid);

				try
				{
					ThrowIfFailed<GraphicsException>(Native->InitializeFromIStreamRegion(
						(IStream*)pStream.ToPointer(),
						*(ULARGE_INTEGER*)& offset,
						*(ULARGE_INTEGER*)& maxSize));
				}
				finally
				{
					Marshal::Release(pStream);
				}
			}

			void WicStream::Initialize(array<Byte>^ buffer)
			{
				if (buffer == nullptr)
					throw gcnew ArgumentNullException("buffer");

				pin_ptr<Byte> pBuffer = &buffer[0];

				ThrowIfFailed<GraphicsException>(Native->InitializeFromMemory((BYTE*)pBuffer, buffer->Length));
			}
		}
	}
}
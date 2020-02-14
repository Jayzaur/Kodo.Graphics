#pragma once

#include "..\Graphics.hpp"

namespace Kodo
{
	namespace Graphics
	{
		namespace Imaging
		{
			using namespace System;
			using namespace System::IO;
			using namespace System::Runtime::InteropServices;

			public ref class WicPixelFormat abstract sealed
			{
			public:
				initonly static Guid DontCare = Guid(0x6fddc324, 0x4e03, 0x4bfe, 0xb1, 0x85, 0x3d, 0x77, 0x76, 0x8d, 0xc9, 0x00);
				initonly static Guid RGB24bpp = Guid(0x6fddc324, 0x4e03, 0x4bfe, 0xb1, 0x85, 0x3d, 0x77, 0x76, 0x8d, 0xc9, 0x0d);
				initonly static Guid PBGRA32bpp = Guid(0x6fddc324, 0x4e03, 0x4bfe, 0xb1, 0x85, 0x3d, 0x77, 0x76, 0x8d, 0xc9, 0x10);
			};

			[StructLayout(LayoutKind::Sequential)]
			public value struct WicColor
			{
			public:
				Byte R;
				Byte G;
				Byte B;
				Byte A;
			};

			public enum class WicBitmapCreateCacheOption
			{
				NoCache = 0x00000000,
				CacheOnDemand = 0x00000001,
				CacheOnLoad = 0x00000002
			};

			public enum class WicDecodeOptions
			{
				MetadataCacheOnDemand = 0x00000000,
				MetadataCacheOnLoad = 0x00000001
			};

			/// <summary>
			/// Specifies the sampling or filtering mode to use when scaling an image.
			/// </summary>
			public enum class WicInterpolationMode
			{
				/// <summary>
				/// A nearest neighbor interpolation algorithm. Also known as nearest pixel or point interpolation.
				/// The output pixel is assigned the value of the pixel that the point falls within. No other pixels are considered.
				/// </summary>
				NearestNeighbor = 0x00000000,
				/// <summary>
				/// A bilinear interpolation algorithm. 
				/// The output pixel values are computed as a weighted average of the nearest four pixels in a 2x2 grid.
				/// </summary>
				Linear = 0x00000001,
				/// <summary>
				/// A bicubic interpolation algorithm. 
				/// Destination pixel values are computed as a weighted average of the nearest sixteen pixels in a 4x4 grid.
				/// </summary>
				Cubic = 0x00000002,
				/// <summary>
				/// A Fant resampling algorithm. 
				/// Destination pixel values are computed as a weighted average of the all the pixels that map to the new pixel.
				/// </summary>
				Fant = 0x00000003
			};

			public enum class WicPaletteType
			{
				Custom = 0x00000000,
				MedianCut = 0x00000001,
				FixedBW = 0x00000002,
				FixedHalftone8 = 0x00000003,
				FixedHalftone27 = 0x00000004,
				FixedHalftone64 = 0x00000005,
				FixedHalftone125 = 0x00000006,
				FixedHalftone216 = 0x00000007,
				FixedWebPalette = FixedHalftone216,
				FixedHalftone252 = 0x00000008,
				FixedHalftone256 = 0x00000009,
				FixedGray4 = 0x0000000A,
				FixedGray16 = 0x0000000B,
				FixedGray256 = 0x0000000C
			};

			public enum class WicDitherType
			{
				None = 0x00000000,
				Solid = 0x00000000,
				Ordered4x4 = 0x00000001,
				Ordered8x8 = 0x00000002,
				Ordered16x16 = 0x00000003,
				Spiral4x4 = 0x00000004,
				Spiral8x8 = 0x00000005,
				DualSpiral4x4 = 0x00000006,
				DualSpiral8x8 = 0x00000007,
				ErrorDiffusion = 0x00000008
			};

			public enum class WicDesiredAccess
			{
				Read = (Int32)GENERIC_READ,
				Write = (Int32)GENERIC_WRITE
			};

			[StructLayout(LayoutKind::Sequential)]
			public value struct WicRect
			{
			public:
				Int32 X;
				Int32 Y;
				Int32 Width;
				Int32 Height;
			};

			ref class WicBitmap;
			ref class WicPalette;

			//--------------------------------------------------------------------------------------
			// WicBitmapSource

			public ref class WicBitmapSource : GraphicsObject
			{
				GRAPHICS_OBJECT(WicBitmapSource, IWICBitmapSource);

			public:

				property Guid PixelFormat
				{
					Guid get() {
						Guid guid;
						ThrowIfFailed<GraphicsException>(Native->GetPixelFormat((WICPixelFormatGUID*)& guid));
						return guid;
					}
				}

				property Kodo::Graphics::Vec2 Size
				{
					Kodo::Graphics::Vec2 get() {
						UINT32 width, height;
						ThrowIfFailed<GraphicsException>(Native->GetSize(&width, &height));
						return Kodo::Graphics::Vec2(float(width), float(height));
					}
				}

				WicBitmapSource() { }

				void CopyPalette(WicPalette^ palette);
				void CopyPixels(WicRect rect, Int32 stride, array<Byte>^ buffer);
				void GetResolution([OutAttribute()]Double% dpiX, [OutAttribute()]Double% dpiY);
			};

			//--------------------------------------------------------------------------------------
			// WicPalette

			public ref class WicPalette : GraphicsObject
			{
				GRAPHICS_OBJECT(WicPalette, IWICPalette);

			public:

				property WicPaletteType PaletteType
				{
					WicPaletteType get() {
						WICBitmapPaletteType value;
						ThrowIfFailed<GraphicsException>(Native->GetType(&value));
						return (WicPaletteType)value;
					}
				}
				property Boolean HasAlpha
				{
					Boolean get() {
						BOOL value;
						ThrowIfFailed<GraphicsException>(Native->HasAlpha(&value));
						return value != 0;
					}
				}

				property Boolean IsBlackWhite
				{
					Boolean get() {
						BOOL value;
						ThrowIfFailed<GraphicsException>(Native->IsBlackWhite(&value));
						return value != 0;
					}
				}
				property Boolean IsGrayscale
				{
					Boolean get() {
						BOOL value;
						ThrowIfFailed<GraphicsException>(Native->IsGrayscale(&value));
						return value != 0;
					}
				}

				array<WicColor>^ GetColors();
				void Initialize(array<WicColor>^ colors);
				void Initialize(WicBitmapSource^ surface, Int32 colorCount, Boolean addTransparentColor);
				void Initialize(WicPalette^ source);
				void Initialize(WICBitmapPaletteType paletteType, Boolean addTransparentColor);
			};

			//--------------------------------------------------------------------------------------
			// WicBitmap

			[Flags]
			public enum class WICBitmapLockFlags
			{
				Read = WICBitmapLockRead,
				Write = WICBitmapLockWrite
			};

			public ref class WicBitmapLock : GraphicsObject
			{
				GRAPHICS_OBJECT(WicBitmapLock, IWICBitmapLock);
			};

			public ref class WicBitmap : WicBitmapSource
			{
				GRAPHICS_OBJECT(WicBitmap, IWICBitmap);

			public:

				void SetPalette(WicPalette^ palette);
				void SetResolution(Double dpiX, Double dpiY);

				WicBitmapLock^ Lock(Kodo::Graphics::Rectangle lockRect, WICBitmapLockFlags flags);
			};

			//--------------------------------------------------------------------------------------
			// WicFactory

			public ref class WicFactory : GraphicsObject
			{
				GRAPHICS_OBJECT(WicFactory, IWICImagingFactory);

			public:

				WicFactory();

				WicBitmap^ CreateBitmap(Int32 width, Int32 height, Guid pixelFormat, WicBitmapCreateCacheOption option);
			};

			//--------------------------------------------------------------------------------------
			// WicStream

			/// <summary> Represents a Windows Imaging Component (WIC) stream for referencing imaging and metadata content. </summary>
			public ref class WicStream : GraphicsObject
			{
				GRAPHICS_OBJECT(WicStream, IWICStream);

			public:

				/// <summary>
				/// Initializes a stream from a resource.
				/// </summary>
				/// <param name="type">The type of the resource.</param>
				/// <param name="resourceName">The name of the resource.</param>
				WicStream(Type^ type, String^ resourceName) { Create(); Initialize(type, resourceName); }
				/// <summary>
				/// Initializes a stream from a file.
				/// </summary>
				/// <param name="fileName">The name of the file.</param>
				WicStream(String^ fileName) { Create(); Initialize(fileName, WicDesiredAccess::Read); }
				/// <summary>
				/// Initializes a stream from a file.
				/// </summary>
				/// <param name="fileName">The name of the file.</param>
				/// <param name="desiredAccess">The desired file access.</param>
				WicStream(String^ fileName, WicDesiredAccess desiredAccess) { Create(); Initialize(fileName, desiredAccess); }
				/// <summary>
				/// Initializes a stream from an existing stream.
				/// </summary>
				/// <param name="stream">The stream.</param>
				WicStream(System::IO::Stream^ stream)
				{
					Create();
					Initialize(stream);
				}
				/// <summary>
				/// Initializes a stream from an existing stream.
				/// </summary>
				/// <param name="stream">The stream.</param>
				/// <param name="offset">The stream in to the stream.</param>
				/// <param name="maxSize">The maximum size of the stream.</param>
				WicStream(System::IO::Stream^ stream, UInt64 offset, UInt64 maxSize) { Create(); Initialize(stream, offset, maxSize); }
				/// <summary>
				/// Initializes a stream from an array of bytes.
				/// </summary>
				/// <param name="buffer">The array to initialize from.</param>
				WicStream(array<Byte>^ buffer) { Create(); Initialize(buffer); }


			private:

				void Create();
				void Initialize(Type^ type, String^ resourceName);
				void Initialize(String^ fileName, WicDesiredAccess desiredAccess);
				void Initialize(System::IO::Stream^ stream);
				void Initialize(System::IO::Stream^ stream, UInt64 offset, UInt64 maxSize);
				void Initialize(array<Byte>^ buffer);
			};

			//--------------------------------------------------------------------------------------
			// WicDecoder

			/// <summary> This class provides methods for decoding individual image frames of an encoded file. </summary>
			public ref class WicDecoderFrame : WicBitmapSource
			{
				GRAPHICS_OBJECT(WicDecoderFrame, IWICBitmapFrameDecode);
			};

			/// <summary> This class provides access to the decoder's properties such as global thumbnails (if supported), frames, and palette. </summary>
			public ref class WicDecoder : GraphicsObject
			{
				GRAPHICS_OBJECT(WicDecoder, IWICBitmapDecoder);

			public:

				/// <summary> Creates a new instance of <see cref="WicDecoder"/> from a stream. </summary>
				/// <param name="stream">The stream.</param>
				/// <param name="options">The decode options.</param>
				WicDecoder(WicStream^ stream, WicDecodeOptions options) : WicDecoder(stream, Guid::Empty, options) { }
				/// <summary> Creates a new instance of <see cref="WicDecoder"/> from a stream. </summary>
				/// <param name="stream">The stream.</param>
				/// <param name="vendor">The vendor guid.</param>
				/// <param name="options">The decode options.</param>
				WicDecoder(WicStream^ stream, Guid vendor, WicDecodeOptions options);

				/// <summary> Creates a new instance of <see cref="WicDecoder"/> from a file. </summary>
				/// <param name="filename">The filename.</param>
				/// <param name="options">The decode options.</param>
				WicDecoder(String^ filename, WicDecodeOptions options) : WicDecoder(filename, Guid::Empty, WicDesiredAccess::Read, options) { }
				/// <summary> Creates a new instance of <see cref="WicDecoder"/> from a file. </summary>
				/// <param name="filename">The filename.</param>
				/// <param name="vendor">The vendor guid.</param>
				/// <param name="desiredAccess">The desired file access.</param>
				/// <param name="options">The decode options.</param>
				WicDecoder(String^ filename, Guid vendor, WicDesiredAccess desiredAccess, WicDecodeOptions options);

				/// <summary> Get the total number of frames in the image. </summary>
				Int32 GetFrameCount()
				{
					UINT count;
					ThrowIfFailed<GraphicsException>(Native->GetFrameCount(&count));
					return (Int32)count;
				}

				/// <summary>
				/// Get the specified streamCurrentFrame of the image.
				/// </summary>
				WicDecoderFrame^ GetFrame(Int32 index)
				{
					IWICBitmapFrameDecode* frame;
					ThrowIfFailed<GraphicsException>(Native->GetFrame((UINT)index, &frame));
					return gcnew WicDecoderFrame(frame);
				}
			};

			//--------------------------------------------------------------------------------------
			// WicFormatConverter

			/// <summary> Represents an <see cref="WicBitmapSource"/> that converts the image data from one pixel format to another, handling dithering and halftoning to indexed formats, palette translation and alpha thresholding. </summary>
			public ref class WicFormatConverter : WicBitmapSource
			{
				GRAPHICS_OBJECT(WicFormatConverter, IWICFormatConverter);

			public:

				/// <summary> Create a new instance of <see cref="WicFormatConverter"/>. </summary>
				WicFormatConverter();

				/// <summary> Determines if the source pixel format can be converted to the destination pixel format. </summary>
				/// <param name="srcPixelFormat">The source pixel format.</param>
				/// <param name="dstPixelFormat">The destination pixel format.</param>
				Boolean CanConvert(Guid srcPixelFormat, Guid dstPixelFormat)
				{
					BOOL value;
					ThrowIfFailed<GraphicsException>(Native->CanConvert(*(GUID*)& srcPixelFormat, *(GUID*)& dstPixelFormat, &value));
					return (value != 0);
				}

				/// <summary> Performs the conversion with no color loss mitigation. </summary>
				/// <param name="source">The input bitmap to convert.</param>
				/// <param name="dstPixelFormat">The destination pixel format GUID.</param>
				void Convert(WicBitmapSource^ source, Guid dstPixelFormat) { Convert(source, dstPixelFormat, WicDitherType::None, nullptr, 0.0, WicPaletteType::Custom); }

				/// <summary> Performs the conversion. </summary>
				/// <param name="source">The input bitmap to convert.</param>
				/// <param name="dstPixelFormat">The destination pixel format GUID.</param>
				/// <param name="dither">The <see cref="WicDitherType"/> used for conversion.</param>
				/// <param name="palette">The palette to use for conversion.</param>
				/// <param name="alphaThresholdPercent">The alpha threshold to use for conversion.</param>
				/// <param name="paletteType">The palette translation type to use for conversion.</param>
				void Convert(WicBitmapSource^ source, Guid dstPixelFormat, WicDitherType dither, WicPalette^ palette, Double alphaThresholdPercent, WicPaletteType paletteType);
			};

			//--------------------------------------------------------------------------------------
			// WicScaler

			/// <summary> Represents a resized version of the input bitmap using a resampling or filtering algorithm. </summary>
			public ref class WicScaler : WicBitmapSource
			{
				GRAPHICS_OBJECT(WicScaler, IWICBitmapScaler);

			public:

				/// <summary> Create a new instance of WicScaler. </summary>
				WicScaler();

				/// <summary> Perform the scaling. </summary>
				/// <param name="source">The input bitmap source.</param>
				/// <param name="width">The destination width.</param>
				/// <param name="height">The destination height.</param>
				/// <param name="mode">The BitmapInterpolationMode to use when scaling.</param>
				void Scale(WicBitmapSource^ source, Int32 width, Int32 height, WicInterpolationMode mode);
			};
		}
	}
}
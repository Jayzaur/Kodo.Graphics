#include "../Graphics.hpp"

#include "../D2D/D2D.hpp"

#include "../D3D/D3D.hpp"

#include "../DW/DW.hpp"

#include "../WIC/WIC.hpp"

namespace Kodo
{
	namespace Graphics
	{
		using namespace System::Collections::Generic;
		using namespace Imaging;

		//--------------------------------------------------------------------------------------
		// Bitmap

		Bitmap::Bitmap(D2DDeviceContext^ deviceContext, DxgiSwapChain^ swapChain)
		{
			com_ptr<IDXGISurface> dxgiBackBuffer;
			ThrowIfFailed(swapChain->Native->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer)));

			HWND hWnd;
			ThrowIfFailed(swapChain->Native->GetHwnd(&hWnd));

			D2D1_BITMAP_PROPERTIES1 backBufferDesc;
			backBufferDesc.colorContext = nullptr;
			backBufferDesc.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
			backBufferDesc.dpiX = static_cast<FLOAT>(GetDpiForWindow(hWnd));
			backBufferDesc.dpiY = backBufferDesc.dpiX;
			backBufferDesc.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
			backBufferDesc.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;

			ID2D1Bitmap1* bitmap;
			ThrowIfFailed(deviceContext->Native->CreateBitmapFromDxgiSurface(dxgiBackBuffer, backBufferDesc, &bitmap));
			Assing(bitmap);
		}

		Bitmap::Bitmap(D2DDeviceContext^ context, Vec2 size, BitmapProperties properties)
		{
			ID2D1Bitmap1* b = nullptr;
			D2D_SIZE_U s;
			s.width = static_cast<UINT32>(size.Width);
			s.height = static_cast<UINT32>(size.Height);
			D2D1_BITMAP_PROPERTIES1 p;
			p.pixelFormat.format = static_cast<DXGI_FORMAT>(properties.PixelFormat.Format);
			p.pixelFormat.alphaMode = static_cast<D2D1_ALPHA_MODE>(properties.PixelFormat.AlphaMode);
			p.dpiX = properties.DPIX;
			p.dpiY = properties.DPIY;
			p.bitmapOptions = static_cast<D2D1_BITMAP_OPTIONS>(properties.BitmapOptions);
			p.colorContext = nullptr;

			ThrowIfFailed(context->Native->CreateBitmap(s, nullptr, 0, p, &b));

			Assing(b);
		}

		Bitmap::Bitmap(D2DDeviceContext^ context, Vec2 size)
		{
			ID2D1Bitmap1* b = nullptr;
			D2D_SIZE_U s;
			s.width = static_cast<UINT32>(size.Width);
			s.height = static_cast<UINT32>(size.Height);

			D2D1_BITMAP_PROPERTIES1 p = D2D1::BitmapProperties1();
			p.pixelFormat = context->Native->GetPixelFormat();

			ThrowIfFailed(context->Native->CreateBitmap(s, nullptr, 0, p, &b));

			Assing(b);
		}

		Bitmap::Bitmap(D2DDeviceContext^ context, Vec2 size, Kodo::Graphics::PixelFormat pixelFormat, Kodo::Graphics::BitmapOptions options, float dpi)
		{
			ID2D1Bitmap1* b = nullptr;
			D2D_SIZE_U s;
			s.width = static_cast<UINT32>(size.Width);
			s.height = static_cast<UINT32>(size.Height);
			D2D1_BITMAP_PROPERTIES1 p;
			p.pixelFormat.format = static_cast<DXGI_FORMAT>(pixelFormat.Format);
			p.pixelFormat.alphaMode = static_cast<D2D1_ALPHA_MODE>(pixelFormat.AlphaMode);
			p.dpiX = dpi;
			p.dpiY = dpi;
			p.bitmapOptions = static_cast<D2D1_BITMAP_OPTIONS>(options);
			p.colorContext = nullptr;

			ThrowIfFailed(context->Native->CreateBitmap(s, nullptr, 0, p, &b));

			Assing(b);
		}

		Bitmap::Bitmap(D2DDeviceContext^ context, WicBitmapSource^ source)
		{
			ID2D1Bitmap1* b = nullptr;

			ThrowIfFailed<GraphicsException>(context->Native->CreateBitmapFromWicBitmap(source->Native, D2D1::BitmapProperties1(), &b));

			Assing(b);
		}

		Bitmap::Bitmap(D2DDeviceContext^ context, WicBitmapSource^ source, BitmapProperties properties)
		{
			ID2D1Bitmap1* b = nullptr;
			D2D1_BITMAP_PROPERTIES1 p;
			p.pixelFormat.format = static_cast<DXGI_FORMAT>(properties.PixelFormat.Format);
			p.pixelFormat.alphaMode = static_cast<D2D1_ALPHA_MODE>(properties.PixelFormat.AlphaMode);
			p.dpiX = properties.DPIX;
			p.dpiY = properties.DPIY;
			p.bitmapOptions = static_cast<D2D1_BITMAP_OPTIONS>(properties.BitmapOptions);
			p.colorContext = nullptr;

			ThrowIfFailed<GraphicsException>(context->Native->CreateBitmapFromWicBitmap(source->Native, p, &b));

			Assing(b);
		}

		//--------------------------------------------------------------------------------------
		// SolidColorBrush

		SolidColorBrush::SolidColorBrush(D2DDeviceContext^ deviceContext)
		{
			ID2D1SolidColorBrush* brush;
			ThrowIfFailed(deviceContext->Native->CreateSolidColorBrush(&D2D1::ColorF(0, 0, 0, 0), nullptr, &brush));
			Assing(brush);
		}

		SolidColorBrush::SolidColorBrush(D2DDeviceContext^ context, Kodo::Graphics::Color color)
		{
			ID2D1SolidColorBrush* brush;
			ThrowIfFailed(context->Native->CreateSolidColorBrush(reinterpret_cast<D2D1_COLOR_F*>(&color), nullptr, &brush));
			Assing(brush);
		}

		//--------------------------------------------------------------------------------------
		// GradientStopCollection

		GradientStopCollection::GradientStopCollection(D2DDeviceContext^ context, array<GradientStop>^ stops)
		{
			pin_ptr<GradientStop> pinnedStops = &stops[0];

			ID2D1GradientStopCollection* collection;
			ThrowIfFailed(context->Native->CreateGradientStopCollection(reinterpret_cast<D2D1_GRADIENT_STOP*>(pinnedStops), stops->Length, &collection));
			Assing(collection);
		}

		GradientStopCollection::GradientStopCollection(D2DDeviceContext^ context, array<GradientStop>^ stops, Kodo::Graphics::Gamma gamma, Kodo::Graphics::ExtendMode extendMode)
		{
			pin_ptr<GradientStop> pinnedStops = &stops[0];

			ID2D1GradientStopCollection* collection;
			ThrowIfFailed(context->Native->CreateGradientStopCollection(reinterpret_cast<D2D1_GRADIENT_STOP*>(pinnedStops), stops->Length, static_cast<D2D1_GAMMA>(gamma), static_cast<D2D1_EXTEND_MODE>(extendMode), &collection));
			Assing(collection);
		}

		//--------------------------------------------------------------------------------------
		// LinearGradientBrush

		LinearGradientBrush::LinearGradientBrush(D2DDeviceContext^ context, GradientStopCollection^ gradientStops, Vec2 gradientStart, Vec2 gradientEnd)
		{
			D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES brushProp;
			brushProp.startPoint = gradientStart;
			brushProp.endPoint = gradientEnd;

			ID2D1LinearGradientBrush* brush;
			ThrowIfFailed(context->Native->CreateLinearGradientBrush(&brushProp, nullptr, gradientStops->Native, &brush));
			Assing(brush);
		}

		//--------------------------------------------------------------------------------------
		// BitmapBrush

		BitmapBrush::BitmapBrush(D2DDeviceContext^ context, Kodo::Graphics::Bitmap^ bitmap)
		{
			ID2D1BitmapBrush* brush;
			ThrowIfFailed(context->Native->CreateBitmapBrush(bitmap->Native, &brush));
			Assing(brush);
		}

		//--------------------------------------------------------------------------------------
		// PathGeometry

		PathGeometry::PathGeometry()
		{
			ID2D1PathGeometry* geometry = nullptr;
			ThrowIfFailed(GlobalAssets::Factory2D->Native->CreatePathGeometry(&geometry));
			Assing(geometry);

		}

		//--------------------------------------------------------------------------------------
		// StrokeStyle

		StrokeStyle::StrokeStyle(CapStyle startCap, CapStyle endCap, CapStyle dashCap, Kodo::Graphics::LineJoin lineJoin, float miterLimit, Kodo::Graphics::DashStyle dashStyle, float dashOffset, array<float>^ dashes)
		{
			D2D1_STROKE_STYLE_PROPERTIES props = D2D1::StrokeStyleProperties((D2D1_CAP_STYLE)startCap, (D2D1_CAP_STYLE)endCap, (D2D1_CAP_STYLE)dashCap, (D2D1_LINE_JOIN)lineJoin, miterLimit, (D2D1_DASH_STYLE)dashStyle, dashOffset);

			if (dashes == nullptr)
			{
				ID2D1StrokeStyle* strokeStyle;
				ThrowIfFailed(GlobalAssets::Factory2D->Native->CreateStrokeStyle(reinterpret_cast<D2D1_STROKE_STYLE_PROPERTIES*>(&props), nullptr, 0, &strokeStyle));
				Assing(strokeStyle);
			}
			else
			{
				pin_ptr<float> pinnedDashes = &dashes[0];

				ID2D1StrokeStyle* strokeStyle;
				ThrowIfFailed(GlobalAssets::Factory2D->Native->CreateStrokeStyle(reinterpret_cast<D2D1_STROKE_STYLE_PROPERTIES*>(&props), pinnedDashes, dashes->Length, &strokeStyle));
				Assing(strokeStyle);
			}
		}

		//--------------------------------------------------------------------------------------
		// D2DDevice

		D2DDevice::D2DDevice(D3DDevice^ d3dDevice, DxgiDevice^ dxgiDevice, bool debug)
		{
			D2D1_FACTORY_OPTIONS opts;
			opts.debugLevel = debug ? D2D1_DEBUG_LEVEL_INFORMATION : D2D1_DEBUG_LEVEL_NONE;

			ID2D1Factory2* d2dFactory;
			ThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, opts, &d2dFactory));

			if (GlobalAssets::Factory2D == nullptr)
			{
				GlobalAssets::Factory2D = gcnew D2DFactory(d2dFactory);
			}

			ID2D1Device1* d2dDevice;
			ThrowIfFailed(d2dFactory->CreateDevice(dxgiDevice->Native, &d2dDevice));
			Assing(d2dDevice);

			ID2D1DeviceContext1* d2dDeviceContext;
			ThrowIfFailed(d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dDeviceContext));
			deviceContext = gcnew D2DDeviceContext(d2dDeviceContext);
		}

		//--------------------------------------------------------------------------------------
		// D2DDeviceContext

		void D2DDeviceContext::SetTarget(Image^ image)
		{
			currentTarget = image;

			if (image == nullptr)
			{
				Native->SetTarget(nullptr);
			}
			else
			{
				Native->SetTarget(image->Native);
			}
		}

		GeometryRealization^ D2DDeviceContext::CreateStrokedGeometryRealization(Geometry^ geometry, float flatteningTolerance, float strokeWidth, StrokeStyle^ stroke)
		{
			ID2D1GeometryRealization* geometryRealization;
			ThrowIfFailed(Native->CreateStrokedGeometryRealization(geometry->Native, flatteningTolerance, strokeWidth, stroke->Native, &geometryRealization));
			return gcnew GeometryRealization(geometryRealization);
		}

		void D2DDeviceContext::DrawLine(Vec2 begin, Vec2 end, Brush^ brush, float strokeWidth) { Native->DrawLine(begin, end, brush->Native, strokeWidth); }
		void D2DDeviceContext::DrawLine(Vec2 begin, Vec2 end, Brush^ brush, float strokeWidth, StrokeStyle^ stroke) { Native->DrawLine(begin, end, brush->Native, strokeWidth, stroke->Native); }
		void D2DDeviceContext::DrawLines(array<Vec2>^ points, Int32 start, Int32 count, Brush^ brush, float strokeWidth)
		{
			pin_ptr<Vec2> pinnedPoints = &points[0];
			D2D1_POINT_2F* nativePoints = reinterpret_cast<D2D1_POINT_2F*>(pinnedPoints);
			for (auto i = start; i < count - 1; i++)
				Native->DrawLine(nativePoints[i], nativePoints[i + 1], brush->Native, strokeWidth, nullptr);
		}
		void D2DDeviceContext::DrawLines(array<Vec2>^ points, Int32 start, Int32 count, Brush^ brush, float strokeWidth, StrokeStyle^ stroke)
		{
			pin_ptr<Vec2> pinnedPoints = &points[0];
			D2D1_POINT_2F* nativePoints = reinterpret_cast<D2D1_POINT_2F*>(pinnedPoints);
			for (auto i = start; i < count - 1; i++)
				Native->DrawLine(nativePoints[i], nativePoints[i + 1], brush->Native, strokeWidth, stroke->Native);
		}
		void D2DDeviceContext::DrawEllipse(Ellipse ellipse, Brush^ brush, float strokeWidth) { Native->DrawEllipse((D2D1_ELLIPSE*)&ellipse, brush->Native, strokeWidth); }
		void D2DDeviceContext::DrawGeometry(Geometry^ geometry, Brush^ brush, float strokeWidth) { Native->DrawGeometry(geometry->Native, brush->Native, strokeWidth); }
		void D2DDeviceContext::DrawGeometry(Geometry^ geometry, Brush^ brush, float strokeWidth, StrokeStyle^ stroke) { Native->DrawGeometry(geometry->Native, brush->Native, strokeWidth, stroke->Native); }
		void D2DDeviceContext::DrawGeometryRealization(GeometryRealization^ geometryRealization, Brush^ brush) { Native->DrawGeometryRealization(geometryRealization->Native, brush->Native); }
		void D2DDeviceContext::DrawRectangle(Rectangle rect, Brush^ brush, float strokeWidth) { Native->DrawRectangle((D2D1_RECT_F*)&rect, brush->Native, strokeWidth); }
		void D2DDeviceContext::DrawRectangle(Rectangle rect, Brush^ brush, float strokeWidth, StrokeStyle^ stroke) { Native->DrawRectangle((D2D1_RECT_F*)&rect, brush->Native, strokeWidth, stroke->Native); }
		void D2DDeviceContext::DrawBitmap(Bitmap^ bitmap, Rectangle sourceArea, Rectangle destinationArea, float opacity, InterpolationMode interpolation) { Native->DrawBitmap(bitmap->Native, (D2D1_RECT_F*)&destinationArea, opacity, (D2D1_BITMAP_INTERPOLATION_MODE)interpolation, (D2D1_RECT_F*)&sourceArea); }
		void D2DDeviceContext::DrawBitmap(Bitmap^ bitmap, Rectangle destinationArea, float opacity, InterpolationMode interpolation) { Native->DrawBitmap(bitmap->Native, (D2D1_RECT_F*)&destinationArea, opacity, static_cast<D2D1_INTERPOLATION_MODE>(interpolation)); }
		void D2DDeviceContext::DrawTextLayout(TextLayout^ layout, Vec2 origin, Brush^ brush, DrawTextOptions options) { Native->DrawTextLayout(origin, layout->Native, brush->Native, static_cast<D2D1_DRAW_TEXT_OPTIONS>(options)); }
		void D2DDeviceContext::DrawText(String^ text, TextFormat^ format, Rectangle area, Brush^ brush, DrawTextOptions options, MeasuringMode mode)
		{
			if (!String::IsNullOrEmpty(text))
			{
				pin_ptr<const wchar_t> pinnedText = PtrToStringChars(text);
				Native->DrawText(pinnedText, text->Length, format->Native, (D2D1_RECT_F*)&area, brush->Native, static_cast<D2D1_DRAW_TEXT_OPTIONS>(options), static_cast<DWRITE_MEASURING_MODE>(mode));
			}
		}
		void D2DDeviceContext::FillEllipse(Ellipse ellipse, Brush^ brush) { Native->FillEllipse((D2D1_ELLIPSE*)&ellipse, brush->Native); }
		void D2DDeviceContext::FillGeometry(Geometry^ geometry, Brush^ brush) { Native->FillGeometry(geometry->Native, brush->Native); }
		void D2DDeviceContext::FillRectangle(Rectangle rect, Brush^ brush) { Native->FillRectangle((D2D1_RECT_F*)&rect, brush->Native); }
		void D2DDeviceContext::FillRoundedRectangle(RoundedRectangle rect, Brush^ brush) { Native->FillRoundedRectangle((D2D1_ROUNDED_RECT*)&rect, brush->Native); }
		void D2DDeviceContext::FillOpacityMask(Bitmap^ bitmap, Brush^ brush, Rectangle destinationArea) { Native->FillOpacityMask(bitmap->Native, brush->Native, (D2D1_RECT_F*)&destinationArea); }

		//--------------------------------------------------------------------------------------
		// DrawingContext

		DrawingContext::DrawingContext(D2DDeviceContext^ d2dDeviceContext)
		{
			deviceContext = d2dDeviceContext;
		}
	}
}
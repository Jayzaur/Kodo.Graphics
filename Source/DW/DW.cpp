#include "DW.hpp"

namespace Kodo
{
	namespace Graphics
	{
		//--------------------------------------------------------------------------------------
		// TextLayout

		TextLayout::TextLayout(String^ text, TextFormat^ textFormat, float maxWidth, float maxHeight)
		{
			pin_ptr<const wchar_t> pinnedText = PtrToStringChars(text);

			IDWriteTextLayout* layout = nullptr;

			ThrowIfFailed<GraphicsException>(
				GlobalAssets::FactoryDw->Native->CreateTextLayout(pinnedText, text->Length,
					textFormat->Native, maxWidth, maxHeight, &layout));

			Assing(layout);
		}

		array<Kodo::Graphics::ClusterMetrics>^ TextLayout::ClusterMetrics::get()
		{
			UINT32 count;
			HRESULT hr = Native->GetClusterMetrics(nullptr, 0, &count);
			if (FAILED(hr) && hr != E_NOT_SUFFICIENT_BUFFER)
				Marshal::ThrowExceptionForHR(hr);
			array<Kodo::Graphics::ClusterMetrics>^ metrics = gcnew array<Kodo::Graphics::ClusterMetrics>(count);
			pin_ptr<Kodo::Graphics::ClusterMetrics> pMetrics = &metrics[0];
			ThrowIfFailed<GraphicsException>(Native->GetClusterMetrics((DWRITE_CLUSTER_METRICS*)pMetrics, count, &count));
			return metrics;
		}

		String^ TextLayout::GetFontFamilyName(Int32 position)
		{
			IDWriteTextLayout* native = Native;
			UINT32 nameLength;
			ThrowIfFailed<GraphicsException>(native->GetFontFamilyNameLength(position, &nameLength, nullptr));
			wchar_t* name = new wchar_t[nameLength + 1];
			try
			{
				ThrowIfFailed<GraphicsException>(native->GetFontFamilyName(position, name, nameLength, nullptr));
				return gcnew String(name);
			}
			finally
			{
				delete name;
			}
		}

		String^ TextLayout::GetFontFamilyName(Int32 position, [Out]TextRange% textRange)
		{
			IDWriteTextLayout* native = Native;
			UINT32 nameLength;
			ThrowIfFailed<GraphicsException>(native->GetFontFamilyNameLength(position, &nameLength, nullptr));
			wchar_t* name = new wchar_t[nameLength + 1];
			try
			{
				pin_ptr<TextRange> pTextRange = &textRange;
				ThrowIfFailed<GraphicsException>(native->GetFontFamilyName(position, name, nameLength, (DWRITE_TEXT_RANGE*)pTextRange));
				return gcnew String(name);
			}
			finally
			{
				delete name;
			}
		}

		float TextLayout::GetFontSize(Int32 position)
		{
			FLOAT fontSize;
			ThrowIfFailed<GraphicsException>(Native->GetFontSize(position, &fontSize, nullptr));
			return fontSize;
		}

		float TextLayout::GetFontSize(Int32 position, [Out]TextRange% textRange)
		{
			FLOAT fontSize;
			pin_ptr<TextRange> pTextRange = &textRange;
			ThrowIfFailed<GraphicsException>(Native->GetFontSize(position, &fontSize, (DWRITE_TEXT_RANGE*)pTextRange));
			return fontSize;
		}

		FontStretch TextLayout::GetFontStretch(Int32 position)
		{
			DWRITE_FONT_STRETCH value;
			ThrowIfFailed<GraphicsException>(Native->GetFontStretch(position, &value, nullptr));
			return (Kodo::Graphics::FontStretch)value;
		}

		FontStretch TextLayout::GetFontStretch(Int32 position, [Out]TextRange% textRange)
		{
			DWRITE_FONT_STRETCH value;
			pin_ptr<TextRange> pTextRange = &textRange;
			ThrowIfFailed<GraphicsException>(Native->GetFontStretch(position, &value, (DWRITE_TEXT_RANGE*)pTextRange));
			return (Kodo::Graphics::FontStretch)value;
		}

		FontStyle TextLayout::GetFontStyle(Int32 position)
		{
			DWRITE_FONT_STYLE value;
			ThrowIfFailed<GraphicsException>(Native->GetFontStyle(position, &value, nullptr));
			return (Kodo::Graphics::FontStyle)value;
		}

		FontStyle TextLayout::GetFontStyle(Int32 position, [Out]TextRange% textRange)
		{
			DWRITE_FONT_STYLE value;
			pin_ptr<TextRange> pTextRange = &textRange;
			ThrowIfFailed<GraphicsException>(Native->GetFontStyle(position, &value, (DWRITE_TEXT_RANGE*)pTextRange));
			return (Kodo::Graphics::FontStyle)value;
		}

		FontWeight TextLayout::GetFontWeight(Int32 position)
		{
			DWRITE_FONT_WEIGHT value;
			ThrowIfFailed<GraphicsException>(Native->GetFontWeight(position, &value, nullptr));
			return (Kodo::Graphics::FontWeight)value;
		}

		FontWeight TextLayout::GetFontWeight(Int32 position, [Out]TextRange% textRange)
		{
			DWRITE_FONT_WEIGHT value;
			pin_ptr<TextRange> pTextRange = &textRange;
			ThrowIfFailed<GraphicsException>(Native->GetFontWeight(position, &value, (DWRITE_TEXT_RANGE*)pTextRange));
			return (Kodo::Graphics::FontWeight)value;
		}

		array<Kodo::Graphics::LineMetrics>^ TextLayout::LineMetrics::get()
		{
			UINT32 count;
			HRESULT hr = Native->GetLineMetrics(nullptr, 0, &count);
			if (FAILED(hr) && hr != E_NOT_SUFFICIENT_BUFFER)
				Marshal::ThrowExceptionForHR(hr);
			array<Kodo::Graphics::LineMetrics>^ metrics = gcnew array<Kodo::Graphics::LineMetrics>(count);
			pin_ptr<Kodo::Graphics::LineMetrics> pMetrics = &metrics[0];
			ThrowIfFailed<GraphicsException>(Native->GetLineMetrics((DWRITE_LINE_METRICS*)pMetrics, count, &count));
			return metrics;
		}

		HitTestMetrics TextLayout::HitTestPoint(float x, float y, [Out]Boolean% isTrailingHit, [Out]Boolean% isInside)
		{
			HitTestMetrics metrics;
			BOOL b1, b2;
			ThrowIfFailed<GraphicsException>(Native->HitTestPoint(x, y, &b1, &b2, (DWRITE_HIT_TEST_METRICS*)& metrics));
			isTrailingHit = b1 != 0;
			isInside = b2 != 0;
			return metrics;
		}

		HitTestMetrics TextLayout::HitTestTextPosition(Int32 textPosition, Boolean isTrailingHit, [Out]float% pointX, [Out]float% pointY)
		{
			HitTestMetrics metrics;
			FLOAT x, y;
			ThrowIfFailed<GraphicsException>(
				Native->HitTestTextPosition(
					textPosition,
					isTrailingHit,
					&x,
					&y,
					(DWRITE_HIT_TEST_METRICS*)& metrics));
			pointX = x;
			pointY = y;
			return metrics;
		}

		array<HitTestMetrics>^ TextLayout::HitTestTextRange(Int32 textPosition, Int32 textLength, float originX, float originY)
		{
			UINT32 count;
			HRESULT hr = Native->HitTestTextRange(textPosition, textLength, originX, originY, nullptr, 0, &count);
			if (hr != E_NOT_SUFFICIENT_BUFFER)
				ThrowIfFailed<GraphicsException>(hr);

			array<HitTestMetrics>^ metrics = gcnew array<HitTestMetrics>(count);
			pin_ptr<HitTestMetrics> pMetrics = &metrics[0];

			ThrowIfFailed<GraphicsException>(
				Native->HitTestTextRange(
					textPosition,
					textLength,
					originX,
					originY,
					(DWRITE_HIT_TEST_METRICS*)pMetrics,
					count,
					&count));

			return metrics;
		}

		System::Globalization::CultureInfo^ TextLayout::GetCulture(Int32 position)
		{
			UINT32 len;
			ThrowIfFailed<GraphicsException>(Native->GetLocaleNameLength(position, &len));
			wchar_t* name = new wchar_t[len + 1];
			try
			{
				ThrowIfFailed<GraphicsException>(Native->GetLocaleName(position, name, len, nullptr));
				return gcnew System::Globalization::CultureInfo(gcnew String(name));
			}
			finally
			{
				delete[] name;
			}
		}

		System::Globalization::CultureInfo^ TextLayout::GetCulture(Int32 position, [Out]TextRange% textRange)
		{
			UINT32 len;
			ThrowIfFailed<GraphicsException>(Native->GetLocaleNameLength(position, &len));
			wchar_t* name = new wchar_t[len + 1];
			try
			{
				pin_ptr<TextRange> pTextRange = &textRange;
				ThrowIfFailed<GraphicsException>(Native->GetLocaleName(position, name, len, (DWRITE_TEXT_RANGE*)pTextRange));
				return gcnew System::Globalization::CultureInfo(gcnew String(name));
			}
			finally
			{
				delete[] name;
			}
		}

		Boolean TextLayout::GetStrikethrough(Int32 position)
		{
			BOOL value;
			ThrowIfFailed<GraphicsException>(Native->GetStrikethrough(position, &value, nullptr));
			return value != 0;
		}

		Boolean TextLayout::GetStrikethrough(Int32 position, [Out]TextRange% textRange)
		{
			BOOL value;
			pin_ptr<TextRange> pTextRange = &textRange;
			ThrowIfFailed<GraphicsException>(Native->GetStrikethrough(position, &value, (DWRITE_TEXT_RANGE*)pTextRange));
			return value != 0;
		}

		Boolean TextLayout::GetUnderline(Int32 position)
		{
			BOOL value;
			ThrowIfFailed<GraphicsException>(Native->GetUnderline(position, &value, nullptr));
			return value != 0;
		}

		Boolean TextLayout::GetUnderline(Int32 position, [Out]TextRange% textRange)
		{
			BOOL value;
			pin_ptr<TextRange> pTextRange = &textRange;
			ThrowIfFailed<GraphicsException>(Native->GetUnderline(position, &value, (DWRITE_TEXT_RANGE*)pTextRange));
			return value != 0;
		}

		void TextLayout::SetDrawingEffect(GraphicsObject^ effect, TextRange range)
		{
			ThrowIfFailed<GraphicsException>(Native->SetDrawingEffect(effect->_native, *(DWRITE_TEXT_RANGE*)& range));
		}

		void TextLayout::SetFontFamilyName(String^ fontFamilyName, TextRange textRange)
		{
			pin_ptr<const wchar_t> name = PtrToStringChars(fontFamilyName);
			ThrowIfFailed<GraphicsException>(Native->SetFontFamilyName(name, *(DWRITE_TEXT_RANGE*)& textRange));
		}

		void TextLayout::SetFontSize(float fontSize, TextRange textRange)
		{
			ThrowIfFailed<GraphicsException>(Native->SetFontSize(fontSize, *(DWRITE_TEXT_RANGE*)& textRange));
		}

		void TextLayout::SetFontStretch(Kodo::Graphics::FontStretch fontStretch, TextRange textRange)
		{
			ThrowIfFailed<GraphicsException>(Native->SetFontStretch((DWRITE_FONT_STRETCH)fontStretch, *(DWRITE_TEXT_RANGE*)& textRange));
		}

		void TextLayout::SetFontStyle(Kodo::Graphics::FontStyle fontStyle, TextRange textRange)
		{
			ThrowIfFailed<GraphicsException>(Native->SetFontStyle((DWRITE_FONT_STYLE)fontStyle, *(DWRITE_TEXT_RANGE*)& textRange));
		}

		void TextLayout::SetFontWeight(Kodo::Graphics::FontWeight fontWeight, TextRange textRange)
		{
			ThrowIfFailed<GraphicsException>(Native->SetFontWeight((DWRITE_FONT_WEIGHT)fontWeight, *(DWRITE_TEXT_RANGE*)& textRange));
		}

		void TextLayout::SetCulture(System::Globalization::CultureInfo^ cultureInfo, TextRange textRange)
		{
			pin_ptr<const wchar_t> name = PtrToStringChars(cultureInfo->Name);
			ThrowIfFailed<GraphicsException>(Native->SetLocaleName(name, *(DWRITE_TEXT_RANGE*)& textRange));
		}

		void TextLayout::SetStrikethrough(Boolean hasStrikethrough, TextRange textRange)
		{
			ThrowIfFailed<GraphicsException>(Native->SetStrikethrough(hasStrikethrough, *(DWRITE_TEXT_RANGE*)& textRange));
		}

		void TextLayout::SetUnderline(Boolean hasUnderline, TextRange textRange)
		{
			ThrowIfFailed<GraphicsException>(Native->SetUnderline(hasUnderline, *(DWRITE_TEXT_RANGE*)& textRange));
		}

		//--------------------------------------------------------------------------------------
		// TextFormat

		TextFormat::TextFormat(String^ familyName, Kodo::Graphics::FontWeight weight, Kodo::Graphics::FontStyle style, Kodo::Graphics::FontStretch stretch, float fontSize, String^ localeName)
		{
			pin_ptr<const wchar_t> pinnedText = PtrToStringChars(localeName);
			pin_ptr<const wchar_t> pinnedName = PtrToStringChars(familyName);

			if (GlobalAssets::FactoryDw == nullptr)
			{
				GlobalAssets::FactoryDw = gcnew FactoryDW();
			}

			IDWriteTextFormat* format;
			ThrowIfFailed(GlobalAssets::FactoryDw->Native->CreateTextFormat(pinnedName, nullptr, (DWRITE_FONT_WEIGHT)weight, (DWRITE_FONT_STYLE)style, (DWRITE_FONT_STRETCH)stretch, fontSize, pinnedText, &format));
			Assing(format);

			IDWriteInlineObject* obj;
			ThrowIfFailed(GlobalAssets::FactoryDw->Native->CreateEllipsisTrimmingSign(Native, &obj));
			ellipsis = gcnew InlineObject(obj);

			DWRITE_TRIMMING nativeTrimming;
			nativeTrimming.delimiter = 0;
			nativeTrimming.delimiterCount = 0;
			nativeTrimming.granularity = DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_CHARACTER;
			Native->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
			Native->SetTrimming(&nativeTrimming, ellipsis->Native);
		}

		Kodo::Graphics::FlowDirection TextFormat::FlowDirection::get() { return static_cast<Kodo::Graphics::FlowDirection>(Native->GetFlowDirection()); }
		void TextFormat::FlowDirection::set(Kodo::Graphics::FlowDirection value) { Native->SetFlowDirection(static_cast<DWRITE_FLOW_DIRECTION>(value)); }

		float TextFormat::IncrementalTabStop::get() { return Native->GetIncrementalTabStop(); }

		void TextFormat::IncrementalTabStop::set(float value)
		{
			Native->SetIncrementalTabStop(value);
		}

		void TextFormat::GetLineSpacing([Out] LineSpacingMethod% method, [Out] float% lineSpacing, [Out] float% baseline)
		{
			DWRITE_LINE_SPACING_METHOD nativeMethod;
			float nativeLineSpacing, nativeBaseline;

			ThrowIfFailed<GraphicsException>(Native->GetLineSpacing(&nativeMethod, &nativeLineSpacing, &nativeBaseline));

			method = static_cast<LineSpacingMethod>(nativeMethod);
			lineSpacing = nativeLineSpacing;
			baseline = nativeBaseline;
		}

		void TextFormat::SetLineSpacing(LineSpacingMethod method, float lineSpacing, float baseline)
		{
			ThrowIfFailed<GraphicsException>(Native->SetLineSpacing(static_cast<DWRITE_LINE_SPACING_METHOD>(method), lineSpacing, baseline));
		}

		Kodo::Graphics::ParagraphAlignment TextFormat::ParagraphAlignment::get()
		{
			return static_cast<Kodo::Graphics::ParagraphAlignment>(Native->GetParagraphAlignment());
		}

		void TextFormat::ParagraphAlignment::set(Kodo::Graphics::ParagraphAlignment value)
		{
			Native->SetParagraphAlignment(static_cast<DWRITE_PARAGRAPH_ALIGNMENT>(value));
		}

		Kodo::Graphics::ReadingDirection TextFormat::ReadingDirection::get()
		{
			return static_cast<Kodo::Graphics::ReadingDirection>(Native->GetReadingDirection());
		}

		void TextFormat::ReadingDirection::set(Kodo::Graphics::ReadingDirection value)
		{
			ThrowIfFailed<GraphicsException>(Native->SetReadingDirection(static_cast<DWRITE_READING_DIRECTION>(value)));
		}

		Kodo::Graphics::TextAlignment TextFormat::TextAlignment::get()
		{
			return static_cast<Kodo::Graphics::TextAlignment>(Native->GetTextAlignment());
		}

		void TextFormat::TextAlignment::set(Kodo::Graphics::TextAlignment value)
		{
			Native->SetTextAlignment(static_cast<DWRITE_TEXT_ALIGNMENT>(value));
		}

		static Trimming GetTrimmingInternal(IDWriteTextFormat* format, IDWriteInlineObject** obj)
		{
			DWRITE_TRIMMING trimming;

			if (FAILED(format->GetTrimming(&trimming, obj)))
			{
				DWRITE_TRIMMING zero = { DWRITE_TRIMMING_GRANULARITY(0) };
				trimming = zero;
			}

			return Trimming(static_cast<TrimmingGranularity>(trimming.granularity), trimming.delimiter, trimming.delimiterCount);
		}

		Trimming TextFormat::GetTrimming()
		{
			return GetTrimmingInternal(Native, 0);
		}

		Trimming TextFormat::GetTrimming([Out] InlineObject^% trimmingSign)
		{
			IDWriteInlineObject* obj = 0;
			Trimming result = GetTrimmingInternal(Native, &obj);

			if (obj)
			{
				trimmingSign = gcnew InlineObject(obj);
			}
			return result;
		}

		void TextFormat::SetTrimming(Trimming trimming, InlineObject^ trimmingSign)
		{
			DWRITE_TRIMMING nativeTrimming;
			nativeTrimming.delimiter = trimming.Delimiter;
			nativeTrimming.delimiterCount = trimming.DelimiterCount;
			nativeTrimming.granularity = (DWRITE_TRIMMING_GRANULARITY)trimming.Granularity;

			Native->SetTrimming(&nativeTrimming, trimmingSign->Native);
		}

		void TextFormat::SetTrimming(Trimming trimming)
		{
			SetTrimming(trimming, ellipsis);
		}

		Kodo::Graphics::WordWrapping TextFormat::WordWrapping::get()
		{
			return static_cast<Kodo::Graphics::WordWrapping>(Native->GetWordWrapping());
		}

		void TextFormat::WordWrapping::set(Kodo::Graphics::WordWrapping value)
		{
			ThrowIfFailed<GraphicsException>(Native->SetWordWrapping(static_cast<DWRITE_WORD_WRAPPING>(value)));
		}
	}
}

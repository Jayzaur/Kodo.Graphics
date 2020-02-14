#pragma once

#include "..\Graphics.hpp"

namespace Kodo
{
	namespace Graphics
	{
		using namespace System;
		using namespace System::Runtime::InteropServices;

		public ref class InlineObject : public GraphicsObject
		{
			GRAPHICS_OBJECT(InlineObject, IDWriteInlineObject)
		};

		public enum class BreakCondition : Int32
		{
			Neutral = DWRITE_BREAK_CONDITION_NEUTRAL,
			CanBreak = DWRITE_BREAK_CONDITION_CAN_BREAK,
			MayNotBreak = DWRITE_BREAK_CONDITION_MAY_NOT_BREAK,
			MustBreak = DWRITE_BREAK_CONDITION_MUST_BREAK
		};

		public enum class FontFileType : Int32
		{
			Unknown = DWRITE_FONT_FILE_TYPE_UNKNOWN,
			Cff = DWRITE_FONT_FILE_TYPE_CFF,
			TrueType = DWRITE_FONT_FILE_TYPE_TRUETYPE,
			TrueTypeCollection = DWRITE_FONT_FILE_TYPE_TRUETYPE_COLLECTION,
			Type1Pfm = DWRITE_FONT_FILE_TYPE_TYPE1_PFM,
			Type1Pfb = DWRITE_FONT_FILE_TYPE_TYPE1_PFB,
			Vector = DWRITE_FONT_FILE_TYPE_VECTOR,
			Bitmap = DWRITE_FONT_FILE_TYPE_BITMAP,
		};

		public enum class InformationalStringId : Int32
		{
			None = DWRITE_INFORMATIONAL_STRING_NONE,
			CopyrightNotice = DWRITE_INFORMATIONAL_STRING_COPYRIGHT_NOTICE,
			VersionStrings = DWRITE_INFORMATIONAL_STRING_VERSION_STRINGS,
			Trademark = DWRITE_INFORMATIONAL_STRING_TRADEMARK,
			Manufacturer = DWRITE_INFORMATIONAL_STRING_MANUFACTURER,
			Designer = DWRITE_INFORMATIONAL_STRING_DESIGNER,
			DesignerUrl = DWRITE_INFORMATIONAL_STRING_DESIGNER_URL,
			Description = DWRITE_INFORMATIONAL_STRING_DESCRIPTION,
			FontVendorUrl = DWRITE_INFORMATIONAL_STRING_FONT_VENDOR_URL,
			LicenseDescription = DWRITE_INFORMATIONAL_STRING_LICENSE_DESCRIPTION,
			LicenseInfoUrl = DWRITE_INFORMATIONAL_STRING_LICENSE_INFO_URL,
			Win32FamilyNames = DWRITE_INFORMATIONAL_STRING_WIN32_FAMILY_NAMES,
			Win32SubfamilyNames = DWRITE_INFORMATIONAL_STRING_WIN32_SUBFAMILY_NAMES,
			PreferredFamilyNames = DWRITE_INFORMATIONAL_STRING_PREFERRED_FAMILY_NAMES,
			PreferredSubfamilyNames = DWRITE_INFORMATIONAL_STRING_PREFERRED_SUBFAMILY_NAMES,
			SampleText = DWRITE_INFORMATIONAL_STRING_SAMPLE_TEXT
		};

		public enum class TextAlignment : Int32
		{
			Leading = DWRITE_TEXT_ALIGNMENT_LEADING,
			Trailing = DWRITE_TEXT_ALIGNMENT_TRAILING,
			Center = DWRITE_TEXT_ALIGNMENT_CENTER
		};

		public enum class TextureType : Int32
		{
			Aliased1x1 = DWRITE_TEXTURE_ALIASED_1x1,
			ClearType3x1 = DWRITE_TEXTURE_CLEARTYPE_3x1
		};

		public enum class TrimmingGranularity : Int32
		{
			None = DWRITE_TRIMMING_GRANULARITY_NONE,
			Character = DWRITE_TRIMMING_GRANULARITY_CHARACTER,
			Word = DWRITE_TRIMMING_GRANULARITY_WORD
		};

		public enum class WordWrapping : Int32
		{
			Wrap = DWRITE_WORD_WRAPPING_WRAP,
			NoWrap = DWRITE_WORD_WRAPPING_NO_WRAP
		};

		public enum class ParagraphAlignment : Int32
		{
			Near = DWRITE_PARAGRAPH_ALIGNMENT_NEAR,
			Far = DWRITE_PARAGRAPH_ALIGNMENT_FAR,
			Center = DWRITE_PARAGRAPH_ALIGNMENT_CENTER
		};

		public enum class ReadingDirection : Int32
		{
			LeftToRight = DWRITE_READING_DIRECTION_LEFT_TO_RIGHT,
			RightToLeft = DWRITE_READING_DIRECTION_RIGHT_TO_LEFT,
			TopToBottom = DWRITE_READING_DIRECTION_TOP_TO_BOTTOM,
			BottomToTop = DWRITE_READING_DIRECTION_BOTTOM_TO_TOP
		};

		public enum class LineSpacingMethod : Int32
		{
			Default = DWRITE_LINE_SPACING_METHOD_DEFAULT,
			Uniform = DWRITE_LINE_SPACING_METHOD_UNIFORM
		};

		public enum class MeasuringMode : Int32
		{
			Natural = DWRITE_MEASURING_MODE_NATURAL,
			GdiClassic = DWRITE_MEASURING_MODE_GDI_CLASSIC,
			GdiNatural = DWRITE_MEASURING_MODE_GDI_NATURAL
		};

		public enum class FlowDirection : Int32
		{
			LeftToRight = DWRITE_FLOW_DIRECTION_LEFT_TO_RIGHT,
			RightToLeft = DWRITE_FLOW_DIRECTION_RIGHT_TO_LEFT,
			TopToBottom = DWRITE_FLOW_DIRECTION_TOP_TO_BOTTOM,
			BottomToTop = DWRITE_FLOW_DIRECTION_BOTTOM_TO_TOP
		};

		public enum class FontFaceType : Int32
		{
			Cff = DWRITE_FONT_FACE_TYPE_CFF,
			TrueType = DWRITE_FONT_FACE_TYPE_TRUETYPE,
			TrueTypeCollection = DWRITE_FONT_FACE_TYPE_TRUETYPE_COLLECTION,
			Type1 = DWRITE_FONT_FACE_TYPE_TYPE1,
			Vector = DWRITE_FONT_FACE_TYPE_VECTOR,
			Bitmap = DWRITE_FONT_FACE_TYPE_BITMAP,
			Unknown = DWRITE_FONT_FACE_TYPE_UNKNOWN
		};

		public enum class FontSimulations : Int32
		{
			None = DWRITE_FONT_SIMULATIONS_NONE,
			Bold = DWRITE_FONT_SIMULATIONS_BOLD,
			Oblique = DWRITE_FONT_SIMULATIONS_OBLIQUE,
		};

		public enum class FontStretch : Int32
		{
			Undefined = DWRITE_FONT_STRETCH_UNDEFINED,
			UltraCondensed = DWRITE_FONT_STRETCH_ULTRA_CONDENSED,
			ExtraCondensed = DWRITE_FONT_STRETCH_EXTRA_CONDENSED,
			Condensed = DWRITE_FONT_STRETCH_CONDENSED,
			SemiCondensed = DWRITE_FONT_STRETCH_SEMI_CONDENSED,
			Normal = DWRITE_FONT_STRETCH_NORMAL,
			Medium = DWRITE_FONT_STRETCH_MEDIUM,
			SemiExpanded = DWRITE_FONT_STRETCH_SEMI_EXPANDED,
			Expanded = DWRITE_FONT_STRETCH_EXPANDED,
			ExtraExpanded = DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			UltraExpanded = DWRITE_FONT_STRETCH_ULTRA_EXPANDED
		};

		public enum class FontStyle : Int32
		{
			Normal = DWRITE_FONT_STYLE_NORMAL,
			Oblique = DWRITE_FONT_STYLE_OBLIQUE,
			Italic = DWRITE_FONT_STYLE_ITALIC
		};

		public enum class FontWeight : Int32
		{
			Thin = DWRITE_FONT_WEIGHT_THIN,
			ExtraLight = DWRITE_FONT_WEIGHT_EXTRA_LIGHT,
			UltraLight = DWRITE_FONT_WEIGHT_ULTRA_LIGHT,
			Light = DWRITE_FONT_WEIGHT_LIGHT,
			Normal = DWRITE_FONT_WEIGHT_NORMAL,
			Regular = DWRITE_FONT_WEIGHT_REGULAR,
			Medium = DWRITE_FONT_WEIGHT_MEDIUM,
			DemiBold = DWRITE_FONT_WEIGHT_DEMI_BOLD,
			SemiBold = DWRITE_FONT_WEIGHT_SEMI_BOLD,
			Bold = DWRITE_FONT_WEIGHT_BOLD,
			ExtraBold = DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			UltraBold = DWRITE_FONT_WEIGHT_ULTRA_BOLD,
			Black = DWRITE_FONT_WEIGHT_BLACK,
			Heavy = DWRITE_FONT_WEIGHT_HEAVY,
			ExtraBlack = DWRITE_FONT_WEIGHT_EXTRA_BLACK,
			UltraBlack = DWRITE_FONT_WEIGHT_ULTRA_BLACK
		};

		/// <summary>
		/// Represents the internal structure of a device pixel (that is, the physical arrangement of red, green, and blue color components) that is assumed for purposes of rendering text.
		/// </summary>
		public enum class PixelGeometry : Int32
		{
			/// <summary>
			/// The red, green, and blue color components of each pixel are assumed to occupy the same point.
			/// </summary>
			Flat = DWRITE_PIXEL_GEOMETRY_FLAT,
			/// <summary>
			/// Each pixel is composed of three vertical stripes, with red on the left, green in the center, and blue on the right. This is the most common pixel geometry for LCD monitors.
			/// </summary>
			Rgb = DWRITE_PIXEL_GEOMETRY_RGB,
			/// <summary>
			/// Each pixel is composed of three vertical stripes, with blue on the left, green in the center, and red on the right.
			/// </summary>
			Bgr = DWRITE_PIXEL_GEOMETRY_BGR
		};

		/// <summary>
		/// Represents a method of rendering glyphs.
		/// </summary>
		public enum class RenderingMode : Int32
		{
			/// <summary>
			/// Specifies that the rendering mode is determined automatically, based on the font and size.
			/// </summary>
			Default = DWRITE_RENDERING_MODE_DEFAULT,
			/// <summary>
			/// Specifies that no anti-aliasing is performed. 
			/// Each pixel is either set to the foreground color of the text or retains the color of the background.
			/// </summary>
			Aliased = DWRITE_RENDERING_MODE_ALIASED,
			/// <summary>
			/// Specifies ClearType rendering with the same metrics as bi-level text. 
			/// Glyphs can only be positioned on whole-pixel boundaries.
			/// </summary>
			ClearTypeGdiClassic = DWRITE_RENDERING_MODE_CLEARTYPE_GDI_CLASSIC,
			/// <summary>
			/// Specifies ClearType rendering with the same metrics as text rendering using GDI using a font created with CLEARTYPE_NATURAL_QUALITY. 
			/// Glyph metrics are closer to their ideal values than with bi-level text, but glyphs are still positioned on whole-pixel boundaries.
			/// </summary>
			ClearTypeGdiNatural = DWRITE_RENDERING_MODE_CLEARTYPE_GDI_NATURAL,
			/// <summary>
			/// Specifies ClearType rendering with anti-aliasing in the horizontal dimension only. 
			/// This is typically used with small to medium font sizes (up to 16 ppem).
			/// </summary>
			ClearTypeNatural = DWRITE_RENDERING_MODE_CLEARTYPE_NATURAL,
			/// <summary>
			/// Specifies ClearType rendering with anti-aliasing in both horizontal and vertical dimensions. 
			/// This is typically used at larger sizes to makes curves and diagonal lines look smoother, at the expense of some softness.
			/// </summary>
			ClearTypeNaturalSymmetric = DWRITE_RENDERING_MODE_CLEARTYPE_NATURAL_SYMMETRIC,
			/// <summary>
			/// Specifies that rendering should bypass the rasterizer and use the outlines directly. 
			/// This is typically used at very large sizes.
			/// </summary>
			Outline = DWRITE_RENDERING_MODE_OUTLINE
		};

		/// <summary>
		/// ColorScheme describing the geometric measurement of an
		/// application-defined inline object.
		/// </summary>
		public value class InlineObjectMetrics
		{
		public:
			InlineObjectMetrics(float width, float height, float baseline, bool supportsSideways)
			{
				Width = width;
				Height = height;
				Baseline = baseline;
				SupportsSideways = supportsSideways;
			}

			/// <summary>
			/// Width of the inline object.
			/// </summary>
			property float Width;

			/// <summary>
			/// Height of the inline object as measured from top to bottom.
			/// </summary>
			property float Height;

			/// <summary>
			/// Distance from the top of the object to the baseline where it is lined up with the adjacent text.
			/// If the baseline is at the bottom, baseline simply equals height.
			/// </summary>
			property float Baseline;

			/// <summary>
			/// Flag indicating whether the object is to be placed upright or alongside the text baseline
			/// for vertical text.
			/// </summary>
			property bool SupportsSideways;
		};

		/// <summary>
		/// Indicates how much any visible DIPs (device independent pixels)
		/// overshoot each side of the layout or inline objects.
		///
		/// Positive overhangs indicate that the visible area extends outside
		/// the layout box or inline object, while negative values mean there
		/// is whitespace inside. The returned values are unaffected by
		/// rendering transforms or pixel snapping. Additionally, they may not
		/// exactly match the final target's pixel bounds after applying grid 
		/// fitting and hinting.
		/// </summary>
		public value class OverhangMetrics
		{
		public:
			OverhangMetrics(float left, float top, float right, float bottom)
			{
				Left = left;
				Top = top;
				Right = right;
				Bottom = bottom;
			}

			/// <summary>
			/// The distance from the left-most visible DIP to its left
			/// alignment edge.
			/// </summary>
			property float Left;

			/// <summary>
			/// The distance from the top-most visible DIP to its top
			/// alignment edge.
			/// </summary>
			property float Top;

			/// <summary>
			/// The distance from the right-most visible DIP to its right
			/// alignment edge.
			/// </summary>
			property float Right;

			/// <summary>
			/// The distance from the bottom-most visible DIP to its lower
			/// alignment edge.
			/// </summary>
			property float Bottom;
		};

		[StructLayout(LayoutKind::Sequential)]
		public value struct TextMetrics
		{
		private:
			float  _left;
			float  _top;
			float  _width;
			float  _widthIncludingTrailingWhitespace;
			float  _height;
			float  _layoutWidth;
			float  _layoutHeight;
			UInt32 _maxBidiReorderingDepth;
		public:
			property float Left
			{
				float get() { return _left; }
				void set(float value) { _left = value; }
			}
			property float Top
			{
				float get() { return _top; }
				void set(float value) { _top = value; }
			}
			property float Width
			{
				float get() { return _width; }
				void set(float value) { _width = value; }
			}
			property float WidthIncludingTrailingWhitespace
			{
				float get() { return _widthIncludingTrailingWhitespace; }
				void set(float value) { _widthIncludingTrailingWhitespace = value; }
			}
			property float Height
			{
				float get() { return _height; }
				void set(float value) { _height = value; }
			}
			property float LayoutWidth
			{
				float get() { return _layoutWidth; }
				void set(float value) { _layoutWidth = value; }
			}
			property float LayoutHeight
			{
				float get() { return _layoutHeight; }
				void set(float value) { _layoutHeight = value; }
			}
			property UInt32 MaxBidiReorderingDepth
			{
				UInt32 get() { return _maxBidiReorderingDepth; }
				void set(UInt32 value) { _maxBidiReorderingDepth = value; }
			}
		};

		[StructLayout(LayoutKind::Sequential)]
		public value struct ClusterMetrics
		{
		private:
			float _width;
			UInt16 _length;
			UInt16 _flags;
			static const UInt16 CAN_WRAP_LINE_AFTER = 1;
			static const UInt16 IS_WHITESPACE = 2;
			static const UInt16 IS_NEWLINE = 4;
			static const UInt16 IS_SOFT_HYPHEN = 8;
			static const UInt16 IS_RIGHT_TO_LEFT = 16;
		public:
			property float Width
			{
				float get() { return _width; }
				void set(float value) { _width = value; }
			}
			property UInt16 Length
			{
				UInt16 get() { return _length; }
				void set(UInt16 value) { _length = value; }
			}

			property Boolean CanWrapLineAfter
			{
				Boolean get() { return (_flags & CAN_WRAP_LINE_AFTER) != 0; }
				void set(Boolean value)
				{
					if (value)
						_flags |= CAN_WRAP_LINE_AFTER;
					else
						_flags &= ~CAN_WRAP_LINE_AFTER;
				}
			}

			property Boolean IsWhitespace
			{
				Boolean get() { return (_flags & IS_WHITESPACE) != 0; }
				void set(Boolean value)
				{
					if (value)
						_flags |= IS_WHITESPACE;
					else
						_flags &= ~IS_WHITESPACE;
				}
			}

			property Boolean IsNewline
			{
				Boolean get() { return (_flags & IS_NEWLINE) != 0; }
				void set(Boolean value)
				{
					if (value)
						_flags |= IS_NEWLINE;
					else
						_flags &= ~IS_NEWLINE;
				}
			}

			property Boolean IsSoftHyphen
			{
				Boolean get() { return (_flags & IS_SOFT_HYPHEN) != 0; }
				void set(Boolean value)
				{
					if (value)
						_flags |= IS_SOFT_HYPHEN;
					else
						_flags &= ~IS_SOFT_HYPHEN;
				}
			}

			property Boolean IsRightToLeft
			{
				Boolean get() { return (_flags & IS_RIGHT_TO_LEFT) != 0; }
				void set(Boolean value)
				{
					if (value)
						_flags |= IS_RIGHT_TO_LEFT;
					else
						_flags &= ~IS_RIGHT_TO_LEFT;
				}
			}
		};

		[StructLayout(LayoutKind::Sequential)]
		public value struct LineMetrics
		{
		private:
			UInt32 _length;
			UInt32 _trailingWhitespaceLength;
			UInt32 _newlineLength;
			float  _height;
			float  _baseline;
		public:
			property UInt32 Length
			{
				UInt32 get() { return _length; }
				void set(UInt32 value) { _length = value; }
			}
			property UInt32 TrailingWhitespaceLength
			{
				UInt32 get() { return _trailingWhitespaceLength; }
				void set(UInt32 value) { _trailingWhitespaceLength = value; }
			}
			property UInt32 NewlineLength
			{
				UInt32 get() { return _newlineLength; }
				void set(UInt32 value) { _newlineLength = value; }
			}
			property float Height
			{
				float get() { return _height; }
				void set(float value) { _height = value; }
			}
			property float Baseline
			{
				float get() { return _baseline; }
				void set(float value) { _baseline = value; }
			}
		};

		[StructLayout(LayoutKind::Sequential)]
		public value struct HitTestMetrics
		{
		private:
			UInt32 _textPosition;
			UInt32 _length;
			float  _left;
			float  _top;
			float  _width;
			float  _height;
			UInt32 _bidiLevel;
			Boolean   _isText;
		public:
			property UInt32 TextPosition
			{
				UInt32 get() { return _textPosition; }
			}
			property UInt32 Length
			{
				UInt32 get() { return _length; }
			}
			property float Left
			{
				float get() { return _left; }
			}
			property float Top
			{
				float get() { return _top; }
			}
			property float Width
			{
				float get() { return _width; }
			}
			property float Height
			{
				float get() { return _height; }
			}
			property UInt32 BidiLevel
			{
				UInt32 get() { return _bidiLevel; }
			}
			property Boolean IsText
			{
				Boolean get() { return _isText != 0; }
			}
		};

		public ref class FactoryDW : public GraphicsObject {
			GRAPHICS_OBJECT(FactoryDW, IDWriteFactory2);

		public:
			FactoryDW()
			{
				IDWriteFactory2* factory;
				ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory2), reinterpret_cast<IUnknown * *>(&factory)));
				Assing(factory);
			}
		};

		/// <summary> Specifies the trimming option for text overflowing the layout box. </summary>
		public value class Trimming
		{
		public:

			/// <summary> Text granularity of which trimming applies. </summary>
			property TrimmingGranularity Granularity;

			/// <summary> Character code used as the delimiter signaling the beginning of the portion of text to be preserved, most useful for path ellipsis, where the delimiter would be a slash. </summary>
			property Int32 Delimiter;

			/// <summary> How many occurrences of the delimiter to step back. </summary>
			property Int32 DelimiterCount;

			Trimming(TrimmingGranularity granularity, Int32 delimiter, Int32 delimiterCount)
			{
				Granularity = granularity;
				Delimiter = delimiter;
				DelimiterCount = delimiterCount;
			}
		};

		public ref class TextFormat : public GraphicsObject
		{
			GRAPHICS_OBJECT(TextFormat, IDWriteTextFormat);

		private protected:

			TextFormat() { }

		private:

			InlineObject^ ellipsis;

		public:

			TextFormat(String^ familyName, float fontSize) : TextFormat(familyName, Kodo::Graphics::FontWeight::Normal, Kodo::Graphics::FontStyle::Normal, Kodo::Graphics::FontStretch::Normal, fontSize, "en-US") {}
			TextFormat(String^ familyName, float fontSize, FontWeight weight) : TextFormat(familyName, weight, Kodo::Graphics::FontStyle::Normal, Kodo::Graphics::FontStretch::Normal, fontSize, "en-US") {}
			TextFormat(String^ familyName, FontWeight weight, FontStyle style, FontStretch stretch, float fontSize, String^ localeName);

			property FlowDirection FlowDirection
			{
				Kodo::Graphics::FlowDirection get();
				void set(Kodo::Graphics::FlowDirection value);
			}

			property float IncrementalTabStop
			{
				float get();
				void set(float value);
			}

			void SetAlignment(TextAlignment horizontal, ParagraphAlignment vertical)
			{
				Native->SetTextAlignment((DWRITE_TEXT_ALIGNMENT)horizontal);
				Native->SetParagraphAlignment((DWRITE_PARAGRAPH_ALIGNMENT)vertical);
			}

			void GetLineSpacing([Out] LineSpacingMethod% method, [Out] float% lineSpacing, [Out] float% baseline);
			void SetLineSpacing(LineSpacingMethod method, float lineSpacing, float baseline);

			property ParagraphAlignment ParagraphAlignment
			{
				Kodo::Graphics::ParagraphAlignment get();
				void set(Kodo::Graphics::ParagraphAlignment value);
			}

			property Kodo::Graphics::ReadingDirection ReadingDirection
			{
				Kodo::Graphics::ReadingDirection get();
				void set(Kodo::Graphics::ReadingDirection value);
			}

			property TextAlignment TextAlignment
			{
				Kodo::Graphics::TextAlignment get();
				void set(Kodo::Graphics::TextAlignment value);
			}

			Trimming GetTrimming();
			Trimming GetTrimming([Out] InlineObject^% trimmingSign);
			void SetTrimming(Trimming trimming, InlineObject^ trimmingSign);
			void SetTrimming(Trimming trimming);

			property Kodo::Graphics::WordWrapping WordWrapping
			{
				Kodo::Graphics::WordWrapping get();
				void set(Kodo::Graphics::WordWrapping value);
			}
		};

		/// <summary> 
		/// Represents a range of text. 
		/// </summary>
		[StructLayout(LayoutKind::Sequential)]
		public value struct TextRange
		{
		private:
			UINT32 _startPosition;
			UINT32 _length;

		public:

			/// <summary>
			/// The starting index of the range.
			/// </summary>
			property Int32 Start
			{
				Int32 get() { return _startPosition; }
				void set(Int32 value) { _startPosition = value; }
			}

			/// <summary>
			/// THe length of the range.
			/// </summary>
			property Int32 Length
			{
				Int32 get() { return _length; }
				void set(Int32 value) { _length = value; }
			}

			/// <summary>
			/// Create a new <see cref="Kodo::Graphics::TextRange"/>.
			/// </summary>
			/// <param name="start">The starting index of the range.</param>
			/// <param name="length">The length of the range.</param>
			TextRange(Int32 start, Int32 length)
			{
				_startPosition = start;
				_length = length;
			}
		};

		/// <summary> 
		/// Represents a block of text after it has been fully analyzed and formatted. 
		/// </summary>
		public ref class TextLayout : TextFormat
		{
			GRAPHICS_OBJECT(TextLayout, IDWriteTextLayout);

		public:

			/// <summary>
			/// Create a new <see cref="Kodo::Graphics::TextLayout"/>.
			/// </summary>
			/// <param name="text">The text of the layout.</param>
			/// <param name="textFormat">The <see cref="TextFormat"/> of the layout.</param>
			/// <param name="maxSize">The maximum size of the layout.</param>
			TextLayout(String^ text, TextFormat^ textFormat, Vec2 maxSize) : TextLayout(text, textFormat, maxSize.Width, maxSize.Height) { }
			/// <summary>
			/// Create a new <see cref="Kodo::Graphics::TextLayout"/>.
			/// </summary>
			/// <param name="text">The text of the layout.</param>
			/// <param name="textFormat">The <see cref="TextFormat"/> of the layout.</param>
			/// <param name="maxWidth">The maximum width of the layout.</param>
			/// <param name="maxHeight">The maximum height of the layout.</param>
			TextLayout(String^ text, TextFormat^ textFormat, float maxWidth, float maxHeight);

			property array<Kodo::Graphics::ClusterMetrics>^ ClusterMetrics
			{
				array<Kodo::Graphics::ClusterMetrics>^ get();
			}
			property float MinWidth
			{
				float get()
				{
					FLOAT minWidth;
					ThrowIfFailed<GraphicsException>(GetNative()->DetermineMinWidth(&minWidth));
					return minWidth;
				}
			}

			/// <summary>
			/// Get the maximum width and height of this layout.
			/// </summary>
			Kodo::Graphics::Vec2 GetMaximumSize()
			{
				return Kodo::Graphics::Vec2(GetNative()->GetMaxWidth(), GetNative()->GetMaxHeight());
			}

			/// <summary>
			/// Set the maximum width and height of this layout.
			/// </summary>
			/// <param name="value">The <see cref="Kodo::Graphics::Vec2"/>.</param>
			void SetMaximumSize(Kodo::Graphics::Vec2 value)
			{
				ThrowIfFailed<GraphicsException>(GetNative()->SetMaxWidth(value.Width));
				ThrowIfFailed<GraphicsException>(GetNative()->SetMaxHeight(value.Height));
			}

			String^ GetFontFamilyName(Int32 position);
			String^ GetFontFamilyName(Int32 position, [Out]TextRange% textRange);

			float GetFontSize(Int32 position);
			float GetFontSize(Int32 position, [Out]TextRange% textRange);

			Kodo::Graphics::FontStretch GetFontStretch(Int32 position);
			Kodo::Graphics::FontStretch GetFontStretch(Int32 position, [Out]TextRange% textRange);

			Kodo::Graphics::FontStyle GetFontStyle(Int32 position);
			Kodo::Graphics::FontStyle GetFontStyle(Int32 position, [Out]TextRange% textRange);

			Kodo::Graphics::FontWeight GetFontWeight(Int32 position);
			Kodo::Graphics::FontWeight GetFontWeight(Int32 position, [Out]TextRange% textRange);

			property array<Kodo::Graphics::LineMetrics>^ LineMetrics
			{
				array<Kodo::Graphics::LineMetrics>^ get();
			}

			System::Globalization::CultureInfo^ TextLayout::GetCulture(Int32 position);
			System::Globalization::CultureInfo^ TextLayout::GetCulture(Int32 position, [Out]TextRange% textRange);

			property TextMetrics Metrics
			{
				TextMetrics get()
				{
					TextMetrics value;
					GetNative()->GetMetrics((DWRITE_TEXT_METRICS*)& value);
					return value;
				}
			}
			property Kodo::Graphics::OverhangMetrics OverhangMetrics
			{
				Kodo::Graphics::OverhangMetrics get()
				{
					Kodo::Graphics::OverhangMetrics value;
					ThrowIfFailed<GraphicsException>(GetNative()->GetOverhangMetrics((DWRITE_OVERHANG_METRICS*)& value));
					return value;
				}
			}

			Boolean GetStrikethrough(Int32 position);
			Boolean GetStrikethrough(Int32 position, [Out]TextRange% textRange);

			Boolean GetUnderline(Int32 position);
			Boolean GetUnderline(Int32 position, [Out]TextRange% textRange);

			HitTestMetrics HitTestPoint(float x, float y, [Out]Boolean% isTrailingHit, [Out]Boolean% isInside);
			HitTestMetrics HitTestTextPosition(Int32 textPosition, Boolean isTrailingHit, [Out]float% pointX, [Out]float% pointY);
			array<HitTestMetrics>^ HitTestTextRange(Int32 textPosition, Int32 textLength, float originX, float originY);

			void SetDrawingEffect(GraphicsObject^ effect, TextRange range);

			void SetFontFamilyName(String^ fontFamilyName, TextRange textRange);
			void SetFontSize(float fontSize, TextRange textRange);
			void SetFontStretch(Kodo::Graphics::FontStretch fontStretch, TextRange textRange);
			void SetFontStyle(Kodo::Graphics::FontStyle fontStyle, TextRange textRange);
			void SetFontWeight(Kodo::Graphics::FontWeight fontWeight, TextRange textRange);
			void SetCulture(System::Globalization::CultureInfo^ cultureInfo, TextRange textRange);
			void SetStrikethrough(Boolean hasStrikethrough, TextRange textRange);
			void SetUnderline(Boolean hasUnderline, TextRange textRange);
		internal:

			IDWriteTextLayout* GetNative()
			{
				return (IDWriteTextLayout*)TextFormat::Native;
			}
		};
	}
}
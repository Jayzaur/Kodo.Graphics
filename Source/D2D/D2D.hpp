#pragma once

#include "../Graphics.hpp"

#include "../DW/DW.hpp"

namespace Kodo
{
	namespace Graphics
	{
		using namespace System::Collections::Generic;
		using namespace System::Runtime::InteropServices;

		/// <summary> </summary>
		public ref class Image abstract : GraphicsObject
		{
			GRAPHICS_OBJECT(Image, ID2D1Image);

		protected:

			/// <summary> </summary>			
			Image() { }
		};

		ref class D3DDevice;
		ref class DxgiDevice;
		ref class DxgiSwapChain;
		ref class SolidColorBrush;

		/// <summary> </summary>
		public enum class BitmapInterpolationMode
		{
			/// <summary>
			/// Nearest Neighbor filtering.
			/// Also known as nearest pixel or nearest point sampling.
			/// </summary>
			NearestNeighbor = D2D1_INTERPOLATION_MODE_DEFINITION_NEAREST_NEIGHBOR,
			/// <summary>
			/// Linear filtering.
			/// </summary>
			Linear
		};

		/// <summary>
		/// Specifies whether text snapping is suppressed or clipping to the layout rectangle is enabled. This enumeration allows a bitwise combination of its member values.
		/// </summary>
		public enum class DrawTextOptions : Int32
		{
			/// <summary>
			/// Text is not vertically snapped to pixel boundaries. This setting is recommended for text that is being animated.
			/// </summary>
			None = D2D1_DRAW_TEXT_OPTIONS_NONE,
			/// <summary>
			/// Text is clipped to the layout rectangle.
			/// </summary>
			NoSnap = D2D1_DRAW_TEXT_OPTIONS_NO_SNAP,
			/// <summary>
			/// Text is vertically snapped to pixel boundaries and is not clipped to the layout rectangle.
			/// </summary>
			Clip = D2D1_DRAW_TEXT_OPTIONS_CLIP
		};

		/// <summary>
		/// Specifies how a brush paints areas outside of its normal content area.
		/// </summary>
		public enum class ExtendMode : Int32
		{
			/// <summary>
			/// Repeat the edge pixels of the brush's content for all regions outside the normal content area.
			/// </summary>
			Clamp = D2D1_EXTEND_MODE_CLAMP,
			/// <summary>
			/// Repeat the brush's content.
			/// </summary>
			Wrap = D2D1_EXTEND_MODE_WRAP,
			/// <summary>
			/// The same as ExtendMode.Wrap, except that alternate tiles of the brush's content are flipped. (The brush's normal content is drawn untransformed.)
			/// </summary>
			Mirror = D2D1_EXTEND_MODE_MIRROR
		};

		/// <summary>
		/// Specifies which gamma is used for interpolation.
		/// </summary>
		public enum class Gamma : Int32
		{
			/// <summary>
			/// Interpolation is performed in the standard RGB (sRGB) gamma.
			/// </summary>
			StandardRgb = D2D1_GAMMA_2_2,
			/// <summary>
			/// Interpolation is performed in the linear-gamma color space.
			/// </summary>
			Linear = D2D1_GAMMA_1_0
		};

		/// <summary>
		/// Specifies options that can be applied when a layer resource is applied to create a layer.
		/// </summary>
		public enum class LayerOptions : Int32
		{
			/// <summary>
			/// The text in this layer does not use ClearType antialiasing.
			/// </summary>
			None = D2D1_LAYER_OPTIONS_NONE,
			/// <summary>
			/// The layer renders correctly for ClearType text. 
			/// If the render target is set to ClearType, the layer continues to render ClearType. 
			/// If the render target is set to ClearType and this option is not specified, the render target will be set to render gray-scale until the layer is popped. 
			/// The caller can override this default by calling SetTextAntialiasMode while within the layer. 
			/// This flag is slightly slower than the default.
			/// </summary>
			InitializeForClearType = D2D1_LAYER_OPTIONS_INITIALIZE_FOR_CLEARTYPE
		};

		/// <summary>
		/// Describes whether an opacity mask contains graphics or text. 
		/// Direct2D uses this information to determine which gamma space to use when blending the opacity mask.
		/// </summary>
		public enum class OpacityMaskContent : Int32
		{
			/// <summary>
			/// The opacity mask contains graphics. The opacity mask is blended in the gamma 2.2 color space.
			/// </summary>
			Graphics = D2D1_OPACITY_MASK_CONTENT_GRAPHICS,
			/// <summary>
			/// The opacity mask contains non-GDI text. The gamma space used for blending is obtained from the render target's text rendering parameters. 
			/// </summary>
			NaturalText = D2D1_OPACITY_MASK_CONTENT_TEXT_NATURAL,
			/// <summary>
			/// The opacity mask contains text rendered using the GDI-compatible rendering mode. The opacity mask is blended using the gamma for GDI rendering.
			/// </summary>
			GdiCompatibleText = D2D1_OPACITY_MASK_CONTENT_TEXT_GDI_COMPATIBLE
		};

		/// <summary>
		/// Specifies the algorithm that is used when images are scaled or rotated.
		/// </summary>
		public enum class InterpolationMode : Int32
		{
			/// <summary>
			/// Use the exact color of the nearest bitmap pixel to the current rendering pixel.
			/// </summary>
			NearestNeighbor = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
			/// <summary>
			/// Interpolate a color from the four bitmap pixels that are the nearest to the rendering pixel.
			/// </summary>
			Linear = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
		};

		/// <summary>
		/// Specifies how the edges of nontext primitives are rendered.
		/// </summary>
		public enum class AntialiasMode : Int32
		{
			/// <summary>
			/// Edges are antialiased using the Direct2D per-primitive method of high-quality antialiasing.
			/// </summary>
			PerPrimitive = D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
			/// <summary>
			/// Objects are aliased in most cases. 
			/// </summary>
			Aliased = D2D1_ANTIALIAS_MODE_ALIASED
		};

		/// <summary>
		/// Describes the antialiasing mode used for drawing text.
		/// </summary>
		public enum class TextAntialiasMode : Int32
		{
			/// <summary>
			/// Use the system default.
			/// </summary>
			Default = D2D1_TEXT_ANTIALIAS_MODE_DEFAULT,
			/// <summary>
			/// Use ClearType antialiasing.
			/// </summary>
			ClearType = D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE,
			/// <summary>
			/// Use grayscale antialiasing.
			/// </summary>
			Grayscale = D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE,
			/// <summary>
			/// Do not use antialiasing.
			/// </summary>
			Aliased = D2D1_TEXT_ANTIALIAS_MODE_ALIASED
		};

		/// <summary>
		/// Describes the minimum DirectX support required for hardware rendering by a render target.
		/// </summary>
		public enum class FeatureLevel : Int32
		{
			/// <summary>
			/// Direct2D determines whether the video card provides adequate hardware rendering support.
			/// </summary>
			Default = D2D1_FEATURE_LEVEL_DEFAULT,
			/// <summary>
			/// The video card must support DirectX 9.
			/// </summary>
			Direct3D9 = D2D1_FEATURE_LEVEL_9,
			/// <summary>
			/// The video card must support DirectX 10.
			/// </summary>
			Direct3D10 = D2D1_FEATURE_LEVEL_10
		};

		/// <summary>
		/// Specifies how the alpha value of a bitmap or render target should be treated.
		/// </summary>
		public enum class AlphaMode : Int32
		{
			/// <summary>
			/// The alpha value might not be meaningful.
			/// </summary>
			Unknown = D2D1_ALPHA_MODE_UNKNOWN,
			/// <summary>
			/// The alpha value has been premultiplied. 
			/// Each color is first scaled by the alpha value. 
			/// The alpha value itself is the same in both straight and premultiplied alpha. 
			/// Typically, no color channel value is greater than the alpha channel value.
			/// If a color channel value in a premultiplied format is greater than the alpha channel, the standard source-over blending math results in an additive blend.
			/// </summary>
			Premultiplied = D2D1_ALPHA_MODE_PREMULTIPLIED,
			/// <summary>
			/// The alpha value has not been premultiplied. The alpha channel indicates the transparency of the color.
			/// </summary>
			Straight = D2D1_ALPHA_MODE_STRAIGHT,
			/// <summary>
			/// The alpha value is ignored.
			/// </summary>
			Ignore = D2D1_ALPHA_MODE_IGNORE
		};

		/// <summary>
		/// Describes how a render target behaves when it presents its content. This enumeration allows a bitwise combination of its member values.
		/// </summary>
		public enum class PresentOptions : Int32
		{
			/// <summary>
			/// The render target waits until the display refreshes to present and discards the streamCurrentFrame upon presenting.
			/// </summary>
			None = D2D1_PRESENT_OPTIONS_NONE,
			/// <summary>
			/// The render target does not discard the streamCurrentFrame upon presenting.
			/// </summary>
			RetainContents = D2D1_PRESENT_OPTIONS_RETAIN_CONTENTS,
			/// <summary>
			/// The render target does not wait until the display refreshes to present.
			/// </summary>
			Immediately = D2D1_PRESENT_OPTIONS_IMMEDIATELY
		};

		/// <summary>
		/// Describes whether a render target uses hardware or software rendering, or if Direct2D should select the rendering mode.
		/// </summary>
		public enum class RenderTargetType : Int32
		{
			/// <summary>
			/// The render target uses hardware rendering, if available; otherwise, it uses software rendering.
			/// </summary>
			Default = D2D1_RENDER_TARGET_TYPE_DEFAULT,
			/// <summary>
			/// The render target uses software rendering only.
			/// </summary>
			Software = D2D1_RENDER_TARGET_TYPE_SOFTWARE,
			/// <summary>
			/// The render target uses hardware rendering only.
			/// </summary>
			Hardware = D2D1_RENDER_TARGET_TYPE_HARDWARE
		};

		/// <summary>
		/// Describes how a render target is remoted and whether it should be GDI-compatible. This enumeration allows a bitwise combination of its member values.
		/// </summary>
		public enum class RenderTargetUsage : Int32
		{
			/// <summary>
			/// The render target attempts to use Direct3D command-stream remoting and uses bitmap remoting if stream remoting fails. The render target is not GDI-compatible.
			/// </summary>
			None = D2D1_RENDER_TARGET_USAGE_NONE,
			/// <summary>
			/// The render target renders content locally and sends it to the terminal services client as a bitmap.
			/// </summary>
			ForceBitmapRemoting = D2D1_RENDER_TARGET_USAGE_FORCE_BITMAP_REMOTING,
			/// <summary>
			/// The render target can be used efficiently with GDI.
			/// </summary>
			GdiCompatible = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE
		};

		/// <summary>
		/// Describes the sequence of dashes and gaps in a stroke.
		/// </summary>
		public enum class DashStyle : Int32
		{
			/// <summary>
			/// A solid line with no breaks.
			/// </summary>
			Solid = D2D1_DASH_STYLE_SOLID,
			/// <summary>
			/// A dash followed by a gap of equal length. The dash and the gap are each twice as long as the stroke thickness.
			/// </summary>
			Dash = D2D1_DASH_STYLE_DASH,
			/// <summary>
			/// A dot followed by a longer gap.
			/// </summary>
			Dot = D2D1_DASH_STYLE_DOT,
			/// <summary>
			/// A dash, followed by a gap, followed by a dot, followed by another gap.
			/// </summary>
			DashDot = D2D1_DASH_STYLE_DASH_DOT,
			/// <summary>
			/// A dash, followed by a gap, followed by a dot, followed by another gap, followed by another dot, followed by another gap.
			/// </summary>
			DashDotDot = D2D1_DASH_STYLE_DASH_DOT_DOT,
			/// <summary>
			/// The dash pattern is specified by an array of floating-point values.
			/// </summary>
			Custom = D2D1_DASH_STYLE_CUSTOM
		};

		/// <summary>
		/// Describes the shape that joins two lines or segments.
		/// </summary>
		public enum class LineJoin : Int32
		{
			/// <summary>
			/// Regular angular vertices.
			/// </summary>
			Miter = D2D1_LINE_JOIN_MITER,
			/// <summary>
			/// Beveled vertices.
			/// </summary>
			Bevel = D2D1_LINE_JOIN_BEVEL,
			/// <summary>
			/// Rounded vertices.
			/// </summary>
			Round = D2D1_LINE_JOIN_ROUND,
			/// <summary>
			/// Regular angular vertices unless the join would extend beyond the miter limit; otherwise, beveled vertices.
			/// </summary>
			MiterOrBevel = D2D1_LINE_JOIN_MITER_OR_BEVEL
		};

		/// <summary>
		/// Describes the shape at the end of a line or segment.
		/// </summary>
		public enum class CapStyle : Int32
		{
			/// <summary>
			/// A cap that does not extend past the last point of the line. Comparable to cap used for objects other than lines.
			/// </summary>
			Flat = D2D1_CAP_STYLE_FLAT,
			/// <summary>
			/// Half of a square that has a length equal to the line thickness.
			/// </summary>
			Square = D2D1_CAP_STYLE_SQUARE,
			/// <summary>
			/// A semicircle that has a diameter equal to the line thickness.
			/// </summary>
			Round = D2D1_CAP_STYLE_ROUND,
			/// <summary>
			/// An isosceles right triangle whose hypotenuse is equal in length to the thickness of the line.
			/// </summary>
			Triangle = D2D1_CAP_STYLE_TRIANGLE
		};

		/// <summary>
		/// Specifies the different methods by which two geometries can be combined.
		/// </summary>
		public enum class CombineMode : Int32
		{
			/// <summary>
			/// The two regions are combined by taking the union of both. Given two geometries, A and B, the resulting geometry is geometry A + geometry B.
			/// </summary>
			Union = D2D1_COMBINE_MODE_UNION,
			/// <summary>
			/// The two regions are combined by taking their intersection. The new area consists of the overlapping region between the two geometries.
			/// </summary>
			Intersect = D2D1_COMBINE_MODE_INTERSECT,
			/// <summary>
			/// The two regions are combined by taking the area that exists in the first region but not the second and the area that exists in the second region but not the first. Given two geometries, A and B, the new region consists of (A-B) + (B-A).
			/// </summary>
			Xor = D2D1_COMBINE_MODE_XOR,
			/// <summary>
			/// The second region is excluded from the first. Given two geometries, A and B, the area of geometry B is removed from the area of geometry A, producing a region that is A-B.
			/// </summary>
			Exclude = D2D1_COMBINE_MODE_EXCLUDE
		};

		/// <summary>
		/// Describes how one geometry object is spatially related to another geometry object.
		/// </summary>
		public enum class GeometryRelation : Int32
		{
			/// <summary>
			/// The relationship between the two geometries cannot be determined. This value is never returned by any D2D method.
			/// </summary>
			Unknown = D2D1_GEOMETRY_RELATION_UNKNOWN,
			/// <summary>
			/// The two geometries do not intersect at all.
			/// </summary>
			Disjoint = D2D1_GEOMETRY_RELATION_DISJOINT,
			/// <summary>
			/// The instance geometry is entirely contained by the passed-in geometry.
			/// </summary>
			IsContained = D2D1_GEOMETRY_RELATION_IS_CONTAINED,
			/// <summary>
			/// The instance geometry entirely contains the passed-in geometry.
			/// </summary>
			Contains = D2D1_GEOMETRY_RELATION_CONTAINS,
			/// <summary>
			/// The two geometries overlap but neither completely contains the other.
			/// </summary>
			Overlap = D2D1_GEOMETRY_RELATION_OVERLAP
		};

		/// <summary>
		///Specifies how a geometry is simplified to an SimplifiedGeometrySink.
		/// </summary>
		public enum class GeometrySimplificationOption : Int32
		{
			/// <summary>
			/// The output can contain cubic Bezier curves and line segments.
			/// </summary>
			CubicsAndLines = D2D1_GEOMETRY_SIMPLIFICATION_OPTION_CUBICS_AND_LINES,
			/// <summary>
			/// The output is flattened so that it contains only line segments.
			/// </summary>
			Lines = D2D1_GEOMETRY_SIMPLIFICATION_OPTION_LINES
		};

		/// <summary>
		/// Defines the direction that an elliptical arc is drawn.
		/// </summary>
		public enum class SweepDirection : Int32
		{
			/// <summary>
			/// Arcs are drawn in a counterclockwise (negative-angle) direction.
			/// </summary>
			CounterClockwise = 0, //D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,
			/// <summary>
			/// Arcs are drawn in a clockwise (positive-angle) direction.
			/// </summary>
			Clockwise = 1 //D2D1_SWEEP_DIRECTION_CLOCKWISE
		};

		/// <summary>
		/// Specifies whether an arc should be greater than 180 degrees.
		/// </summary>
		public enum class ArcSize : Int32
		{
			/// <summary>
			/// An arc's sweep should be 180 degrees or less.
			/// </summary>
			Small = D2D1_ARC_SIZE_SMALL,
			/// <summary>
			/// An arc's sweep should be 180 degrees or greater.
			/// </summary>
			Large = D2D1_ARC_SIZE_LARGE
		};

		/// <summary>
		/// Specifies how the intersecting areas of geometries are combined to form the area of the composite geometry. 
		/// </summary>
		public enum class FillMode : Int32
		{
			/// <summary>
			/// Determines whether a point is in the fill region by drawing a ray from that point to infinity 
			/// in any direction, and then counting the number of path segments within the given shape 
			/// that the ray crosses. If this number is odd, the point is in the fill region; 
			/// if even, the point is outside the fill region.
			/// </summary>
			Alternate = D2D1_FILL_MODE_ALTERNATE,
			/// <summary>
			/// Determines whether a point is in the fill region of the path by drawing a ray from that point to infinity in any direction, and then examining the places where a segment of the shape crosses the ray. 
			/// Starting with a count of zero, add one each time a segment crosses the ray from left to right and subtract one each time a path segment crosses the ray from right to left, as long as left and right are seen from the perspective of the ray. 
			/// After counting the crossings, if the result is zero, then the point is outside the path. 
			/// Otherwise, it is inside the path.
			/// </summary>   
			Winding = D2D1_FILL_MODE_WINDING
		};

		/// <summary>
		/// Indicates whether a segment should be stroked and whether the join between this segment and the previous one should be smooth. 
		/// This enumeration allows a bitwise combination of its member values.
		/// </summary>
		public enum class PathSegment : Int32
		{
			/// <summary>
			/// The segment is joined as specified by the StrokeStyle, and it is stroked.
			/// </summary>
			None = D2D1_PATH_SEGMENT_NONE,
			/// <summary>
			/// The segment is not stroked.
			/// </summary>
			ForceUnstroked = D2D1_PATH_SEGMENT_FORCE_UNSTROKED,
			/// <summary>
			/// The segment is always joined with the one preceding it using a round line join, regardless of which LineJoin is specified by the StrokeStyle. 
			/// If this segment is the first segment and the figure is closed, a round line join is used to connect the closing segment with the first segment. 
			/// If the figure is not closed, this setting has no effect on the first segment of the figure. 
			/// If SimplifiedGeometrySink::SetSegmentFlags is called just before SimplifiedGeometrySink::EndFigure, the join between the closing segment and the last explicitly specified segment is affected.
			/// </summary>
			ForceRoundLineJoin = D2D1_PATH_SEGMENT_FORCE_ROUND_LINE_JOIN
		};

		/// <summary>
		/// Indicates whether a specific GeometrySink figure is filled or hollow.
		/// </summary>
		public enum class FigureBegin : Int32
		{
			/// <summary>
			/// The figure is filled.
			/// </summary>
			Filled = D2D1_FIGURE_BEGIN_FILLED,
			/// <summary>
			/// The figure is hollow.
			/// </summary>
			Hollow = D2D1_FIGURE_BEGIN_HOLLOW
		};

		/// <summary>
		/// Indicates whether a specific GeometrySink figure is open or closed.
		/// </summary>
		public enum class FigureEnd : Int32
		{
			/// <summary>
			/// The figure is open.
			/// </summary>
			Open = D2D1_FIGURE_END_OPEN,
			/// <summary>
			/// The figure is closed.
			/// </summary>
			Closed = D2D1_FIGURE_END_CLOSED
		};

		/// <summary>
		/// Indicates the type of information provided by the Direct2D Debug Layer.
		/// </summary>
		public enum class DebugLevel : Int32
		{
			/// <summary>
			/// Direct2D does not produce any debugging output.
			/// </summary>
			None = D2D1_DEBUG_LEVEL_NONE,
			/// <summary>
			/// Direct2D sends error messages to the debug layer.
			/// </summary>
			Error = D2D1_DEBUG_LEVEL_ERROR,
			/// <summary>
			/// Direct2D sends error messages and warnings to the debug layer.
			/// </summary>
			Warning = D2D1_DEBUG_LEVEL_WARNING,
			/// <summary>
			/// Direct2D sends error messages, warnings, and additional diagnostic information that can help improve performance to the debug layer.
			/// </summary>
			Information = D2D1_DEBUG_LEVEL_INFORMATION
		};

		/// <summary>
		/// Contains the center point, x-radius, and y-radius of an ellipse.
		/// </summary>
		public value class Ellipse
		{
		private:

			Vec2 center;
			float radiusX;
			float radiusY;

		public:

			/// <summary>
			/// The center point of the ellipse.
			/// </summary>
			property Vec2 Center { Vec2 get() { return center; } void set(Vec2 value) { center = value; } }
			/// <summary>
			/// The X-radius of the ellipse.
			/// </summary>
			property float RadiusX { float get() { return radiusX; } void set(float value) { radiusX = value; } }
			/// <summary>
			/// The Y-radius of the ellipse.
			/// </summary>
			property float RadiusY { float get() { return radiusY; } void set(float value) { radiusY = value; } }

			Ellipse(Rectangle r) : center(r.Center), radiusX(r.Width), radiusY(r.Height) {}
			Ellipse(Vec2 center, float r) : center(center), radiusX(r), radiusY(r) {}
			Ellipse(Vec2 center, float rx, float ry) : center(center), radiusX(rx), radiusY(ry) {}
		};

		/// <summary>
		/// Contains the dimensions and corner radii of a rounded rectangle.
		/// </summary>
		[StructLayout(LayoutKind::Sequential)]
		public value class RoundedRectangle
		{
		public:
			/// <summary>
			/// The coordinates of the rectangle.
			/// </summary>
			property Rectangle Rectangle;
			/// <summary>
			/// The x-radius for the quarter ellipse that is drawn to replace every corner of the rectangle.
			/// </summary>
			property float RadiusX;
			/// <summary>
			/// The y-radius for the quarter ellipse that is drawn to replace every corner of the rectangle.
			/// </summary>
			property float RadiusY;
		};

		/// <summary>
		/// Contains the three vertices that describe a triangle.
		/// </summary>
		public value struct Triangle
		{
		public:

			/// <summary>
			/// The first vertex of a triangle.
			/// </summary>
			property Vec2 Point1;
			/// <summary>
			/// The second vertex of a triangle.
			/// </summary>
			property Vec2 Point2;
			/// <summary>
			/// The third vertex of a triangle.
			/// </summary>
			property Vec2 Point3;

			/// <summary>
			/// Create a new instance of Triangle.
			/// </summary>
			/// <param name="point1">The first vertex of a triangle.</param>
			/// <param name="point2">The second vertex of a triangle.</param>
			/// <param name="point3">The third vertex of a triangle.</param>
			Triangle(Vec2 point1, Vec2 point2, Vec2 point3)
			{
				Point1 = point1;
				Point2 = point2;
				Point3 = point3;
			}
		};

		/// <summary>
		/// Represents a cubic bezier segment drawn between two points.
		/// </summary>
		[StructLayout(LayoutKind::Sequential)]
		public value struct BezierSegment
		{
		private:

			Vec2 _point1;
			Vec2 _point2;
			Vec2 _point3;

		public:

			/// <summary>
			/// Create a new instance of BezierSegment.
			/// </summary>
			BezierSegment(Vec2 point1, Vec2 point2, Vec2 point3)
			{
				_point1 = point1;
				_point2 = point2;
				_point3 = point3;
			}

			/// <summary>
			/// The first control point for the Bezier segment.
			/// </summary>
			property Vec2 Point1 { Vec2 get() { return _point1; } }

			/// <summary>
			/// The second control point for the Bezier segment.
			/// </summary>
			property Vec2 Point2 { Vec2 get() { return _point2; } }

			/// <summary>
			/// The end point for the Bezier segment.
			/// </summary>
			property Vec2 Point3 { Vec2 get() { return _point3; } }
		};

		/// <summary>
		/// Describes an elliptical arc between two points.
		/// </summary>
		[StructLayout(LayoutKind::Sequential)]
		public value struct ArcSegment
		{
		private:

			Vec2        _point;
			Vec2         _size;
			float         _rotationAngle;
			D2D1_SWEEP_DIRECTION _sweepDirection;
			D2D1_ARC_SIZE        _arcSize;

		public:

			/// <summary>
			/// Create a new instance of ArcSegment.
			/// </summary>
			ArcSegment(Vec2 point, Vec2 size, float rotationAngle, SweepDirection sweepDirection, ArcSize arcSize)
			{
				_point = point;
				_size = size;
				_rotationAngle = rotationAngle;
				_sweepDirection = (D2D1_SWEEP_DIRECTION)sweepDirection;
				_arcSize = (D2D1_ARC_SIZE)arcSize;
			}

			/// <summary>
			/// The end point of the arc.
			/// </summary>
			property Kodo::Graphics::Vec2 Point { Kodo::Graphics::Vec2 get() { return _point; } }

			/// <summary>
			/// The x-radius and y-radius of the arc.
			/// </summary>
			property Kodo::Graphics::Vec2 Size { Kodo::Graphics::Vec2 get() { return _size; } }

			/// <summary>
			/// A value that specifies how many degrees in the clockwise direction the ellipse is rotated relative to the current coordinate system.
			/// </summary>
			property float RotationAngle { float get() { return _rotationAngle; } }

			/// <summary>
			/// A value that specifies whether the arc sweep is clockwise or counterclockwise.
			/// </summary>
			property Kodo::Graphics::SweepDirection SweepDirection { Kodo::Graphics::SweepDirection get() { return (Kodo::Graphics::SweepDirection)_sweepDirection; } }

			/// <summary>
			/// A value that specifies whether the given arc is larger than 180 degrees.
			/// </summary>
			property Kodo::Graphics::ArcSize ArcSize { Kodo::Graphics::ArcSize get() { return (Kodo::Graphics::ArcSize)_arcSize; } }
		};

		/// <summary>
		/// Contains the control point and end point for a quadratic Bezier segment.
		/// </summary>
		[StructLayout(LayoutKind::Sequential)]
		public value struct QuadraticBezierSegment
		{
		private:

			Vec2 _point1;
			Vec2 _point2;

		public:

			/// <summary>
			/// Create a new instance of QuadraticBezierSegment.
			/// </summary>
			QuadraticBezierSegment(Vec2 point1, Vec2 point2)
			{
				_point1 = point1;
				_point2 = point2;
			}

			/// <summary>
			/// The control point of the quadratic Bezier segment.
			/// </summary>
			property Vec2 Point1 { Vec2 get() { return _point1; } }

			/// <summary>
			/// The end point of the quadratic Bezier segment.
			/// </summary>
			property Vec2 Point2 { Vec2 get() { return _point2; } }
		};

		/// <summary>
		/// Contains the data format and alpha mode for a bitmap or render target.
		/// </summary>
		[StructLayout(LayoutKind::Sequential)]
		public value class PixelFormat
		{
		public:

			/// <summary>
			/// A value that specifies the size and arrangement of channels in each pixel.
			/// </summary>
			property Kodo::Graphics::DxgiFormat Format;
			/// <summary>
			/// A value that specifies whether the alpha channel is using pre-multiplied alpha, straight alpha, whether it should be ignored and considered opaque, or whether it is unknown.
			/// </summary>
			property Kodo::Graphics::AlphaMode AlphaMode;

			/// <summary>
			/// Create new instance of PixelFormat.
			/// </summary>
			PixelFormat(Kodo::Graphics::DxgiFormat format, Kodo::Graphics::AlphaMode alphaMode)
			{
				Format = format;
				AlphaMode = alphaMode;
			}
		};

		/// <summary> Describes the caps, miter limit, line join, and dash information for a stroke. </summary>
		public ref class StrokeStyle : GraphicsObject
		{
			GRAPHICS_OBJECT(StrokeStyle, ID2D1StrokeStyle);

		public:

			/// <summary> Create a new instance of StrokeStyle. </summary>
			/// <param name="startCap">The cap applied to the start of all the open figures in a stroked geometry.</param>
			/// <param name="endCap">The cap applied to the end of all the open figures in a stroked geometry.</param>
			/// <param name="dashCap">The shape at either end of each dash segment.</param>
			/// <param name="lineJoin">A value that describes how segments are joined. This value is ignored for a vertex if the segment flags specify that the segment should have a smooth join.</param>
			/// <param name="miterLimit">The limit of the thickness of the join on a mitered corner. This value is always treated as though it is greater than or equal to 1.0f.</param>
			StrokeStyle(CapStyle startCap, CapStyle endCap, CapStyle dashCap, Kodo::Graphics::LineJoin lineJoin, float miterLimit) : StrokeStyle(startCap, endCap, dashCap, lineJoin, miterLimit, Kodo::Graphics::DashStyle::Solid, 0.0f, nullptr) {}
			/// <summary> Create a new instance of StrokeStyle. </summary>
			/// <param name="startCap">The cap applied to the start of all the open figures in a stroked geometry.</param>
			/// <param name="endCap">The cap applied to the end of all the open figures in a stroked geometry.</param>
			/// <param name="dashCap">The shape at either end of each dash segment.</param>
			/// <param name="lineJoin">A value that describes how segments are joined. This value is ignored for a vertex if the segment flags specify that the segment should have a smooth join.</param>
			/// <param name="miterLimit">The limit of the thickness of the join on a mitered corner. This value is always treated as though it is greater than or equal to 1.0f.</param>
			/// <param name="dashStyle">A value that specifies whether the stroke has a dash pattern and, if so, the dash style.</param>
			/// <param name="dashOffset">A value that specifies an offset in the dash sequence. A positive dash offset value shifts the dash pattern, in units of stroke width, toward the start of the stroked geometry. A negative dash offset value shifts the dash pattern, in units of stroke width, toward the end of the stroked geometry.</param>
			StrokeStyle(CapStyle startCap, CapStyle endCap, CapStyle dashCap, Kodo::Graphics::LineJoin lineJoin, float miterLimit, Kodo::Graphics::DashStyle dashStyle, float dashOffset, array<float>^ dashes);
		};

		public ref class D2DFactory : GraphicsObject
		{
			GRAPHICS_OBJECT(D2DFactory, ID2D1Factory2);

		public:

		};

		ref class D2DDeviceContext;
		ref class DxgiSwapChain;
		ref class Brush;
		ref class Bitmap;
		ref class Geometry;
		ref class TextFormat;
		ref class TextLayout;
		ref class SimplifiedGeometrySink;

		namespace Imaging
		{
			ref class WicBitmapSource;
		}

		/// <summary> Bitmap options. </summary>
		public enum class BitmapOptions
		{
			/// <summary> The bitmap is created with default properties. </summary>
			None = 0x00000000,
			/// <summary> The bitmap can be specified as a drawing target. </summary>
			Target = 0x00000001,
			/// <summary> The bitmap cannot be used as an input to DrawBitmap, DrawImage, in a bitmap brush or as an input to an effect. </summary>
			CannotDraw = 0x00000002,
			/// <summary> The bitmap can be read by the CPU. </summary>
			CPURead = 0x00000004,
			/// <summary> The bitmap works with the ID2D1GdiInteropRenderTarget::GetDC API. </summary>
			GDICompatible = 0x00000008
		};

		public value struct BitmapProperties
		{
			property Kodo::Graphics::PixelFormat PixelFormat;
			property float DPIX;
			property float DPIY;
			property Kodo::Graphics::BitmapOptions BitmapOptions;

			BitmapProperties(Kodo::Graphics::BitmapOptions options, float dpi, Kodo::Graphics::PixelFormat pixelFormat)
			{
				BitmapOptions = options;
				DPIX = dpi;
				DPIY = dpi;
				PixelFormat = pixelFormat;
			}
		};

		public ref class Bitmap : Image
		{
			GRAPHICS_OBJECT(Bitmap, ID2D1Bitmap1);

		public:

			Bitmap(D2DDeviceContext^ context, DxgiSwapChain^ swapChain);
			Bitmap(D2DDeviceContext^ context, Vec2 size);
			Bitmap(D2DDeviceContext^ context, Vec2 size, BitmapProperties properties);
			Bitmap(D2DDeviceContext^ context, Vec2 size, Kodo::Graphics::PixelFormat pixelFormat, Kodo::Graphics::BitmapOptions options, float dpi);
			Bitmap(D2DDeviceContext^ context, Imaging::WicBitmapSource^ source);
			Bitmap(D2DDeviceContext^ context, Imaging::WicBitmapSource^ source, BitmapProperties properties);
		};

		/// <summary> Defines an object that paints an area. </summary>
		public ref class Brush abstract : GraphicsObject
		{
			GRAPHICS_OBJECT(Brush, ID2D1Brush)

		protected:

			/// <summary> </summary>
			Brush() { }

		public:

			/// <summary> Get or set the degree of opacity of this brush. </summary>
			property float Opacity
			{
				float get() { return Native->GetOpacity(); }
				void set(float value) { Native->SetOpacity(value); }
			}

			/// <summary> Get or set the transform applied to this brush. </summary>
			property Mat3x2 Transform
			{
				Mat3x2 get() {
					Mat3x2 result;
					Native->GetTransform(reinterpret_cast<D2D1_MATRIX_3X2_F*>(&result));
					return result;
				}
				void set(Mat3x2 value) {
					Native->SetTransform(reinterpret_cast<D2D1_MATRIX_3X2_F*>(&value));
				}
			}
		};

		/// <summary> Paints an area with a solid color. </summary>
		public ref class SolidColorBrush : Brush
		{
			GRAPHICS_OBJECT(SolidColorBrush, ID2D1SolidColorBrush);

		public:

			/// <summary> Get or set the color of the brush. </summary>
			property Kodo::Graphics::Color Color
			{
				Kodo::Graphics::Color get() {
					D2D1_COLOR_F color = Native->GetColor();
					return Kodo::Graphics::Color(color.r, color.g, color.b, color.a);
				}
				void set(Kodo::Graphics::Color value) {
					Native->SetColor(reinterpret_cast<D2D1_COLOR_F*>(&value));
				}
			}

			/// <summary> Creates a new <see cref="SolidColorBrush"/>. </summary>
			/// <param name="context"> The <see cref="D2DDeviceContext"/> that will own the brush. </param>
			SolidColorBrush(D2DDeviceContext^ context);
			/// <summary> Creates a new <see cref="SolidColorBrush"/>. </summary>
			/// <param name="context"> The context that will own the brush. </param>
			/// <param name="color"> The color of the brush.</param>
			SolidColorBrush(D2DDeviceContext^ context, Kodo::Graphics::Color color);
		};

		/// <summary> Contains the position and color of a gradient stop. </summary>
		public value struct GradientStop
		{
			/// <summary> A value that indicates the relative position of the gradient stop in the brush. This value must be in the [0.0f, 1.0f] range if the gradient stop is to be seen explicitly. </summary>
			property float Position;
			/// <summary> The color of the gradient stop. </summary>
			property Kodo::Graphics::Color Color;

			/// <summary> </summary>
			GradientStop(float pos, Kodo::Graphics::Color color)
			{
				Position = pos;
				Color = color;
			}
		};

		/// <summary> Represents an collection of GradientStop objects for linear and radial gradient brushes. </summary>
		public ref class GradientStopCollection : GraphicsObject
		{
			GRAPHICS_OBJECT(GradientStopCollection, ID2D1GradientStopCollection);

		public:

			/// <summary> Indicates the gamma space in which the gradient stops are interpolated. </summary>
			property Kodo::Graphics::Gamma InterpolationGamma
			{
				Kodo::Graphics::Gamma get() { return static_cast<Kodo::Graphics::Gamma>(Native->GetColorInterpolationGamma()); }
			}

			/// <summary> Indicates the behavior of the gradient outside the normalized gradient range. </summary>
			property Kodo::Graphics::ExtendMode ExtendMode
			{
				Kodo::Graphics::ExtendMode get() { return static_cast<Kodo::Graphics::ExtendMode>(Native->GetExtendMode()); }
			}

			/// <summary> Create a new instance of <see cref="GradientStopCollection"/> </summary>
			/// <param name="context">The render target from which to create this brush.</param>
			GradientStopCollection(D2DDeviceContext^ context, array<GradientStop>^ stops);
			/// <summary> Create a new instance of <see cref="GradientStopCollection"/> </summary>
			/// <param name="context">The render target from which to create this brush.</param>
			GradientStopCollection(D2DDeviceContext^ context, array<GradientStop>^ stops, Kodo::Graphics::Gamma gamma, Kodo::Graphics::ExtendMode extendMode);

			/// <summary> Copies the gradient stops from the collection into an array of GradientStop structures. </summary>
			array<GradientStop>^ GetStops()
			{
				auto count = Native->GetGradientStopCount();
				auto results = gcnew array<GradientStop>(count);
				pin_ptr<GradientStop> pinnedResults = &results[0];

				Native->GetGradientStops(reinterpret_cast<D2D1_GRADIENT_STOP*>(pinnedResults), count);

				return results;
			}
		};

		/// <summary> Paints an area with a linear gradient. </summary>
		public ref class LinearGradientBrush : Brush
		{
			GRAPHICS_OBJECT(LinearGradientBrush, ID2D1LinearGradientBrush);

		public:

			/// <summary> Get or set the starting coordinates of the linear gradient. </summary>
			property Vec2 Start
			{
				Vec2 get() { return Vec2(Native->GetStartPoint()); }
				void set(Vec2 value) { Native->SetStartPoint(value); }
			}
			/// <summary> Get or set the ending coordinates of the linear gradient. </summary>
			property Vec2 End
			{
				Vec2 get() { return Vec2(Native->GetEndPoint()); }
				void set(Vec2 value) { Native->SetEndPoint(value); }
			}
			/// <summary> Get the <see cref="GradientStopCollection"/> associated with this linear gradient brush. </summary>
			property GradientStopCollection^ GradientStops
			{
				GradientStopCollection^ get()
				{
					ID2D1GradientStopCollection* stops;
					Native->GetGradientStopCollection(&stops);
					return gcnew GradientStopCollection(stops);
				}
			}

			/// <summary> </summary>
			LinearGradientBrush(D2DDeviceContext^ context, GradientStopCollection^ gradientStops) : LinearGradientBrush(context, gradientStops, Vec2(), Vec2()) {}
			/// <summary> </summary>
			LinearGradientBrush(D2DDeviceContext^ context, GradientStopCollection^ gradientStops, Vec2 gradientStart, Vec2 gradientEnd);
		};

		/// <summary> Paints an area with a bitmap. </summary>
		public ref class BitmapBrush : Brush
		{
			GRAPHICS_OBJECT(BitmapBrush, ID2D1BitmapBrush1);

		public:

			/// <summary> </summary>
			property ExtendMode ExtendModeX
			{
				ExtendMode get() { return (ExtendMode)Native->GetExtendModeX(); }
				void set(ExtendMode value) { Native->SetExtendModeX((D2D1_EXTEND_MODE)value); }
			}
			/// <summary> </summary>
			property ExtendMode ExtendModeY
			{
				ExtendMode get() { return (ExtendMode)Native->GetExtendModeY(); }
				void set(ExtendMode value) { Native->SetExtendModeY((D2D1_EXTEND_MODE)value); }
			}
			/// <summary> </summary>
			property BitmapInterpolationMode InterpolationMode
			{
				BitmapInterpolationMode get() { return (BitmapInterpolationMode)Native->GetInterpolationMode(); }
				void set(BitmapInterpolationMode value) { Native->SetInterpolationMode((D2D1_BITMAP_INTERPOLATION_MODE)value); }
			}
			/// <summary> </summary>
			BitmapBrush(D2DDeviceContext^ context, Kodo::Graphics::Bitmap^ bitmap);
		};

		/// <summary> </summary>
		public ref class GeometryRealization : GraphicsObject {
			GRAPHICS_OBJECT(GeometryRealization, ID2D1GeometryRealization);

		protected:
			/// <summary> </summary>
			GeometryRealization() { };
		};

		/// <summary> Describes a geometric path that does not contain quadratic bezier curves or arcs. </summary>
		public ref class SimplifiedGeometrySink : GraphicsObject {
			GRAPHICS_OBJECT(SimplifiedGeometrySink, ID2D1SimplifiedGeometrySink);

		protected:
			/// <summary> </summary>
			SimplifiedGeometrySink() {}

		public:
			/// <summary> Creates a sequence of cubic Bezier curves and adds them to the geometry sink. </summary>
			/// <param name="beziers">
			/// An array of Bezier segments that describes the Bezier curves to create. 
			/// A curve is drawn from the geometry sink's current point (the end point of the last segment drawn or the location specified by BeginFigure) to the end point of the first Bezier segment in the array. 
			/// If the array contains additional Bezier segments, each subsequent Bezier segment uses the end point of the preceding Bezier segment as its start point.
			/// </param>
			void AddBeziers(array<BezierSegment>^ beziers)
			{
				pin_ptr<BezierSegment> pinnedBeziers = &beziers[0];
				Native->AddBeziers(reinterpret_cast<D2D1_BEZIER_SEGMENT*>(pinnedBeziers), beziers->Length);
			}
			/// <summary> Creates a sequence of lines using the specified points and adds them to the geometry sink. </summary>
			/// <param name="points">
			/// An array of one or more points that describe the lines to draw. 
			/// A line is drawn from the geometry sink's current point (the end point of the last segment drawn or the location specified by BeginFigure) to the first point in the array. 
			/// If the array contains additional points, a line is drawn from the first point to the second point in the array, from the second point to the third point, and so on.
			/// </param>
			void AddLines(array<Vec2>^ points)
			{
				pin_ptr<Vec2> pinnedPoints = &points[0];
				Native->AddLines(reinterpret_cast<D2D1_POINT_2F*>(pinnedPoints), points->Length);
			}
			/// <summary> Starts a new figure at the specified point. </summary>
			/// <param name="startPoint">The point at which to begin the new figure.</param>
			/// <param name="style">Whether the new figure should be hollow or filled.</param>
			void BeginFigure(Vec2 startPoint, FigureBegin style) { Native->BeginFigure(startPoint, static_cast<D2D1_FIGURE_BEGIN>(style)); }
			/// <summary> Closes the geometry sink, indicates whether it is in an error state, and resets the sink's error state. </summary>
			void Close() { ThrowIfFailed(Native->Close()); }
			/// <summary> Ends the current figure. </summary>
			/// <param name="style">A value that indicates whether the current figure is closed. If the figure is closed, a line is drawn between the current point and the start point specified by BeginFigure.</param>
			void EndFigure(FigureEnd style) { Native->EndFigure(static_cast<D2D1_FIGURE_END>(style)); }
			/// <summary> Specifies the method used to determine which points are inside the geometry described by this geometry sink and which points are outside.
			/// </summary>
			/// <param name="fillMode">The method used to determine whether a given point is part of the geometry.</param>
			void SetFillMode(FillMode fillMode) { Native->SetFillMode(static_cast<D2D1_FILL_MODE>(fillMode)); }
			/// <summary> Specifies stroke and join options to be applied to new segments added to the geometry sink. </summary>
			/// <param name="vertexFlags">Stroke and join options to be applied to new segments added to the geometry sink.</param>
			void SetSegmentFlags(PathSegment vertexFlags) { Native->SetSegmentFlags(static_cast<D2D1_PATH_SEGMENT>(vertexFlags)); }
		};

		/// <summary> Represents a geometry resource and defines a set of helper methods for manipulating and measuring geometric shapes. Classes that inherit from Geometry define specific shapes. </summary>
		public ref class Geometry : GraphicsObject {
			GRAPHICS_OBJECT(Geometry, ID2D1Geometry);

		protected:
			/// <summary> </summary>
			Geometry() { };

		public:
			/// <summary> </summary>
			void Outline(SimplifiedGeometrySink^ sink) { ThrowIfFailed(Native->Outline(nullptr, sink->Native)); }

			bool Contains(Vec2 point)
			{
				BOOL contains = false;
				ThrowIfFailed(Native->FillContainsPoint(point, nullptr, &contains));
				return contains;
			}
		};

		/// <summary> Describes a geometric path that can contain lines, arcs, cubic bezier curves, and quadratic bezier curves. </summary>
		public ref class GeometrySink : SimplifiedGeometrySink {
			GRAPHICS_OBJECT(GeometrySink, ID2D1GeometrySink);

		public:
			/// <summary> Creates a single arc and adds it to the path geometry. </summary>
			/// <param name="arc">The arc segment to add to the figure.</param>
			void AddArc(ArcSegment arc) { Native->AddArc(reinterpret_cast<D2D1_ARC_SEGMENT*>(&arc)); }
			/// <summary> Creates a cubic Bezier curve between the current point and the specified end point and adds it to the geometry sink. </summary>
			/// <param name="bezier">A structure that describes the control points and end point of the Bezier curve to add.</param>
			void AddBezier(BezierSegment bezier) { Native->AddBezier(reinterpret_cast<D2D1_BEZIER_SEGMENT*>(&bezier)); }
			/// <summary> Creates a line segment between the current point and the specified end point and adds it to the geometry sink. </summary>
			/// <param name="point">The end point of the line to draw.</param>
			void AddLine(Vec2 point) { Native->AddLine(point); }
			/// <summary> Creates a quadratic Bezier curve between the current point and the specified end point and adds it to the geometry sink. </summary>
			/// <param name="bezier">A structure that describes the control point and the end point of the quadratic Bezier curve to add.</param>
			void AddQuadraticBezier(QuadraticBezierSegment bezier) { Native->AddQuadraticBezier(reinterpret_cast<D2D1_QUADRATIC_BEZIER_SEGMENT*>(&bezier)); }
			/// <summary> Adds a sequence of quadratic Bezier segments as an array in a single call. </summary>
			/// <param name="beziers">An array of a sequence of quadratic Bezier segments.</param>
			void AddQuadraticBeziers(array<QuadraticBezierSegment>^ beziers)
			{
				pin_ptr<QuadraticBezierSegment> pinnedBeziers = &beziers[0];
				Native->AddQuadraticBeziers(reinterpret_cast<D2D1_QUADRATIC_BEZIER_SEGMENT*>(pinnedBeziers), beziers->Length);
			}
		};

		/// <summary> </summary>
		public ref class PathGeometry : Geometry {
			GRAPHICS_OBJECT(PathGeometry, ID2D1PathGeometry);

		public:
			/// <summary> Creates an empty path geometry. </summary>
			PathGeometry();
			/// <summary> Retrieves the geometry sink that is used to populate the path geometry with figures and segments. </summary>
			GeometrySink^ Open()
			{
				ID2D1GeometrySink* sink;
				ThrowIfFailed(Native->Open(&sink));
				return gcnew GeometrySink(sink);
			}
		};

		public ref class D2DDeviceContext : GraphicsObject
		{
			GRAPHICS_OBJECT(D2DDeviceContext, ID2D1DeviceContext1);

			Image^ currentTarget;
			bool isDrawing;

			Stack<Rectangle> activeClips;
			Stack<Rectangle> savedClips;

		public:

			property bool IsDrawing { bool get() { return isDrawing; } }

			Image^ GetTarget() { return currentTarget; }
			void SetTarget(Image^ image);

			AntialiasMode GetAntialiasMode() { return (AntialiasMode)Native->GetAntialiasMode(); }
			void SetAntialiasMode(AntialiasMode mode) { Native->SetAntialiasMode((D2D1_ANTIALIAS_MODE)mode); }

			void BeginDraw() { isDrawing = true; Native->BeginDraw(); savedClips.Clear(); }
			void BeginDrawRestoreState()
			{
				isDrawing = true;
				Native->BeginDraw();

				int savedClipsCount = savedClips.Count;

				for (auto i = 0; i < savedClipsCount; i++)
				{
					PushAxisAlignedClip(savedClips.Pop());
				}
			}
			void EndDraw()
			{
				ThrowIf(activeClips.Count > 0, "There are active clips! All clips must be cleared before ending the drawing sequence.");

				isDrawing = false;
				Native->EndDraw();
			}
			void EndDrawSaveState()
			{
				isDrawing = false;

				int activeClipsCount = activeClips.Count;

				for (auto i = 0; i < activeClipsCount; i++)
				{
					savedClips.Push(activeClips.Pop());

					Native->PopAxisAlignedClip();
				}

				Native->EndDraw();
			}
			void Clear(Color clearColor) { Native->Clear((D2D1_COLOR_F*)&clearColor); }

			void PopAxisAlignedClip()
			{
				activeClips.Pop();

				Native->PopAxisAlignedClip();
			}

			void PushAxisAlignedClip(Rectangle axisAlignedClip)
			{
				activeClips.Push(axisAlignedClip);

				Native->PushAxisAlignedClip((D2D1_RECT_F&)axisAlignedClip, D2D1_ANTIALIAS_MODE_ALIASED);
			}

			void FillRectangle(Rectangle rectangle, Brush^ brush);

			Mat3x2 GetTransform()
			{
				Mat3x2 mat;
				Native->GetTransform((D2D1_MATRIX_3X2_F*)&mat);
				return mat;
			}

			void SetTransform(Mat3x2 transform)
			{
				Native->SetTransform((D2D1_MATRIX_3X2_F*)&transform);
			}

			GeometryRealization^ CreateStrokedGeometryRealization(Geometry^ geometry, float flatteningTolerance, float strokeWidth, StrokeStyle^ stroke);

			void DrawLine(Line line, Brush^ brush, float strokeWidth) { DrawLine(line.Begin, line.End, brush, strokeWidth); }
			void DrawLine(Line line, Brush^ brush, float strokeWidth, StrokeStyle^ stroke) { DrawLine(line.Begin, line.End, brush, strokeWidth, stroke); }
			void DrawLine(Vec2 begin, Vec2 end, Brush^ brush, float strokeWidth);
			void DrawLine(Vec2 begin, Vec2 end, Brush^ brush, float strokeWidth, StrokeStyle^ stroke);

			void DrawLines(array<Vec2>^ points, Brush^ brush, float strokeWidth) { DrawLines(points, 0, points->Length, brush, strokeWidth); }
			void DrawLines(array<Vec2>^ points, Brush^ brush, float strokeWidth, StrokeStyle^ stroke) { DrawLines(points, 0, points->Length, brush, strokeWidth, stroke); }

			void DrawLines(array<Vec2>^ points, Int32 start, Int32 count, Brush^ brush, float strokeWidth);
			void DrawLines(array<Vec2>^ points, Int32 start, Int32 count, Brush^ brush, float strokeWidth, StrokeStyle^ stroke);

			void DrawRectangle(Rectangle rectangle, Brush^ brush, float strokeWidth);
			void DrawRectangle(Rectangle rect, Brush^ brush, float strokeWidth, StrokeStyle^ stroke);

			void DrawEllipse(Ellipse ellipse, Brush^ brush, float strokeWidth);
			void FillEllipse(Ellipse ellipse, Brush^ brush);

			void DrawGeometry(Geometry^ geometry, Brush^ brush, float strokeWidth);
			void DrawGeometry(Geometry^ geometry, Brush^ brush, float strokeWidth, StrokeStyle^ stroke);
			void DrawGeometryRealization(GeometryRealization^ geometryRealization, Brush^ brush);

			void FillRoundedRectangle(RoundedRectangle rect, Brush^ brush);
			void FillGeometry(Geometry^ geometry, Brush^ brush);
			void FillOpacityMask(Bitmap^ bitmap, Brush^ brush, Rectangle destinationArea);
			void DrawBitmap(Bitmap^ bitmap, Rectangle destinationArea, float opacity, InterpolationMode interpolation);
			void DrawBitmap(Bitmap^ bitmap, Rectangle sourceArea, Rectangle destinationArea, float opacity, InterpolationMode interpolation);

			void DrawText(String^ text, TextFormat^ format, Rectangle area, Brush^ brush) { DrawText(text, format, area, brush, DrawTextOptions::Clip, MeasuringMode::Natural); }
			void DrawText(String^ text, TextFormat^ format, Rectangle area, Brush^ brush, DrawTextOptions options, MeasuringMode mode);

			void DrawTextLayout(TextLayout^ layout, Vec2 origin, Brush^ brush) { DrawTextLayout(layout, origin, brush, Kodo::Graphics::DrawTextOptions::None); }
			void DrawTextLayout(TextLayout^ layout, Vec2 origin, Brush^ brush, DrawTextOptions options);
		};

		/*public ref class D2DEffect : GraphicsObject {
			GRAPHICS_OBJECT(D2DEffect, ID2D1Effect);

		public:

			initonly static Guid BitmapSource = Guid(0x5fb6c24d, 0xc6dd, 0x4231, 0x94, 0x4, 0x50, 0xf4, 0xd5, 0xc3, 0x25, 0x2d);

			D2DEffect(D2DDeviceContext^ d2dContext, Guid^ effectId)
			{
				ID2D1Effect* effect;

				ThrowIfFailed(d2dContext->Native->CreateEffect(*(GUID*)& effectId, &effect));
				Assing(effect);
			}
		};*/

		public ref class DrawingContext
		{
			D2DDeviceContext^ deviceContext;

		internal:

			DrawingContext(D2DDeviceContext^ d2dDeviceContext);

		public:

			void PopClip() { deviceContext->PopAxisAlignedClip(); }
			void PushClip(Rectangle axisAlignedClip) { deviceContext->PushAxisAlignedClip(axisAlignedClip); }

			void Draw(Rectangle rectangle, Brush^ brush, FLOAT strokeWidth) { deviceContext->DrawRectangle(rectangle, brush, strokeWidth); }
			void Fill(Rectangle rectangle, Brush^ brush) { deviceContext->FillRectangle(rectangle, brush); }
		};

		public ref class D2DDevice : GraphicsObject
		{
			GRAPHICS_OBJECT(D2DDevice, ID2D1Device1);

			D2DDeviceContext^ deviceContext;

		public:

			property D2DDeviceContext^ Context { D2DDeviceContext^ get() { return deviceContext; } }

			D2DDevice(D3DDevice^ d3dDevice, DxgiDevice^ dxgiDevice, bool debug);

		protected:

		};
	}
}
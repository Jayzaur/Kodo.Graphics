#pragma once

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "Windowscodecs.lib")
#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "dwmapi.lib") 

#include <SDKDDKVer.h>

#include <stdint.h>
#include <memory>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <vcclr.h>
#include <dwmapi.h>
#include <unknwn.h>
#include <shellapi.h>
#include <Wincodec.h>
#include <comdef.h>

#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
//#define GLM_FORCE_XYZW_ONLY
#define GLM_FORCE_INTRINSICS
#include <glm/glm.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective

#pragma comment (lib, "GLFW/glfw3.lib")
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#pragma comment(lib,"d3d11.lib")
#include <d3d11.h>
#include <dxgi1_3.h>
#pragma comment(lib,"d2d1.lib")
#include <d2d1_2.h>
#include <d2d1_1helper.h>
#include <d2d1_2helper.h>

#pragma comment(lib, "dwrite.lib")
#include <dwrite_2.h>

#ifdef SUPPORT_MF

#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "mfreadwrite.lib")
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mferror.h>
#include <mfmediaengine.h>

#pragma comment(lib, "shlwapi.lib")
#include <shlwapi.h>

#endif

using namespace System;

#ifndef GRAPHICS_OBJECT
#define GRAPHICS_OBJECT(managed_type, native_type) \
	internal: \
		property native_type* Native { native_type* get() { return static_cast<native_type*>( _native ); } } \
		managed_type( native_type* n ) { _native = n; } 
#endif

namespace Kodo
{
	namespace Graphics
	{
		/// <summary>
		/// Represents errors that occur within the graphics system.
		/// </summary>
		public ref class GraphicsException : Exception
		{
		public:
			GraphicsException(Int32 hr) : Exception(String::Format(System::Globalization::CultureInfo::CurrentCulture, "Graphics operation failed with HRESULT: {0}", hr)) { }
			GraphicsException(String^ message) : Exception(message) { }
			GraphicsException(Int32 hr, String^ message) : Exception(String::Format(System::Globalization::CultureInfo::CurrentCulture, "{0} HRESULT: {1}", message, hr)) { }
		};

		public ref class GraphicsObject abstract
		{
		internal:

			IUnknown* _native;

			GraphicsObject() : _native(nullptr) { }
			GraphicsObject(IUnknown* n) : _native(n) { }

			void Assing(IUnknown* native) {
				System::Diagnostics::Debug::Assert(_native == nullptr, "This object has already been assigned!");
				_native = native;
			}

			void Release()
			{
				if (_native != nullptr) {
					_native->Release();
					_native = nullptr;
				}
			}

		public:

			GraphicsObject^ operator=(GraphicsObject^ obj)
			{
				if (_native != obj->_native) {
					IUnknown* oldNative = obj->_native;

					_native = obj->_native;
					obj->_native = nullptr;

					if (oldNative != nullptr) {
						oldNative->Release();
					}
				}

				return this;
			}

			virtual ~GraphicsObject() {
				this->!GraphicsObject();
				GC::SuppressFinalize(this);
			}

			!GraphicsObject() {
				if (_native) {
					_native->Release();
					_native = nullptr;
				}
			}
		};
	}
}

template<class T, class U>
static System::Boolean Is(U u)
{
	return dynamic_cast<T>(u) != nullptr;
}

template<typename E>
static void ThrowIfFailed(HRESULT result, String^ message)
{
	if (FAILED(result))
	{
		throw gcnew E(message);
	}
}

template<typename E = Kodo::Graphics::GraphicsException, typename T>
static void ThrowIfNull(T thing, String^ message)
{
	if (thing == nullptr)
	{
		throw gcnew E(message);
	}
}
template<typename E = Kodo::Graphics::GraphicsException, typename T>
static void ThrowIfNull(T thing) { ThrowIfNull<E>(thing, "This was null!"); }

template<typename E = Kodo::Graphics::GraphicsException, typename T>
static void ThrowIfZero(T thing, String^ message) {
	if (thing == 0)
		throw gcnew E(message);
}
template<typename E = Kodo::Graphics::GraphicsException, typename T>
static void ThrowIfZero(T thing) { ThrowIfZero<E>(thing, "This was 0!"); }

template<typename E = Kodo::Graphics::GraphicsException, typename T>
static void ThrowIf(T thing, String^ message)
{
	if (thing == TRUE)
	{
		throw gcnew E(message);
	}
}
template<typename E = Kodo::Graphics::GraphicsException, typename T>
static void ThrowIf(T thing) { ThrowIf<E>(thing, "Thing was true!"); }

template<typename E = Kodo::Graphics::GraphicsException>
static void ThrowIfFailed(HRESULT result)
{
	if (FAILED(result))
	{
		throw gcnew E(gcnew String(_com_error(result).ErrorMessage()));
	}
}

template<typename T>
class com_ptr
{
	_COM_SMARTPTR_TYPEDEF(T, __uuidof(T));
	TPtr ptr;

public:

	com_ptr() :
		com_ptr(nullptr)
	{
	}
	com_ptr(TPtr p) :
		ptr(p)
	{
	}
	com_ptr(T* p)
	{
		ptr.Attach(p);
	}

	IUnknown* toIUnknown()
	{
		return reinterpret_cast<IUnknown*>(ptr);
	}

	template<typename TResult>
	TResult* queryInterface()
	{
		HRESULT hr = E_POINTER;
		TResult* result = nullptr;

		if (ptr)
		{
			hr = ptr->QueryInterface(__uuidof(TResult), (void**)&result);
		}

		if (FAILED(hr))
		{
			_com_error err(hr);
			LPCTSTR errMsg = err.ErrorMessage();
			printf("%s\n", errMsg);
			result = nullptr;
			abort();
		}

		return result;
	}

	void release()
	{
		this->~com_ptr();
	}

	~com_ptr()
	{
		if (ptr)
		{
			ptr = nullptr;
		}
	}

	operator T* ()
	{
		return ptr.GetInterfacePtr();
	}

	T* operator -> ()
	{
		return ptr.GetInterfacePtr();
	}

	T** operator & ()
	{
		return &(ptr.GetInterfacePtr());
	}
};

namespace Kodo
{
	namespace Graphics
	{
		using namespace System;
		using namespace System::Runtime::InteropServices;

		ref class D2DFactory;
		ref class FactoryDW;
		ref class DxgiFactory;

		namespace Imaging
		{
			ref class WicFactory;
		}

		public ref class Common abstract
		{
		public:

			static float Radians(float degrees) { return (float)(degrees * 0.01745329251994329576923690768489); }
			static float Degrees(float radians) { return (float)(radians / 0.01745329251994329576923690768489); }
			static int Clamp(int x, int min, int max) { return Math::Min(Math::Max(x, min), max); }
			static float Clamp(float x, float min, float max) { return Math::Min(Math::Max(x, min), max); }

			static float Clamp(float x, float min, float max, [Out] bool% clamped)
			{
				if (x <= min)
				{
					clamped = true;
					return min;
				}
				if (x >= max)
				{
					clamped = true;
					return max;
				}

				clamped = false;
				return x;
			}

			/// <summary>
			/// Projects the input value to the specified range.
			/// </summary>
			/// <param name="val">Value to project.</param>
			/// <param name="inMin">Minimum value of the input.</param>
			/// <param name="inMax">Maximum value of the input.</param>
			/// <param name="outMin">Minimum value of the output range.</param>
			/// <param name="outMax">Maximum value of the output range.</param>
			static float LinearScale(float val, float inMin, float inMax, float outMin, float outMax)
			{
				// out = f(in) = a*in+b
				auto a = (outMin - outMax) / (inMin - inMax);
				auto b = outMin - inMin * (outMin - outMax) / (inMin - inMax);
				return a * val + b;
			}
		};

		public ref class GlobalAssets abstract sealed
		{
		public:

			static D2DFactory^ Factory2D;
			static FactoryDW^ FactoryDw;
			static Imaging::WicFactory^ FactoryWic;

			static DxgiFactory^ DXGI;

			static property Imaging::WicFactory^ WIC
			{
				Imaging::WicFactory^ get();
			}
		};

		value struct Rectangle;

		[StructLayout(LayoutKind::Sequential)]
		public value struct Vec2
		{
			initonly float X;
			initonly float Y;

			Vec2(float scalar) : Vec2(scalar, scalar) {}
			Vec2(double x, double y) : X((float)x), Y((float)y) {}
			Vec2(int x, int y) : X((float)x), Y((float)y) {}
			Vec2(float x, float y) : X(x), Y(y) {}

			property float default[int]
			{
				float get(int i)
				{
					System::Diagnostics::Debug::Assert(i < 2);
					switch (i) { default: case 0: return X; case 1: return Y; }
				}
			}

			property float Width { float get() { return X; } }
			property float Height { float get() { return Y; } }
			property float U { float get() { return X; } }
			property float V { float get() { return Y; } }

			static Vec2 operator / (Vec2 a, float scalar) { return Vec2(a.X / scalar, a.Y / scalar); }
			static Vec2 operator / (float scalar, Vec2 b) { return Vec2(scalar / b.X, scalar / b.Y); }
			static Vec2 operator / (Vec2 a, Vec2 b) { return Vec2(a.X / b.X, a.Y / b.Y); }
			static Vec2 operator * (Vec2 a, float scalar) { return Vec2(a.X * scalar, a.Y * scalar); }
			static Vec2 operator * (Vec2 a, Vec2 b) { return Vec2(a.X * b.X, a.Y * b.Y); }
			static Vec2 operator + (Vec2 a, Vec2 b) { return Vec2(a.X + b.X, a.Y + b.Y); }
			static Vec2 operator + (Vec2 a, float scalar) { return Vec2(a.X + scalar, a.Y + scalar); }
			static Vec2 operator - (Vec2 a, Vec2 b) { return Vec2(a.X - b.X, a.Y - b.Y); }
			static Vec2 operator - (Vec2 a, float scalar) { return Vec2(a.X - scalar, a.Y - scalar); }

			static bool operator != (Vec2 a, Vec2 b) { return !a.Equals(b); }
			static bool operator == (Vec2 a, Vec2 b) { return a.Equals(b); }
			virtual bool Equals(Vec2 other) { return X == other.X && Y == other.Y; }
			bool Equals(Object^ obj) override { return obj != nullptr && GetType() == obj->GetType() && Equals((Vec2)obj); }
			int GetHashCode() override { return (X.GetHashCode()) + Y.GetHashCode(); }
			String^ ToString() override { return String::Format(System::Globalization::CultureInfo::InvariantCulture, "{0}, {1}", X, Y); }

		internal:

			static operator D2D1_SIZE_F(Vec2 v) { return D2D1::SizeF(v.X, v.Y); }
			static operator D2D1_POINT_2F(Vec2 v) { return D2D1::Point2F(v.X, v.Y); }

			Vec2(D2D1_POINT_2F point) : Vec2(point.x, point.y) {}
			Vec2(D2D1_SIZE_F size) : Vec2(size.width, size.height) {}
		};

		[StructLayout(LayoutKind::Sequential)]
		public value struct Vec3
		{
			static initonly Vec3 Up = Vec3(0, +1, 0);
			static initonly Vec3 Down = Vec3(0, -1, 0);
			static initonly Vec3 Left = Vec3(-1, 0, 0);
			static initonly Vec3 Right = Vec3(+1, 0, 0);
			static initonly Vec3 Forward = Vec3(0, 0, +1);
			static initonly Vec3 Backward = Vec3(0, 0, -1);

			initonly float X;
			initonly float Y;
			initonly float Z;

			Vec3(float scalar) : Vec3(scalar, scalar, scalar) {}
			Vec3(float x, float y, float z) : X(x), Y(y), Z(z) {}
			Vec3(int x, int y, int z) : X((float)x), Y((float)y), Z((float)z) {}
			Vec3(double x, double y, double z) : X((float)x), Y((float)y), Z((float)z) {}

			property float default[int]
			{
				float get(int i)
				{
					System::Diagnostics::Debug::Assert(i < 3);
					switch (i) { default: case 0: return X; case 1: return Y; case 2: return Z; }
				}
			}

			property Vec2 XY { Vec2 get() { return Vec2(X, Y); } }

			property float R { float get() { return X; } }
			property float G { float get() { return Y; } }
			property float B { float get() { return Z; } }

			static float Dot(Vec3 a, Vec3 b)
			{
				auto tmp = a * b;
				return tmp.X + tmp.Y + tmp.Z;
			}

			static Vec3 Normalize(Vec3 v) { return v * (1 / (float)Math::Sqrt(Dot(v, v))); }

			static Vec3 Cross(Vec3 x, Vec3 y)
			{
				return Vec3(
					x.Y * y.Z - y.Y * x.Z,
					x.Z * y.X - y.Z * x.X,
					x.X * y.Y - y.X * x.Y);
			}

			static Vec3 operator / (Vec3 a, float scalar) { return Vec3(a.X / scalar, a.Y / scalar, a.Z / scalar); }
			static Vec3 operator / (float scalar, Vec3 b) { return Vec3(scalar / b.X, scalar / b.Y, scalar / b.Z); }
			static Vec3 operator / (Vec3 a, Vec3 b) { return Vec3(a.X / b.X, a.Y / b.Y, a.Z / b.Z); }
			static Vec3 operator * (Vec3 a, float scalar) { return Vec3(a.X * scalar, a.Y * scalar, a.Z * scalar); }
			static Vec3 operator * (Vec3 a, Vec3 b) { return Vec3(a.X * b.X, a.Y * b.Y, a.Z * b.Z); }
			static Vec3 operator + (Vec3 a, Vec3 b) { return Vec3(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
			static Vec3 operator - (Vec3 a, Vec3 b) { return Vec3(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }

			static bool operator != (Vec3 a, Vec3 b) { return !a.Equals(b); }
			static bool operator == (Vec3 a, Vec3 b) { return a.Equals(b); }
			virtual bool Equals(Vec3 other) { return X == other.X && Y == other.Y && Z == other.Z; }
			bool Equals(Object^ obj) override { return obj != nullptr && GetType() == obj->GetType() && Equals((Vec3)obj); }
			int GetHashCode() override { return ((X.GetHashCode()) + Y.GetHashCode()) + Z.GetHashCode(); }
			String^ ToString() override { return String::Format(System::Globalization::CultureInfo::InvariantCulture, "{0}, {1}, {2}", X, Y, Z); }
		};


		[StructLayout(LayoutKind::Sequential)]
		public value struct Vec4
		{
			initonly float X;
			initonly float Y;
			initonly float Z;
			initonly float W;

			Vec4(float scalar) : Vec4(scalar, scalar, scalar, scalar) {}
			Vec4(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}

			Vec4(Vec2 xy, float z, float w) : X(xy.X), Y(xy.Y), Z(z), W(w) {}
			Vec4(Vec3 xyz, float w) : X(xyz.X), Y(xyz.Y), Z(xyz.Z), W(w) {}

			property float default[int]
			{
				float get(int i)
				{
					System::Diagnostics::Debug::Assert(i < 4);
					switch (i) { default: case 0: return X; case 1: return Y; case 2: return Z; case 3: return W; }
				}
			}

			property float Left { float get() { return X; } }
			property float Top { float get() { return Y; } }
			property float Right { float get() { return Z; } }
			property float Bottom { float get() { return W; } }

			property float R { float get() { return X; } }
			property float G { float get() { return Y; } }
			property float B { float get() { return Z; } }
			property float A { float get() { return W; } }

			property Vec2 XY { Vec2 get() { return Vec2(X, Y); } }
			property Vec2 ZW { Vec2 get() { return Vec2(Z, W); } }

			static Vec4 operator / (Vec4 a, float scalar) { return Vec4(a.X / scalar, a.Y / scalar, a.Z / scalar, a.W / scalar); }
			static Vec4 operator / (float scalar, Vec4 b) { return Vec4(scalar / b.X, scalar / b.Y, scalar / b.Z, scalar / b.W); }
			static Vec4 operator / (Vec4 a, Vec4 b) { return Vec4(a.X / b.X, a.Y / b.Y, a.Z / b.Z, a.W / b.W); }
			static Vec4 operator * (Vec4 a, float scalar) { return Vec4(a.X * scalar, a.Y * scalar, a.Z * scalar, a.W * scalar); }
			static Vec4 operator * (Vec4 a, Vec4 b) { return Vec4(a.X * b.X, a.Y * b.Y, a.Z * b.Z, a.W * b.W); }
			static Vec4 operator + (Vec4 a, Vec4 b) { return Vec4(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W + b.W); }

			static bool operator != (Vec4 a, Vec4 b) { return !a.Equals(b); }
			static bool operator == (Vec4 a, Vec4 b) { return a.Equals(b); }
			virtual bool Equals(Vec4 other) { return X == other.X && Y == other.Y && Z == other.Z && W == other.W; }
			bool Equals(Object^ obj) override { return obj != nullptr && GetType() == obj->GetType() && Equals((Vec4)obj); }
			int GetHashCode() override { return (((X.GetHashCode()) + Y.GetHashCode()) + Z.GetHashCode()) + W.GetHashCode(); }
			String^ ToString() override { return String::Format(System::Globalization::CultureInfo::InvariantCulture, "{0}, {1}, {2}, {3}", X, Y, Z, W); }
		};

		[StructLayout(LayoutKind::Sequential)]
		public value class Mat4x4
		{
			Vec4 C1;
			Vec4 C2;
			Vec4 C3;
			Vec4 C4;

		public:

			static property Mat4x4 Identity
			{
				Mat4x4 get()
				{
					Mat4x4 result;
					result[0] = Vec4(1.0f, 0.0f, 0.0f, 0.0f);
					result[1] = Vec4(0.0f, 1.0f, 0.0f, 0.0f);
					result[2] = Vec4(0.0f, 0.0f, 1.0f, 0.0f);
					result[3] = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
					return result;
				}
			};

			static Mat4x4 LookAt(Vec3 eye, Vec3 center, Vec3 up)
			{
				auto mat = glm::lookAt(*((glm::vec3*) & eye), *((glm::vec3*) & center), *((glm::vec3*) & up));
				Mat4x4 result;
				result[0] = Vec4(mat[0].x, mat[0].y, mat[0].z, mat[0].w);
				result[1] = Vec4(mat[1].x, mat[1].y, mat[1].z, mat[1].w);
				result[2] = Vec4(mat[2].x, mat[2].y, mat[2].z, mat[2].w);
				result[3] = Vec4(mat[3].x, mat[3].y, mat[3].z, mat[3].w);
				return result;
			}

			static Mat4x4 Ortho(float left, float right, float bottom, float top)
			{
				auto mat = glm::ortho(left, right, bottom, top);
				Mat4x4 result;
				result[0] = Vec4(mat[0].x, mat[0].y, mat[0].z, mat[0].w);
				result[1] = Vec4(mat[1].x, mat[1].y, mat[1].z, mat[1].w);
				result[2] = Vec4(mat[2].x, mat[2].y, mat[2].z, mat[2].w);
				result[3] = Vec4(mat[3].x, mat[3].y, mat[3].z, mat[3].w);
				return result;
			}

			static Mat4x4 Perspective(float fov, float aspect, float n, float f)
			{
				auto mat = glm::perspective(fov, aspect, n, f);
				Mat4x4 result;
				result[0] = Vec4(mat[0].x, mat[0].y, mat[0].z, mat[0].w);
				result[1] = Vec4(mat[1].x, mat[1].y, mat[1].z, mat[1].w);
				result[2] = Vec4(mat[2].x, mat[2].y, mat[2].z, mat[2].w);
				result[3] = Vec4(mat[3].x, mat[3].y, mat[3].z, mat[3].w);
				return result;
			}

			static Mat4x4 Transpose(Mat4x4 m)
			{
				Mat4x4 result;
				result[0] = Vec4(m[0][0], m[1][0], m[2][0], m[3][0]);
				result[1] = Vec4(m[0][1], m[1][1], m[2][1], m[3][1]);
				result[2] = Vec4(m[0][2], m[1][2], m[2][2], m[3][2]);
				result[3] = Vec4(m[0][3], m[1][3], m[2][3], m[3][3]);
				return result;
			}

			property float default[int, int]
			{
				float get(int i, int ii)
				{
					System::Diagnostics::Debug::Assert(i < 4 && ii < 4);
					switch (i) { default: case 0: return C1[ii]; case 1: return C2[ii]; case 2: return C3[ii]; case 3: return C4[ii]; }
				}
			};

			property Vec4 default[int]
			{
				Vec4 get(int i)
				{
					System::Diagnostics::Debug::Assert(i < 4);
					switch (i) { default: case 0: return C1; case 1: return C2; case 2: return C3; case 3: return C4; }
				}

				void set(int i, Vec4 value)
				{
					System::Diagnostics::Debug::Assert(i < 4);
					switch (i) { default: case 0: C1 = value; break; case 1: C2 = value; break; case 2: C3 = value; break; case 3: C4 = value; break; }
				}
			};

			static Mat4x4 operator * (Mat4x4 m1, Mat4x4 m2)
			{
				Vec4 srcA0 = m1[0];
				Vec4 srcA1 = m1[1];
				Vec4 srcA2 = m1[2];
				Vec4 srcA3 = m1[3];

				Vec4 srcB0 = m2[0];
				Vec4 srcB1 = m2[1];
				Vec4 srcB2 = m2[2];
				Vec4 srcB3 = m2[3];

				auto result = Mat4x4();
				result[0] = srcA0 * srcB0[0] + srcA1 * srcB0[1] + srcA2 * srcB0[2] + srcA3 * srcB0[3];
				result[1] = srcA0 * srcB1[0] + srcA1 * srcB1[1] + srcA2 * srcB1[2] + srcA3 * srcB1[3];
				result[2] = srcA0 * srcB2[0] + srcA1 * srcB2[1] + srcA2 * srcB2[2] + srcA3 * srcB2[3];
				result[3] = srcA0 * srcB3[0] + srcA1 * srcB3[1] + srcA2 * srcB3[2] + srcA3 * srcB3[3];
				return result;
			}
		};

		[System::Runtime::InteropServices::StructLayout(System::Runtime::InteropServices::LayoutKind::Sequential, Pack = 4)]
		public value class Mat3x2 : System::IEquatable<Mat3x2>
		{
			float M11;
			float M12;
			float M21;
			float M22;
			float M31;
			float M32;

		public:

			static property Mat3x2 Identity
			{
				Mat3x2 get()
				{
					Mat3x2 result;
					result.M11 = 1.0f;
					result.M22 = 1.0f;
					return result;
				}
			};

			static Mat3x2 Invert(Mat3x2 m)
			{
				Mat3x2 result = m;
				(D2D1::Matrix3x2F::ReinterpretBaseType((D2D1_MATRIX_3X2_F*)&result))->Invert();
				return result;
			}

			static float Determinant(Mat3x2 m)
			{
				return m.M11 * m.M22 - m.M12 * m.M21;
			}

			/// <summary> 
			/// Multiplies this matrix with the specified matrix. 
			/// </summary>
			/// <param name="x">The matrix to multiply.</param>
			/// <param name="y">The matrix to multiply with.</param>
			static Mat3x2 Multiply(Mat3x2 x, Mat3x2 y)
			{
				Mat3x2 r;
				r.M11 = (x.M11 * y.M11) + (x.M12 * y.M21);
				r.M12 = (x.M11 * y.M12) + (x.M12 * y.M22);
				r.M21 = (x.M21 * y.M11) + (x.M22 * y.M21);
				r.M22 = (x.M21 * y.M12) + (x.M22 * y.M22);
				r.M31 = (x.M31 * y.M11) + (x.M32 * y.M21) + y.M31;
				r.M32 = (x.M31 * y.M12) + (x.M32 * y.M22) + y.M32;
				return r;
			}

			/// <summary>  
			/// Creates a rotation transformation that has the specified angle. 
			/// </summary>
			/// <param name="angle">The rotation angle in degrees. A positive angle creates a clockwise rotation, and a negative angle creates a counterclockwise rotation.</param>
			static Mat3x2 Rotation(float angle)
			{
				auto radians = (Math::PI * angle) / 180.0;
				auto cos = static_cast<float>(Math::Cos(radians));
				auto sin = static_cast<float>(Math::Sin(radians));

				Mat3x2 result;
				result.M11 = cos;
				result.M12 = sin;
				result.M21 = -sin;
				result.M22 = cos;
				result.M31 = 0;
				result.M32 = 0;
				return result;
			}

			/// <summary> 
			/// Creates a scale transformation that has the specified scale factors. 
			/// </summary>
			/// <param name="scale">The x and y axis scale factors of the scale transformation.</param>
			static Mat3x2 Scale(Vec2 scale)
			{
				D2D1::Matrix3x2F::Scale((D2D1_SIZE_F)scale);
				Mat3x2 r;
				r.M11 = scale.X; r.M12 = 0;
				r.M21 = 0; r.M22 = scale.Y;
				r.M31 = 0; r.M32 = 0;
				return r;
			}

			/// <summary> 
			/// Creates a skew transformation that has the specified x-axis and y-axis values and center point. 
			/// </summary>
			/// <param name="angleX">The x-axis skew angle, which is measured in degrees counterclockwise from the y-axis.</param>
			/// <param name="angleY">The y-axis skew angle, which is measured in degrees clockwise from the x-axis.</param>
			/// <param name="centerPoint">The point about which the skew is performed.</param>
			static Mat3x2 Skew(float angleX, float angleY, Vec2 centerPoint)
			{
				auto centerPoint_n = D2D1::Point2F(centerPoint.X, centerPoint.Y);
				auto mat = D2D1::Matrix3x2F::Skew(angleX, angleY, centerPoint_n);

				Mat3x2 r;
				r.M11 = mat._11; r.M12 = mat._12;
				r.M21 = mat._21; r.M22 = mat._22;
				r.M31 = mat._31; r.M32 = mat._32;
				return r;
			}

			/// <summary> 
			/// Creates a translation transformation that has the specified x and y displacements. 
			/// </summary>
			/// <param name="p">The distance to translate along the x and y axis.</param>
			static Mat3x2 Translation(Vec2 p)
			{
				Mat3x2 r;
				r.M11 = 1; r.M12 = 0;
				r.M21 = 0; r.M22 = 1;
				r.M31 = p.X; r.M32 = p.Y;
				return r;
			}

			/// <summary> 
			/// Transform the specified point. 
			/// </summary>
			/// <param name="m">The matrix to use.</param>
			/// <param name="p">The point to transform.</param>
			static Vec2 TransformPoint(Mat3x2 m, Vec2 p)
			{
				float x = (p.X * m.M11) + (p.Y * m.M21) + m.M31;
				float y = (p.X * m.M12) + (p.Y * m.M22) + m.M32;
				return Vec2(x, y);
			}

			static Mat3x2 operator *(Mat3x2 x, Mat3x2 y) { return Multiply(x, y); }
			static Boolean operator ==(Mat3x2 x, Mat3x2 y) { return x.Equals(y); }
			static Boolean operator !=(Mat3x2 x, Mat3x2 y) { return !x.Equals(y); }

			virtual System::String^ ToString() override
			{
				return String::Format(System::Globalization::CultureInfo::InvariantCulture, "[[M11:{0} M12:{1}] [M21:{2} M22:{3}] [M31:{4} M32:{5}]]",
					M11.ToString(System::Globalization::CultureInfo::InvariantCulture), M12.ToString(System::Globalization::CultureInfo::InvariantCulture),
					M21.ToString(System::Globalization::CultureInfo::InvariantCulture), M22.ToString(System::Globalization::CultureInfo::InvariantCulture),
					M31.ToString(System::Globalization::CultureInfo::InvariantCulture), M32.ToString(System::Globalization::CultureInfo::InvariantCulture));
			}

			virtual Int32 GetHashCode() override
			{
				return M11.GetHashCode() + M12.GetHashCode() + M21.GetHashCode() + M22.GetHashCode() + M31.GetHashCode() + M32.GetHashCode();
			}

			virtual Boolean Equals(System::Object^ obj) override { return obj != nullptr && obj->GetType() == GetType() && Equals(safe_cast<Mat3x2>(obj)); }
			virtual Boolean Equals(Mat3x2 other) {
				return M11 == other.M11 && M12 == other.M12 &&
					M21 == other.M21 && M22 == other.M22 &&
					M31 == other.M31 && M32 == other.M32;
			}
		};

		/// <summary>
		/// Describes a line on a two-dimensional plane.
		/// </summary>
		public value struct Line
		{
			Vec2 begin;
			Vec2 end;

		public:

			/// <summary>
			/// Get or set the start <see cref="Kodo::Graphics::Vec2"/> of the line.
			/// </summary>
			property Vec2 Begin { Vec2 get() { return begin; } void set(Vec2 value) { begin = value; } }
			/// <summary>
			/// Get or set the end <see cref="Kodo::Graphics::Vec2"/> the line.
			/// </summary>
			property Vec2 End { Vec2 get() { return end; } void set(Vec2 value) { end = value; } }

			/// <summary>
			/// Create a new <see cref="Kodo::Graphics::Line"/>.
			/// </summary>
			/// <param name="a">Start <see cref="Kodo::Graphics::Vec2"/> of the line.</param>
			/// <param name="b">End <see cref="Kodo::Graphics::Vec2"/> of the line.</param>
			Line(Vec2 a, Vec2 b) : begin(a), end(b) {}

			/// <summary>
			/// Moves the line.
			/// </summary>
			/// <param name="x">Movement on the x-axis.</param>
			/// <param name="y">Movement on the y-axis.</param>
			Line Move(float x, float y)
			{
				return Line(Vec2(begin.X + x, begin.Y + y), Vec2(end.X + x, end.Y + y));
			}
		};

		/// <summary>
		/// Describes a rectangle on a two-dimensional plane.
		/// </summary>
		[StructLayout(LayoutKind::Sequential)]
		public value struct Rectangle : IEquatable<Rectangle>
		{
		private:

			initonly float left;
			initonly float top;
			initonly float right;
			initonly float bottom;

		public:

			static initonly Rectangle Zero = Rectangle();

			property Vec2 TopLeft { Vec2 get() { return Vec2(left, top); } }
			property Vec2 TopRight { Vec2 get() { return Vec2(right, top); } }
			property Vec2 BottomLeft { Vec2 get() { return Vec2(left, bottom); } }
			property Vec2 BottomRight { Vec2 get() { return Vec2(right, bottom); } }
			property Vec2 Center { Vec2 get() { return Vec2(left + (Width / 2), top + (Height / 2)); } }

			property float Left { float get() { return left; }}
			property float Top { float get() { return top; } }
			property float Right { float get() { return right; } }
			property float Bottom { float get() { return bottom; } }

			/// <summary>
			/// Width of the rectangle.
			/// </summary>
			property float Width { float get() { return (right - left); } }
			/// <summary>
			/// Height of the rectangle.
			/// </summary>
			property float Height { float get() { return (bottom - top); } }

			/// <summary>
			/// Location (upper-left corner) of the rectangle.
			/// </summary>
			property Vec2 Location { Vec2 get() { return Vec2(left, top); } }

			/// <summary>
			/// Dimensions (size and width) of the rectangle.
			/// </summary>
			property Vec2 Dimensions { Vec2 get() { return Vec2(Width, Height); } }

			/// <summary>
			/// Creates a new <see cref="Kodo::Graphics::Rectangle"/> located at (0,0) with the specified dimensions.
			/// </summary>
			/// <param name="dim"><see cref="Vec2"/> that describes the dimensions.</param>
			Rectangle(Vec2 dim) : Rectangle((float)0, (float)0, dim.Width, dim.Height) { }
			/// <summary>
			/// Creates a new <see cref="Kodo::Graphics::Rectangle"/> with the specified location and dimensions.
			/// </summary>
			/// <param name="loc"><see cref="Vec2"/> that describes the location.</param>
			/// <param name="dim"><see cref="Vec2"/> that describes the dimensions.</param>
			Rectangle(Vec2 loc, Vec2 dim) : Rectangle(loc.X, loc.Y, loc.X + dim.Width, loc.Y + dim.Height) { }
			/// <summary>
			/// Creates a new <see cref="Kodo::Graphics::Rectangle"/> with the specified boundary coordinates.
			/// </summary>
			/// <param name="l">The left side.</param>
			/// <param name="t">The top side.</param>
			/// <param name="r">The right side.</param>
			/// <param name="b">The bottom side.</param>
			Rectangle(double l, double t, double r, double b) : Rectangle((float)l, (float)t, (float)r, (float)b) { }
			/// <summary>
			/// Creates a new <see cref="Kodo::Graphics::Rectangle"/> with the specified boundary coordinates.
			/// </summary>
			/// <param name="l">The left side.</param>
			/// <param name="t">The top side.</param>
			/// <param name="r">The right side.</param>
			/// <param name="b">The bottom side.</param>
			Rectangle(int l, int t, int r, int b) : Rectangle((float)l, (float)t, (float)r, (float)b) { }
			/// <summary>
			/// Creates a new <see cref="Kodo::Graphics::Rectangle"/> with the specified boundary coordinates.
			/// </summary>
			/// <param name="l">The left side.</param>
			/// <param name="t">The top side.</param>
			/// <param name="r">The right side.</param>
			/// <param name="b">The bottom side.</param>
			Rectangle(float l, float t, float r, float b)
			{
				left = l;
				top = t;
				right = r;
				bottom = b;
			}

			bool Contains(Vec2 p)
			{
				return (Left <= p.X) && (Top <= p.Y) && (Right >= p.X) && (Bottom >= p.Y);
			}

			Rectangle Crop(float l, float t, float r, float b)
			{
				return Rectangle(left + l, top + t, right - r, bottom - b);
			}

			Rectangle Scale(float factor) { return Scale(factor, factor); }
			Rectangle Scale(float x, float y)
			{
				return Rectangle(left, top, (left + Width * x), (top + Height * y));
			}

			Rectangle Move(float x, float y)
			{
				return Rectangle(left + x, top + y, right + x, bottom + y);
			}

			Rectangle Clamp(Rectangle other)
			{
				auto l = (left < other.left) ? other.left : left;
				auto t = (top < other.top) ? other.top : top;
				auto r = (right > other.right) ? other.right : right;
				auto b = (bottom > other.bottom) ? other.bottom : bottom;
				return Rectangle(l, t, r, b);
			}

			/// <summary>
			/// Center the rectangle on the specified <see cref="Kodo::Graphics::Rectangle"/>.
			/// </summary>
			/// <param name="r"><see cref="Kodo::Graphics::Rectangle"/> to center on.</param>
			Rectangle CenterTo(Rectangle r)
			{
				return Move((r.Left - left) + ((r.Width - Width) / 2), (r.Top - top) + ((r.Height - Height) / 2));
			}
			/// <summary>
			/// Center the rectangle on the specified <see cref="Kodo::Graphics::Vec2"/>.
			/// </summary>
			/// <param name="p"><see cref="Kodo::Graphics::Vec2"/> to center on.</param>
			Rectangle CenterTo(Vec2 p) { return FromXYWH(p.X - Width / 2, p.Y - Height / 2, Width, Height); }

			static Rectangle FromXYWH(float x, float y, float w, float h) { return Rectangle(x, y, x + w, y + h); }
			static Rectangle FromLTRB(float l, float t, float r, float b) { return Rectangle(l, t, r, b); }

			static bool operator !=(Rectangle left, Rectangle right) { return !left.Equals(right); }
			static bool operator ==(Rectangle left, Rectangle right) { return left.Equals(right); }
			virtual bool Equals(Rectangle r) { return (left == r.left && top == r.top && right == r.right && bottom == r.bottom); }
			bool Equals(Object^ obj) override { return obj != nullptr && GetType() == obj->GetType() && Equals((Rectangle)obj); }
			Int32 GetHashCode() override { return left.GetHashCode() + top.GetHashCode() + right.GetHashCode() + bottom.GetHashCode(); }
			String^ ToString() override { return String::Format(System::Globalization::CultureInfo::InvariantCulture, "{0},{1},{2},{3}", left, top, right, bottom); }
		};

		[StructLayout(LayoutKind::Sequential)]
		public value struct Color
		{
		private:
			Vec4 color;

		public:
			property float R { float get() { return color.R; }}
			property float G { float get() { return color.G; }}
			property float B { float get() { return color.B; }}
			property float A { float get() { return color.A; }}

			property Vec3 RGB {Vec3 get() { return Vec3(R, G, B); }}
			property Vec4 RGBA {Vec4 get() { return color; }}

			Color(Color rgb, float a) : color(rgb.RGB, a) { }
			Color(Color rgb, double a) : color(rgb.RGB, (float)a) { }
			Color(UInt32 rgba) : color(((rgba >> 24) & 255) / 255.0f, ((rgba >> 16) & 255) / 255.0f, ((rgba >> 8) & 255) / 255.0f, (rgba & 255) / 255.0f) {}
			Color(float r, float g, float b) : color((float)r, (float)g, (float)b, 1.0f) {}
			Color(float r, float g, float b, float a) : color((float)r, (float)g, (float)b, (float)a) { }
			Color(double r, double g, double b) : color((float)r, (float)g, (float)b, 1.0f) {}
			Color(double r, double g, double b, double a) : color((float)r, (float)g, (float)b, (float)a) { }

			static Color UnpackRGBA(UInt32 rgba) { return Color(rgba); }
			static Color UnpackRGB(UInt32 rgb) { return Color((rgb << 8) | 0xff); }
			static UInt32 PackRGBA(Color color)
			{
				UInt32 value;
				value |= static_cast<UInt32>(color.R * 255.0f) << 24;
				value |= static_cast<UInt32>(color.G * 255.0f) << 16;
				value |= static_cast<UInt32>(color.B * 255.0f) << 8;
				value |= static_cast<UInt32>(color.A * 255.0f) << 0;
				return value;
			}

			static Color Lerp(Color color1, Color color2, float amount)
			{
				float r = color1.R + amount * (color2.R - color1.R);
				float g = color1.G + amount * (color2.G - color1.G);
				float b = color1.B + amount * (color2.B - color1.B);
				float a = color1.A + amount * (color2.A - color1.A);
				return Color(r, g, b, a);
			}

			static initonly Color AliceBlue = Color(0xF0F8FFFF);
			static initonly Color AntiqueWhite = Color(0xFAEBD7FF);
			static initonly Color Aqua = Color(0x00FFFFFF);
			static initonly Color Aquamarine = Color(0x7FFFD4FF);
			static initonly Color Azure = Color(0xF0FFFFFF);
			static initonly Color Beige = Color(0xF5F5DCFF);
			static initonly Color Bisque = Color(0xFFE4C4FF);
			static initonly Color Black = Color(0x000000FF);
			static initonly Color BlanchedAlmond = Color(0xFFEBCDFF);
			static initonly Color BlueViolet = Color(0x8A2BE2FF);
			static initonly Color Blue = Color(0x0000FFFF);
			static initonly Color Brown = Color(0xA52A2AFF);
			static initonly Color BurlyWood = Color(0xDEB887FF);
			static initonly Color CadetBlue = Color(0x5F9EA0FF);
			static initonly Color Chartreuse = Color(0x7FFF00FF);
			static initonly Color Chocolate = Color(0xD2691EFF);
			static initonly Color Coral = Color(0xFF7F50FF);
			static initonly Color CornflowerBlue = Color(0x6495EDFF);
			static initonly Color Cornsilk = Color(0xFFF8DCFF);
			static initonly Color Crimson = Color(0xDC143CFF);
			static initonly Color Cyan = Color(0x00FFFFFF);
			static initonly Color DarkBlue = Color(0x00008BFF);
			static initonly Color DarkCyan = Color(0x008B8BFF);
			static initonly Color DarkGoldenrod = Color(0xB8860BFF);
			static initonly Color DarkGray = Color(0xA9A9A9FF);
			static initonly Color DarkGreen = Color(0x006400FF);
			static initonly Color DarkKhaki = Color(0xBDB76BFF);
			static initonly Color DarkMagenta = Color(0x8B008BFF);
			static initonly Color DarkOliveGreen = Color(0x556B2FFF);
			static initonly Color DarkOrange = Color(0xFF8C00FF);
			static initonly Color DarkOrchid = Color(0x9932CCFF);
			static initonly Color DarkRed = Color(0x8B0000FF);
			static initonly Color DarkSalmon = Color(0xE9967AFF);
			static initonly Color DarkSeaGreen = Color(0x8FBC8FFF);
			static initonly Color DarkSlateBlue = Color(0x483D8BFF);
			static initonly Color DarkSlateGray = Color(0x2F4F4FFF);
			static initonly Color DarkTurquoise = Color(0x00CED1FF);
			static initonly Color DarkViolet = Color(0x9400D3FF);
			static initonly Color DeepPink = Color(0xFF1493FF);
			static initonly Color DeepSkyBlue = Color(0x00BFFFFF);
			static initonly Color DimGray = Color(0x696969FF);
			static initonly Color DodgerBlue = Color(0x1E90FFFF);
			static initonly Color Firebrick = Color(0xB22222FF);
			static initonly Color FloralWhite = Color(0xFFFAF0FF);
			static initonly Color ForestGreen = Color(0x228B22FF);
			static initonly Color Fuchsia = Color(0xFF00FFFF);
			static initonly Color Gainsboro = Color(0xDCDCDCFF);
			static initonly Color GhostWhite = Color(0xF8F8FFFF);
			static initonly Color Gold = Color(0xFFD700FF);
			static initonly Color Goldenrod = Color(0xDAA520FF);
			static initonly Color Gray = Color(0x808080FF);
			static initonly Color Green = Color(0x00FF00FF);
			static initonly Color GreenYellow = Color(0xADFF2FFF);
			static initonly Color Honeydew = Color(0xF0FFF0FF);
			static initonly Color HotPink = Color(0xFF69B4FF);
			static initonly Color IndianRed = Color(0xCD5C5CFF);
			static initonly Color Indigo = Color(0x4B0082FF);
			static initonly Color Ivory = Color(0xFFFFF0FF);
			static initonly Color Khaki = Color(0xF0E68CFF);
			static initonly Color Lavender = Color(0xE6E6FAFF);
			static initonly Color LavenderBlush = Color(0xFFF0F5FF);
			static initonly Color LawnGreen = Color(0x7CFC00FF);
			static initonly Color LemonChiffon = Color(0xFFFACDFF);
			static initonly Color LightBlue = Color(0xADD8E6FF);
			static initonly Color LightCoral = Color(0xF08080FF);
			static initonly Color LightCyan = Color(0xE0FFFFFF);
			static initonly Color LightGoldenrodYellow = Color(0xFAFAD2FF);
			static initonly Color LightGray = Color(0xD3D3D3FF);
			static initonly Color LightGreen = Color(0x90EE90FF);
			static initonly Color LightPink = Color(0xFFB6C1FF);
			static initonly Color LightSalmon = Color(0xFFA07AFF);
			static initonly Color LightSeaGreen = Color(0x20B2AAFF);
			static initonly Color LightSkyBlue = Color(0x87CEFAFF);
			static initonly Color LightSlateGray = Color(0x778899FF);
			static initonly Color LightSteelBlue = Color(0xB0C4DEFF);
			static initonly Color LightYellow = Color(0xFFFFE0FF);
			static initonly Color Lime = Color(0x00FF00FF);
			static initonly Color LimeGreen = Color(0x32CD32FF);
			static initonly Color Linen = Color(0xFAF0E6FF);
			static initonly Color Magenta = Color(0xFF00FFFF);
			static initonly Color Maroon = Color(0x800000FF);
			static initonly Color MediumAquamarine = Color(0x66CDAAFF);
			static initonly Color MediumBlue = Color(0x0000CDFF);
			static initonly Color MediumOrchid = Color(0xBA55D3FF);
			static initonly Color MediumPurple = Color(0x9370DBFF);
			static initonly Color MediumSeaGreen = Color(0x3CB371FF);
			static initonly Color MediumSlateBlue = Color(0x7B68EEFF);
			static initonly Color MediumSpringGreen = Color(0x00FA9AFF);
			static initonly Color MediumTurquoise = Color(0x48D1CCFF);
			static initonly Color MediumVioletRed = Color(0xC71585FF);
			static initonly Color MidnightBlue = Color(0x191970FF);
			static initonly Color MintCream = Color(0xF5FFFAFF);
			static initonly Color MistyRose = Color(0xFFE4E1FF);
			static initonly Color Moccasin = Color(0xFFE4B5FF);
			static initonly Color NavajoWhite = Color(0xFFDEADFF);
			static initonly Color Navy = Color(0x000080FF);
			static initonly Color OldLace = Color(0xFDF5E6FF);
			static initonly Color Olive = Color(0x808000FF);
			static initonly Color OliveDrab = Color(0x6B8E23FF);
			static initonly Color Orange = Color(0xFFA500FF);
			static initonly Color OrangeRed = Color(0xFF4500FF);
			static initonly Color Orchid = Color(0xDA70D6FF);
			static initonly Color PaleGoldenrod = Color(0xEEE8AAFF);
			static initonly Color PaleGreen = Color(0x98FB98FF);
			static initonly Color PaleTurquoise = Color(0xAFEEEEFF);
			static initonly Color PaleVioletRed = Color(0xDB7093FF);
			static initonly Color PapayaWhip = Color(0xFFEFD5FF);
			static initonly Color PeachPuff = Color(0xFFDAB9FF);
			static initonly Color Peru = Color(0xCD853FFF);
			static initonly Color Pink = Color(0xFFC0CBFF);
			static initonly Color Plum = Color(0xDDA0DDFF);
			static initonly Color PowderBlue = Color(0xB0E0E6FF);
			static initonly Color Purple = Color(0x800080FF);
			static initonly Color Red = Color(0xFF0000FF);
			static initonly Color RosyBrown = Color(0xBC8F8FFF);
			static initonly Color RoyalBlue = Color(0x4169E1FF);
			static initonly Color SaddleBrown = Color(0x8B4513FF);
			static initonly Color Salmon = Color(0xFA8072FF);
			static initonly Color SandyBrown = Color(0xF4A460FF);
			static initonly Color SeaGreen = Color(0x2E8B57FF);
			static initonly Color SeaShell = Color(0xFFF5EEFF);
			static initonly Color Sienna = Color(0xA0522DFF);
			static initonly Color Silver = Color(0xC0C0C0FF);
			static initonly Color SkyBlue = Color(0x87CEEBFF);
			static initonly Color SlateBlue = Color(0x6A5ACDFF);
			static initonly Color SlateGray = Color(0x708090FF);
			static initonly Color Snow = Color(0xFFFAFAFF);
			static initonly Color SpringGreen = Color(0x00FF7FFF);
			static initonly Color SteelBlue = Color(0x4682B4FF);
			static initonly Color Tan = Color(0xD2B48CFF);
			static initonly Color Teal = Color(0x008080FF);
			static initonly Color Thistle = Color(0xD8BFD8FF);
			static initonly Color Tomato = Color(0xFF6347FF);
			static initonly Color Transparent = Color(0x00FFFFFFFF);
			static initonly Color Turquoise = Color(0x40E0D0FF);
			static initonly Color Violet = Color(0xEE82EEFF);
			static initonly Color Wheat = Color(0xF5DEB3FF);
			static initonly Color White = Color(0xFFFFFFFF);
			static initonly Color WhiteSmoke = Color(0xF5F5F5FF);
			static initonly Color Yellow = Color(0xFFFF00FF);
			static initonly Color YellowGreen = Color(0xCD32FF9A);
		};

		public enum class DxgiFormat : System::Int32
		{
			Unknown = DXGI_FORMAT_UNKNOWN,

			NV12 = DXGI_FORMAT_NV12,
			R8_UNORM = DXGI_FORMAT_R8_UNORM,
			R8G8_UNORM = DXGI_FORMAT_R8G8_UNORM,
			B8G8R8A8_UNORM = DXGI_FORMAT_B8G8R8A8_UNORM,

			R32G32B32A32_Typeless = DXGI_FORMAT_R32G32B32A32_TYPELESS,
			R32G32B32A32_Float = DXGI_FORMAT_R32G32B32A32_FLOAT,
			R32G32B32A32_UInt = DXGI_FORMAT_R32G32B32A32_UINT,
			R32G32B32A32_SInt = DXGI_FORMAT_R32G32B32A32_SINT,
			R32G32B32_Typeless = DXGI_FORMAT_R32G32B32_TYPELESS,
			R32G32B32_Float = DXGI_FORMAT_R32G32B32_FLOAT,
			R32G32B32_UInt = DXGI_FORMAT_R32G32B32_UINT,
			R32G32B32_SInt = DXGI_FORMAT_R32G32B32_SINT,
			R16G16B16A16_Typeless = DXGI_FORMAT_R16G16B16A16_TYPELESS,
			R16G16B16A16_Float = DXGI_FORMAT_R16G16B16A16_FLOAT,
			R16G16B16A16_UNorm = DXGI_FORMAT_R16G16B16A16_UNORM,
			R16G16B16A16_UInt = DXGI_FORMAT_R16G16B16A16_UINT,
			R16G16B16A16_SNorm = DXGI_FORMAT_R16G16B16A16_SNORM,
			R16G16B16A16_SInt = DXGI_FORMAT_R16G16B16A16_SINT,
			R32G32_Typeless = DXGI_FORMAT_R32G32_TYPELESS,
			R32G32_Float = DXGI_FORMAT_R32G32_FLOAT,
			R32G32_UInt = DXGI_FORMAT_R32G32_UINT,
			R32G32_SInt = DXGI_FORMAT_R32G32_SINT,
			R32G8X24_Typeless = DXGI_FORMAT_R32G8X24_TYPELESS,
			D32_Float_S8X24_UInt = DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
			R32_Float_X8X24_Typeless = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS,
			X32_Typeless_G8X24_UInt = DXGI_FORMAT_X32_TYPELESS_G8X24_UINT,
			R10G10B10A2_Typeless = DXGI_FORMAT_R10G10B10A2_TYPELESS,
			R10G10B10A2_UNorm = DXGI_FORMAT_R10G10B10A2_UNORM,
			R10G10B10A2_UInt = DXGI_FORMAT_R10G10B10A2_UINT,
			R11G11B10_Float = DXGI_FORMAT_R11G11B10_FLOAT,
			R8G8B8A8_Typeless = DXGI_FORMAT_R8G8B8A8_TYPELESS,
			R8G8B8A8_UNorm = DXGI_FORMAT_R8G8B8A8_UNORM,
			R8G8B8A8_UNorm_SRGB = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			R8G8B8A8_UInt = DXGI_FORMAT_R8G8B8A8_UINT,
			R8G8B8A8_SNorm = DXGI_FORMAT_R8G8B8A8_SNORM,
			R8G8B8A8_SInt = DXGI_FORMAT_R8G8B8A8_SINT,
			R16G16_Typeless = DXGI_FORMAT_R16G16_TYPELESS,
			R16G16_Float = DXGI_FORMAT_R16G16_FLOAT,
			R16G16_UNorm = DXGI_FORMAT_R16G16_UNORM,
			R16G16_UInt = DXGI_FORMAT_R16G16_UINT,
			R16G16_SNorm = DXGI_FORMAT_R16G16_SNORM,
			R16G16_SInt = DXGI_FORMAT_R16G16_SINT,
			R32_Typeless = DXGI_FORMAT_R32_TYPELESS,
			D32_Float = DXGI_FORMAT_D32_FLOAT,
			R32_Float = DXGI_FORMAT_R32_FLOAT,
			R32_UInt = DXGI_FORMAT_R32_UINT,
			R32_SInt = DXGI_FORMAT_R32_SINT,
			R24G8_Typeless = DXGI_FORMAT_R24G8_TYPELESS,
			D24_UNorm_S8_UInt = DXGI_FORMAT_D24_UNORM_S8_UINT,
			R24_UNorm_X8_Typeless = DXGI_FORMAT_R24_UNORM_X8_TYPELESS,
			X24_Typeless_G8_UInt = DXGI_FORMAT_X24_TYPELESS_G8_UINT,
			R8G8_Typeless = DXGI_FORMAT_R8G8_TYPELESS,
			R8G8_UInt = DXGI_FORMAT_R8G8_UINT,
			R8G8_SNorm = DXGI_FORMAT_R8G8_SNORM,
			R8G8_SInt = DXGI_FORMAT_R8G8_SINT,
			R16_Typeless = DXGI_FORMAT_R16_TYPELESS,
			R16_Float = DXGI_FORMAT_R16_FLOAT,
			D16_UNorm = DXGI_FORMAT_D16_UNORM,
			R16_UNorm = DXGI_FORMAT_R16_UNORM,
			R16_UInt = DXGI_FORMAT_R16_UINT,
			R16_SNorm = DXGI_FORMAT_R16_SNORM,
			R16_SInt = DXGI_FORMAT_R16_SINT,
			R8_Typeless = DXGI_FORMAT_R8_TYPELESS,
			R8_UInt = DXGI_FORMAT_R8_UINT,
			R8_SNorm = DXGI_FORMAT_R8_SNORM,
			R8_SInt = DXGI_FORMAT_R8_SINT,
			A8_UNorm = DXGI_FORMAT_A8_UNORM,
			R1_UNorm = DXGI_FORMAT_R1_UNORM,
			R9G9B9E5_SharedExp = DXGI_FORMAT_R9G9B9E5_SHAREDEXP,
			R8G8_B8G8_UNorm = DXGI_FORMAT_R8G8_B8G8_UNORM,
			G8R8_G8B8_UNorm = DXGI_FORMAT_G8R8_G8B8_UNORM,
			BC1_Typeless = DXGI_FORMAT_BC1_TYPELESS,
			BC1_UNorm = DXGI_FORMAT_BC1_UNORM,
			BC1_UNorm_SRGB = DXGI_FORMAT_BC1_UNORM_SRGB,
			BC2_Typeless = DXGI_FORMAT_BC2_TYPELESS,
			BC2_UNorm = DXGI_FORMAT_BC2_UNORM,
			BC2_UNorm_SRGB = DXGI_FORMAT_BC2_UNORM_SRGB,
			BC3_Typeless = DXGI_FORMAT_BC3_TYPELESS,
			BC3_UNorm = DXGI_FORMAT_BC3_UNORM,
			BC3_UNorm_SRGB = DXGI_FORMAT_BC3_UNORM_SRGB,
			BC4_Typeless = DXGI_FORMAT_BC4_TYPELESS,
			BC4_UNorm = DXGI_FORMAT_BC4_UNORM,
			BC4_SNorm = DXGI_FORMAT_BC4_SNORM,
			BC5_Typeless = DXGI_FORMAT_BC5_TYPELESS,
			BC5_UNorm = DXGI_FORMAT_BC5_UNORM,
			BC5_SNorm = DXGI_FORMAT_BC5_SNORM,
			B5G6R5_UNorm = DXGI_FORMAT_B5G6R5_UNORM,
			B5G5R5A1_UNorm = DXGI_FORMAT_B5G5R5A1_UNORM,
			B8G8R8X8_UNorm = DXGI_FORMAT_B8G8R8X8_UNORM,
			R10G10B10_XR_Bias_A2_UNorm = DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM,
			B8G8R8A8_Typeless = DXGI_FORMAT_B8G8R8A8_TYPELESS,
			B8G8R8A8_UNorm_SRGB = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
			B8G8R8X8_Typeless = DXGI_FORMAT_B8G8R8X8_TYPELESS,
			B8G8R8X8_UNorm_SRGB = DXGI_FORMAT_B8G8R8X8_UNORM_SRGB,
			BC6_Typeless = DXGI_FORMAT_BC6H_TYPELESS,
			BC6_UFloat16 = DXGI_FORMAT_BC6H_UF16,
			BC6_SFloat16 = DXGI_FORMAT_BC6H_SF16,
			BC7_Typeless = DXGI_FORMAT_BC7_TYPELESS,
			BC7_UNorm = DXGI_FORMAT_BC7_UNORM,
			BC7_UNorm_SRGB = DXGI_FORMAT_BC7_UNORM_SRGB
		};
	}
}
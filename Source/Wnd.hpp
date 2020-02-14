#include "Graphics.hpp"

#include "D3D/D3D.hpp"
#include "D2D/D2D.hpp"
#include "DW/DW.hpp"
#include "WIC/WIC.hpp"

#include <fstream>
#include <sstream>

struct VertexOfModel
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 Texture;
};

static bool readObj(std::wstring objPath, std::vector<VertexOfModel>& vertices)
{
	UINT numberOfFaces = 0;
	std::vector<UINT> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> verticesTemp;
	std::vector<glm::vec2> uvsTemp;
	std::vector<glm::vec3> normalsTemp;
	glm::vec3 vertex;
	glm::vec2 uv;
	glm::vec3 normal;

	std::string objLine;
	std::ifstream objStream(objPath, std::ifstream::in | std::ifstream::binary);
	if (!objStream)
		return false;

	while (objStream.eof() == false)
	{
		std::getline(objStream, objLine);

		if (objLine.compare(0, 2, "v ", 2) == 0)
		{
			std::stringstream(objLine.substr(2))
				>> vertex.x >> vertex.z >> vertex.y;

			verticesTemp.push_back(vertex);
		}
		else if (objLine.compare(0, 3, "vt ", 3) == 0)
		{
			std::stringstream(objLine.substr(3))
				>> uv.x >> uv.y;

			uvsTemp.push_back(glm::vec2(uv.x, glm::abs(uv.y - 1.0f)));
		}
		else if (objLine.compare(0, 3, "vn ", 3) == 0)
		{
			std::stringstream(objLine.substr(3))
				>> normal.x >> normal.z >> normal.y;

			normalsTemp.push_back(normal);
		}
		else if (objLine.compare(0, 2, "f ", 2) == 0)
		{
			CHAR c;
			UINT vertexIndex1, uvIndex1, normalIndex1;
			UINT vertexIndex2, uvIndex2, normalIndex2;
			UINT vertexIndex3, uvIndex3, normalIndex3;

			std::stringstream(objLine.substr(2))
				>> vertexIndex1 >> c >> uvIndex1 >> c >> normalIndex1
				>> vertexIndex2 >> c >> uvIndex2 >> c >> normalIndex2
				>> vertexIndex3 >> c >> uvIndex3 >> c >> normalIndex3;

			vertexIndices.push_back(vertexIndex1);
			vertexIndices.push_back(vertexIndex2);
			vertexIndices.push_back(vertexIndex3);
			uvIndices.push_back(uvIndex1);
			uvIndices.push_back(uvIndex2);
			uvIndices.push_back(uvIndex3);
			normalIndices.push_back(normalIndex1);
			normalIndices.push_back(normalIndex2);
			normalIndices.push_back(normalIndex3);
			numberOfFaces++;
		}
	}

	for (UINT i = 0; i < numberOfFaces * 3; i++)
	{
		VertexOfModel vertex;
		vertex.Position = verticesTemp[vertexIndices[i] - 1];
		vertex.Normal = normalsTemp[normalIndices[i] - 1];
		vertex.Texture = uvsTemp[uvIndices[i] - 1];
		vertices.push_back(vertex);
	}

	return true;
}

static BOOL createTextureFromDDS(ID3D11Device* d3dDevice, std::wstring fileName, ID3D11ShaderResourceView** textureView)
{
	struct DDS_PIXELFORMAT
	{
		uint32_t    size;
		uint32_t    flags;
		uint32_t    fourCC;
		uint32_t    RGBBitCount;
		uint32_t    RBitMask;
		uint32_t    GBitMask;
		uint32_t    BBitMask;
		uint32_t    ABitMask;
	};

	struct DDS_HEADER
	{
		uint32_t        size;
		uint32_t        flags;
		uint32_t        height;
		uint32_t        width;
		uint32_t        pitchOrLinearSize;
		uint32_t        depth;
		uint32_t        mipMapCount;
		uint32_t        reserved1[11];
		DDS_PIXELFORMAT ddspf;
		uint32_t        caps;
		uint32_t        caps2;
		uint32_t        caps3;
		uint32_t        caps4;
		uint32_t        reserved2;
	};

	HRESULT hr;
	UINT textureWidth, textureHeight;
	std::vector<uint8_t> textureBytes;

	{
		std::ifstream ddsStream(fileName, std::ifstream::in | std::ifstream::binary);
		if (!ddsStream) return false;
		std::size_t ddsLength = static_cast<std::size_t>(ddsStream.seekg(0, ddsStream.end).tellg());
		ddsStream.seekg(0, ddsStream.beg);

		textureBytes.resize(ddsLength);
		ddsStream.read((char*)&textureBytes[0], ddsLength);

		auto ddsMagic = *(const uint32_t*)(&textureBytes[0]);
		auto ddsHeader = reinterpret_cast<DDS_HEADER*>((&textureBytes[0]) + sizeof(uint32_t));

		if (ddsMagic != 0x20534444)
			return false;
		if ((ddsHeader->size != sizeof(DDS_HEADER) || ddsHeader->ddspf.size != sizeof(DDS_PIXELFORMAT)))
			return false;
		if (ddsHeader->depth > 1)
			return false;
		if ((ddsHeader->ddspf.flags & 0x00000040) && ddsHeader->ddspf.RGBBitCount != 32 && !(ddsHeader->ddspf.RBitMask == 0x00ff0000 && ddsHeader->ddspf.GBitMask == 0x0000ff00 && ddsHeader->ddspf.BBitMask == 0x000000ff && ddsHeader->ddspf.ABitMask == 0xff000000))
			return false;
		if ((ddsHeader->width > D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION) || (ddsHeader->height > D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION))
			return false;

		textureWidth = ddsHeader->width;
		textureHeight = ddsHeader->height;
	}

	std::size_t	rowBytes = (textureWidth * 32 + 7) / 8;
	std::size_t	numBytes = rowBytes * textureHeight;

	D3D11_SUBRESOURCE_DATA textureData;
	textureData.pSysMem = (const void*)((&textureBytes[0]) + (sizeof(uint32_t) + sizeof(DDS_HEADER)));
	textureData.SysMemPitch = static_cast<UINT>(rowBytes);
	textureData.SysMemSlicePitch = static_cast<UINT>(numBytes);

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;

	com_ptr<ID3D11Texture2D> texture;
	hr = d3dDevice->CreateTexture2D(&textureDesc, &textureData, &texture);
	if (FAILED(hr)) return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc;
	ZeroMemory(&textureViewDesc, sizeof(textureViewDesc));
	textureViewDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	textureViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;
	hr = d3dDevice->CreateShaderResourceView(texture, &textureViewDesc, textureView);
	if (FAILED(hr)) return false;

	return true;
}

namespace Kodo
{
	namespace Graphics
	{
		using namespace System::Collections::Generic;
		using namespace System::Security;
		using namespace System::Runtime::InteropServices;
		using namespace System::IO;

		using namespace Imaging;

		public ref class LoaderOfBitmap
		{
		public:

			static Bitmap^ Load(D2DDeviceContext^ d2dContext, Type^ resourceType, String^ resourceName, Vec2 scaledSize)
			{
				WicStream stream(resourceType, resourceName);
				WicDecoder decoder(% stream, WicDecodeOptions::MetadataCacheOnDemand);
				WicFormatConverter converter;
				converter.Convert(decoder.GetFrame(0), WicPixelFormat::PBGRA32bpp);
				WicScaler scaler;
				scaler.Scale(% converter, (int)scaledSize.Width, (int)scaledSize.Height, WicInterpolationMode::Fant);

				Bitmap^ result = gcnew Bitmap(d2dContext, % scaler);
				return result;
			}

			static Bitmap^ Load(D2DDeviceContext^ d2dContext, String^ filename, Vec2 scaledSize)
			{
				WicStream stream(filename);
				WicDecoder decoder(% stream, WicDecodeOptions::MetadataCacheOnDemand);
				WicFormatConverter converter;
				converter.Convert(decoder.GetFrame(0), WicPixelFormat::PBGRA32bpp);
				WicScaler scaler;
				scaler.Scale(% converter, (int)scaledSize.Width, (int)scaledSize.Height, WicInterpolationMode::Fant);

				Bitmap^ result = gcnew Bitmap(d2dContext, % scaler);
				return result;
			}
		};

		public ref class LoaderOfCSO
		{
		public:
			static array<Byte>^ Load(String^ filename)
			{
				FileStream csoStream(filename, FileMode::Open);
				auto result = gcnew array<Byte>((UINT)csoStream.Length);
				csoStream.Read(result, 0, (UINT)csoStream.Length);
				return result;
			}
		};

		public ref class LoaderOfDDS
		{
		public:
			static D3DShaderResourceView^ Load(D3DDevice^ d3dDevice, String^ filename)
			{
				pin_ptr<const wchar_t> pinnedFilename = PtrToStringChars(filename);

				ID3D11ShaderResourceView* shaderResourceView;
				createTextureFromDDS(d3dDevice->Native, &pinnedFilename[0], &shaderResourceView);
				return gcnew D3DShaderResourceView(shaderResourceView);
			}
		};

		public ref class LoaderOfOBJ
		{
		public:
			[StructLayout(LayoutKind::Sequential)]
			value struct Vertex
			{
				Vec3 Position;
				Vec3 Normal;
				Vec2 Texture;
			};

			generic<typename T> static array<T>^ Load(String^ filename, Func<Vertex, T>^ converter)
			{
				pin_ptr<const wchar_t> pinnedPath = PtrToStringChars(filename);

				std::vector<VertexOfModel> verts;
				ThrowIfZero(readObj(pinnedPath, verts));

				auto result = gcnew array<T>(verts.size());

				for (std::size_t i = 0; i < verts.size(); i++)
				{
					Vertex v;
					v.Position = Vec3(verts[i].Position.x, verts[i].Position.y, verts[i].Position.z);
					v.Normal = Vec3(verts[i].Normal.x, verts[i].Normal.y, verts[i].Normal.z);
					v.Texture = Vec2(verts[i].Texture.x, verts[i].Texture.y);
					result[i] = converter(v);
				}

				return result;
			}
		};

		ref class Wnd;
		public ref class WndManager
		{
			System::Collections::Generic::List<Wnd^> wnds;

		public:

			WndManager();
			~WndManager();

			void PollEvents() { glfwPollEvents(); }
			void WaitEvents() { glfwWaitEvents(); }

		internal:

			void AddWindow(Wnd^ wnd);
		};

		public enum class InputButton
		{
			X1 = GLFW_MOUSE_BUTTON_1,
			X2 = GLFW_MOUSE_BUTTON_2,
			X3 = GLFW_MOUSE_BUTTON_3,
			X4 = GLFW_MOUSE_BUTTON_4,
			X5 = GLFW_MOUSE_BUTTON_5,
			X6 = GLFW_MOUSE_BUTTON_6,
			X7 = GLFW_MOUSE_BUTTON_7,
			X8 = GLFW_MOUSE_BUTTON_8,
			Left = GLFW_MOUSE_BUTTON_LEFT,
			Right = GLFW_MOUSE_BUTTON_RIGHT,
			Middle = GLFW_MOUSE_BUTTON_MIDDLE,
		};

		public enum class InputAction
		{
			Release = GLFW_RELEASE,
			Press = GLFW_PRESS,
			Repeat = GLFW_REPEAT,
		};

		[Flags]
		public enum class InputMod
		{
			Control = GLFW_MOD_CONTROL,
			Shift = GLFW_MOD_SHIFT,
			Alt = GLFW_MOD_ALT,
			Super = GLFW_MOD_SUPER,
		};

		public enum class MouseMode
		{
			Normal = GLFW_CURSOR_NORMAL,
			Hidden = GLFW_CURSOR_HIDDEN,
			Disable = GLFW_CURSOR_DISABLED
		};

		public enum class InputKey
		{
			UNKNOWN = -1,
			SPACE = 32,
			APOSTROPHE = 39,
			COMMA = 44,
			MINUS = 45,
			PERIOD = 46,
			SLASH = 47,
			_0 = 48,
			_1 = 49,
			_2 = 50,
			_3 = 51,
			_4 = 52,
			_5 = 53,
			_6 = 54,
			_7 = 55,
			_8 = 56,
			_9 = 57,
			SEMICOLON = 59,
			EQUAL = 61,
			A = 65,
			B = 66,
			C = 67,
			D = 68,
			E = 69,
			F = 70,
			G = 71,
			H = 72,
			I = 73,
			J = 74,
			K = 75,
			L = 76,
			M = 77,
			N = 78,
			O = 79,
			P = 80,
			Q = 81,
			R = 82,
			S = 83,
			T = 84,
			U = 85,
			V = 86,
			W = 87,
			X = 88,
			Y = 89,
			Z = 90,
			LEFT_BRACKET = 91,
			BACKSLASH = 92,
			RIGHT_BRACKET = 93,
			GRAVE_ACCENT = 96,
			WORLD_1 = 161,
			WORLD_2 = 162,

			ESCAPE = 256,
			ENTER = 257,
			TAB = 258,
			BACKSPACE = 259,
			INSERT = 260,
			_DELETE = 261,
			RIGHT = 262,
			LEFT = 263,
			DOWN = 264,
			UP = 265,
			PAGE_UP = 266,
			PAGE_DOWN = 267,
			HOME = 268,
			END = 269,
			CAPS_LOCK = 280,
			SCROLL_LOCK = 281,
			NUM_LOCK = 282,
			PRINT_SCREEN = 283,
			PAUSE = 284,
			F1 = 290,
			F2 = 291,
			F3 = 292,
			F4 = 293,
			F5 = 294,
			F6 = 295,
			F7 = 296,
			F8 = 297,
			F9 = 298,
			F10 = 299,
			F11 = 300,
			F12 = 301,
			F13 = 302,
			F14 = 303,
			F15 = 304,
			F16 = 305,
			F17 = 306,
			F18 = 307,
			F19 = 308,
			F20 = 309,
			F21 = 310,
			F22 = 311,
			F23 = 312,
			F24 = 313,
			F25 = 314,
			KP_0 = 320,
			KP_1 = 321,
			KP_2 = 322,
			KP_3 = 323,
			KP_4 = 324,
			KP_5 = 325,
			KP_6 = 326,
			KP_7 = 327,
			KP_8 = 328,
			KP_9 = 329,
			KP_DECIMAL = 330,
			KP_DIVIDE = 331,
			KP_MULTIPLY = 332,
			KP_SUBTRACT = 333,
			KP_ADD = 334,
			KP_ENTER = 335,
			KP_EQUAL = 336,
			LEFT_SHIFT = 340,
			LEFT_CONTROL = 341,
			LEFT_ALT = 342,
			LEFT_SUPER = 343,
			RIGHT_SHIFT = 344,
			RIGHT_CONTROL = 345,
			RIGHT_ALT = 346,
			RIGHT_SUPER = 347,
			MENU = 348,
		};

		ref class WndElement;
		ref class WndRoot;
		ref class Wnd2DElement;
		ref class Wnd3DElement;

		public ref class WndContext
		{

		};

		public ref class Wnd abstract
		{
			enum class FrameState
			{
				Skip, Part, Whole
			};

			enum class SuspendState
			{
				None,
				Load,
				LoadElement,
				Update,
				UpdateElement
			};

		internal:

			[UnmanagedFunctionPointer(CallingConvention::Cdecl), SuppressUnmanagedCodeSecurity]
			delegate LRESULT WndProcDelegate(HWND, UINT, LPARAM, WPARAM);
			[UnmanagedFunctionPointer(CallingConvention::Cdecl), SuppressUnmanagedCodeSecurity]
			delegate void CallbackOfRefreshDelegate(GLFWwindow*);
			[UnmanagedFunctionPointer(CallingConvention::Cdecl), SuppressUnmanagedCodeSecurity]
			delegate void CallbackOfMouseContainedDelegate(GLFWwindow*, bool);
			[UnmanagedFunctionPointer(CallingConvention::Cdecl), SuppressUnmanagedCodeSecurity]
			delegate void CallbackOfMouseMoveDelegate(GLFWwindow*, double, double);
			[UnmanagedFunctionPointer(CallingConvention::Cdecl), SuppressUnmanagedCodeSecurity]
			delegate void CallbackOfMouseButtonDelegate(GLFWwindow*, int, int, int);
			[UnmanagedFunctionPointer(CallingConvention::Cdecl), SuppressUnmanagedCodeSecurity]
			delegate void CallbackOfSizeCallbackDelegate(GLFWwindow*, int, int);
			[UnmanagedFunctionPointer(CallingConvention::Cdecl), SuppressUnmanagedCodeSecurity]
			delegate void CallbackOfScrollDelegate(GLFWwindow*, double, double);
			[UnmanagedFunctionPointer(CallingConvention::Cdecl), SuppressUnmanagedCodeSecurity]
			delegate void CallbackOfKeyDelegate(GLFWwindow*, int, int, int, int);
			[UnmanagedFunctionPointer(CallingConvention::Cdecl), SuppressUnmanagedCodeSecurity]
			delegate void CallbackOfIconifyDelegate(GLFWwindow*, int);
			[UnmanagedFunctionPointer(CallingConvention::Cdecl), SuppressUnmanagedCodeSecurity]
			delegate void CallbackOfCloseDelegate(GLFWwindow*);

			WndProcDelegate^ WndProc_;
			HandleRef WndProc_Ref;
			CallbackOfRefreshDelegate^ CallbackOfRefresh_;
			HandleRef CallbackOfRefresh_Ref;
			CallbackOfMouseContainedDelegate^ CallbackOfMouseContained_;
			HandleRef CallbackOfMouseContained_Ref;
			CallbackOfMouseMoveDelegate^ CallbackOfMouseMove_;
			HandleRef CallbackOfMouseMove_Ref;
			CallbackOfMouseButtonDelegate^ CallbackOfMouseButton_;
			HandleRef CallbackOfMouseButton_Ref;
			CallbackOfSizeCallbackDelegate^ CallbackOfSize_;
			HandleRef CallbackOfSize_Ref;
			CallbackOfScrollDelegate^ CallbackOfScroll_;
			HandleRef CallbackOfScroll_Ref;
			CallbackOfKeyDelegate^ CallbackOfKey_;
			HandleRef CallbackOfKey_Ref;
			CallbackOfIconifyDelegate^ CallbackOfIconify_;
			HandleRef CallbackOfIconify_Ref;
			CallbackOfCloseDelegate^ CallbackOfClose_;
			HandleRef CallbackOfClose_Ref;

			HWND windowOfWin32;
			GLFWwindow* windowOfGLFW;
			LRESULT(CALLBACK* wndProcOfGLFW) (HWND, UINT, WPARAM, LPARAM);

			HWND windowOfWin322;

			//List<WndElement^> windowElements;
			WndElement^ elementThatCapturedMouse = nullptr;
			WndElement^ elementThatHasMouse = nullptr;
			WndElement^ elementThatHasKeyboard = nullptr;

			//List<Wnd2DElement^> window2DElements;
			List<Wnd3DElement^> window3DElements;

			Color windowBorderColor;
			Color windowClearColor;
			Vec4 windowMargins;

			// full size of the window
			Rectangle windowArea;
			// full size of the window minus margins
			Rectangle windowContentArea;

			Rectangle windowCloseArea;
			Rectangle windowCloseIconArea;
			Rectangle windowMaximizeArea;
			Rectangle windowMaximizeIconArea;
			Rectangle windowMinimizeArea;

			bool windowIsCreated;

			bool mouseOnCls;
			bool mouseOnMin;
			bool mouseOnMax;
			bool mouseIsDown;

			bool drawWindowControls;
			bool updateRequested;

			bool isMaximized;
			bool isIconified;

			FrameState frameState;
			SuspendState suspendedState;

		public:

			property IntPtr Handle { IntPtr get() { return IntPtr(windowOfWin322); }}

			property WndRoot^ Root { WndRoot^ get() { return rootElement; } }

			property Vec4 Margings { Vec4 get() { return windowMargins; } }
			property Rectangle Area { Rectangle get() { return windowArea; }}
			property Rectangle AreaOfContent { Rectangle get() { return windowContentArea; }}

			property bool ShouldClose { bool get() { return glfwWindowShouldClose(windowOfGLFW); } }

			Wnd(WndManager^ manager);
			~Wnd();

			void Create(String^ name, Vec4 area, Vec4 margins, Color borderColor, Color backgroundColor, UINT swapInterval);
			void Destroy();

			void Draw();

			void Update();
			void Update(WndElement^ element);
			void Draw(WndElement^ element);

			bool Suspend(SuspendState state);
			void Resume();

		public protected:

			double GetTimeSinceLastDraw()
			{
				return timeSinceLastFrame;
			}

			virtual void OnCreated() {}
			virtual void OnUpdate() = 0;

		private:

			D3DDevice^ d3dDevice;
			D3DDeviceContext^ d3dContext;
			D3DRenderTargetView^ d3dRenderTarget;
			D3DDepthStencilView^ d3dDepthStencil;

			D2DDevice^ d2dDevice;
			D2DDeviceContext^ d2dContext;
			Bitmap^ d2dRenderTarget;

			DxgiDevice^ dxgiDevice;
			DxgiSwapChain^ dxgiSwapChain;

			array<Rectangle>^ dxgiDirtyRects;
			UINT dxgiDirtyElements;
			UINT dxgiSwapInterval;

			WndRoot^ rootElement;
			SolidColorBrush^ basicBrush;

			bool deviceIsCreated;

			double timeSinceLastFrame;
			double timeOfLastFrame;

			void CreateDevice();
			void ResizeDevice();
			void ReleaseDevice();

			bool HitTestWindowControls(Vec2 point);

			void CallbackOfRefresh(GLFWwindow*);
			void CallbackOfMouseContained(GLFWwindow*, bool);
			void CallbackOfMouseMove(GLFWwindow*, double, double);
			void CallbackOfMouseButton(GLFWwindow*, int, int, int);
			void CallbackOfSize(GLFWwindow*, int, int);
			void CallbackOfScroll(GLFWwindow*, double, double);
			void CallbackOfKey(GLFWwindow*, int, int, int, int);
			void CallbackOfIconify(GLFWwindow*, int);
			void CallbackOfClose(GLFWwindow*);

			LRESULT WndProc(HWND hWnd, UINT msg, LPARAM wParam, WPARAM lParam);
		};

		public ref class WndElement abstract
		{
			Wnd^ wnd;
			Rectangle drawingArea;

			List<WndElement^> children;

		public:
			property Wnd^ Parent { Wnd^ get() { return wnd; } }

			property virtual Rectangle Area
			{
				Rectangle get() { return drawingArea; }
				void set(Rectangle value) { drawingArea = value; }
			}

			void RequestDraw()
			{
				DrawRequested = true;
			}

			void RequestUpdate()
			{
				UpdateRequested = true;
				DrawRequested = true;
			}

			void AddChild(WndElement^ child)
			{
				children.Add(child);
			}

		internal:
			bool DrawRequested;
			bool UpdateRequested;
			bool Visible;

			bool Contains(Vec2 mousePosition, WndElement^% element)
			{
				if (Area.Contains(mousePosition))
				{
					for (auto i = 0; i < children.Count; i++)
					{
						if (children[i]->Contains(mousePosition, element))
						{
							return true;
						}
					}

					element = this;
					return true;
				}

				element = nullptr;
				return false;
			}

			virtual void OnUpdate_(D2DDeviceContext^ d2dContext, D3DDeviceContext^ d3dContext, bool parentForced)
			{
				for (auto i = 0; i < children.Count; i++)
				{
					children[i]->OnUpdate_(d2dContext, d3dContext, parentForced);
				}
			}

			virtual void OnDraw_(D2DDeviceContext^ d2dContext, D3DDeviceContext^ d3dContext, bool parentForced)
			{
				for (auto i = 0; i < children.Count; i++)
				{
					children[i]->OnDraw_(d2dContext, d3dContext, parentForced);
				}
			}

			WndElement(Wnd^ wnd)
			{
				this->wnd = wnd;
				this->Visible = true;
			}

		public protected:
			virtual void OnDeviceCreated(D2DDevice^ d2dDevice, D2DDeviceContext^ d2dContext, D3DDevice^ d3dDevice, D3DDeviceContext^ d3dContext)
			{
				for (auto i = 0; i < children.Count; i++)
					children[i]->OnDeviceCreated(d2dDevice, d2dContext, d3dDevice, d3dContext);
			}

			virtual void OnDeviceLost() = 0;
			virtual void OnTargetCreated(D3DRenderTargetView^ a, D3DDepthStencilView^ b)
			{
				for (auto i = 0; i < children.Count; i++)
					children[i]->OnTargetCreated(a, b);
			}
			virtual void OnTargetLost() = 0;

			double GetTimeSinceLastDraw()
			{
				return wnd->GetTimeSinceLastDraw();
			}

			MouseMode GetMouseMode()
			{
				return (MouseMode)glfwGetInputMode(wnd->windowOfGLFW, GLFW_CURSOR);
			}
			void SetMouseMode(MouseMode mode)
			{
				glfwSetInputMode(wnd->windowOfGLFW, GLFW_CURSOR, (int)mode);
			}

			virtual void OnKey(InputKey button, InputAction action, InputMod mod) {}
			void OnKey_(int key, int action, int mods) { OnKey((InputKey)key, (InputAction)action, (InputMod)mods); }

			virtual void OnScroll(float dx, float dy) {}
			void OnScroll_(double dx, double dy) { OnScroll((float)dx, (float)dy); }

			virtual void OnMouseButton(InputButton button, InputAction action, InputMod mod) {}
			void OnMouseButton_(int button, int action, int mods) { OnMouseButton((InputButton)button, (InputAction)action, (InputMod)mods); }
			virtual void OnMouseMove(Vec2 mouseOnElement) {}
			void OnMouseMove_(float x, float y) { OnMouseMove(Vec2(x, y) - Vec2(Area.Left, Area.Top)); }

			virtual void OnMouseLeave() {}
			void OnMouseLeave_() { OnMouseLeave(); }

			WndElement(WndElement^ element)
			{
				this->wnd = element->wnd;
				this->Visible = true;

				element->AddChild(this);
				Area = Rectangle(Vec2(1));
			}
		};

		public ref class Wnd2DElement abstract : public WndElement
		{
			Mat3x2 drawingTransform;

		public:
			property Mat3x2 Transform { Mat3x2 get() { return drawingTransform; } }
			property virtual Rectangle Area
			{
				Rectangle get() override { return WndElement::Area; }
				void set(Rectangle value) override
				{
					WndElement::Area::set(value);
					drawingTransform = Mat3x2::Translation(Area.Location);

					UpdateRequested = true;
				}
			}

		internal:

			virtual void OnUpdate_(D2DDeviceContext^ d2dContext, D3DDeviceContext^ d3dContext, bool parentForced) override
			{
				if (UpdateRequested || parentForced)
				{
					parentForced = true;
					UpdateRequested = false;
					OnUpdate(d2dContext);
				}

				// ancestor will handle calling the children
				WndElement::OnUpdate_(d2dContext, d3dContext, parentForced);
			}

			virtual void OnDraw_(D2DDeviceContext^ d2dContext, D3DDeviceContext^ d3dContext, bool parentForced) override
			{
				// restart 2D drawing sequence with state from the previous one if we are not in sequence here
				if (d2dContext->IsDrawing == false)
				{
					d2dContext->BeginDrawRestoreState();
				}

				d2dContext->SetTransform(drawingTransform);
				d2dContext->PushAxisAlignedClip(Rectangle(Area.Dimensions));

				if (DrawRequested || parentForced)
				{
					parentForced = true;
					DrawRequested = false;
					OnDraw(d2dContext);
				}

				d2dContext->PopAxisAlignedClip();

				// ancestor will handle calling the children
				WndElement::OnDraw_(d2dContext, d3dContext, parentForced);
			}
		public protected:
			Wnd2DElement(WndElement^ element) : WndElement(element) { }

			virtual void OnLoad(D2DDevice^ d2dDevice, D2DDeviceContext^ d2dContext) = 0;
			virtual void OnUpdate(D2DDeviceContext^ d2dContext) = 0;
			virtual void OnDraw(D2DDeviceContext^ d2dContext) = 0;

			virtual void OnDeviceCreated(D2DDevice^ d2dDevice, D2DDeviceContext^ d2dContext, D3DDevice^ d3dDevice, D3DDeviceContext^ d3dContext) override
			{
				OnLoad(d2dDevice, d2dContext);

				// ancestor will handle calling the children
				WndElement::OnDeviceCreated(d2dDevice, d2dContext, d3dDevice, d3dContext);
			}
			virtual void OnDeviceLost() override {}
			virtual void OnTargetCreated(D3DRenderTargetView^ d3dTarget, D3DDepthStencilView^ d3dDepthStencil) override {}
			virtual void OnTargetLost() override {}
		};

		public ref class Wnd3DElement abstract : public WndElement
		{
		internal:
			virtual void OnUpdate_(D2DDeviceContext^, D3DDeviceContext^ d3dContext, bool parentForced) override
			{

			}
			virtual void OnDraw_(D2DDeviceContext^ d2dContext, D3DDeviceContext^ d3dContext, bool parentForced) override
			{
				// end any current 2D drawing sequence in such a way that it can be restored later
				if (d2dContext->IsDrawing) 
				{
					d2dContext->EndDrawSaveState();
				}

				auto thisElementArea = Area;
				auto thisElementViewport = Vec4(thisElementArea.Left, thisElementArea.Top, thisElementArea.Width, thisElementArea.Height);
				d3dContext->RSSetScissor(thisElementArea);
				d3dContext->RSSetViewport(thisElementViewport, 0.0f, 1.0f);
				OnDraw(d3dContext);

				// ancestor will handle calling the children
				WndElement::OnDraw_(d2dContext, d3dContext, parentForced);
			}
		public protected:
			Wnd3DElement(WndElement^ element) : WndElement(element) { }

			virtual void OnCreatedDevice(D3DDevice^ d3dDevice, D3DDeviceContext^ d3dContext) = 0;
			virtual void OnCreatedTarget(D3DRenderTargetView^ d3dTarget, D3DDepthStencilView^ d3dDepthStencil) = 0;

			virtual void OnDeviceCreated(D2DDevice^ d2dDevice, D2DDeviceContext^ d2dContext, D3DDevice^ d3dDevice, D3DDeviceContext^ d3dContext) override { OnCreatedDevice(d3dDevice, d3dContext); }
			virtual void OnDeviceLost() override {}
			virtual void OnTargetCreated(D3DRenderTargetView^ d3dTarget, D3DDepthStencilView^ d3dDepthStencil) override 
			{ 
				OnCreatedTarget(d3dTarget, d3dDepthStencil);
				WndElement::OnTargetCreated(d3dTarget, d3dDepthStencil);
			}
			virtual void OnTargetLost() override {}

			virtual void OnDraw(D3DDeviceContext^ d3dContext) = 0;
		};

		public ref class WndRoot sealed : public WndElement
		{
		public protected:

			WndRoot(Wnd^ wnd) : WndElement(wnd) { }

			virtual void OnDeviceLost() override {}
			virtual void OnTargetCreated(D3DRenderTargetView^ d3dTarget, D3DDepthStencilView^ d3dDepthStencil) override { WndElement::OnTargetCreated(d3dTarget, d3dDepthStencil); }
			virtual void OnTargetLost() override {}
		};
	}
}
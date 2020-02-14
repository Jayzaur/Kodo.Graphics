#include "Wnd.hpp"

typedef LRESULT(CALLBACK* Win32WndProc)(HWND, UINT, WPARAM, LPARAM);

namespace Kodo
{
	namespace Graphics
	{
		using namespace System::Diagnostics;

		WndManager::WndManager()
		{
			ThrowIfZero(glfwInit());
		}

		WndManager::~WndManager()
		{
			glfwTerminate();
		}

		void WndManager::AddWindow(Wnd^ wnd)
		{
			wnds.Add(wnd);
		}

		bool Wnd::Suspend(SuspendState state)
		{
			if (suspendedState != SuspendState::None)
			{
				return false;
			}

			suspendedState = state;
			return true;
		}

		void Wnd::Resume()
		{
			/*if (suspendedState == SuspendState::Load)
			{
				suspendedState = SuspendState::None;
				Update();
			}
			else if (suspendedState == SuspendState::Update)
			{
				suspendedState = SuspendState::None;
				Draw();
			}

			suspendedState = SuspendState::None;*/
		}

		void Wnd::Update()
		{
			auto forceForChildren = false;

			if (updateRequested)
			{
				forceForChildren = true;
				updateRequested = false;
				OnUpdate();
			}

			rootElement->OnUpdate_(d2dContext, d3dContext, forceForChildren);
		}

		void Wnd::Update(WndElement^ element)
		{
			element->UpdateRequested = true;

			/*if (Suspend(SuspendState::UpdateElement))
			{
				element->OnUpdate_(d2dContext, d3dContext);
				Resume();
				Draw(element);
			}*/
		}

		void Wnd::Draw(WndElement^ element)
		{
			if (element->Visible && frameState != FrameState::Whole)
			{
				frameState = FrameState::Part;
				element->DrawRequested = true;
			}
		}

		void Wnd::CallbackOfRefresh(GLFWwindow*)
		{
			Update();
			Draw();
		}
		void Wnd::CallbackOfMouseContained(GLFWwindow*, bool contained)
		{
			if (!contained)
			{
				// clear mouse state
				mouseOnCls = mouseOnMax = mouseOnMin = false;
			}
		}
		void Wnd::CallbackOfMouseMove(GLFWwindow*, double x, double y)
		{
			Vec2 mousePosition((float)x, (float)y);

			if (elementThatCapturedMouse == nullptr)
			{
				if (elementThatHasMouse != nullptr)
				{
					elementThatHasMouse->OnMouseLeave_();
					elementThatHasMouse = nullptr;
				}

				if (rootElement->Contains(mousePosition, elementThatHasMouse))
				{
					if (elementThatHasMouse == rootElement)
					{
						elementThatHasMouse = nullptr;
					}
					else
					{
						//elementThatHasMouse = windowElements[i];
						elementThatHasMouse->OnMouseMove_((float)x, (float)y);
					}
				}

				/*for (auto i = windowElements.Count - 1; i >= 0; i--)
				{
					if (windowElements[i]->Contains(mousePosition, elementThatHasMouse))
					{
						//elementThatHasMouse = windowElements[i];
						elementThatHasMouse->OnMouseMove_((float)x, (float)y);
						break;
					}
				}*/
			}
			else
			{
				elementThatCapturedMouse->OnMouseMove_((float)x, (float)y);
			}
		}
		bool Wnd::HitTestWindowControls(Vec2 point)
		{
			auto lastOnCls = mouseOnCls;
			auto lastOnMin = mouseOnMin;
			auto lastOnMax = mouseOnMax;
			mouseOnCls = windowCloseArea.Contains(point);
			mouseOnMin = windowMinimizeArea.Contains(point);
			mouseOnMax = windowMaximizeArea.Contains(point);

			return lastOnCls != mouseOnCls || lastOnMin != mouseOnMin || lastOnMax != mouseOnMax;
		}
		void Wnd::CallbackOfMouseButton(GLFWwindow*, int button, int action, int mods)
		{
			mouseIsDown = button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS;

			if (action == GLFW_RELEASE)
			{
				static auto before = glfwGetTime();
				auto now = glfwGetTime();
				auto dt = now - before;
				before = now;

				if (dt > 0.010 && dt < 0.200)
				{
					Debug::WriteLine("doubleclick");
				}
			}

			if (elementThatCapturedMouse)
			{
				elementThatCapturedMouse->OnMouseButton_(button, action, mods);

				if (action == GLFW_RELEASE)
				{
					elementThatCapturedMouse = nullptr;
				}
			}
			else if (elementThatHasMouse)
			{
				elementThatHasMouse->OnMouseButton_(button, action, mods);

				if (action == GLFW_PRESS)
				{
					elementThatCapturedMouse = elementThatHasMouse;
					elementThatHasKeyboard = elementThatHasMouse;
				}
			}
			else
			{
				// left button state changed?
				if (button == GLFW_MOUSE_BUTTON_LEFT && glfwGetInputMode(windowOfGLFW, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
				{
					// left button released?
					if (action == GLFW_RELEASE)
					{
						if (mouseOnCls || mouseOnMin || mouseOnMax)
						{
							double mouseX, mouseY;
							glfwGetCursorPos(windowOfGLFW, &mouseX, &mouseY);
							HitTestWindowControls(Vec2(mouseX, mouseY));
						}

						if (mouseOnCls) glfwSetWindowShouldClose(windowOfGLFW, true);
						if (mouseOnMax && isMaximized) glfwRestoreWindow(windowOfGLFW);
						else if (mouseOnMax && !isMaximized) glfwMaximizeWindow(windowOfGLFW);
						if (mouseOnMin) glfwIconifyWindow(windowOfGLFW);
					}
				}
			}
		}
		void Wnd::CallbackOfSize(GLFWwindow*, int, int)
		{
			ResizeDevice();
		}
		void Wnd::CallbackOfScroll(GLFWwindow*, double x, double y)
		{
			if (elementThatHasMouse != nullptr)
			{
				elementThatHasMouse->OnScroll_(x, y);
			}

		}
		void Wnd::CallbackOfKey(GLFWwindow*, int key, int code, int action, int mods)
		{
			if (elementThatHasKeyboard)
			{
				elementThatHasKeyboard->OnKey_(key, action, mods);
			}
			else
			{
				if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				{
					// clear mouse state -> user can cancel action on window control
					mouseOnCls = mouseOnMax = mouseOnMin = false;
				}
			}
		}
		void Wnd::CallbackOfIconify(GLFWwindow*, int iconified)
		{
		}
		void Wnd::CallbackOfClose(GLFWwindow*)
		{
		}

		LRESULT Wnd::WndProc(HWND hWnd, UINT msg, LPARAM wParam, WPARAM lParam)
		{
			if (msg == WM_NCHITTEST)
			{
				double mouseX, mouseY;
				glfwGetCursorPos(windowOfGLFW, &mouseX, &mouseY);

				auto windowHitSize = 5;
				auto windowHitResult = HTCLIENT;

				HitTestWindowControls(Vec2(mouseX, mouseY));

				if (!drawWindowControls || (mouseOnCls || mouseOnMin || mouseOnMax) || elementThatHasMouse)
					return windowHitResult;

				if (mouseX <= windowMargins.X)
				{
					if (mouseY <= windowHitSize) windowHitResult = HTTOPLEFT;
					else if (mouseY >= windowContentArea.Bottom - windowHitSize) windowHitResult = HTBOTTOMLEFT;
					else windowHitResult = HTLEFT;
				}
				else if (mouseX >= windowContentArea.Right)
				{
					if (mouseY < windowHitSize) windowHitResult = HTTOPRIGHT;
					else if (mouseY >= windowContentArea.Bottom - windowHitSize) windowHitResult = HTBOTTOMRIGHT;
					else windowHitResult = HTRIGHT;
				}
				else if (mouseY <= windowContentArea.Top)
				{
					if (mouseY <= windowHitSize) windowHitResult = HTTOP;
					else
					{
						windowHitResult = HTCAPTION;
					}
				}
				else if (mouseY >= windowContentArea.Bottom - windowHitSize) windowHitResult = HTBOTTOM;
				else windowHitResult = HTCLIENT;

				return windowHitResult;
			}

			return CallWindowProc(wndProcOfGLFW, hWnd, msg, wParam, lParam);
		}

		Wnd::Wnd(WndManager^ manager)
		{
			rootElement = gcnew WndRoot(this);
		}

		Wnd::~Wnd()
		{
			ReleaseDevice();

			if (windowOfGLFW != nullptr)
			{
				glfwDestroyWindow(windowOfGLFW);
				windowOfGLFW = nullptr;
			}
		}

		void Wnd::Create(String^ name, Vec4 area, Vec4 margins, Color borderColor, Color backgroundColor, UINT swapInterval)
		{
			// destroy first if window is already created
			if (windowIsCreated)
			{
				Destroy();
			}

			pin_ptr<const wchar_t> pinnedName = PtrToStringChars(name);

			// create the window
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_DECORATED, 0);
			windowOfGLFW = glfwCreateWindow((int)area.Z, (int)area.W, std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(pinnedName).c_str(), nullptr, nullptr);
			ThrowIfNull(windowOfGLFW);
			windowOfWin32 = glfwGetWin32Window(windowOfGLFW);
			windowIsCreated = true;

			// move the window to the correct position
			{
				glfwSetWindowAspectRatio(windowOfGLFW, 16, 9);

				MONITORINFO info;
				info.cbSize = sizeof(MONITORINFO);
				GetMonitorInfo(MonitorFromWindow(windowOfWin32, MONITOR_DEFAULTTONEAREST), &info);

				if (float::IsNaN(area.X) || float::IsNaN(area.Y))
					glfwSetWindowPos(windowOfGLFW, (int)(((info.rcWork.right - info.rcWork.left) - area.Z) / 2.0f), (int)(((info.rcWork.bottom - info.rcWork.top) - area.W) / 2.0f));
				else
					glfwSetWindowPos(windowOfGLFW, (int)area.Left, (int)area.Top);
			}

			dxgiSwapInterval = swapInterval;
			windowMargins = margins;
			windowBorderColor = borderColor;
			windowClearColor = backgroundColor;
			drawWindowControls = true;

			// setup callbacks
			WndProc_ = gcnew WndProcDelegate(this, &Wnd::WndProc);
			WndProc_Ref = HandleRef(WndProc_, Marshal::GetFunctionPointerForDelegate(WndProc_));
			CallbackOfRefresh_ = gcnew CallbackOfRefreshDelegate(this, &Wnd::CallbackOfRefresh);
			CallbackOfRefresh_Ref = HandleRef(CallbackOfRefresh_, Marshal::GetFunctionPointerForDelegate(CallbackOfRefresh_));
			CallbackOfMouseContained_ = gcnew CallbackOfMouseContainedDelegate(this, &Wnd::CallbackOfMouseContained);
			CallbackOfMouseContained_Ref = HandleRef(CallbackOfMouseContained_, Marshal::GetFunctionPointerForDelegate(CallbackOfMouseContained_));
			CallbackOfMouseMove_ = gcnew CallbackOfMouseMoveDelegate(this, &Wnd::CallbackOfMouseMove);
			CallbackOfMouseMove_Ref = HandleRef(CallbackOfMouseMove_, Marshal::GetFunctionPointerForDelegate(CallbackOfMouseMove_));
			CallbackOfMouseButton_ = gcnew CallbackOfMouseButtonDelegate(this, &Wnd::CallbackOfMouseButton);
			CallbackOfMouseButton_Ref = HandleRef(CallbackOfMouseButton_, Marshal::GetFunctionPointerForDelegate(CallbackOfMouseButton_));
			CallbackOfSize_ = gcnew CallbackOfSizeCallbackDelegate(this, &Wnd::CallbackOfSize);
			CallbackOfSize_Ref = HandleRef(CallbackOfSize_, Marshal::GetFunctionPointerForDelegate(CallbackOfSize_));
			CallbackOfScroll_ = gcnew CallbackOfScrollDelegate(this, &Wnd::CallbackOfScroll);
			CallbackOfScroll_Ref = HandleRef(CallbackOfScroll_, Marshal::GetFunctionPointerForDelegate(CallbackOfScroll_));
			CallbackOfKey_ = gcnew CallbackOfKeyDelegate(this, &Wnd::CallbackOfKey);
			CallbackOfKey_Ref = HandleRef(CallbackOfKey_, Marshal::GetFunctionPointerForDelegate(CallbackOfKey_));
			CallbackOfIconify_ = gcnew CallbackOfIconifyDelegate(this, &Wnd::CallbackOfIconify);
			CallbackOfIconify_Ref = HandleRef(CallbackOfIconify_, Marshal::GetFunctionPointerForDelegate(CallbackOfIconify_));
			CallbackOfClose_ = gcnew CallbackOfCloseDelegate(this, &Wnd::CallbackOfClose);
			//glfwsetc
			CallbackOfClose_Ref = HandleRef(CallbackOfClose_, Marshal::GetFunctionPointerForDelegate(CallbackOfClose_));
			glfwSetWindowRefreshCallback(windowOfGLFW, reinterpret_cast<GLFWwindowrefreshfun>(CallbackOfRefresh_Ref.Handle.ToPointer()));
			glfwSetCursorEnterCallback(windowOfGLFW, reinterpret_cast<GLFWcursorenterfun>(CallbackOfMouseContained_Ref.Handle.ToPointer()));
			glfwSetCursorPosCallback(windowOfGLFW, reinterpret_cast<GLFWcursorposfun>(CallbackOfMouseMove_Ref.Handle.ToPointer()));
			glfwSetMouseButtonCallback(windowOfGLFW, reinterpret_cast<GLFWmousebuttonfun>(CallbackOfMouseButton_Ref.Handle.ToPointer()));
			glfwSetWindowSizeCallback(windowOfGLFW, reinterpret_cast<GLFWwindowsizefun>(CallbackOfSize_Ref.Handle.ToPointer()));
			glfwSetScrollCallback(windowOfGLFW, reinterpret_cast<GLFWscrollfun>(CallbackOfScroll_Ref.Handle.ToPointer()));
			glfwSetKeyCallback(windowOfGLFW, reinterpret_cast<GLFWkeyfun>(CallbackOfKey_Ref.Handle.ToPointer()));
			glfwSetWindowIconifyCallback(windowOfGLFW, reinterpret_cast<GLFWwindowiconifyfun>(CallbackOfIconify_Ref.Handle.ToPointer()));
			glfwSetWindowCloseCallback(windowOfGLFW, reinterpret_cast<GLFWwindowclosefun>(CallbackOfClose_Ref.Handle.ToPointer()));
			// setup wndproc override
			wndProcOfGLFW = reinterpret_cast<Win32WndProc>(GetWindowLongPtr(windowOfWin32, GWLP_WNDPROC));
			SetWindowLongPtr(windowOfWin32, GWLP_WNDPROC, (LONG)reinterpret_cast<Win32WndProc>(WndProc_Ref.Handle.ToPointer()));

			//windowOfWin322 = CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE, 0, 0, 500, 500, windowOfWin32, nullptr, nullptr, nullptr);

			OnCreated();

			CreateDevice();
		}

		void Wnd::Destroy()
		{
			windowIsCreated = false;

			ReleaseDevice();

			if (windowOfGLFW != nullptr)
			{
				glfwDestroyWindow(windowOfGLFW);
				windowOfGLFW = nullptr;
			}
		}

		void Wnd::CreateDevice()
		{
			ThrowIf(deviceIsCreated, "Wnd::CreateDevice/Device is already created!");

			d3dDevice = gcnew D3DDevice(true);
			dxgiDevice = gcnew DxgiDevice(d3dDevice);
			d2dDevice = gcnew D2DDevice(d3dDevice, dxgiDevice, true);

			d3dContext = d3dDevice->Context;
			d2dContext = d2dDevice->Context;

			dxgiSwapChain = gcnew DxgiSwapChain(d3dDevice, IntPtr(windowOfWin32));

			//
			// notify elements
			//
			rootElement->OnDeviceCreated(d2dDevice, d2dContext, d3dDevice, d3dContext);

			deviceIsCreated = true;

			ResizeDevice();

			{	// device-dependent resources for window control buttons drawing 
				basicBrush = gcnew SolidColorBrush(d2dContext);
			}
		}

		void Wnd::ReleaseDevice()
		{
			//
			// notify elements
			//
			rootElement->OnDeviceLost();

			deviceIsCreated = false;
		}

		void Wnd::ResizeDevice()
		{
			ThrowIf(!deviceIsCreated, "Wnd::ResizeDevice/Device is not created!");

			{	// clear window control buttons state
				mouseOnCls = mouseOnMax = mouseOnMin = false;
			}

			// retrieve window state
			isIconified = glfwGetWindowAttrib(windowOfGLFW, GLFW_ICONIFIED);
			isMaximized = glfwGetWindowAttrib(windowOfGLFW, GLFW_MAXIMIZED);

			// ignore resize if the window is iconified
			if (isIconified)
				return;

			//
			// notify elements
			//
			rootElement->OnTargetLost();

			// retrieve and calculate all viewport dimensions
			INT width, height;
			//glfwGetWindowSize(windowOfGLFW, &width, &height);
			RECT windowRect;
			GetWindowRect(windowOfWin32, &windowRect);
			width = windowRect.right - windowRect.left;
			height = windowRect.bottom - windowRect.top;

			windowArea = Rectangle(0.0f, 0.0f, (float)width, (float)height);
			windowContentArea = isMaximized
				? Rectangle::FromXYWH(0.0f, windowMargins.Y, windowArea.Width, windowArea.Height - windowMargins.Y)
				: Rectangle::FromXYWH(windowMargins.X, windowMargins.Y, windowArea.Width - (windowMargins.X + windowMargins.Z), windowArea.Height - (windowMargins.Y + windowMargins.W));

			// release render targets
			d2dContext->SetTarget(nullptr);
			d3dContext->OMSetRenderTarget(nullptr);
			if (d2dRenderTarget) d2dRenderTarget->Release();
			if (d3dRenderTarget) d3dRenderTarget->Release();
			if (d3dDepthStencil) d3dDepthStencil->Release();
			d2dRenderTarget = nullptr;
			d3dRenderTarget = nullptr;
			d3dDepthStencil = nullptr;

			// resize swap chain
			dxgiSwapChain->ResizeToWindow();

			// recreate render targets
			d2dRenderTarget = gcnew Bitmap(d2dContext, dxgiSwapChain);
			d3dRenderTarget = gcnew D3DRenderTargetView(d3dDevice, dxgiSwapChain);
			d3dDepthStencil = gcnew D3DDepthStencilView(d3dDevice, windowArea);

			updateRequested = true;

			//
			// notify elements
			//
			rootElement->Area = windowContentArea;
			rootElement->OnTargetCreated(d3dRenderTarget, d3dDepthStencil);

			{	// calculations for window control buttons drawing
				windowCloseArea = Rectangle(windowArea.Width - 45 - 7, 0.0f, windowArea.Width - 7, 20.0f);
				windowCloseIconArea = Rectangle(Vec2(8, 7)).CenterTo(windowCloseArea);
				windowMaximizeArea = Rectangle(windowCloseArea.Left - 2 - 22, 0.0f, windowCloseArea.Left - 2, 20.0f);
				windowMaximizeIconArea = Rectangle(Vec2(10, 8)).CenterTo(windowMaximizeArea);
				windowMinimizeArea = Rectangle(windowMaximizeArea.Left - 2 - 22, 0.0f, windowMaximizeArea.Left - 2, 20.0f);
			}

			// whole window needs to be redrawn
			frameState = FrameState::Whole;
		}

		void Wnd::Draw()
		{
			//if (!deviceIsCreated)
				//return;

			ThrowIf(!deviceIsCreated, "Wnd::Draw/Device is not created!");

			DXGIResult presentResult = DXGIResult::OK;

			// skip drawing when iconified
			if (isIconified)
			{
				return;
			}

			auto currentTime = glfwGetTime();
			timeSinceLastFrame = currentTime - timeOfLastFrame;
			timeOfLastFrame = currentTime;

			dxgiDirtyElements = 0;

			frameState = FrameState::Whole;

			if (frameState == FrameState::Whole)
			{
				d2dContext->SetTarget(d2dRenderTarget);
				// begin window/2D render stage
				d2dContext->BeginDraw();
				d2dContext->SetTransform(Mat3x2::Identity);
				// clear to border color to "draw" it
				d2dContext->Clear(windowBorderColor);
				// clear the content area
				d2dContext->PushAxisAlignedClip(windowContentArea);
				d2dContext->Clear(windowClearColor);
				d2dContext->PopAxisAlignedClip();

				if (drawWindowControls)
				{
					// draw close button
					basicBrush->Color = mouseOnCls ? mouseIsDown ? Color::SkyBlue : Color::SpringGreen : Color::IndianRed;
					d2dContext->FillRectangle(windowCloseArea, basicBrush);
					basicBrush->Color = Color::White;
					d2dContext->DrawLine(windowCloseIconArea.TopLeft, windowCloseIconArea.BottomRight, basicBrush, 1.5f);
					d2dContext->DrawLine(windowCloseIconArea.BottomLeft, windowCloseIconArea.TopRight, basicBrush, 1.5f);
					// draw maximize button
					basicBrush->Color = mouseOnMax ? mouseIsDown ? Color::SkyBlue : Color::SpringGreen : windowBorderColor;
					d2dContext->FillRectangle(windowMaximizeArea, basicBrush);
					basicBrush->Color = mouseOnMax ? Color::White : Color::Black;
					d2dContext->DrawRectangle(windowMaximizeIconArea, basicBrush, 1.5f);
					// draw minimize button
					basicBrush->Color = mouseOnMin ? mouseIsDown ? Color::SkyBlue : Color::SpringGreen : windowBorderColor;
					d2dContext->FillRectangle(windowMinimizeArea, basicBrush);
					basicBrush->Color = mouseOnMin ? Color::White : Color::Black;
					d2dContext->DrawLine(Vec2(windowMinimizeArea.Left + 7, windowMinimizeArea.Bottom - 6), Vec2(windowMinimizeArea.Right - 7, windowMinimizeArea.Bottom - 6), basicBrush, 2.0f);
				}

				// draw the window element graph
				rootElement->OnDraw_(d2dContext, d3dContext, true);
			}
			else
			{
				// begin element render stage
				/*d2dContext->BeginDraw();

				// draw elements
				for (auto i = 0; i < windowElements.Count; i++)
				{
					auto element = windowElements[i];

					if (element->Visible && element->DrawRequested)
					{
						dxgiDirtyRects[dxgiDirtyElements++] = element->Area;

						d2dContext->PushAxisAlignedClip(element->Area, AntialiasMode::Aliased);
						d2dContext->Clear(windowClearColor);
						element->OnDraw_(d2dContext, d3dContext);
						d2dContext->PopAxisAlignedClip();

						element->DrawRequested = false;
					}
				}

				// end element render stage
				d2dContext->EndDraw();*/
			}

			// present
			presentResult = dxgiSwapChain->Present(dxgiSwapInterval, dxgiDirtyRects, dxgiDirtyElements);

			if (presentResult == DXGIResult::ShouldResize)
			{
				ResizeDevice();
			}
			else if (presentResult == DXGIResult::ShouldRecreate)
			{
				ReleaseDevice();
				CreateDevice();
			}
		}
	}
}
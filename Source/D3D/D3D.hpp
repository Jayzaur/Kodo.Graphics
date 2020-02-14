#pragma once

#include "..\Graphics.hpp"

#include <fstream>

namespace Kodo
{
	namespace Graphics
	{
		using namespace System::Runtime::InteropServices;

		ref class D3DDeviceContext;
		ref class D3DRenderTargetView;
		ref class D3DDepthStencilView;
		ref class D3DTexture2D;
		ref class DxgiDevice;
		ref class DxgiSwapChain;

		ref class D3DVertexShader;
		ref class D3DPixelShader;
		ref class D3DInputLayout;
		ref class D3DResource;
		ref class D3DBuffer;
		ref class D3DShaderResourceView;
		ref class D3DSamplerState;
		ref class D3DRasterizerState;
		ref class D3DBlendState;

		public enum class D3DAccess
		{
			None = 0,
			Write = D3D11_CPU_ACCESS_WRITE,
			Read = D3D11_CPU_ACCESS_READ
		};
		public enum class D3DUsage
		{
			Default = D3D11_USAGE_DEFAULT,
			Immutable = D3D11_USAGE_IMMUTABLE,
			Dynamic = D3D11_USAGE_DYNAMIC,
			Staging = D3D11_USAGE_STAGING
		};
		public enum class D3DBind
		{
			VertexBuffer = D3D11_BIND_VERTEX_BUFFER,
			IndexBuffer = D3D11_BIND_INDEX_BUFFER,
			ConstantBuffer = D3D11_BIND_CONSTANT_BUFFER,
			ShaderResource = D3D11_BIND_SHADER_RESOURCE,
			StreamOutput = D3D11_BIND_STREAM_OUTPUT,
			RenderTarget = D3D11_BIND_RENDER_TARGET,
			DepthStencil = D3D11_BIND_DEPTH_STENCIL,
			UnorderedAccess = D3D11_BIND_UNORDERED_ACCESS,
			Decoder = D3D11_BIND_DECODER,
			Encoder = D3D11_BIND_VIDEO_ENCODER
		};

		public enum class D3DMisc
		{
			D3D11_RESOURCE_MISC_GENERATE_MIPS = 0x1L,
			Shared = D3D11_RESOURCE_MISC_SHARED,
			D3D11_RESOURCE_MISC_TEXTURECUBE = 0x4L,
			D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS = 0x10L,
			D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS = 0x20L,
			D3D11_RESOURCE_MISC_BUFFER_STRUCTURED = 0x40L,
			D3D11_RESOURCE_MISC_RESOURCE_CLAMP = 0x80L,
			D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX = 0x100L,
			D3D11_RESOURCE_MISC_GDI_COMPATIBLE = 0x200L,
			D3D11_RESOURCE_MISC_SHARED_NTHANDLE = 0x800L,
			D3D11_RESOURCE_MISC_RESTRICTED_CONTENT = 0x1000L,
			D3D11_RESOURCE_MISC_RESTRICT_SHARED_RESOURCE = 0x2000L,
			D3D11_RESOURCE_MISC_RESTRICT_SHARED_RESOURCE_DRIVER = 0x4000L,
			D3D11_RESOURCE_MISC_GUARDED = 0x8000L,
			D3D11_RESOURCE_MISC_TILE_POOL = 0x20000L,
			D3D11_RESOURCE_MISC_TILED = 0x40000L,
			D3D11_RESOURCE_MISC_HW_PROTECTED = 0x80000L
		};

		//--------------------------------------------------------------------------------------
		public ref class D3DDevice : GraphicsObject {
			GRAPHICS_OBJECT(D3DDevice, ID3D11Device);
			D3DDeviceContext^ deviceContext;
		public:
			property D3DDeviceContext^ Context { D3DDeviceContext^ get() { return deviceContext; } }

			D3DDevice(bool debug);
		};
		//--------------------------------------------------------------------------------------
		public enum class ClearFlag
		{
			Depth = 0x1L,
			Stencil = 0x2L
		};

		public enum class D3DMap
		{
			Read = D3D11_MAP_READ,
			Write = D3D11_MAP_WRITE,
			ReadWrite = D3D11_MAP_READ_WRITE,
			WriteDiscard = D3D11_MAP_WRITE_DISCARD,
			WriteNoOverwrite = D3D11_MAP_WRITE_NO_OVERWRITE
		};

		public enum class PrimitiveTopology
		{
			Triangles = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		};

		public ref class D3DDeviceContext : GraphicsObject {
			GRAPHICS_OBJECT(D3DDeviceContext, ID3D11DeviceContext);
		public:
			/// <summary>
			/// Performs an UpdateSubresource on the specified resource. Use when updating constant buffers etc. Does not support arrays, use other overload for arrays.
			/// </summary>
			generic<typename T> void UpdateGPUResource(D3DResource^ destinationResource, T sourceData);
			/// <summary>
			/// Performs a Map/Unmap on the specified resource. Use when updating vertex buffers etc.
			/// </summary>
			generic<typename T> void UpdateGPUResource(D3DResource^ destinationResource, array<T>^ sourceData, D3DMap mapType);

			void Draw(INT vertexCount) { Native->Draw((UINT)vertexCount, 0); }
			void Draw(UINT vertexCount) { Native->Draw(vertexCount, 0); }
			void Draw(INT vertexCount, INT startLocation) { Native->Draw((UINT)vertexCount, (UINT)startLocation); }
			void Draw(UINT vertexCount, UINT startLocation) { Native->Draw(vertexCount, startLocation); }

			void DrawInstanced(INT vertexCountPerInstance, INT instanceCount) { DrawInstanced((UINT)vertexCountPerInstance, (UINT)instanceCount, 0, 0); }
			void DrawInstanced(UINT vertexCountPerInstance, UINT instanceCount) { DrawInstanced(vertexCountPerInstance, instanceCount, 0, 0); }
			void DrawInstanced(UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation, UINT startInstanceLocation);

			void ClearTarget(D3DRenderTargetView^ renderTargetView, Color clearColor);
			void ClearDepth(D3DDepthStencilView^ depthStencilView) { ClearDepth(depthStencilView, 1.0f); }
			void ClearDepth(D3DDepthStencilView^ depthStencilView, FLOAT depth);
			void ClearDepthAndStencil(D3DDepthStencilView^ depthStencilView, ClearFlag clearFlag, FLOAT depth, BYTE stencil);

			void IASetInputLayout(D3DInputLayout^ inputLayout);
			void IASetPrimitiveTopology(PrimitiveTopology topology) { Native->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)topology); }
			void IASetVertexBuffer(D3DBuffer^ buffer);
			void IASetVertexBuffers(D3DBuffer^ bufferA, D3DBuffer^ bufferB);

			void VSSetShader(D3DVertexShader^ vertexShader);
			/// <summary>
			/// Sets the specified constant buffer to slot 0
			/// </summary>
			void VSSetConstantBuffer(D3DBuffer^ constantBuffer) { VSSetConstantBuffer(0, constantBuffer); }
			/// <summary>
			/// Sets the specified constant buffer to the desired slot
			/// </summary>
			void VSSetConstantBuffer(UINT slot, D3DBuffer^ constantBuffer);
			void PSSetShader(D3DPixelShader^ pixelShader);
			void PSSetConstantBuffer(UINT slot, D3DBuffer^ constantBuffer);
			void PSSetSampler(D3DSamplerState^ samplerState);
			void PSSetSamplers(D3DSamplerState^ samplerA, D3DSamplerState^ samplerB);
			void PSSetResource(D3DShaderResourceView^ shaderResource);
			void PSSetResources(D3DShaderResourceView^ resourceA, D3DShaderResourceView^ resourceB);

			void OMSetBlendState(D3DBlendState^ blendState);

			void RSSetState(D3DRasterizerState^ rasterizerState);
			void RSSetScissor(Rectangle scissorRect)
			{
				D3D11_RECT rects[1];
				rects[0].left = static_cast<LONG>(scissorRect.Left);
				rects[0].right = static_cast<LONG>(scissorRect.Right);
				rects[0].top = static_cast<LONG>(scissorRect.Top);
				rects[0].bottom = static_cast<LONG>(scissorRect.Bottom);
				Native->RSSetScissorRects(1, rects);
			}

			/// <summary>
			/// Sets the rasterizer viewport to the specified coordinates and with depth minimum depth of 0 and maximum of 1.
			/// </summary>
			void RSSetViewport(Vec4 viewportXYWH) { RSSetViewport(viewportXYWH, 0, 1); }
			/// <summary>
			/// Sets the rasterizer viewport to the specified coordinates and depth values.
			/// </summary>
			void RSSetViewport(Vec4 viewportXYWH, FLOAT depthMin, FLOAT depthMax)
			{
				D3D11_VIEWPORT viewport;
				viewport.Width = viewportXYWH.Z;
				viewport.Height = viewportXYWH.W;
				viewport.MinDepth = depthMin;
				viewport.MaxDepth = depthMax;
				viewport.TopLeftX = viewportXYWH.X;
				viewport.TopLeftY = viewportXYWH.Y;
				Native->RSSetViewports(1, &viewport);
			}

			void OMSetRenderTarget(D3DRenderTargetView^ renderTargetView);
			void OMSetRenderTarget(D3DRenderTargetView^ renderTargetView, D3DDepthStencilView^ depthStencilView);
		};
		//--------------------------------------------------------------------------------------
		public ref class D3DRenderTargetView : GraphicsObject {
			GRAPHICS_OBJECT(D3DRenderTargetView, ID3D11RenderTargetView);
		public:
			D3DRenderTargetView(D3DDevice^ d3dDevice, DxgiSwapChain^ dxgiSwapChain);
			D3DRenderTargetView(D3DDevice^ d3dDevice, D3DTexture2D^ backBufferTexture);
		};
		//--------------------------------------------------------------------------------------
		public ref class D3DTexture2D : GraphicsObject {
			GRAPHICS_OBJECT(D3DTexture2D, ID3D11Texture2D);

			DxgiFormat _format;
			UINT _mipLevels;

		public:

			D3DTexture2D(DxgiSwapChain^ dxgiSwapChain);
			D3DTexture2D(D3DDevice^ d3dDevice, INT width, INT height, DxgiFormat format, D3DBind bind, D3DUsage usage, D3DAccess access) : D3DTexture2D(d3dDevice, width, height, format, bind, usage, access, (D3DMisc)0) {}
			D3DTexture2D(D3DDevice^ d3dDevice, UINT width, UINT height, DxgiFormat format, D3DBind bind, D3DUsage usage, D3DAccess access) : D3DTexture2D(d3dDevice, width, height, format, bind, usage, access, (D3DMisc)0) {}
			D3DTexture2D(D3DDevice^ d3dDevice, INT width, INT height, DxgiFormat format, D3DBind bind, D3DUsage usage, D3DAccess access, D3DMisc misc) : D3DTexture2D(d3dDevice, width, height, format, bind, usage, access, misc, 1, 1, 0) {}
			D3DTexture2D(D3DDevice^ d3dDevice, UINT width, UINT height, DxgiFormat format, D3DBind bind, D3DUsage usage, D3DAccess access, D3DMisc misc) : D3DTexture2D(d3dDevice, width, height, format, bind, usage, access, misc, 1, 1, 0) {}
			D3DTexture2D(D3DDevice^ d3dDevice, UINT width, UINT height, DxgiFormat format, D3DBind bind, D3DUsage usage, D3DAccess access, D3DMisc misc, UINT mipLevels, UINT sampleCount, UINT sampleQuality);
		};
		//--------------------------------------------------------------------------------------

		public enum class D3DFill
		{
			Wireframe = D3D11_FILL_WIREFRAME,
			Solid = D3D11_FILL_SOLID
		};

		public enum class D3DCull
		{
			None = D3D11_CULL_NONE,
			Front = D3D11_CULL_FRONT,
			Back = D3D11_CULL_BACK
		};

		public ref class D3DRasterizerState : GraphicsObject {
			GRAPHICS_OBJECT(D3DRasterizerState, ID3D11RasterizerState);
		public:
			D3DRasterizerState(D3DDevice^ d3dDevice, D3DFill fill, D3DCull cull, Boolean scissorEnable) : D3DRasterizerState(d3dDevice, fill, cull, scissorEnable, false, false, false, false, 0, 0.0f, 0.0f) {}
			D3DRasterizerState(D3DDevice^ d3dDevice, D3DFill fill, D3DCull cull, Boolean scissorEnable, Boolean frontCounterClockwise, Boolean antialisedLineEnable, Boolean multiSampleEnable, Boolean depthClipping, Int32 depthBias, Single depthBiasClamp, Single slopeScaledDepthBias)
			{
				D3D11_RASTERIZER_DESC rasterizerDesc;
				rasterizerDesc.FillMode = static_cast<D3D11_FILL_MODE>(fill);
				rasterizerDesc.CullMode = static_cast<D3D11_CULL_MODE>(cull);
				rasterizerDesc.FrontCounterClockwise = frontCounterClockwise;
				rasterizerDesc.DepthClipEnable = depthClipping;
				rasterizerDesc.DepthBias = depthBias;
				rasterizerDesc.DepthBiasClamp = depthBiasClamp;
				rasterizerDesc.SlopeScaledDepthBias = slopeScaledDepthBias;
				rasterizerDesc.ScissorEnable = scissorEnable;
				rasterizerDesc.MultisampleEnable = multiSampleEnable;
				rasterizerDesc.AntialiasedLineEnable = antialisedLineEnable;

				ID3D11RasterizerState* state;
				d3dDevice->Native->CreateRasterizerState(&rasterizerDesc, &state);
				Assing(state);
			}
		};
		//--------------------------------------------------------------------------------------
		public ref class D3DDepthStencilView : GraphicsObject {
			GRAPHICS_OBJECT(D3DDepthStencilView, ID3D11DepthStencilView);
		public:
			D3DDepthStencilView(D3DDevice^ d3dDevice, Rectangle windowSize)
			{
				// Create depth stencil texture
				com_ptr<ID3D11Texture2D> _d3dDepthStencilTexture;
				D3D11_TEXTURE2D_DESC textureDesc;
				ZeroMemory(&textureDesc, sizeof(textureDesc));
				textureDesc.Width = (UINT)windowSize.Width;
				textureDesc.Height = (UINT)windowSize.Height;
				textureDesc.MipLevels = 1;
				textureDesc.ArraySize = 1;
				textureDesc.Format = DXGI_FORMAT_D16_UNORM;
				textureDesc.SampleDesc.Count = 1;
				textureDesc.SampleDesc.Quality = 0;
				textureDesc.Usage = D3D11_USAGE_DEFAULT;
				textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				textureDesc.CPUAccessFlags = 0;
				textureDesc.MiscFlags = 0;
				ThrowIfFailed(d3dDevice->Native->CreateTexture2D(&textureDesc, nullptr, &_d3dDepthStencilTexture));

				// Create the depth stencil view
				D3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc;
				ZeroMemory(&stencilDesc, sizeof(stencilDesc));
				stencilDesc.Format = textureDesc.Format;
				stencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
				stencilDesc.Texture2D.MipSlice = 0;

				ID3D11DepthStencilView* view;
				ThrowIfFailed(d3dDevice->Native->CreateDepthStencilView(_d3dDepthStencilTexture, &stencilDesc, &view));
				Assing(view);
			}

			D3DDepthStencilView(D3DDevice^ d3dDevice, D3DTexture2D^ texture)
			{
				D3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc;
				ZeroMemory(&stencilDesc, sizeof(stencilDesc));
				stencilDesc.Format = static_cast<DXGI_FORMAT>(texture->_format);
				stencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
				stencilDesc.Texture2D.MipSlice = 0;

				ID3D11DepthStencilView* view;
				ThrowIfFailed(d3dDevice->Native->CreateDepthStencilView(texture->Native, &stencilDesc, &view));
				Assing(view);
			}
		};
		//--------------------------------------------------------------------------------------
		public ref class D3DResource abstract : GraphicsObject {
			GRAPHICS_OBJECT(D3DResource, ID3D11Resource);
			D3DResource() {}
		};

		public ref class D3DBuffer : public D3DResource {
			GRAPHICS_OBJECT(D3DBuffer, ID3D11Buffer);
		internal:
			Type^ TypeOfData;
		public:
			static D3DBuffer^ Create(D3DDevice^ d3dDevice, D3DBind bind, D3DUsage usage, D3DAccess access, Type^ type)
			{
				D3D11_BUFFER_DESC bufferDesc;
				D3D11_SUBRESOURCE_DATA bufferData;
				D3D11_SUBRESOURCE_DATA* bufferDataPointer = nullptr;
				ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
				ZeroMemory(&bufferData, sizeof(D3D11_SUBRESOURCE_DATA));

				bufferDesc.CPUAccessFlags = (UINT)access;
				bufferDesc.BindFlags = (D3D11_BIND_FLAG)bind;
				bufferDesc.ByteWidth = Marshal::SizeOf(type);
				bufferDesc.Usage = (D3D11_USAGE)usage;

				ID3D11Buffer* buffer;
				ThrowIfFailed(d3dDevice->Native->CreateBuffer(&bufferDesc, bufferDataPointer, &buffer));
				D3DBuffer^ result = gcnew D3DBuffer(buffer);
				result->TypeOfData = type;
				return result;
			}

			generic<typename T> static D3DBuffer^ Create(D3DDevice^ d3dDevice, D3DBind bind, D3DUsage usage, D3DAccess access, array<T>^ data)
			{
				ThrowIf(data == nullptr);

				D3D11_BUFFER_DESC bufferDesc;
				D3D11_SUBRESOURCE_DATA bufferData;
				D3D11_SUBRESOURCE_DATA* bufferDataPointer = nullptr;
				ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
				ZeroMemory(&bufferData, sizeof(D3D11_SUBRESOURCE_DATA));

				bufferDesc.CPUAccessFlags = (UINT)access;
				bufferDesc.BindFlags = (D3D11_BIND_FLAG)bind;
				bufferDesc.ByteWidth = Marshal::SizeOf(T::typeid);
				bufferDesc.Usage = (D3D11_USAGE)usage;

				pin_ptr<T> pinnedInstance = &data[0];

				bufferDesc.ByteWidth = Marshal::SizeOf(T::typeid) * data->Length;
				bufferData.pSysMem = pinnedInstance;
				bufferDataPointer = &bufferData;

				ID3D11Buffer* buffer;
				ThrowIfFailed(d3dDevice->Native->CreateBuffer(&bufferDesc, bufferDataPointer, &buffer));
				D3DBuffer^ result = gcnew D3DBuffer(buffer);
				result->TypeOfData = T::typeid;
				return result;
			}
		};
		//--------------------------------------------------------------------------------------

		public enum class D3DSemantic
		{
			COLOR,
			NORMAL,
			POSITION,
			TEXCOORD,
		};
		public enum class D3DInputClassification
		{
			PerVertex = D3D11_INPUT_PER_VERTEX_DATA,
			PerInstance = D3D11_INPUT_PER_INSTANCE_DATA
		};
		public value struct D3DInputElement
		{
			D3DSemantic SemanticName;
			UINT SemanticIndex;
			DxgiFormat Format;
			UINT InputSlot;
			UINT AlignedByteOffset;
			D3DInputClassification InputSlotClass;
			UINT InstanceDataStepRate;

			D3DInputElement(
				D3DSemantic semanticName,
				UINT semanticIndex,
				DxgiFormat format,
				UINT inputSlot,
				UINT alignedByteOffset,
				D3DInputClassification inputSlotClass,
				UINT instanceDataStepRate)
			{
				SemanticName = semanticName;
				SemanticIndex = semanticIndex;
				Format = format;
				InputSlot = inputSlot;
				AlignedByteOffset = alignedByteOffset;
				InputSlotClass = inputSlotClass;
				InstanceDataStepRate = instanceDataStepRate;
			}
		};

		public ref class D3DInputLayout : GraphicsObject {
			GRAPHICS_OBJECT(D3DInputLayout, ID3D11InputLayout);
		public:
			D3DInputLayout(D3DDevice^ d3dDevice, array<D3DInputElement>^ inputElements, array<Byte>^ shaderBytecode)
			{
				pin_ptr<BYTE> pinnedShaderBytecode = &shaderBytecode[0];

				std::vector<D3D11_INPUT_ELEMENT_DESC> elements;

				for (auto i = 0; i < inputElements->Length; i++)
				{
					D3DInputElement element = inputElements[i];
					D3D11_INPUT_ELEMENT_DESC desc;
					switch (element.SemanticName)
					{
					case D3DSemantic::COLOR:desc.SemanticName = "COLOR"; break;
					case D3DSemantic::NORMAL:desc.SemanticName = "NORMAL"; break;
					case D3DSemantic::TEXCOORD:desc.SemanticName = "TEXCOORD"; break;
					case D3DSemantic::POSITION:desc.SemanticName = "POSITION"; break;
					}
					desc.SemanticIndex = element.SemanticIndex;
					desc.Format = (DXGI_FORMAT)element.Format;
					desc.InputSlot = element.InputSlot;
					desc.AlignedByteOffset = element.AlignedByteOffset;
					desc.InputSlotClass = (D3D11_INPUT_CLASSIFICATION)element.InputSlotClass;
					desc.InstanceDataStepRate = element.InstanceDataStepRate;
					elements.push_back(desc);
				}

				ID3D11InputLayout* layout;
				ThrowIfFailed(d3dDevice->Native->CreateInputLayout(&elements[0], elements.size(), &pinnedShaderBytecode[0], shaderBytecode->Length, &layout));
				Assing(layout);
			}
		};
		//--------------------------------------------------------------------------------------
		public ref class D3DVertexShader : GraphicsObject {
			GRAPHICS_OBJECT(D3DVertexShader, ID3D11VertexShader);
		public:
			D3DVertexShader(D3DDevice^ d3dDevice, array<Byte>^ shaderBytecode)
			{
				pin_ptr<BYTE> pinnedShaderBytecode = &shaderBytecode[0];

				ID3D11VertexShader* shader;
				ThrowIfFailed(d3dDevice->Native->CreateVertexShader(&pinnedShaderBytecode[0], shaderBytecode->Length, nullptr, &shader));
				Assing(shader);
			}
		};
		//--------------------------------------------------------------------------------------
		public ref class D3DPixelShader : GraphicsObject {
			GRAPHICS_OBJECT(D3DPixelShader, ID3D11PixelShader);
		public:
			D3DPixelShader(D3DDevice^ d3dDevice, array<Byte>^ shaderBytecode)
			{
				pin_ptr<BYTE> pinnedShaderBytecode = &shaderBytecode[0];

				ID3D11PixelShader* shader;
				ThrowIfFailed(d3dDevice->Native->CreatePixelShader(&pinnedShaderBytecode[0], shaderBytecode->Length, nullptr, &shader));
				Assing(shader);
			}
		};
		//--------------------------------------------------------------------------------------
		public enum class D3DTextureFiltering
		{
			D3D11_FILTER_MIN_MAG_MIP_POINT = 0,
			Linear = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR,
			D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x4,
			D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR = 0x5,
			D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT = 0x10,
			D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x11,
			D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT = 0x14,
			D3D11_FILTER_MIN_MAG_MIP_LINEAR = 0x15,
			Anisotropic = D3D11_FILTER_ANISOTROPIC,
			D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT = 0x80,
			D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR = 0x81,
			D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x84,
			D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR = 0x85,
			D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT = 0x90,
			D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
			D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT = 0x94,
			D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR = 0x95,
			D3D11_FILTER_COMPARISON_ANISOTROPIC = 0xd5,
			D3D11_FILTER_MINIMUM_MIN_MAG_MIP_POINT = 0x100,
			D3D11_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR = 0x101,
			D3D11_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x104,
			D3D11_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR = 0x105,
			D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT = 0x110,
			D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x111,
			D3D11_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT = 0x114,
			D3D11_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR = 0x115,
			D3D11_FILTER_MINIMUM_ANISOTROPIC = 0x155,
			D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_POINT = 0x180,
			D3D11_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR = 0x181,
			D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x184,
			D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR = 0x185,
			D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT = 0x190,
			D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x191,
			D3D11_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT = 0x194,
			D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR = 0x195,
			D3D11_FILTER_MAXIMUM_ANISOTROPIC = 0x1d5
		};

		public enum class D3DTextureAddressing
		{
			Wrap = D3D11_TEXTURE_ADDRESS_WRAP,
			Mirror = D3D11_TEXTURE_ADDRESS_MIRROR,
			Clamp = D3D11_TEXTURE_ADDRESS_CLAMP,
			Border = D3D11_TEXTURE_ADDRESS_BORDER,
			MirrorOnce = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
		};

		public ref class D3DSamplerState : GraphicsObject {
			GRAPHICS_OBJECT(D3DSamplerState, ID3D11SamplerState);

		public:

			D3DSamplerState(D3DDevice^ d3dDevice, D3DTextureAddressing addressU, D3DTextureAddressing addressV, D3DTextureFiltering filter, UINT anisotropy) : D3DSamplerState(d3dDevice, addressU, addressV, filter, anisotropy, 0, D3D11_MAX_MAXANISOTROPY) {}
			D3DSamplerState(D3DDevice^ d3dDevice, D3DTextureAddressing addressU, D3DTextureAddressing addressV, D3DTextureFiltering filter, UINT anisotropy, FLOAT minLOD, FLOAT maxLOD)
			{
				D3D11_SAMPLER_DESC samplerDesc;
				ZeroMemory(&samplerDesc, sizeof(samplerDesc));
				samplerDesc.MinLOD = minLOD;
				samplerDesc.MaxLOD = maxLOD;
				samplerDesc.Filter = static_cast<D3D11_FILTER>(filter);
				samplerDesc.MaxAnisotropy = anisotropy;
				samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
				samplerDesc.AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(addressU);
				samplerDesc.AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(addressV);
				samplerDesc.AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(addressV); // just duplicate V

				ID3D11SamplerState* samplerState;
				ThrowIfFailed(d3dDevice->Native->CreateSamplerState(&samplerDesc, &samplerState));
				Assing(samplerState);
			}

			static D3DSamplerState^ CreateWrapSampler(D3DDevice^ d3dDevice)
			{
				D3D11_SAMPLER_DESC samplerDesc;
				ZeroMemory(&samplerDesc, sizeof(samplerDesc));
				samplerDesc.MinLOD = 0;
				samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
				samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
				samplerDesc.MaxAnisotropy = D3D11_MAX_MAXANISOTROPY;
				samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
				samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
				samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
				samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

				ID3D11SamplerState* samplerState;
				ThrowIfFailed(d3dDevice->Native->CreateSamplerState(&samplerDesc, &samplerState));
				return gcnew D3DSamplerState(samplerState);
			}

			static D3DSamplerState^ CreateClampSampler(D3DDevice^ d3dDevice)
			{
				D3D11_SAMPLER_DESC samplerDesc;
				ZeroMemory(&samplerDesc, sizeof(samplerDesc));
				samplerDesc.MinLOD = 0;
				samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
				samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				samplerDesc.MaxAnisotropy = 1;
				samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
				samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
				samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
				samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

				ID3D11SamplerState* samplerState;
				ThrowIfFailed(d3dDevice->Native->CreateSamplerState(&samplerDesc, &samplerState));
				return gcnew D3DSamplerState(samplerState);
			}
		};
		//--------------------------------------------------------------------------------------
		public ref class D3DBlendState : GraphicsObject {
			GRAPHICS_OBJECT(D3DBlendState, ID3D11BlendState);
		public:
			D3DBlendState(D3DDevice^ d3dDevice)
			{
				D3D11_BLEND_DESC blendDesc;
				ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
				blendDesc.AlphaToCoverageEnable = FALSE;
				blendDesc.IndependentBlendEnable = FALSE;
				blendDesc.RenderTarget[0].BlendEnable = TRUE;
				blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
				blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
				blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
				blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
				blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
				blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
				blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

				ID3D11BlendState* blendState;
				ThrowIfFailed(d3dDevice->Native->CreateBlendState(&blendDesc, &blendState));
				Assing(blendState);
			}
		};
		//--------------------------------------------------------------------------------------

		public enum class D3DSrvDimension {
			D3D_SRV_DIMENSION_UNKNOWN = 0,
			D3D_SRV_DIMENSION_BUFFER = 1,
			D3D_SRV_DIMENSION_TEXTURE1D = 2,
			D3D_SRV_DIMENSION_TEXTURE1DARRAY = 3,
			D3D_SRV_DIMENSION_TEXTURE2D = 4,
			D3D_SRV_DIMENSION_TEXTURE2DARRAY = 5,
			D3D_SRV_DIMENSION_TEXTURE2DMS = 6,
			D3D_SRV_DIMENSION_TEXTURE2DMSARRAY = 7,
			D3D_SRV_DIMENSION_TEXTURE3D = 8,
			D3D_SRV_DIMENSION_TEXTURECUBE = 9,
			D3D_SRV_DIMENSION_TEXTURECUBEARRAY = 10,
			D3D_SRV_DIMENSION_BUFFEREX = 11,
			D3D10_SRV_DIMENSION_UNKNOWN = D3D_SRV_DIMENSION_UNKNOWN,
			D3D10_SRV_DIMENSION_BUFFER = D3D_SRV_DIMENSION_BUFFER,
			D3D10_SRV_DIMENSION_TEXTURE1D = D3D_SRV_DIMENSION_TEXTURE1D,
			D3D10_SRV_DIMENSION_TEXTURE1DARRAY = D3D_SRV_DIMENSION_TEXTURE1DARRAY,
			D3D10_SRV_DIMENSION_TEXTURE2D = D3D_SRV_DIMENSION_TEXTURE2D,
			D3D10_SRV_DIMENSION_TEXTURE2DARRAY = D3D_SRV_DIMENSION_TEXTURE2DARRAY,
			D3D10_SRV_DIMENSION_TEXTURE2DMS = D3D_SRV_DIMENSION_TEXTURE2DMS,
			D3D10_SRV_DIMENSION_TEXTURE2DMSARRAY = D3D_SRV_DIMENSION_TEXTURE2DMSARRAY,
			D3D10_SRV_DIMENSION_TEXTURE3D = D3D_SRV_DIMENSION_TEXTURE3D,
			D3D10_SRV_DIMENSION_TEXTURECUBE = D3D_SRV_DIMENSION_TEXTURECUBE,
			D3D10_1_SRV_DIMENSION_UNKNOWN = D3D_SRV_DIMENSION_UNKNOWN,
			D3D10_1_SRV_DIMENSION_BUFFER = D3D_SRV_DIMENSION_BUFFER,
			D3D10_1_SRV_DIMENSION_TEXTURE1D = D3D_SRV_DIMENSION_TEXTURE1D,
			D3D10_1_SRV_DIMENSION_TEXTURE1DARRAY = D3D_SRV_DIMENSION_TEXTURE1DARRAY,
			D3D10_1_SRV_DIMENSION_TEXTURE2D = D3D_SRV_DIMENSION_TEXTURE2D,
			D3D10_1_SRV_DIMENSION_TEXTURE2DARRAY = D3D_SRV_DIMENSION_TEXTURE2DARRAY,
			D3D10_1_SRV_DIMENSION_TEXTURE2DMS = D3D_SRV_DIMENSION_TEXTURE2DMS,
			D3D10_1_SRV_DIMENSION_TEXTURE2DMSARRAY = D3D_SRV_DIMENSION_TEXTURE2DMSARRAY,
			D3D10_1_SRV_DIMENSION_TEXTURE3D = D3D_SRV_DIMENSION_TEXTURE3D,
			D3D10_1_SRV_DIMENSION_TEXTURECUBE = D3D_SRV_DIMENSION_TEXTURECUBE,
			D3D10_1_SRV_DIMENSION_TEXTURECUBEARRAY = D3D_SRV_DIMENSION_TEXTURECUBEARRAY,
			D3D11_SRV_DIMENSION_UNKNOWN = D3D_SRV_DIMENSION_UNKNOWN,
			D3D11_SRV_DIMENSION_BUFFER = D3D_SRV_DIMENSION_BUFFER,
			D3D11_SRV_DIMENSION_TEXTURE1D = D3D_SRV_DIMENSION_TEXTURE1D,
			D3D11_SRV_DIMENSION_TEXTURE1DARRAY = D3D_SRV_DIMENSION_TEXTURE1DARRAY,
			D3D11_SRV_DIMENSION_TEXTURE2D = D3D_SRV_DIMENSION_TEXTURE2D,
			D3D11_SRV_DIMENSION_TEXTURE2DARRAY = D3D_SRV_DIMENSION_TEXTURE2DARRAY,
			D3D11_SRV_DIMENSION_TEXTURE2DMS = D3D_SRV_DIMENSION_TEXTURE2DMS,
			D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY = D3D_SRV_DIMENSION_TEXTURE2DMSARRAY,
			D3D11_SRV_DIMENSION_TEXTURE3D = D3D_SRV_DIMENSION_TEXTURE3D,
			D3D11_SRV_DIMENSION_TEXTURECUBE = D3D_SRV_DIMENSION_TEXTURECUBE,
			D3D11_SRV_DIMENSION_TEXTURECUBEARRAY = D3D_SRV_DIMENSION_TEXTURECUBEARRAY,
			D3D11_SRV_DIMENSION_BUFFEREX = D3D_SRV_DIMENSION_BUFFEREX
		};

		public ref class D3DShaderResourceView : GraphicsObject {
			GRAPHICS_OBJECT(D3DShaderResourceView, ID3D11ShaderResourceView);

		public:
			D3DShaderResourceView(D3DDevice^ d3dDevice, D3DTexture2D^ texture, DxgiFormat format)
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.Format = static_cast<DXGI_FORMAT>(format);
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MostDetailedMip = 0;
				desc.Texture2D.MipLevels = texture->_mipLevels;

				ID3D11ShaderResourceView* view;
				ThrowIfFailed(d3dDevice->Native->CreateShaderResourceView(texture->Native, &desc, &view));
				Assing(view);
			}
		};
		//--------------------------------------------------------------------------------------
		public ref class DxgiFactory : GraphicsObject {
			GRAPHICS_OBJECT(DxgiFactory, IDXGIFactory2);
		public:
		};
		//--------------------------------------------------------------------------------------
		public ref class DxgiDevice : GraphicsObject {
			GRAPHICS_OBJECT(DxgiDevice, IDXGIDevice1);
		public:
			DxgiDevice(D3DDevice^ d3dDevice);
		};
		//--------------------------------------------------------------------------------------
		public enum class DXGIResult
		{
			OK,
			ShouldIdle,
			ShouldResize,
			ShouldRecreate,
		};

		public ref class DxgiSwapChain : GraphicsObject {
			GRAPHICS_OBJECT(DxgiSwapChain, IDXGISwapChain1);

			static DXGIResult MakeResult(HRESULT hr)
			{
				if (FAILED(hr))
				{
					if (hr == DXGI_STATUS_OCCLUDED) return DXGIResult::ShouldIdle;
					if (hr == DXGI_ERROR_DEVICE_RESET) return DXGIResult::ShouldRecreate;
					if (hr == DXGI_ERROR_DEVICE_REMOVED) return DXGIResult::ShouldRecreate;
					if (hr == DXGI_STATUS_MODE_CHANGED) return DXGIResult::ShouldResize;

					ThrowIfFailed(hr);
				}

				return DXGIResult::OK;
			}

		public:

			DxgiSwapChain(D3DDevice^ d3dDevice, IntPtr windowHandle);

			void ResizeToWindow()
			{
				ThrowIfFailed(Native->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0));
			}

			DXGIResult Present(UINT interval, array<Rectangle>^ dirties, UINT dirtiesCount)
			{
				DXGI_PRESENT_PARAMETERS presentParams;
				ZeroMemory(&presentParams, (sizeof(presentParams)));
				presentParams.DirtyRectsCount = dirtiesCount;

				if (dirties == nullptr || dirties->Length == 0)
				{
					return MakeResult(Native->Present1(interval, 0, &presentParams));
				}
				else
				{
					pin_ptr<Rectangle> dirtiesPinned = &dirties[0];
					presentParams.pDirtyRects = reinterpret_cast<RECT*>(dirtiesPinned);
					return MakeResult(Native->Present1(interval, 0, &presentParams));
				}
			}

			DXGIResult Present(UINT interval)
			{
				return MakeResult(Native->Present(interval, 0));
			}
		};
	}
}
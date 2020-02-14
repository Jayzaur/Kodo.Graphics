#include "D3D.hpp"

namespace Kodo
{
	namespace Graphics
	{
		//--------------------------------------------------------------------------------------

		D3DDevice::D3DDevice(bool debug)
		{
			ID3D11Device* d3dDevice;
			ID3D11DeviceContext* d3dDeviceContext;

			D3D_FEATURE_LEVEL featureLevels[]{ D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_9_1 };

			ThrowIfFailed(D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				debug ? D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG : D3D11_CREATE_DEVICE_BGRA_SUPPORT,
				featureLevels, 1,
				D3D11_SDK_VERSION,
				&d3dDevice,
				&featureLevels[1],
				&d3dDeviceContext));

			Assing(d3dDevice);
			deviceContext = gcnew D3DDeviceContext(d3dDeviceContext);

			if (debug)
			{
				com_ptr<ID3D11Debug> d3dDebug;
				com_ptr<ID3D11InfoQueue> d3dInfoQueue;

				ThrowIfFailed(d3dDevice->QueryInterface(IID_PPV_ARGS(&d3dDebug)));
				ThrowIfFailed(d3dDebug->QueryInterface(IID_PPV_ARGS(&d3dInfoQueue)));
				ThrowIfFailed(d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true));
				ThrowIfFailed(d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING, true));
				ThrowIfFailed(d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true));
			}
		}
		//--------------------------------------------------------------------------------------
		generic<typename T> void D3DDeviceContext::UpdateGPUResource(D3DResource^ destinationResource, T sourceData)
		{
			pin_ptr<T> pinnedSourceData = &sourceData;
			Native->UpdateSubresource(destinationResource->Native, 0, nullptr, pinnedSourceData, 0, 0);
		}

		generic<typename T> void D3DDeviceContext::UpdateGPUResource(D3DResource^ destinationResource, array<T>^ sourceData, D3DMap mapType)
		{
			pin_ptr<T> pinnedSourceData = &sourceData[0];

			D3D11_MAPPED_SUBRESOURCE mappedBuffer;
			ZeroMemory(&mappedBuffer, sizeof(D3D11_MAPPED_SUBRESOURCE));

			ThrowIfFailed(Native->Map(destinationResource->Native, 0, static_cast<D3D11_MAP>(mapType), 0, &mappedBuffer));

			size_t dataSizeInBytes = Marshal::SizeOf(T::typeid) * sourceData->Length;

			memcpy_s(mappedBuffer.pData, dataSizeInBytes, pinnedSourceData, dataSizeInBytes);

			Native->Unmap(destinationResource->Native, 0);
		}

		void D3DDeviceContext::DrawInstanced(UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation, UINT startInstanceLocation)
		{
			Native->DrawInstanced(vertexCountPerInstance, instanceCount, startVertexLocation, startInstanceLocation);
		}
		void D3DDeviceContext::ClearTarget(D3DRenderTargetView^ renderTargetView, Color clearColor)
		{
			Native->ClearRenderTargetView(renderTargetView->Native, (const FLOAT*)&clearColor);
		}
		void D3DDeviceContext::ClearDepth(D3DDepthStencilView^ depthStencilView, FLOAT depth)
		{
			Native->ClearDepthStencilView(depthStencilView->Native, D3D11_CLEAR_DEPTH, depth, 0);
		}
		void D3DDeviceContext::ClearDepthAndStencil(D3DDepthStencilView^ depthStencilView, ClearFlag clearFlag, FLOAT depth, BYTE stencil)
		{
			Native->ClearDepthStencilView(depthStencilView->Native, static_cast<UINT>(clearFlag), depth, stencil);
		}
		void D3DDeviceContext::IASetInputLayout(D3DInputLayout^ inputLayout)
		{
			Native->IASetInputLayout(inputLayout->Native);
		}
		void D3DDeviceContext::IASetVertexBuffer(D3DBuffer^ buffer)
		{
			UINT bufferStrides[]{ (UINT)Marshal::SizeOf(buffer->TypeOfData) };
			UINT bufferOffsets[]{ 0 };
			ID3D11Buffer* bufferPointers[]{ buffer->Native };
			Native->IASetVertexBuffers(0, 1, bufferPointers, bufferStrides, bufferOffsets);
		}
		void D3DDeviceContext::IASetVertexBuffers(D3DBuffer^ bufferA, D3DBuffer^ bufferB)
		{
			UINT bufferStrides[]{ (UINT)Marshal::SizeOf(bufferA->TypeOfData),(UINT)Marshal::SizeOf(bufferB->TypeOfData) };
			UINT bufferOffsets[]{ 0, 0 };
			ID3D11Buffer* bufferPointers[]{ bufferA->Native, bufferB->Native };
			Native->IASetVertexBuffers(0, 2, bufferPointers, bufferStrides, bufferOffsets);
		}
		void D3DDeviceContext::VSSetShader(D3DVertexShader^ vertexShader)
		{
			Native->VSSetShader(vertexShader->Native, nullptr, 0);
		}
		void D3DDeviceContext::VSSetConstantBuffer(UINT slot, D3DBuffer^ constantBuffer)
		{
			ID3D11Buffer* buffer = constantBuffer->Native;
			Native->VSSetConstantBuffers(slot, 1, &buffer);
		}
		void D3DDeviceContext::PSSetShader(D3DPixelShader^ pixelShader)
		{
			Native->PSSetShader(pixelShader->Native, nullptr, 0);
		}
		void D3DDeviceContext::PSSetConstantBuffer(UINT slot, D3DBuffer^ constantBuffer)
		{
			ID3D11Buffer* buffer = constantBuffer->Native;
			Native->PSSetConstantBuffers(slot, 1, &buffer);
		}
		void D3DDeviceContext::PSSetSampler(D3DSamplerState^ samplerState)
		{
			if (samplerState == nullptr)
			{
				ID3D11SamplerState* sampler = nullptr;
				Native->PSSetSamplers(0, 1, &sampler);
			}
			else
			{
				ID3D11SamplerState* sampler = samplerState->Native;
				Native->PSSetSamplers(0, 1, &sampler);
			}
		}
		void D3DDeviceContext::PSSetSamplers(D3DSamplerState^ samplerA, D3DSamplerState^ samplerB)
		{
			ID3D11SamplerState* samplers[]{ samplerA == nullptr ? nullptr : samplerA->Native, samplerB == nullptr ? nullptr : samplerB->Native };
			Native->PSSetSamplers(0, 2, samplers);
		}
		void D3DDeviceContext::PSSetResource(D3DShaderResourceView^ shaderResource)
		{
			if (shaderResource == nullptr)
			{
				ID3D11ShaderResourceView* resource = nullptr;
				Native->PSSetShaderResources(0, 1, &resource);
			}
			else
			{
				ID3D11ShaderResourceView* resource = shaderResource->Native;
				Native->PSSetShaderResources(0, 1, &resource);
			}
		}
		void D3DDeviceContext::PSSetResources(D3DShaderResourceView^ resourceA, D3DShaderResourceView^ resourceB)
		{
			ID3D11ShaderResourceView* resources[]{ resourceA == nullptr ? nullptr : resourceA->Native, resourceB == nullptr ? nullptr : resourceB->Native };
			Native->PSSetShaderResources(0, 2, resources);
		}
		void D3DDeviceContext::OMSetBlendState(D3DBlendState^ blendState)
		{
			Native->OMSetBlendState(blendState->Native, 0, 0xffffffff);
		}
		void D3DDeviceContext::RSSetState(D3DRasterizerState^ rasterizerState)
		{
			Native->RSSetState(rasterizerState == nullptr ? nullptr : rasterizerState->Native);
		}
		void D3DDeviceContext::OMSetRenderTarget(D3DRenderTargetView^ renderTargetView)
		{
			if (renderTargetView == nullptr)
			{
				ID3D11RenderTargetView* empty = nullptr;
				Native->OMSetRenderTargets(1, &empty, nullptr);
			}
			else
			{
				ID3D11RenderTargetView* view = renderTargetView->Native;
				Native->OMSetRenderTargets(1, &view, nullptr);
			}
		}
		void D3DDeviceContext::OMSetRenderTarget(D3DRenderTargetView^ renderTargetView, D3DDepthStencilView^ depthStencilView)
		{
			ID3D11RenderTargetView* view = renderTargetView->Native;
			ID3D11DepthStencilView* dview = depthStencilView->Native;
			Native->OMSetRenderTargets(1, &view, dview);
		}
		//--------------------------------------------------------------------------------------
		DxgiDevice::DxgiDevice(D3DDevice^ d3dDevice)
		{
			IDXGIDevice1* dxgiDevice;
			ThrowIfFailed(d3dDevice->Native->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));
			Assing(dxgiDevice);

			com_ptr<IDXGIAdapter> dxgiAdapter;
			ThrowIfFailed(dxgiDevice->GetAdapter(&dxgiAdapter));

			IDXGIFactory2* dxgiFactory;
			ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));

			if (GlobalAssets::DXGI == nullptr)
			{
				GlobalAssets::DXGI = gcnew DxgiFactory(dxgiFactory);
			}
		}
		//--------------------------------------------------------------------------------------
		DxgiSwapChain::DxgiSwapChain(D3DDevice^ d3dDevice, IntPtr windowHandle)
		{
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
			ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapChainDesc.Width = 0;
			swapChainDesc.Height = 0;
			swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 2;

			IDXGISwapChain1* swapChain;
			ThrowIfFailed(GlobalAssets::DXGI->Native->CreateSwapChainForHwnd(d3dDevice->Native, (HWND)windowHandle.ToPointer(), &swapChainDesc, nullptr, nullptr, &swapChain));
			Assing(swapChain);
		}
		//--------------------------------------------------------------------------------------
		D3DTexture2D::D3DTexture2D(DxgiSwapChain^ dxgiSwapChain)
		{
			ID3D11Texture2D* d3dTexture2D;
			ThrowIfFailed(dxgiSwapChain->Native->GetBuffer(0, IID_PPV_ARGS(&d3dTexture2D)));
			Assing(d3dTexture2D);

			// retrieve the texture format
			D3D11_TEXTURE2D_DESC textureDesc;
			d3dTexture2D->GetDesc(&textureDesc);
			_format = (DxgiFormat)textureDesc.Format;
		}
		D3DTexture2D::D3DTexture2D(D3DDevice^ d3dDevice, UINT width, UINT height, DxgiFormat format, D3DBind bind, D3DUsage usage, D3DAccess access, D3DMisc misc, UINT mipLevels, UINT sampleCount, UINT sampleQuality)
		{
			_format = format;
			_mipLevels = mipLevels;

			D3D11_TEXTURE2D_DESC textureDesc;
			ZeroMemory(&textureDesc, sizeof(textureDesc));
			textureDesc.Width = width;
			textureDesc.Height = height;
			textureDesc.MipLevels = mipLevels;
			textureDesc.ArraySize = 1;
			textureDesc.Format = static_cast<DXGI_FORMAT>(format);
			textureDesc.SampleDesc.Count = sampleCount;
			textureDesc.SampleDesc.Quality = sampleQuality;
			textureDesc.CPUAccessFlags = static_cast<UINT>(access);
			textureDesc.BindFlags = static_cast<UINT>(bind);
			textureDesc.Usage = static_cast<D3D11_USAGE>(usage);
			textureDesc.MiscFlags = static_cast<UINT>(misc);

			ID3D11Texture2D* texture;
			ThrowIfFailed(d3dDevice->Native->CreateTexture2D(&textureDesc, nullptr, &texture));
			Assing(texture);
		}
		//--------------------------------------------------------------------------------------
		D3DRenderTargetView::D3DRenderTargetView(D3DDevice^ d3dDevice, DxgiSwapChain^ dxgiSwapChain)
		{
			com_ptr<ID3D11Texture2D> backBufferTexture;
			ThrowIfFailed(dxgiSwapChain->Native->GetBuffer(0, IID_PPV_ARGS(&backBufferTexture)));

			ID3D11RenderTargetView* d3dRenderTargetView;
			ThrowIfFailed(d3dDevice->Native->CreateRenderTargetView(backBufferTexture, nullptr, &d3dRenderTargetView));
			Assing(d3dRenderTargetView);
		}

		D3DRenderTargetView::D3DRenderTargetView(D3DDevice^ d3dDevice, D3DTexture2D^ backBufferTexture)
		{
			D3D11_RENDER_TARGET_VIEW_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Format = static_cast<DXGI_FORMAT>(backBufferTexture->_format);
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = 0;

			ID3D11RenderTargetView* d3dRenderTargetView;
			ThrowIfFailed(d3dDevice->Native->CreateRenderTargetView(backBufferTexture->Native, &desc, &d3dRenderTargetView));
			Assing(d3dRenderTargetView);
		}
	}
}
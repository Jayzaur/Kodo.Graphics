#include "Graphics.hpp"

#include "WIC/WIC.hpp"

namespace Kodo
{
	namespace Graphics
	{
		Imaging::WicFactory^ GlobalAssets::WIC::get()
		{
			if (FactoryWic == nullptr)
			{
				IWICImagingFactory2* wicFactory;
				ThrowIfFailed(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory)));
				FactoryWic = gcnew Imaging::WicFactory(wicFactory);
			}

			return FactoryWic;
		}
	}
}

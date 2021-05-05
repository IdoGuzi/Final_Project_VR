#include <memory>
#include <openvr_driver.h>
#include <HeadSetProvider.h>

#define HMD_DLL_EXPORT extern "C" __declspec( dllexport )
#define HMD_DLL_IMPORT extern "C" __declspec( dllimport )

static std::shared_ptr<HeadsetProvider> Driver;

HMD_DLL_EXPORT void* HmdDriverFactory(const char* pInterfaceName, int* pReturnCode)
{
	if (0 == strcmp(IServerTrackedDeviceProvider_Version, pInterfaceName)){
		if (!Driver) Driver = std::make_shared<HeadsetProvider>();
		return &Driver;
	}

	if (pReturnCode)
		*pReturnCode = VRInitError_Init_InterfaceNotFound;

	return nullptr;
}
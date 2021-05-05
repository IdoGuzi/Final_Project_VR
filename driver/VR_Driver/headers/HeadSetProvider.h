#pragma once

#include <openvr_driver.h>
#include <HeadSetDriver.h>
using namespace vr;

class HeadsetProvider : public IServerTrackedDeviceProvider {
private:
	HeadSetDriver* deviceDriver = nullptr;

public:

	/**
	* adds the object to openvr
	*/
	EVRInitError Init(IVRDriverContext* pDriverContext);
	void Cleanup();
	void RunFrame();
	const char* const* GetInterfaceVersions(){ return vr::k_InterfaceVersions; }
	bool ShouldBlockStandbyMode();
	void EnterStandby() {}
	void LeaveStandby() {}
};
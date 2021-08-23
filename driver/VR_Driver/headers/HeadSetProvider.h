#pragma once

#include <openvr_driver.h>
#include <driverlog.h>
#include <HeadSetDriver.h>
#include <ControllerDriver.h>

/**
 * this class represent the provider of the headset driver.
 * holds the driver of the hardware and make the appropriate calls to all of the drivers.
 * (same as provider in windows drivers)
 */
class HeadSetProvider : public vr::IServerTrackedDeviceProvider{
private:
	HeadSetDriver* m_pNullHmdLatest = nullptr;
	CSampleControllerDriver* m_pController = nullptr;
public:

	//inhareted from IServerTrackedDeviceProvider
	//https://github.com/ValveSoftware/openvr/wiki/IServerTrackedDeviceProvider_Overview
	
	virtual vr::EVRInitError Init(vr::IVRDriverContext* pDriverContext);
	virtual void Cleanup();
	virtual const char* const* GetInterfaceVersions() { return vr::k_InterfaceVersions; }
	virtual void RunFrame();
	virtual bool ShouldBlockStandbyMode() { return false; }
	virtual void EnterStandby() {}
	virtual void LeaveStandby() {}
};
#pragma once
#include <thread>
#include <openvr_driver.h>

#if defined( _WINDOWS )
#include <windows.h>
#endif

/**
 * this class watch over the status of the drivers and notify about changes in the driver.
 */
class HeadSetWatchDog : public vr::IVRWatchdogProvider{
public:
	HeadSetWatchDog(){
		m_pWatchdogThread = nullptr;
	}

	virtual vr::EVRInitError Init(vr::IVRDriverContext* pDriverContext);
	virtual void Cleanup();

private:
	std::thread* m_pWatchdogThread;
};


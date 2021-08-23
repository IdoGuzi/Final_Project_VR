#pragma once

#include <openvr_driver.h>
#include <driverlog.h>
#include <MpuReader.h>





// keys for use with the settings API
static const char* const k_pch_Sample_Section = "driver_sample";
static const char* const k_pch_Sample_Port = "port";
static const char* const k_pch_Sample_SerialNumber_String = "serialNumber";
static const char* const k_pch_Sample_ModelNumber_String = "modelNumber";
static const char* const k_pch_Sample_WindowX_Int32 = "windowX";
static const char* const k_pch_Sample_WindowY_Int32 = "windowY";
static const char* const k_pch_Sample_WindowWidth_Int32 = "windowWidth";
static const char* const k_pch_Sample_WindowHeight_Int32 = "windowHeight";
static const char* const k_pch_Sample_RenderWidth_Int32 = "renderWidth";
static const char* const k_pch_Sample_RenderHeight_Int32 = "renderHeight";
static const char* const k_pch_Sample_SecondsFromVsyncToPhotons_Float = "secondsFromVsyncToPhotons";
static const char* const k_pch_Sample_DisplayFrequency_Float = "displayFrequency";

/**
 * create a new instace of a quaternion
 * @param w - the real value of the quaternion
 * @param x - the coefficient of the i axis
 * @param y - the coefficient of the j axis
 * @param z - the coefficient of the k axis
 * @return vr::HmdQuaternion_t object of quaternion
 */
inline vr::HmdQuaternion_t HmdQuaternion_Init(double w, double x, double y, double z){
	vr::HmdQuaternion_t quat;
	quat.w = w;
	quat.x = x;
	quat.y = y;
	quat.z = z;
	return quat;
}

/**
 * this class represent the object of the headset
 * by inhareting a tracking device and a display
 */
class HeadSetDriver : public vr::ITrackedDeviceServerDriver, public vr::IVRDisplayComponent{
private:
	//reader to read location of head set
	MpuReader* IMU;
	//flip the moment on the axises (in casee of backwords mounted device)
	bool xInvert, yInvert, zInvert;

	vr::TrackedDeviceIndex_t m_unObjectId;
	vr::PropertyContainerHandle_t m_ulPropertyContainer;

	std::string m_sSerialNumber;
	std::string m_sModelNumber;

	//display properties
	int32_t m_nWindowX;
	int32_t m_nWindowY;
	int32_t m_nWindowWidth;
	int32_t m_nWindowHeight;
	int32_t m_nRenderWidth;
	int32_t m_nRenderHeight;
	float m_flSecondsFromVsyncToPhotons;
	float m_flDisplayFrequency;
	float m_flIPD;

public:
	HeadSetDriver();
	virtual ~HeadSetDriver() { delete(IMU); }

	/**
	 * the serial number of a device doesn't matter but needed to work properly
	 * @returns the serial number of the device 
	 */
	std::string GetSerialNumber() const { return m_sSerialNumber; }
	/**
	 * call to getPose with relevent data of the object
	 */
	void RunFrame();
	virtual void PowerOff(){}

	//inhareted from ITracked DeviceServerDriver
	//https://github.com/ValveSoftware/openvr/wiki/vr::ITrackedDeviceServerDriver-Overview

	virtual vr::EVRInitError Activate(vr::TrackedDeviceIndex_t unObjectId);
	virtual void Deactivate(){m_unObjectId = vr::k_unTrackedDeviceIndexInvalid;}
	virtual void EnterStandby(){}
	void* GetComponent(const char* pchComponentNameAndVersion);
	virtual void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize);
	virtual vr::DriverPose_t GetPose();


	//inhareted from IVRDisplayComponent
	//can be found in "openvr_driver.h"

	virtual void GetWindowBounds(int32_t* pnX, int32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight);
	virtual bool IsDisplayOnDesktop(){return true;}
	virtual bool IsDisplayRealDisplay(){return false;}
	virtual void GetRecommendedRenderTargetSize(uint32_t* pnWidth, uint32_t* pnHeight);
	virtual void GetEyeOutputViewport(vr::EVREye eEye, uint32_t* pnX, uint32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight);
	virtual void GetProjectionRaw(vr::EVREye eEye, float* pfLeft, float* pfRight, float* pfTop, float* pfBottom);
	virtual vr::DistortionCoordinates_t ComputeDistortion(vr::EVREye eEye, float fU, float fV);

};
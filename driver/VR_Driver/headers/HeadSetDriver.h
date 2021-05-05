#pragma once

#include <string>
#include <openvr_driver.h>
#include <MpuReader.h>

//all the global variables are variables in json file default.vrsettings
//variables with "Section" in the end are names of json object inside the main file object.
static const char* const k_pch_VR_Section = "driver_VR";
static const char* const k_pch_VR_Port_String = "post";
static const char* const k_pch_VR_SerialNumber_String = "SerialNumber";
static const char* const k_pch_VR_ModelNumber_String = "ModelNumber";
static const char* const k_pch_VR_PoseTimeOffset_Float = "PoseTimeOffset";

static const char* const k_pch_Display_Section = "displaySettings";
static const char* const k_pch_Display_WindowX_Int32 = "windowX";
static const char* const k_pch_Display_WindowY_Int32 = "windowY";
static const char* const k_pch_Display_WindowWidth_Int32 = "windowWidth";
static const char* const k_pch_Display_WindowHeight_Int32 = "windowHeight";
static const char* const k_pch_Display_RenderWidth_Int32 = "renderWidth";
static const char* const k_pch_Display_RenderHeight_Int32 = "renderHeight";
static const char* const k_pch_Display_EyeGapOffset_Int = "EyeGapOffsetPx";
static const char* const k_pch_Display_DisplayFrequency_Float = "displayFrequency";
static const char* const k_pch_Display_SecondsFromVsyncToPhotons_Float = "secondsFromVsyncToPhotons";
static const char* const k_pch_Display_IsDisplayReal_Bool = "IsDisplayRealDisplay";
static const char* const k_pch_Display_IsDisplayOnDesktop_bool = "IsDisplayOnDesktop";



class HeadSetDriver : public vr::ITrackedDeviceServerDriver, public vr::IVRDisplayComponent {
	private:
		MpuReader* IMU;


		vr::TrackedDeviceIndex_t m_unObjectId;
		vr::PropertyContainerHandle_t m_ulPropertyContainer;

		std::string m_sSerialNumber;
		std::string m_sModelNumber;

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
		HeadSetDriver(MpuReader mr);

		std::string getSerial() { return this->m_sSerialNumber; }

		//****** tracked device driver function ********
		/**
		* activate will tell openvr to start reading from the device
		*/
		vr::EVRInitError Activate(uint32_t unObjectId);

		/**
		* getPose is the primary method of the driver
		* calling the function is a request to get the device (imu) data
		*/
		vr::DriverPose_t GetPose();

		void runframe();

		/**
		* stop reading from the device
		*/
		void Deactivate();
		void EnterStandby();
		void* GetComponent(const char* pchComponentNameAndVersion);
		void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize);

		//******* display commponent function *******
		void GetWindowBounds(int32_t* pnX, int32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight);
		bool IsDisplayOnDesktop();
		bool IsDisplayRealDisplay();
		void GetRecommendedRenderTargetSize(uint32_t* pnWidth, uint32_t* pnHeight);
		void GetEyeOutputViewport(vr::EVREye eEye, uint32_t* pnX, uint32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight);
		void GetProjectionRaw(vr::EVREye eEye, float* pfLeft, float* pfRight, float* pfTop, float* pfBottom);
		vr::DistortionCoordinates_t ComputeDistortion(vr::EVREye eEye, float fU, float fV);
};

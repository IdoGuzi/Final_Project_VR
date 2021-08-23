#pragma once

#include <Windows.h>
#include <array>
#include <string>


/**
 * @brief mpureader reads from a micro controller connected to an IMU (MPU6050)
 * this class will hold the updated values of the mpu after calling the method "read"
 */
class MpuReader {
private:
	HANDLE discriptor;
	std::array<double, 4> zero;
	std::array<double, 4> quaternion;
	std::array<double, 3> acceleration;

public:
	MpuReader(std::string port);
	~MpuReader() {}

	/**
	 * @returns Get the unit quaternion of the zero point of the mpu
	 */
	std::array<double, 4> getZero() { return zero; }
	/**
	 * reset the mpu to start point
	 */
	void setZero();

	/**
	 * @returns the quaternion the object hold.
	 * to get the updated values use "read" method before.
	 */
	std::array<double, 4> getQuaternion() { return quaternion; }

	/**
	 * TODO: make this method private
	 * used to update the current point of the mpu
	 * @param str the updated value of the mpu
	 */
	void setQuaternion(std::string str);

	/**
	 * @returns the acceleretion of the mpu at a given moment
	 */
	std::array<double, 3> getAcceleration() { return acceleration; }
	/**
	 * TODO: make this method private
	 * used to set the acceleration values of the mpu by the read method
	 * @param str the acceleration value as a string
	 */
	void setAcceleration(std::string str);

	/**
	 * reads the current value of the mpu
	 * this method should be used before getting data from this object
	 */
	void read();

private:
	std::string readline();
};
#pragma once

#include <Windows.h>
#include <array>
#include <string>



class MpuReader {
private:
	HANDLE discriptor;
	std::array<double, 4> zero;
	std::array<double, 4> quaternion;
	std::array<double, 3> acceleration;

public:
	MpuReader(std::string port);
	~MpuReader() {}

	std::array<double, 4> getZero() { return zero; }
	void setZero();
	std::array<double, 4> getQuaternion() { return quaternion; }
	void setQuaternion(std::string str);
	std::array<double, 3> getAcceleration() { return acceleration; }
	void setAcceleration(std::string str);

	void read();

private:
	std::string readline();
};
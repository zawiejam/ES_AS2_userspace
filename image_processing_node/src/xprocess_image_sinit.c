// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xprocess_image.h"

extern XProcess_image_Config XProcess_image_ConfigTable[];

XProcess_image_Config *XProcess_image_LookupConfig(u16 DeviceId) {
	XProcess_image_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XPROCESS_IMAGE_NUM_INSTANCES; Index++) {
		if (XProcess_image_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XProcess_image_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XProcess_image_Initialize(XProcess_image *InstancePtr, u16 DeviceId) {
	XProcess_image_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XProcess_image_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XProcess_image_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif


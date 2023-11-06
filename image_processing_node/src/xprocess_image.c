// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2020.2 (64-bit)
// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xprocess_image.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XProcess_image_CfgInitialize(XProcess_image *InstancePtr, XProcess_image_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Axi_cpu_BaseAddress = ConfigPtr->Axi_cpu_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XProcess_image_Start(XProcess_image *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XProcess_image_ReadReg(InstancePtr->Axi_cpu_BaseAddress, XPROCESS_IMAGE_AXI_CPU_ADDR_AP_CTRL) & 0x80;
    XProcess_image_WriteReg(InstancePtr->Axi_cpu_BaseAddress, XPROCESS_IMAGE_AXI_CPU_ADDR_AP_CTRL, Data | 0x01);
}

u32 XProcess_image_IsDone(XProcess_image *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XProcess_image_ReadReg(InstancePtr->Axi_cpu_BaseAddress, XPROCESS_IMAGE_AXI_CPU_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XProcess_image_IsIdle(XProcess_image *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XProcess_image_ReadReg(InstancePtr->Axi_cpu_BaseAddress, XPROCESS_IMAGE_AXI_CPU_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XProcess_image_IsReady(XProcess_image *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XProcess_image_ReadReg(InstancePtr->Axi_cpu_BaseAddress, XPROCESS_IMAGE_AXI_CPU_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XProcess_image_EnableAutoRestart(XProcess_image *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XProcess_image_WriteReg(InstancePtr->Axi_cpu_BaseAddress, XPROCESS_IMAGE_AXI_CPU_ADDR_AP_CTRL, 0x80);
}

void XProcess_image_DisableAutoRestart(XProcess_image *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XProcess_image_WriteReg(InstancePtr->Axi_cpu_BaseAddress, XPROCESS_IMAGE_AXI_CPU_ADDR_AP_CTRL, 0);
}

u32 XProcess_image_Get_input_image_BaseAddress(XProcess_image *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Axi_cpu_BaseAddress + XPROCESS_IMAGE_AXI_CPU_ADDR_INPUT_IMAGE_BASE);
}

u32 XProcess_image_Get_input_image_HighAddress(XProcess_image *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Axi_cpu_BaseAddress + XPROCESS_IMAGE_AXI_CPU_ADDR_INPUT_IMAGE_HIGH);
}

u32 XProcess_image_Get_input_image_TotalBytes(XProcess_image *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XPROCESS_IMAGE_AXI_CPU_ADDR_INPUT_IMAGE_HIGH - XPROCESS_IMAGE_AXI_CPU_ADDR_INPUT_IMAGE_BASE + 1);
}

u32 XProcess_image_Get_input_image_BitWidth(XProcess_image *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XPROCESS_IMAGE_AXI_CPU_WIDTH_INPUT_IMAGE;
}

u32 XProcess_image_Get_input_image_Depth(XProcess_image *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XPROCESS_IMAGE_AXI_CPU_DEPTH_INPUT_IMAGE;
}

u32 XProcess_image_Write_input_image_Words(XProcess_image *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XPROCESS_IMAGE_AXI_CPU_ADDR_INPUT_IMAGE_HIGH - XPROCESS_IMAGE_AXI_CPU_ADDR_INPUT_IMAGE_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Axi_cpu_BaseAddress + XPROCESS_IMAGE_AXI_CPU_ADDR_INPUT_IMAGE_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XProcess_image_Read_input_image_Words(XProcess_image *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XPROCESS_IMAGE_AXI_CPU_ADDR_INPUT_IMAGE_HIGH - XPROCESS_IMAGE_AXI_CPU_ADDR_INPUT_IMAGE_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Axi_cpu_BaseAddress + XPROCESS_IMAGE_AXI_CPU_ADDR_INPUT_IMAGE_BASE + (offset + i)*4);
    }
    return length;
}

u32 XProcess_image_Write_input_image_Bytes(XProcess_image *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XPROCESS_IMAGE_AXI_CPU_ADDR_INPUT_IMAGE_HIGH - XPROCESS_IMAGE_AXI_CPU_ADDR_INPUT_IMAGE_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Axi_cpu_BaseAddress + XPROCESS_IMAGE_AXI_CPU_ADDR_INPUT_IMAGE_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XProcess_image_Read_input_image_Bytes(XProcess_image *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XPROCESS_IMAGE_AXI_CPU_ADDR_INPUT_IMAGE_HIGH - XPROCESS_IMAGE_AXI_CPU_ADDR_INPUT_IMAGE_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Axi_cpu_BaseAddress + XPROCESS_IMAGE_AXI_CPU_ADDR_INPUT_IMAGE_BASE + offset + i);
    }
    return length;
}

u32 XProcess_image_Get_output_image_BaseAddress(XProcess_image *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Axi_cpu_BaseAddress + XPROCESS_IMAGE_AXI_CPU_ADDR_OUTPUT_IMAGE_BASE);
}

u32 XProcess_image_Get_output_image_HighAddress(XProcess_image *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Axi_cpu_BaseAddress + XPROCESS_IMAGE_AXI_CPU_ADDR_OUTPUT_IMAGE_HIGH);
}

u32 XProcess_image_Get_output_image_TotalBytes(XProcess_image *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XPROCESS_IMAGE_AXI_CPU_ADDR_OUTPUT_IMAGE_HIGH - XPROCESS_IMAGE_AXI_CPU_ADDR_OUTPUT_IMAGE_BASE + 1);
}

u32 XProcess_image_Get_output_image_BitWidth(XProcess_image *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XPROCESS_IMAGE_AXI_CPU_WIDTH_OUTPUT_IMAGE;
}

u32 XProcess_image_Get_output_image_Depth(XProcess_image *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XPROCESS_IMAGE_AXI_CPU_DEPTH_OUTPUT_IMAGE;
}

u32 XProcess_image_Write_output_image_Words(XProcess_image *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XPROCESS_IMAGE_AXI_CPU_ADDR_OUTPUT_IMAGE_HIGH - XPROCESS_IMAGE_AXI_CPU_ADDR_OUTPUT_IMAGE_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Axi_cpu_BaseAddress + XPROCESS_IMAGE_AXI_CPU_ADDR_OUTPUT_IMAGE_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XProcess_image_Read_output_image_Words(XProcess_image *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XPROCESS_IMAGE_AXI_CPU_ADDR_OUTPUT_IMAGE_HIGH - XPROCESS_IMAGE_AXI_CPU_ADDR_OUTPUT_IMAGE_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Axi_cpu_BaseAddress + XPROCESS_IMAGE_AXI_CPU_ADDR_OUTPUT_IMAGE_BASE + (offset + i)*4);
    }
    return length;
}

u32 XProcess_image_Write_output_image_Bytes(XProcess_image *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XPROCESS_IMAGE_AXI_CPU_ADDR_OUTPUT_IMAGE_HIGH - XPROCESS_IMAGE_AXI_CPU_ADDR_OUTPUT_IMAGE_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Axi_cpu_BaseAddress + XPROCESS_IMAGE_AXI_CPU_ADDR_OUTPUT_IMAGE_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XProcess_image_Read_output_image_Bytes(XProcess_image *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XPROCESS_IMAGE_AXI_CPU_ADDR_OUTPUT_IMAGE_HIGH - XPROCESS_IMAGE_AXI_CPU_ADDR_OUTPUT_IMAGE_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Axi_cpu_BaseAddress + XPROCESS_IMAGE_AXI_CPU_ADDR_OUTPUT_IMAGE_BASE + offset + i);
    }
    return length;
}

void XProcess_image_InterruptGlobalEnable(XProcess_image *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XProcess_image_WriteReg(InstancePtr->Axi_cpu_BaseAddress, XPROCESS_IMAGE_AXI_CPU_ADDR_GIE, 1);
}

void XProcess_image_InterruptGlobalDisable(XProcess_image *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XProcess_image_WriteReg(InstancePtr->Axi_cpu_BaseAddress, XPROCESS_IMAGE_AXI_CPU_ADDR_GIE, 0);
}

void XProcess_image_InterruptEnable(XProcess_image *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XProcess_image_ReadReg(InstancePtr->Axi_cpu_BaseAddress, XPROCESS_IMAGE_AXI_CPU_ADDR_IER);
    XProcess_image_WriteReg(InstancePtr->Axi_cpu_BaseAddress, XPROCESS_IMAGE_AXI_CPU_ADDR_IER, Register | Mask);
}

void XProcess_image_InterruptDisable(XProcess_image *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XProcess_image_ReadReg(InstancePtr->Axi_cpu_BaseAddress, XPROCESS_IMAGE_AXI_CPU_ADDR_IER);
    XProcess_image_WriteReg(InstancePtr->Axi_cpu_BaseAddress, XPROCESS_IMAGE_AXI_CPU_ADDR_IER, Register & (~Mask));
}

void XProcess_image_InterruptClear(XProcess_image *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XProcess_image_WriteReg(InstancePtr->Axi_cpu_BaseAddress, XPROCESS_IMAGE_AXI_CPU_ADDR_ISR, Mask);
}

u32 XProcess_image_InterruptGetEnabled(XProcess_image *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XProcess_image_ReadReg(InstancePtr->Axi_cpu_BaseAddress, XPROCESS_IMAGE_AXI_CPU_ADDR_IER);
}

u32 XProcess_image_InterruptGetStatus(XProcess_image *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XProcess_image_ReadReg(InstancePtr->Axi_cpu_BaseAddress, XPROCESS_IMAGE_AXI_CPU_ADDR_ISR);
}


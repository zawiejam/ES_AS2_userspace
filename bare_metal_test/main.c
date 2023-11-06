#include <stdint.h>
#include <stdio.h>
#include "platform.h"
#include "xprocess_image.h"

#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480
#define IMAGE_SIZE IMAGE_WIDTH * IMAGE_HEIGHT

int main()
{
    init_platform();
    print("Starting bare metal test\n\r");

    XProcess_image ip_inst;
    XProcess_image_Config *ip_cfg;

    ip_cfg = XProcess_image_LookupConfig(XPAR_PROCESS_IMAGE_0_DEVICE_ID);
    if (ip_cfg == NULL) {
        print("Error: Could not find the IP core configuration.\r\n");
        return XST_FAILURE;
    }

    int status = XProcess_image_CfgInitialize(&ip_inst, ip_cfg);
    if (status != XST_SUCCESS) {
        print("Error: Could not initialize the IP core.\r\n");
        return XST_FAILURE;
    }

    // Test the IP core functionality
    unsigned char in_image[IMAGE_SIZE];
    unsigned char in_image_cpy[IMAGE_SIZE];
    unsigned char out_image[IMAGE_SIZE];

    // Initialize input arrays
	for (int i = 0; i < IMAGE_SIZE; i++) {
		in_image[i] = i % UINT8_MAX;
		in_image_cpy[i] = i % UINT8_MAX;
	}

    // Call the IP core function
    XProcess_image_Write_input_image_Words(&ip_inst, 0, (unsigned int *)in_image, IMAGE_SIZE / 4);
    XProcess_image_Start(&ip_inst);

    // Wait for the IP core to finish
    while (!XProcess_image_IsDone(&ip_inst));

    XProcess_image_Read_input_image_Words(&ip_inst, 0, (unsigned int *)in_image, IMAGE_SIZE / 4);
    XProcess_image_Read_output_image_Words(&ip_inst, 0, (unsigned int *)out_image, IMAGE_SIZE / 4);

    // Check the results (the input has not changed and the output is the inversion of the input)
    int errors = 0;
	for (int i = 0; i < IMAGE_SIZE; i++) {
		if (in_image[i] != in_image_cpy[i]) {
			errors++;
		}
		unsigned char expected = UINT8_MAX - in_image_cpy[i];
		if (out_image[i] != expected) {
			errors++;
		}
    }

    if (errors == 0) {
        print("Test passed.\r\n");
    } else {
        printf("Test failed with %d errors.\r\n", errors);
    }

    // Cleanup
    XProcess_image_DisableAutoRestart(&ip_inst);
    cleanup_platform();

    return XST_SUCCESS;
}


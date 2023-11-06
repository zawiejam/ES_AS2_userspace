#include <climits>

#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480
#define IMAGE_SIZE IMAGE_WIDTH * IMAGE_HEIGHT

// Image inversion implementation
void process_image(
	volatile unsigned char input_image[IMAGE_SIZE],
	volatile unsigned char output_image[IMAGE_SIZE]
)
{
	#pragma HLS INTERFACE s_axilite port=return bundle=AXI_CPU
	#pragma HLS INTERFACE s_axilite port=input_image bundle=AXI_CPU
    #pragma HLS INTERFACE s_axilite port=output_image bundle=AXI_CPU
    
	for (int i = 0; i < IMAGE_SIZE; i++) {
		#pragma HLS PIPELINE
		output_image[i] = UCHAR_MAX - input_image[i];
	}
}


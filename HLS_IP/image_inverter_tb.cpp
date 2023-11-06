#include <iostream>
#include <ctime>
#include <cstdlib>
#include <climits>

#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480
#define IMAGE_SIZE IMAGE_WIDTH * IMAGE_HEIGHT

void process_image(
	volatile unsigned char input_image[IMAGE_SIZE],
	volatile unsigned char output_image[IMAGE_SIZE]
);

int main() {
	std::cout << "Test bench starts" << std::endl;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Initialize input arrays and expected output array
    volatile unsigned char in_image[IMAGE_SIZE];
    volatile unsigned char out_image[IMAGE_SIZE];
    volatile unsigned char expected_out[IMAGE_SIZE];

    // Populate input arrays with random data
	for (int i = 0; i < IMAGE_SIZE; i++) {
		unsigned char random_pixel_value = (unsigned char)(std::rand() % (UCHAR_MAX + 1));
		in_image[i] = random_pixel_value;
		expected_out[i] = UCHAR_MAX - random_pixel_value;
    }

    // Call the function to test
	process_image(in_image, out_image);

    // Check the results
    bool success = true;
	for (int i = 0; i < IMAGE_SIZE; i++) {
		if (out_image[i] != expected_out[i]) {
			std::cout << "Mismatch at index (" << i <<
				"): Expected " << (unsigned int)expected_out[i] <<
				" but got " << (unsigned int)out_image[i] << std::endl;
			success = false;
		}
    }

    if (success) {
        std::cout << "Test passed!" << std::endl;
    } else {
        std::cout << "Test failed!" << std::endl;
    }

    return 0;
}

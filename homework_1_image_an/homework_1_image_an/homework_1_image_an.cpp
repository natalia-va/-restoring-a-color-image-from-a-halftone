#include "my_image.h"
#include <fstream>
#include <iostream>
#include <Windows.h>


double get_mse(my_image img1, my_image img2) {
	unsigned long long int razn = 0;
	for (int i = 0; i < img1.get_height(); i++) {
		for (int j = 0; j < img1.get_width(); j++) {
			razn += pow((img1.get_ser(i, j) - img2.get_ser(i, j)), 2);
		}
	}
	return razn / (img1.get_width() * img1.get_height());
}


int main()
{
	const char* file_name = "RGB_CFA.bmp";
	const char* file_name_load = "result.bmp";
	const char* file_name_orig = "Original.bmp";
	my_image BPM_IMAGE(file_name);
	BPM_IMAGE.Make_better();
	BPM_IMAGE.Load_to_file(file_name_load);

	my_image BPM_IMAGE_FOR_TEST(file_name_orig);
	my_image BPM_IMAGE_1(file_name_load);

	double result = get_mse(BPM_IMAGE_1, BPM_IMAGE_FOR_TEST);
	std::cout << "PSNR " << 10 * log10((255.0 * 255.0) / result) << "\n";

}


#include "my_image.h"

unsigned char hue_transit(unsigned char L_1, unsigned char L_2, unsigned char L_3,
	unsigned char V_1, unsigned char V_3) {
	if ((L_1 < L_2 && L_2 < L_3) or (L_1 > L_2&& L_2 > L_3)) {
		return V_1 + (V_3 - V_1) * (L_2 - L_1) / (L_3 - L_1);
	}
	else
	{
		return (V_1 + V_3) / 2 + (L_2 - (L_1 + L_3) / 2) / 2;
	}
}
my_image::my_image(const char* name_file) {

	RGBTRIPLE RGB;

	std::ifstream f1(name_file, std::ios::in | std::ios::binary);
	f1.read((char*)&BMP_FILE_HEADER, sizeof(BMP_FILE_HEADER));
	f1.read((char*)&BMP_IMAGE_HEADER, sizeof(BMP_IMAGE_HEADER));

	if (!f1.is_open()) {
		std::cout << "Error";
	}
	BMP_MATRIX_MAP = new unsigned char** [BMP_IMAGE_HEADER.biHeight];
	for (int i = 0;i < BMP_IMAGE_HEADER.biHeight;i++)
	{
		BMP_MATRIX_MAP[i] = new unsigned char* [BMP_IMAGE_HEADER.biWidth];
		for (int j = 0;j < BMP_IMAGE_HEADER.biWidth;j++)
		{
			BMP_MATRIX_MAP[i][j] = new unsigned char[3];
		}
	}

	size_t padding = 0;
	if ((BMP_IMAGE_HEADER.biWidth * 3) % 4) padding = 4 - (BMP_IMAGE_HEADER.biWidth * 3) % 4;

	for (int i = 0;i < BMP_IMAGE_HEADER.biHeight;i++)
	{
		for (int j = 0; j < BMP_IMAGE_HEADER.biWidth; j++)
		{
			f1.read((char*)&RGB, 3);
			BMP_MATRIX_MAP[i][j][0] = RGB.rgbtBlue;
			BMP_MATRIX_MAP[i][j][1] = RGB.rgbtGreen;
			BMP_MATRIX_MAP[i][j][2] = RGB.rgbtRed;
		}
		int j = padding;
		while (j > 0)
		{
			f1.read((char*)&RGB, padding);
			j--;
		}
	}
	f1.close();
}
/*
my_image::~my_image() {

	for (int i = 0; i < BMP_IMAGE_HEADER.biHeight; i++) {
		for (int j = 0; j < BMP_IMAGE_HEADER.biWidth; j++){
			delete[] BMP_MATRIX_MAP[i][j];
		}
		delete[] BMP_MATRIX_MAP[i];
	}
	delete[] BMP_MATRIX_MAP;
}*/

void my_image::Load_to_file(const char* file_name) {

	RGBTRIPLE RGB;

	std::ofstream f2(file_name, std::ios::out | std::ios::binary);

	if (!f2.is_open()) {
		std::cout << "Error";
	}

	f2.write((char*)&BMP_FILE_HEADER, sizeof(BITMAPFILEHEADER));
	f2.write((char*)&BMP_IMAGE_HEADER, sizeof(BITMAPINFOHEADER));

	size_t padding = 0;
	if ((BMP_IMAGE_HEADER.biWidth * 3) % 4) padding = 4 - (BMP_IMAGE_HEADER.biWidth * 3) % 4;

	for (int i = 0;i < BMP_IMAGE_HEADER.biHeight;i++)
	{
		for (int j = 0; j < BMP_IMAGE_HEADER.biWidth; j++)
		{

			RGB.rgbtBlue = BMP_MATRIX_MAP[i][j][0];
			RGB.rgbtGreen = BMP_MATRIX_MAP[i][j][1];
			RGB.rgbtRed = BMP_MATRIX_MAP[i][j][2];

			f2.write((char*)&RGB, 3);
		}
		int j = padding;
		while (j > 0)
		{
			f2.write((char*)&RGB, padding);
			j--;
		}
	}
	f2.close();
}

void my_image::Make_better() {
	g_for_r();
	g_for_b();
	b_and_r_for_g();
	r_for_having_b();
	b_for_having_r();
}

void my_image::r_for_having_b() {

	unsigned char d_ne, d_nw;
	for (int i = 2;i < BMP_IMAGE_HEADER.biHeight - 2; i++)
	{
		for (int j = 2; j < BMP_IMAGE_HEADER.biWidth - 2; j++)
		{
			if (BMP_MATRIX_MAP[i][j][2] == 0) {
				d_ne = abs(BMP_MATRIX_MAP[i - 1][j + 1][2] - BMP_MATRIX_MAP[i + 1][j - 1][2]) +
					abs(BMP_MATRIX_MAP[i - 2][j + 2][0] - BMP_MATRIX_MAP[i][j][0]) +
					abs(BMP_MATRIX_MAP[i][j][0] - BMP_MATRIX_MAP[i + 2][j - 2][0]) +
					abs(BMP_MATRIX_MAP[i - 1][j + 1][1] - BMP_MATRIX_MAP[i][j][1]) +
					abs(BMP_MATRIX_MAP[i][j][1] - BMP_MATRIX_MAP[i + 1][j - 1][1]);

				d_nw = abs(BMP_MATRIX_MAP[i - 1][j - 1][2] - BMP_MATRIX_MAP[i + 1][j + 1][2]) +
					abs(BMP_MATRIX_MAP[i - 2][j - 2][0] - BMP_MATRIX_MAP[i][j][0]) +
					abs(BMP_MATRIX_MAP[i][j][0] - BMP_MATRIX_MAP[i - 2][j - 2][0]) +
					abs(BMP_MATRIX_MAP[i - 1][j - 1][1] - BMP_MATRIX_MAP[i][j][1]) +
					abs(BMP_MATRIX_MAP[i][j][1] - BMP_MATRIX_MAP[i - 1][j - 1][1]);


				if (d_ne = min(d_ne, d_nw)) {
					BMP_MATRIX_MAP[i][j][2] = hue_transit(BMP_MATRIX_MAP[i - 1][j + 1][1], BMP_MATRIX_MAP[i][j][1],
						BMP_MATRIX_MAP[i + 1][j - 1][1], BMP_MATRIX_MAP[i - 1][j + 1][2],
						BMP_MATRIX_MAP[i + 1][j - 1][2]);
				}
				else
				{
					BMP_MATRIX_MAP[i][j][2] = hue_transit(BMP_MATRIX_MAP[i - 1][j - 1][1], BMP_MATRIX_MAP[i][j][1],
						BMP_MATRIX_MAP[i + 1][j + 1][1], BMP_MATRIX_MAP[i - 1][j - 1][2],
						BMP_MATRIX_MAP[i + 1][j + 1][2]);

				}
			}
		}
	}
}
void my_image::b_for_having_r() {
	unsigned char d_ne, d_nw;
	for (int i = 2;i < BMP_IMAGE_HEADER.biHeight - 2; i++)
	{
		for (int j = 2; j < BMP_IMAGE_HEADER.biWidth - 2; j++)
		{
			if (BMP_MATRIX_MAP[i][j][0] == 0) {
				d_ne = abs(BMP_MATRIX_MAP[i - 1][j + 1][0] - BMP_MATRIX_MAP[i + 1][j - 1][0]) +
					abs(BMP_MATRIX_MAP[i - 2][j + 2][2] - BMP_MATRIX_MAP[i][j][2]) +
					abs(BMP_MATRIX_MAP[i][j][2] - BMP_MATRIX_MAP[i + 2][j - 2][2]) +
					abs(BMP_MATRIX_MAP[i - 1][j + 1][1] - BMP_MATRIX_MAP[i][j][1]) +
					abs(BMP_MATRIX_MAP[i][j][1] - BMP_MATRIX_MAP[i + 1][j - 1][1]);

				d_nw = abs(BMP_MATRIX_MAP[i - 1][j - 1][0] - BMP_MATRIX_MAP[i + 1][j + 1][0]) +
					abs(BMP_MATRIX_MAP[i - 2][j - 2][2] - BMP_MATRIX_MAP[i][j][2]) +
					abs(BMP_MATRIX_MAP[i][j][2] - BMP_MATRIX_MAP[i - 2][j - 2][2]) +
					abs(BMP_MATRIX_MAP[i - 1][j - 1][1] - BMP_MATRIX_MAP[i][j][1]) +
					abs(BMP_MATRIX_MAP[i][j][1] - BMP_MATRIX_MAP[i - 1][j - 1][1]);


				if (d_ne = min(d_ne, d_nw)) {
					BMP_MATRIX_MAP[i][j][0] = hue_transit(BMP_MATRIX_MAP[i - 1][j + 1][1], BMP_MATRIX_MAP[i][j][1],
						BMP_MATRIX_MAP[i + 1][j - 1][1], BMP_MATRIX_MAP[i - 1][j + 1][0],
						BMP_MATRIX_MAP[i + 1][j - 1][0]);
				}
				else
				{
					BMP_MATRIX_MAP[i][j][0] = hue_transit(BMP_MATRIX_MAP[i - 1][j - 1][1], BMP_MATRIX_MAP[i][j][1],
						BMP_MATRIX_MAP[i + 1][j + 1][1], BMP_MATRIX_MAP[i - 1][j - 1][0],
						BMP_MATRIX_MAP[i + 1][j + 1][0]);

				}
			}
		}
	}
}
void my_image::b_and_r_for_g() {
	for (int i = 1;i < BMP_IMAGE_HEADER.biHeight - 1; i++)
	{
		for (int j = 1; j < BMP_IMAGE_HEADER.biWidth - 1; j++)
		{
			if (BMP_MATRIX_MAP[i][j][0] == 0 && BMP_MATRIX_MAP[i][j][2] == 0) {

				BMP_MATRIX_MAP[i][j][0] = hue_transit(BMP_MATRIX_MAP[i][j - 1][1], BMP_MATRIX_MAP[i][j][1],
					BMP_MATRIX_MAP[i][j + 1][1], BMP_MATRIX_MAP[i][j - 1][0], BMP_MATRIX_MAP[i][j + 1][0]);

				BMP_MATRIX_MAP[i][j][2] = hue_transit(BMP_MATRIX_MAP[i - 1][j][1], BMP_MATRIX_MAP[i][j][1],
					BMP_MATRIX_MAP[i + 1][j][1], BMP_MATRIX_MAP[i - 1][j][2], BMP_MATRIX_MAP[i + 1][j][2]);

			}
		}
	}


}
void my_image::g_for_r() {

	int flag = 0;
	for (int i = 2;i < BMP_IMAGE_HEADER.biHeight - 2; i += 2)
	{
		for (int j = 2; j < BMP_IMAGE_HEADER.biWidth - 2; j += 2)
		{


			unsigned char	d_n = abs(BMP_MATRIX_MAP[i][j][2] - BMP_MATRIX_MAP[i - 2][j][2]) * 2 +
				abs(BMP_MATRIX_MAP[i - 1][j][1] - BMP_MATRIX_MAP[i + 1][j][1]);


			unsigned char d_s = abs(BMP_MATRIX_MAP[i][j][2] - BMP_MATRIX_MAP[i + 2][j][2]) * 2 +
				abs(BMP_MATRIX_MAP[i - 1][j][1] - BMP_MATRIX_MAP[i + 1][j][1]);



			unsigned char d_e = abs(BMP_MATRIX_MAP[i][j][2] - BMP_MATRIX_MAP[i][j + 2][2]) * 2 +
				abs(BMP_MATRIX_MAP[i][j + 1][1] - BMP_MATRIX_MAP[i][j - 1][1]);


			unsigned char d_w = abs(BMP_MATRIX_MAP[i][j][2] - BMP_MATRIX_MAP[i][j - 2][2]) * 2 +
				abs(BMP_MATRIX_MAP[i][j + 1][1] - BMP_MATRIX_MAP[i][j - 1][1]);


			if (d_n == min(min(d_n, d_s), min(d_e, d_w))) {
				BMP_MATRIX_MAP[i][j][1] = (BMP_MATRIX_MAP[i - 1][j][1] * 3 +
					BMP_MATRIX_MAP[i + 1][j][1] + BMP_MATRIX_MAP[i][j][2] - BMP_MATRIX_MAP[i - 2][j][2]) / 4;

			}

			if (d_s == min(min(d_n, d_s), min(d_e, d_w))) {
				BMP_MATRIX_MAP[i][j][1] = (BMP_MATRIX_MAP[i + 1][j][1] * 3 +
					BMP_MATRIX_MAP[i - 1][j][1] + BMP_MATRIX_MAP[i][j][2] - BMP_MATRIX_MAP[i + 2][j][2]) / 4;

			}

			if (d_e == min(min(d_n, d_s), min(d_e, d_w))) {
				BMP_MATRIX_MAP[i][j][1] = (BMP_MATRIX_MAP[i][j + 1][1] * 3 +
					BMP_MATRIX_MAP[i][j - 1][1] + BMP_MATRIX_MAP[i][j][2] - BMP_MATRIX_MAP[i][j + 2][2]) / 4;

			}

			if (d_w == min(min(d_n, d_s), min(d_e, d_w))) {
				BMP_MATRIX_MAP[i][j][1] = (BMP_MATRIX_MAP[i][j - 1][1] * 3 +
					BMP_MATRIX_MAP[i][j + 1][1] + BMP_MATRIX_MAP[i][j][2] - BMP_MATRIX_MAP[i][j - 2][2]) / 4;

			}


		}
	}
}
void my_image::g_for_b() {
	int flag = 0;
	for (int i = 3;i < BMP_IMAGE_HEADER.biHeight - 2;i += 2)
	{
		for (int j = 3; j < BMP_IMAGE_HEADER.biWidth - 2; j += 2)
		{

			unsigned char d_n = abs(BMP_MATRIX_MAP[i][j][0] - BMP_MATRIX_MAP[i - 2][j][0]) * 2 +
				abs(BMP_MATRIX_MAP[i - 1][j][1] - BMP_MATRIX_MAP[i + 1][j][1]);


			unsigned char d_s = abs(BMP_MATRIX_MAP[i][j][0] - BMP_MATRIX_MAP[i + 2][j][0]) * 2 +
				abs(BMP_MATRIX_MAP[i - 1][j][1] - BMP_MATRIX_MAP[i + 1][j][1]);


			unsigned char d_e = abs(BMP_MATRIX_MAP[i][j][0] - BMP_MATRIX_MAP[i][j + 2][0]) * 2 +
				abs(BMP_MATRIX_MAP[i][j + 1][1] - BMP_MATRIX_MAP[i][j - 1][1]);


			unsigned char d_w = abs(BMP_MATRIX_MAP[i][j][0] - BMP_MATRIX_MAP[i][j - 2][0]) * 2 +
				abs(BMP_MATRIX_MAP[i][j + 1][1] - BMP_MATRIX_MAP[i][j - 1][1]);

			if (d_n == min(min(d_n, d_s), min(d_e, d_w))) {
				BMP_MATRIX_MAP[i][j][1] = (BMP_MATRIX_MAP[i - 1][j][1] * 3 +
					BMP_MATRIX_MAP[i + 1][j][1] + BMP_MATRIX_MAP[i][j][0] - BMP_MATRIX_MAP[i - 2][j][0]) / 4;

			}

			if (d_s == min(min(d_n, d_s), min(d_e, d_w))) {
				BMP_MATRIX_MAP[i][j][1] = (BMP_MATRIX_MAP[i + 1][j][1] * 3 +
					BMP_MATRIX_MAP[i - 1][j][1] + BMP_MATRIX_MAP[i][j][0] - BMP_MATRIX_MAP[i + 2][j][0]) / 4;

			}

			if (d_e == min(min(d_n, d_s), min(d_e, d_w))) {
				BMP_MATRIX_MAP[i][j][1] = (BMP_MATRIX_MAP[i][j + 1][1] * 3 +
					BMP_MATRIX_MAP[i][j - 1][1] + BMP_MATRIX_MAP[i][j][0] - BMP_MATRIX_MAP[i][j + 2][0]) / 4;

			}

			if (d_w == min(min(d_n, d_s), min(d_e, d_w))) {
				BMP_MATRIX_MAP[i][j][1] = (BMP_MATRIX_MAP[i][j - 1][1] * 3 +
					BMP_MATRIX_MAP[i][j + 1][1] + BMP_MATRIX_MAP[i][j][0] - BMP_MATRIX_MAP[i][j - 2][0]) / 4;

			}

		}
	}

}
int my_image::get_ser(int i, int j) {
	return 0.144 * BMP_MATRIX_MAP[i][j][0] + 0.587 * BMP_MATRIX_MAP[i][j][1] + 0.299 * BMP_MATRIX_MAP[i][j][2];
}
int my_image::get_height(){
	return BMP_IMAGE_HEADER.biHeight;
}
int my_image::get_width(){
	return BMP_IMAGE_HEADER.biWidth;
}
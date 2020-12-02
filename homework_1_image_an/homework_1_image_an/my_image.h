#pragma once
#include <Windows.h>
#include <fstream>
#include <iostream>

class my_image
{

	unsigned char*** BMP_MATRIX_MAP;
	BITMAPFILEHEADER BMP_FILE_HEADER;
	BITMAPINFOHEADER BMP_IMAGE_HEADER;
public:
	my_image(const char * name_file);
	//~my_image();
	void Load_to_file(const char* name_file);
	void Make_better();
	void b_and_r_for_g();
	void b_for_having_r();
	void r_for_having_b();
	void g_for_r();
	void g_for_b();
	int get_ser(int i, int j);
	int get_height();
	int get_width();
};


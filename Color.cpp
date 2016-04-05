#include "Color.h"
#include <cstring>

const int Color::color_strlen = 10;

Color::Color(int r_, int g_, int b_)
{
	set(r_, g_, b_);
}

Color::Color(const char* init_cstr)
{
	char color_buf[4];
	color_buf[3] = 0;

	//set r
	memcpy(color_buf, init_cstr, sizeof(char)*3);
	r = atoi(color_buf);
	//set g
	memcpy(color_buf, init_cstr + 3, sizeof(char)*3);
	g = atoi(color_buf);
	//set b
	memcpy(color_buf, init_cstr + 6, sizeof(char)*3);
	b = atoi(color_buf);

	memcpy(cstr_buf, init_cstr, color_strlen);
}

Color& Color::operator=(const Color& other)
{
	r = other.r;
	g = other.g;
	b = other.b;
	memcpy(cstr_buf, other.cstr_buf, color_strlen);

	return *this;
}

void Color::set(int r_, int g_, int b_)
{
	r = r_;
	g = g_;
	b = b_;

	cstr_buf[0] = get_hundreds_place(r);
	cstr_buf[1] = get_tens_place(r);
	cstr_buf[2] = get_ones_place(r);

	cstr_buf[3] = get_hundreds_place(g);
	cstr_buf[4] = get_tens_place(g);
	cstr_buf[5] = get_ones_place(g);

	cstr_buf[6] = get_hundreds_place(b);
	cstr_buf[7] = get_tens_place(b);
	cstr_buf[8] = get_ones_place(b);

	cstr_buf[9] = 0;
}

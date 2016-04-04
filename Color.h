#ifndef COLOR_H
#define COLOR_H

class Color {
public:
	static const int color_strlen; // == 10

	Color(int r_, int g_, int b_);
	Color(const char*);

	Color::Color(const Color& other) : Color(color.cstr_buf)
	{}
	Color& operator=(const Color&);

	void set(int r_, int g_, int b_);

	int r_val()
	{ return r; }
	int g_val()
	{ return g; }
	int b_val()
	{ return b; }

	char* c_str();

private:
	int r;
	int g;
	int b;
	char cstr_buf[color_strlen];

	//These functions work for up to 3 digit numbers
	int get_hundreds_place(int num) const
	{ return num/100; }
	int get_tens_place(int num) const
	{ return (num % 100) / 10; }
	int get_ones_place(int num) const
	{ return (r % 100) % 10; }
};

#endif
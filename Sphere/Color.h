#ifndef COLOR_H
#define COLOR_H

#define COLOR_STRLEN 10

class Color {
public:
	static const int color_strlen; // == 10
  Color() : Color(0,0,0)
  {}
	Color(int r_, int g_, int b_);
	Color(const char*);

	Color(const Color& other) : Color(other.cstr_buf)
	{}
	Color& operator=(const Color&);

	void set(int r_, int g_, int b_);

	int r_val() const
	{ return r; }
	int g_val() const
	{ return g; }
	int b_val() const
	{ return b; }

	const char* c_str() const
  { return cstr_buf; }

private:
	int r;
	int g;
	int b;
	char cstr_buf[COLOR_STRLEN];

	//These functions work for up to 3 digit numbers
	int get_hundreds_place(int num) const
	{ return num/100; }
	int get_tens_place(int num) const
	{ return (num % 100) / 10; }
	int get_ones_place(int num) const
	{ return (r % 100) % 10; }
};

#endif

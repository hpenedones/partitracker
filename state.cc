#include <state.h>

#include <iostream>

using namespace std;

State::State(double x, double y, double width, double height, double speed_x, double speed_y)
	: x_(x), y_(y), width_(width), height_(height), speed_x_(speed_x), speed_y_(speed_y) {}

State::State(const CvRect& rect, double speed_x, double speed_y)
	: x_(rect.x), y_(rect.y), width_(rect.width), height_(rect.height), speed_x_(speed_x), speed_y_(speed_y) {}

State::~State() {}

CvRect State::GetRectangle() const { return cvRect(x_, y_, width_, height_); }

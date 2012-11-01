#include <cv.h>

#ifndef STATE_H_KNY243K
#define STATE_H_KNY243K


class State
{
	friend class Particle;
	friend class DynamicsModel;
	
public:
	
	State(double x, double y, double width, double height, double speed_x, double speed_y );
	State(const CvRect & rect, double speed_x=0.0, double speed_y=0.0);
	
	CvRect GetRectangle() const;
	
	~State ();

private:
	
	double x_, y_;
	double width_, height_;
	double speed_x_, speed_y_;
	
};




#endif 

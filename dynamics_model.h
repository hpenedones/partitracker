#include <particle.h>
#include <cv.h>

#ifndef DYNAMICS_MODEL_H_XS4UX957
#define DYNAMICS_MODEL_H_XS4UX957


class DynamicsModel
{
public:
	DynamicsModel (int max_x, int max_y);
	~DynamicsModel ();

	void move(Particle & particle);
	
	
private:

	int max_x_, max_y_;	
    CvRNG rng_state; // opencv structure for random number generation
	static const int bufferSize = 4;
	float normalValueBuffer[bufferSize];
	CvMat normalValueMat;
};

#endif 

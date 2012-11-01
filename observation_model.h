#include <color_model.h>
#include <state.h>

#include <cv.h>

#ifndef OBSERVATION_MODEL_H_K0LRQQJR
#define OBSERVATION_MODEL_H_K0LRQQJR


class ObservationModel
{
	
public:
	
	ObservationModel(const IplImage * base_image, const State & true_initial_state, double lambda, int nhistbins);
	~ObservationModel();


	double Likelihood(const IplImage * image, const State & state) const;

private:
	
		
	double lambda_;	
		
	ColorModel object_color_model_;
	mutable ColorModel state_color_model_;
	
};


#endif 

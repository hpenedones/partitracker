#include <math.h>
#include <observation_model.h>


ObservationModel::ObservationModel(const IplImage * base_image, const State & true_initial_state, double lambda, int nhistbins):
lambda_(lambda), object_color_model_(nhistbins), state_color_model_(nhistbins)
{
	object_color_model_.AddPixelsColors(base_image, true_initial_state.GetRectangle());
}


ObservationModel::~ObservationModel()
{
	
}


double ObservationModel::Likelihood(const IplImage * image, const State & state ) const
{	
	state_color_model_.Reset();
	
	state_color_model_.AddPixelsColors(image, state.GetRectangle()); 
	
	double divergence = object_color_model_.DistanceTo(state_color_model_);
	double state_likelihood = exp(-lambda_ * divergence);
	
//	cout  << " divergence = " << divergence << " state_likelihood = " << state_likelihood << endl;
	
	return state_likelihood;
}

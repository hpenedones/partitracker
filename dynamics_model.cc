#include <dynamics_model.h>
#include <algorithm>

DynamicsModel::DynamicsModel(int max_x, int max_y):
max_x_(max_x), max_y_(max_y) 
{
	
	rng_state = cvRNG(0xffffffff);
	normalValueMat = cvMat( bufferSize, 1, CV_32F, normalValueBuffer );
	
	//rng_state.disttype = CV_RAND_NORMAL;
	// cvRandInit( &rng_state,	0, 1, 0xffffffff, CV_RAND_NORMAL);
	
	
}

DynamicsModel::~DynamicsModel()
{
}

void DynamicsModel::move(Particle & particle)
{
	cvRandArr( &rng_state, &normalValueMat, CV_RAND_NORMAL, cvRealScalar(0), cvRealScalar(1) );
	
	double new_width = particle.state_.width_ + cvGetReal1D(&normalValueMat, 0) * max_x_ / 40;
	new_width = std::min(new_width, (double) max_x_);
	new_width = std::max(new_width, 15.0);
	
	double new_height = particle.state_.height_ + cvGetReal1D(&normalValueMat, 1) * max_y_ /40;
	new_height = std::min(new_height, (double) max_y_);
	new_height = std::max(new_height, 15.0);
	
	particle.state_.height_ = new_height;
	particle.state_.width_  = new_width;
	
	// Linear dynamical model does not help, because the assumption does not hold in any of the videos
	
	int shift_x =  /* particle.state_.speed_x_ */ + cvGetReal1D(&normalValueMat, 2) * max_x_ / 20;
	int shift_y =  /* particle.state_.speed_y_ */ + cvGetReal1D(&normalValueMat, 3) * max_y_ / 20;
	
	double new_x = std::min(max_x_ - particle.state_.width_ - 1.0, particle.state_.x_ + shift_x);
	new_x = std::max(new_x,0.0);
	double new_y = std::min(max_y_ - particle.state_.height_ - 1.0, particle.state_.y_ + shift_y);
	new_y = std::max(new_y,0.0);
	
	particle.state_.speed_x_ = new_x - particle.state_.x_;
	particle.state_.speed_y_ = new_y - particle.state_.y_;
	
	particle.state_.x_ = new_x;
	particle.state_.y_ = new_y;	

	
} 
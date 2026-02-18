#include <cv.h>
#include <dynamics_model.h>
#include <observation_model.h>
#include <particle.h>
#include <weighted_sampler.h>

#include <vector>

#ifndef PARTICLE_FILTER_H_LRF0NKXQ
#define PARTICLE_FILTER_H_LRF0NKXQ

class ParticleFilter {
   public:
	ParticleFilter(size_t nparticles, const DynamicsModel& dynamics_model, const ObservationModel& observation_model,
				   const State& initial_state);
	~ParticleFilter();

	size_t NumberOfParticles() const;

	void ProcessObservation(const IplImage* observation);

	std::vector<CvRect> GetParticleLocations() const;

	CvRect GetPrediction() const;

   private:
	void SetTrueState(const State& true_state);
	void SampleParticles();
	void MoveParticles();
	void UpdateParticleWeights(const IplImage* observation);

	DynamicsModel dynamics_model_;
	ObservationModel observation_model_;
	std::vector<Particle> particle_set_;

	size_t nparticles_;
};

#endif

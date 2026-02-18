#include <particle_filter.h>

using namespace std;

ParticleFilter::ParticleFilter(size_t nparticles, const DynamicsModel& dynamics_model,
							   const ObservationModel& observation_model, const State& initial_state)
	: dynamics_model_(dynamics_model), observation_model_(observation_model), particle_set_(), nparticles_(nparticles) {
	SetTrueState(initial_state);
}

ParticleFilter::~ParticleFilter() {}

void ParticleFilter::SetTrueState(const State& true_state) {
	for (size_t i = 0; i < nparticles_; i++) {
		particle_set_.push_back(Particle(true_state, 1.0));
	}
}

inline size_t ParticleFilter::NumberOfParticles() const {
	assert(nparticles_ == particle_set_.size());
	return nparticles_;
}

vector<CvRect> ParticleFilter::GetParticleLocations() const {
	vector<CvRect> particle_locations(NumberOfParticles());

	for (size_t i = 0; i < NumberOfParticles(); i++) {
		particle_locations[i] = particle_set_[i].state().GetRectangle();
	}

	return particle_locations;
}

CvRect ParticleFilter::GetPrediction() const {
	double x = 0, y = 0, width = 0, height = 0;

	double total_mass = 0.0;

	for (size_t i = 0; i < nparticles_; i++) {
		double particle_weight = particle_set_[i].weight();
		CvRect rect = particle_set_[i].state().GetRectangle();

		total_mass += particle_set_[i].weight();

		x += particle_weight * rect.x;
		y += particle_weight * rect.y;
		width += particle_weight * rect.width;
		height += particle_weight * rect.height;
	}

	return cvRect(x / total_mass, y / total_mass, width / total_mass, height / total_mass);
}

void ParticleFilter::SampleParticles() {
	vector<double> weights(nparticles_);

	for (size_t i = 0; i < nparticles_; i++) weights[i] = particle_set_[i].weight();

	vector<int> selected_particles(nparticles_);

	SampleAccordingToWeights(weights, nparticles_, selected_particles);

	// add new particles to the end of particle set
	for (size_t i = 0; i < nparticles_; i++) particle_set_.push_back(particle_set_[selected_particles[i]]);

	// remove the old particles
	assert(particle_set_.size() == 2 * nparticles_);
	particle_set_.erase(particle_set_.begin(), particle_set_.begin() + nparticles_);
	assert(particle_set_.size() == nparticles_);
}

void ParticleFilter::MoveParticles() {
	for (size_t i = 0; i < particle_set_.size(); i++) {
		dynamics_model_.move(particle_set_[i]);
	}
}

void ParticleFilter::UpdateParticleWeights(const IplImage* observation) {
	for (size_t i = 0; i < particle_set_.size(); i++) {
		double likelihood = observation_model_.Likelihood(observation, particle_set_[i].state());
		particle_set_[i].set_weight(likelihood);
	}
}

void ParticleFilter::ProcessObservation(const IplImage* observation) {
	SampleParticles();
	MoveParticles();
	UpdateParticleWeights(observation);
}
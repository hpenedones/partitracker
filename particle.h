#include <state.h>

#ifndef PARTICLE_H_VW4HQRSC
#define PARTICLE_H_VW4HQRSC

class Particle {
	friend class DynamicsModel;

   public:
	Particle(State state, double weight);
	~Particle();

	State state() const;
	double weight() const;
	void set_weight(double weight);

   private:
	State state_;
	double weight_;
};

#endif

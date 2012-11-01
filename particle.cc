#include <particle.h>


Particle::Particle(State state, double weight):
state_(state), weight_(weight)
{

}

Particle::~Particle()
{
	
}

void Particle::set_weight(double weight) 
{
	weight_ = weight;
}

double Particle::weight() const
{
	return weight_;
}

State Particle::state() const
{
	return state_;
}


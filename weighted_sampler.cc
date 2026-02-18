#include <weighted_sampler.h>

// function that samples <nb_to_sample> examples according to the distribution given by <weights>
// Note that the return value = sum(weights)
// adapted from F.Fleuret's code from http://www.idiap.ch/folded-ctf/
double SampleAccordingToWeights(const std::vector<double> &weights, int nb_to_sample, std::vector<int> &sampled) {
	int nb = weights.size();

	assert(nb > 0);

	if (nb == 1) {
		for (int k = 0; k < nb_to_sample; k++) sampled[k] = 0;
		return weights[0];
	} else {
		std::vector<double> pair_weights((nb + 1) / 2);

		for (int k = 0; k < nb / 2; k++) pair_weights[k] = weights[2 * k] + weights[2 * k + 1];

		if (nb % 2) pair_weights[(nb + 1) / 2 - 1] = weights[nb - 1];

		double result = SampleAccordingToWeights(pair_weights, nb_to_sample, sampled);

		for (int k = 0; k < nb_to_sample; k++) {
			int s = sampled[k];
			if (drand48() * pair_weights[s] <= weights[2 * s])
				sampled[k] = 2 * s;
			else
				sampled[k] = 2 * s + 1;
		}
		return result;
	}
}

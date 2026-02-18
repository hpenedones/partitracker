#include <assert.h>
#include <stdlib.h>

#include <vector>

#ifndef WSAMPLER
#define WSAMPLER

double SampleAccordingToWeights(const std::vector<double> &weights, int nb_to_sample, std::vector<int> &sampled);

#endif

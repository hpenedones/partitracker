#include <assert.h>
#include <histogram_3d.h>
#include <math.h>

Histogram3D::Histogram3D(int _nbins, int _upper_limit)
	: upper_limit(_upper_limit),
	  nbins(_nbins),
	  hist(nbins * nbins * nbins),
	  total_mass(0.0),
	  bin_size((_upper_limit + 1.0) / _nbins) {
	Init();
}

Histogram3D::~Histogram3D() {}

void Histogram3D::Init() {
	total_mass = 0.0;

	for (size_t j = 0; j < hist.size(); j++) {
		hist[j] = residual_mass;  // initalize with a uniform distribution (prior)
		total_mass += hist[j];	  // given its small mass, it will fast vanish after the first data arrives
	}
}

// kernel density estimation (using a cube of the same dimension as the histogram bins)
void Histogram3D::AddPoint(int x, int y, int z, double weight) {
	assert(x >= 0 && x <= upper_limit);
	assert(y >= 0 && y <= upper_limit);
	assert(z >= 0 && z <= upper_limit);

	int indX[2], indY[2], indZ[2];
	float fracX[2], fracY[2], fracZ[2];

	// find histogram bins colliding with data point
	indX[0] = floor(x / bin_size);
	indX[1] = (x > (indX[0] + 0.5) * bin_size) ? indX[0] + 1 : indX[0] - 1;

	indY[0] = floor(y / bin_size);
	indY[1] = (y > (indY[0] + 0.5) * bin_size) ? indY[0] + 1 : indY[0] - 1;

	indZ[0] = floor(z / bin_size);
	indZ[1] = (z > (indZ[0] + 0.5) * bin_size) ? indZ[0] + 1 : indZ[0] - 1;

	fracX[0] = x / bin_size - (std::min(indX[0], indX[1]) + 0.5);
	fracX[1] = 1 - fracX[0];

	fracY[0] = y / bin_size - (std::min(indY[0], indY[1]) + 0.5);
	fracY[1] = 1 - fracY[0];

	fracZ[0] = z / bin_size - (std::min(indZ[0], indZ[1]) + 0.5);
	fracZ[1] = 1 - fracZ[0];

	// collect valid bins with their unnormalized mass contributions
	int valid_ix[8], valid_iy[8], valid_iz[8];
	double valid_mass[8];
	int num_valid = 0;
	double mass_inside_hist = 0.0;

	for (int ix = 0; ix <= 1; ix++) {
		if (indX[ix] < 0 || indX[ix] >= nbins) continue;
		for (int iy = 0; iy <= 1; iy++) {
			if (indY[iy] < 0 || indY[iy] >= nbins) continue;
			for (int iz = 0; iz <= 1; iz++) {
				if (indZ[iz] < 0 || indZ[iz] >= nbins) continue;
				assert(num_valid < 8);
				double m = weight * fracX[ix] * fracY[iy] * fracZ[iz];
				valid_ix[num_valid] = indX[ix];
				valid_iy[num_valid] = indY[iy];
				valid_iz[num_valid] = indZ[iz];
				valid_mass[num_valid] = m;
				num_valid++;
				mass_inside_hist += m;
			}
		}
	}

	// add mass to histogram bins according to intersection volumes, normalized to unit weight
	for (int k = 0; k < num_valid; k++) {
		double normalized = valid_mass[k] / mass_inside_hist;
		hist[valid_ix[k] * nbins * nbins + valid_iy[k] * nbins + valid_iz[k]] += normalized;
		total_mass += normalized;
	}
}

double Histogram3D::GetBinProbability(int bin_index) const {
	assert(bin_index >= 0 && bin_index < nbins * nbins * nbins);

	double count = hist[bin_index];

	double point_probability = count / total_mass;

	return point_probability;
}

double Histogram3D::GetPointProbability(int x, int y, int z) const {
	assert(x >= 0 && x <= upper_limit);
	assert(y >= 0 && y <= upper_limit);
	assert(z >= 0 && z <= upper_limit);

	int ind_x = floor(x / (upper_limit + numerical_delta) * nbins);
	int ind_y = floor(y / (upper_limit + numerical_delta) * nbins);
	int ind_z = floor(z / (upper_limit + numerical_delta) * nbins);

	return GetBinProbability(ind_x * nbins * nbins + ind_y * nbins + ind_z);
}

double Histogram3D::KullbackLeiblerDivergence(const Histogram3D& other_hist) const {
	int total_bins = nbins * nbins * nbins;

	double kl_div = 0.0;

	for (int i = 0; i < total_bins; i++) {
		assert(other_hist.GetBinProbability(i) > 0.0);

		double increment = GetBinProbability(i) * (log(GetBinProbability(i)) - log(other_hist.GetBinProbability(i)));

		kl_div += increment;
	}

	return kl_div;
}

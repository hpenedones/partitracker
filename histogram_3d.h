#include <algorithm>
#include <vector>

class Histogram3D {
   public:
	Histogram3D(int nbins, int upper_limit);

	~Histogram3D();

	void Init();

	// kernel density estimation (using a cube of the same dimension as the histogram bins)
	void AddPoint(int x, int y, int z, double weight);

	double GetPointProbability(int x, int y, int z) const;

	double KullbackLeiblerDivergence(const Histogram3D& other_hist) const;

   private:
	double GetBinProbability(int bin_index) const;

	const static double numerical_delta = 0.000001;
	const static double residual_mass = 0.1;

	int upper_limit;
	int nbins;
	std::vector<double> hist;
	double total_mass, bin_size;
};

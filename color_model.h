#include <histogram_3d.h>
#include <cv.h>

class ColorModel
{
public:
	ColorModel (int nbins);
	~ColorModel ();

	void AddPixelsColors(const IplImage * image, const CvRect & roi);

	double DistanceTo(const ColorModel & other_color_model) const;
	
	void Reset();
	
private:

	void Decompose(CvScalar & pixel, int * x, int * y, int *z) const;

	Histogram3D color_histogram;
};

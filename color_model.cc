#include <color_model.h>

ColorModel::ColorModel(int nbins) : color_histogram(nbins, 255) {}

ColorModel::~ColorModel() {}

void ColorModel::Reset() { color_histogram.Init(); }

void ColorModel::Decompose(CvScalar &pixel, int *x, int *y, int *z) const {
	*x = 0;	 // L channel (luminance) intentionally ignored for lighting robustness
	*y = pixel.val[1];
	*z = pixel.val[2];
}

void ColorModel::AddPixelsColors(const IplImage *image, const CvRect &roi) {
	for (int row = 0; row < roi.height; row++) {
		for (int col = 0; col < roi.width; col++) {
			CvScalar pixel_color = cvGet2D(image, row + roi.y, col + roi.x);
			int x, y, z;

			Decompose(pixel_color, &x, &y, &z);

			color_histogram.AddPoint(x, y, z, 1.0);
		}
	}
}

double ColorModel::DistanceTo(const ColorModel &other_color_model) const {
	return color_histogram.KullbackLeiblerDivergence(other_color_model.color_histogram);
}

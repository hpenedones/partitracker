#include <configs.h>
#include <cv.h>
#include <highgui.h>
#include <particle.h>
#include <particle_filter.h>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
	if (argc != 2) {
		cerr << "Usage: " << argv[0] << " <config.xml>" << endl;
		exit(1);
	}

	ifstream cfg_file_stream(argv[1]);
	if (!cfg_file_stream.good()) {
		cerr << "Problems reading the configuration file from: " << argv[1] << endl;
		exit(1);
	}

	boost::archive::xml_iarchive cfg_xml_archv(cfg_file_stream);
	Configs* conf;
	cfg_xml_archv >> BOOST_SERIALIZATION_NVP(conf);

	CvCapture* capture = cvCaptureFromAVI(conf->input_video_path.c_str());

	if (capture == NULL) {
		cerr << "Could not open video file: " << argv[1] << endl;
		exit(1);
	}

	int numFrames = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
	int frameH = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
	int frameW = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);

	fprintf(stdout, "Video info:      %d frames of %d x %d pixels.\n", numFrames, frameW, frameH);

	// Query first video frame in order to estimate object color model
	IplImage* img = cvQueryFrame(capture);

	// this is generated based on a rectangular area known to belong to the object
	CvRect object_region =
		cvRect(conf->object_reg_x, conf->object_reg_y, conf->object_reg_width, conf->object_reg_height);

	cvNamedWindow("Video");

	CvVideoWriter* writer;

	writer = cvCreateVideoWriter(conf->output_video_path.c_str(), CV_FOURCC('D', 'I', 'V', 'X'), conf->output_fps,
								 cvGetSize(img), 1);

	if (writer == NULL) {
		cerr << "Could not write video to " << conf->output_video_path << endl;
		exit(1);
	}

	State true_initial_state(object_region);

	cout << "x = " << true_initial_state.GetRectangle().x << " y = " << true_initial_state.GetRectangle().y << endl;

	IplImage* img_Lab = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);
	cvCvtColor(img, img_Lab, CV_BGR2Lab);

	ObservationModel observation_model(img_Lab, true_initial_state, conf->lambda, conf->nhistbins);

	DynamicsModel dynamics_model(img->width, img->height);

	ParticleFilter particle_filter(conf->nparticles, dynamics_model, observation_model, true_initial_state);

	for (int nframe = 1; nframe < numFrames - 1; nframe++) {
		if (conf->visualize || conf->save_results) {
			vector<CvRect> particle_locations = particle_filter.GetParticleLocations();

			for (size_t i = 0; i < particle_locations.size(); i++) {
				CvRect rect = particle_locations[i];
				cvRectangle(img, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height),
							cvScalar(0, 0, 255), 1);
			}
			CvRect estimate = particle_filter.GetPrediction();
			cvRectangle(img, cvPoint(estimate.x, estimate.y),
						cvPoint(estimate.x + estimate.width, estimate.y + estimate.height), cvScalar(0, 255, 0), 2);

			if (conf->visualize) {
				cvShowImage("Video", img);
				// cvWaitKey(0);
			}
			if (conf->save_results) {
				cvWriteFrame(writer, img);
			}
		}

		double tt = (double)cvGetTickCount();

		img = cvQueryFrame(capture);

		if (img == NULL) break;

		cvCvtColor(img, img_Lab, CV_BGR2Lab);

		particle_filter.ProcessObservation(img_Lab);

		tt = (double)cvGetTickCount() - tt;
	}

	cvReleaseImage(&img_Lab);
	cvReleaseVideoWriter(&writer);
	cvDestroyWindow("Video");
	cvReleaseCapture(&capture);
}

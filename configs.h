#include <string>
#include <vector>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>

#ifndef CONFIGS_H_ZSBH
#define CONFIGS_H_ZSBH



 ///  Stores all the necessary parameters of the application (loaded from a XML file)
class Configs
{

	friend class boost::serialization::access;


public:
	Configs();
	
	void generateXMLEmptyTemplate(std::string filename);

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
	
		ar & BOOST_SERIALIZATION_NVP( input_video_path );
	
		ar & BOOST_SERIALIZATION_NVP( nparticles );
		ar & BOOST_SERIALIZATION_NVP( nhistbins );
		ar & BOOST_SERIALIZATION_NVP( lambda );
		
		ar & BOOST_SERIALIZATION_NVP( object_reg_x 	);
		ar & BOOST_SERIALIZATION_NVP( object_reg_y	);
		ar & BOOST_SERIALIZATION_NVP( object_reg_width  );
		ar & BOOST_SERIALIZATION_NVP( object_reg_height );
		       
		ar & BOOST_SERIALIZATION_NVP( visualize );
		ar & BOOST_SERIALIZATION_NVP( save_results );

		ar & BOOST_SERIALIZATION_NVP( output_video_path );
		ar & BOOST_SERIALIZATION_NVP( output_fps     );
		
	}
	
	std::string input_video_path;
	
	int nparticles;
	int nhistbins;
	double lambda;

	double object_reg_x;
	double object_reg_y;
	double object_reg_width;
	double object_reg_height;

	bool visualize;
	bool save_results;

	std::string output_video_path;
	int output_fps;  
	
	
	
};

#endif

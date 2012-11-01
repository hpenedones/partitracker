
#include <fstream>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <configs.h>
BOOST_CLASS_EXPORT(Configs)  



Configs::Configs()
{

		
}



void Configs::generateXMLEmptyTemplate(string filename)
{
	 ofstream ofs( filename.c_str() );
  	 assert(ofs.good());
  	 boost::archive::xml_oarchive oa(ofs);
  	 oa << boost::serialization::make_nvp("configs", this);
  	 ofs.close();
}
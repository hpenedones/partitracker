
#include <configs.h>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <fstream>
#include <string>
BOOST_CLASS_EXPORT(Configs)

Configs::Configs() {}

void Configs::generateXMLEmptyTemplate(std::string filename) {
	std::ofstream ofs(filename.c_str());
	assert(ofs.good());
	boost::archive::xml_oarchive oa(ofs);
	oa << boost::serialization::make_nvp("configs", this);
	ofs.close();
}
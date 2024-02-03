#include "XMLReader.h"
#include <expat.h>

struct CXMLReader::SImplementation{
    std::shared_ptr< CDataSource > DDataSource;
    XML_Parser DXMLParser;

    SImplementation(std::shared_ptr< CDataSource > src){
        DDataSource = src;
        DXMLParser = XML_ParserCreate(NULL);
    };

    bool End() const{

    };

    bool ReadEntity(SXMLEntity &entity, bool skipcdata){

    };
};
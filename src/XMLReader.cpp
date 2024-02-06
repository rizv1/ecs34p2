#include "XMLReader.h"
#include <expat.h>
#include <iostream>
#include <queue>
#include <vector>
#include <string>


struct CXMLReader::SImplementation{
    std::shared_ptr< CDataSource > DDataSource; //pointer to xml source file
    XML_Parser DXMLParser;
    std::queue<SXMLEntity> DEntityQueue; //queue to store parsed entities

    void StartElementHandler(const std::string &name, const std::vector<std::string> &attrs){ //when parser reaches start of an xml element SXMLEntity object with element's name and attributes is pushed onto entity queue
        SXMLEntity TempEntity;
        TempEntity.DNameData = name; 
        TempEntity.DType = SXMLEntity::EType::StartElement;
        for(size_t Index = 0; Index < attrs.size(); Index += 2){    //
            TempEntity.SetAttribute(attrs[Index], attrs[Index + 1]);
        }
        DEntityQueue.push(TempEntity);
    }

    void EndElementHandler(const std::string &name){
        SXMLEntity TempEntity;
        TempEntity.DNameData = name;
        TempEntity.DType = SXMLEntity::EType::EndElement;
        DEntityQueue.push(TempEntity);

    }
        
    void CharacterDataHandler(const std::string &cdata) {
        if (!DEntityQueue.empty()) {
            // Append character data to the current entity's name or create a new attribute if needed
            if (DEntityQueue.back().DAttributes.empty()) {
                DEntityQueue.back().DNameData += cdata;
            } else {
                DEntityQueue.back().SetAttribute("value", cdata);
            }
        }
    }


    static void StartElementHandlerCallback(void *context, const XML_Char *name, const XML_Char **atts){ //converts expat callback to our struct method call
        SImplementation *ReaderObject = static_cast<SImplementation *>(context);
        std::vector<std::string> Attributes;
        auto AttrPtr = atts;
        while(*AttrPtr){
            Attributes.push_back(*AttrPtr);
            AttrPtr++;
        }
        ReaderObject->StartElementHandler(name, Attributes);
    };

    static void EndElementHandlerCallback(void *context, const XML_Char *name){
        SImplementation *ReaderObject = static_cast <SImplementation *>(context);
        std::cout << "EndElementHandlerCallback called for " << name << std::endl;
        ReaderObject->EndElementHandler(name);
    };

    static void CharacterDataHandlerCallback(void *context, const XML_Char *s, int len){
        SImplementation *ReaderObject = static_cast<SImplementation *>(context);
        ReaderObject->CharacterDataHandler(std::string(s,len));
    };

    SImplementation(std::shared_ptr < CDataSource > src) {  //constructor
        DDataSource = src;
        DXMLParser = XML_ParserCreate(NULL);
        XML_SetStartElementHandler(DXMLParser, StartElementHandlerCallback);
        XML_SetEndElementHandler(DXMLParser, EndElementHandlerCallback);
        XML_SetCharacterDataHandler(DXMLParser, CharacterDataHandlerCallback);
        XML_SetUserData(DXMLParser, this);
    };

    bool End() const{   //calls 'End' method of 'SImplementation' struct
        return DEntityQueue.empty(); //true if queue is empty
    };

    bool ReadEntity(SXMLEntity &entity, bool skipcdata){ //reads data source in 256 byte chunks and passes it to Expat parses using XML_Parse function
        //Read from source if necessary
        // pass to XML_Parse function
        // return entity
        std::vector<char> DataBuffer;
        while(DEntityQueue.empty()){
            std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
            if(DDataSource->Read(DataBuffer, 256)){
                std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
                XML_Parse(DXMLParser, DataBuffer.data(), DataBuffer.size(), DataBuffer.size() < 256);
            }
            else{
                std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
                XML_Parse(DXMLParser, DataBuffer.data(), DataBuffer.size(), true);
            }
        }
        if(DEntityQueue.empty()){
            return false;   //if queue is empty
        }
        entity = DEntityQueue.front();  //parsed entities are stored in DEntityQueue
        DEntityQueue.pop();
        return true;    //if atleast one entity in the queue after parsing, and the front entity is stored in the provided SXMLEtity parameter.
    };
};

CXMLReader::CXMLReader(std::shared_ptr< CDataSource > src){
    DImplementation = std::make_unique < SImplementation > (src);
}

CXMLReader::~CXMLReader(){

}

bool CXMLReader::End() const{
    return DImplementation->End();
}

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){    //calls 'ReadEntity' method of SImplementation struct to read and retrieve the next XML entity
    return DImplementation->ReadEntity(entity, skipcdata);
}

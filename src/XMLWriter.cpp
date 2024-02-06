#include "XMLWriter.h"
#include <expat.h>
#include <iostream>
#include <cstring>


struct CXMLWriter::SImplementation {
    std::shared_ptr<CDataSink> DSink;
    std::vector<char> DBuffer; // Buffer to store intermediate data
    bool DFlushed;

    SImplementation(std::shared_ptr<CDataSink> sink) : DSink(sink), DFlushed(true) {
    }

    void StartElementHandler(const XML_Char *name, const XML_Char **atts) {
        std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
        if (!DFlushed) {
            DSink->Put('\n');
        }

        DSink->Put('<');
        WriteToSink(name);

        for (int i = 0; atts[i]; i += 2) {  // Write attributes
            DSink->Put(' ');
            WriteToSink(atts[i]);
            DSink->Put('=');
            DSink->Put('"');
            WriteToSink(atts[i + 1]);
            DSink->Put('"');
        }

        DSink->Put('>');

        DFlushed = false;
    }

    void EndElementHandler(const XML_Char *name) {
        std::cout<<"end element handler\n\n\n";
        std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
        if (!DFlushed) {
            DSink->Put('\n');
        }

        DSink->Put('<');    //construct and write the closing tag (</name>)
        DSink->Put('/');
        WriteToSink(name);
        DSink->Put('>');

        DFlushed = false;
    }

    void CharacterDataHandler(const XML_Char *s, int len) {
        std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
        if (!DFlushed) {
            DSink->Put('\n');
        }

        WriteToSink(s, len);

        DFlushed = false;
    }

    void WriteToSink(const XML_Char *data) {    // Write data to the buffer
        std::cout << __FILE__ << " @ line: " << __LINE__ << std::endl;

        if (data) {
            size_t len = strlen(data);

            if (len > 0) {
                DBuffer.insert(DBuffer.end(), data, data + len);
            } else {
                std::cerr << "Warning: Attempted to insert empty data into DBuffer." << std::endl;
            }
        } else {
            std::cerr << "Warning: Attempted to insert null data into DBuffer." << std::endl;
        }
    }

    
    void WriteToSink(const XML_Char *data, int len) {   // Write specified length of data to the buffer
        std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
        if (data && len > 0) {      //valid data to be inserted into the buffer
            DBuffer.insert(DBuffer.end(), data, data + len);
        } else {
            std::cerr << "Warning: Attempted to insert null or empty data into DBuffer." << std::endl;
        }
    }

    void FlushBuffer() {        // Flush the buffer to the data sink
        std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
        DSink->Write(DBuffer);
        DBuffer.clear();
    }

    bool Flush() {  // Flush the buffer and put a newline character
        std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
        if (!DFlushed) {
            DSink->Put('\n');
            FlushBuffer();
            DFlushed = true;
        }
        return true;
    }

    bool WriteEntity(const SXMLEntity &entity) {
        std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
        if (entity.DType == SXMLEntity::EType::StartElement) {      //checks the type of the XML entity and calls the respective handler
            StartElementHandler(entity.DNameData.c_str(), nullptr);
        } else if (entity.DType == SXMLEntity::EType::EndElement) {
            EndElementHandler(entity.DNameData.c_str());
        } else if (entity.DType == SXMLEntity::EType::CharData) {
            CharacterDataHandler(entity.DNameData.c_str(), entity.DNameData.length());
        }

        return true;
    }
};

CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink) : DImplementation(std::make_unique<SImplementation>(sink)) {
}

CXMLWriter::~CXMLWriter() {
    std::cout << "CXMLWriter destructor called." << std::endl;
}


bool CXMLWriter::Flush() {
    std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
    DImplementation->FlushBuffer(); // Flush any remaining data in the buffer
    return DImplementation->Flush();
}

bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
    std::cout<<__FILE__<<" @ line: "<<__LINE__<<std::endl;
    return DImplementation->WriteEntity(entity);
}

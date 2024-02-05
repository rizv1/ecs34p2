

#include "DSVReader.h"
#include <vector>
#include <string>


struct CDSVReader::SImplementation {
    std::shared_ptr<CDataSource> DDataSource;
    char DDelimiter;

    SImplementation(std::shared_ptr<CDataSource> src, char delimiter) : DDataSource(src), DDelimiter(delimiter) {
    }

    bool End() const {
        return DDataSource->End();
    }

    bool ReadRow(std::vector<std::string> &row) {
        row.clear();
        char ch;
        std::string field;

        while (DDataSource->Get(ch)) {  //DDataSource->End() was causing the loop to end 1 char too early
            if (ch == DDelimiter) {
                row.push_back(field);
                field.clear();
            } else {
                field += ch;
            }
        }

        row.push_back(field);

        if (row.size() == 1 && row[0].empty()) {    //empty strings were being returned as not empty
        row.pop_back();
        }

        return !row.empty();
    }
};

CDSVReader::CDSVReader(std::shared_ptr<CDataSource> src, char delimiter) : DImplementation(std::make_unique<SImplementation>(src, delimiter)) {
}

CDSVReader::~CDSVReader() {
}

bool CDSVReader::End() const {
    return DImplementation->End();
}

bool CDSVReader::ReadRow(std::vector<std::string> &row) {
    return DImplementation->ReadRow(row);
}

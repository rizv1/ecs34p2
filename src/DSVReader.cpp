
// Constructor for DSV reader, src specifies the data source and delimiter
// specifies the delimiting character
//CDSVReader(std::shared_ptr< CDataSource > src, char delimiter);
// Destructor for DSV reader
//~CDSVReader();
// Returns true if all rows have been read from the DSV
//bool End() const;
// Returns true if the row is successfully read, one string will be put in
// the row per column
//bool ReadRow(std::vector< std::string > &row);
#include "DSVReader.h"
#include <vector>
#include <string>
#include "StringDataSource.h"

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

        while (DDataSource->Get(ch) && !End()) {
            if (ch == DDelimiter) {
                row.push_back(field);
                field.clear();
            } else {
                field += ch;
            }
        }

        // Add the last field after the last delimiter or until the end
        row.push_back(field);

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

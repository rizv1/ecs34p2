#include "DSVWriter.h"
#include <vector>
#include <string>
#include "StringUtils.h"


struct CDSVWriter::SImplementation {
    std::shared_ptr<CDataSink> DSink;
    char DDelimiter;
    bool DQuoteAll;
    bool DFirstRow = true;

    SImplementation(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall) : DSink(sink), DDelimiter(delimiter), DQuoteAll(quoteall) {
    }

    ~SImplementation() {
    
    }

    bool WriteRow(const std::vector<std::string> &row) {
        
        if (!DFirstRow) {
            DSink->Put('\n');  // If not the first row, add newline
        } else {
            DFirstRow = false;
        }
        if (DDelimiter == '"'){
            DDelimiter = ',';
        }
        for (size_t i = 0; i < row.size(); ++i) {
            if (i > 0) {
                DSink->Put(DDelimiter);
            }
            if (DQuoteAll || ContainsSpecialCharacters(row[i])) {
                const std::string quotedString = ConvertToQuotedString(row[i]);
                DSink->Write(std::vector<char>(quotedString.begin(), quotedString.end()));
            } else {
                const std::string unquotedString = ConvertToUnquotedString(row[i]);
                DSink->Write(std::vector<char>(unquotedString.begin(), unquotedString.end()));
            }
            
        }
        return true;
    }

    bool ContainsSpecialCharacters(const std::string &str) {
        return str.find(DDelimiter) != std::string::npos || str.find('"') != std::string::npos || str.find('\n') != std::string::npos;
    }


    std::string ConvertToQuotedString(const std::string &str) {

        if (str.find("\"") != std::string::npos){
            size_t pos = 0;
            std::string quotedString = str;
            while ((pos = quotedString.find("\"", pos)) != std::string::npos) {
                quotedString.replace(pos, 1, "\"\"");
                pos += 2;  // Move past the two double quotes
            }
            //quotedString += "\"";
            return "\"" + quotedString + "\"";
        }
        return "\"" + str + "\"";
    }

    std::string ConvertToUnquotedString(const std::string &str) {
        if (str.find("\"") != std::string::npos){
            size_t pos = 0;
            std::string quotedString = str;
            while ((pos = quotedString.find("\"", pos)) != std::string::npos) {
                quotedString.replace(pos, 1, "\"\"");
                pos += 2;  // Move past the two double quotes
            }
            //quotedString += "\"";
            return "\"" + quotedString + "\"";
        }
        return str;
    }
};

CDSVWriter::CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall) : DImplementation(std::make_unique<SImplementation>(sink, delimiter, quoteall)) {
}

CDSVWriter::~CDSVWriter() {
}

bool CDSVWriter::WriteRow(const std::vector<std::string> &row) {
    return DImplementation->WriteRow(row);
}

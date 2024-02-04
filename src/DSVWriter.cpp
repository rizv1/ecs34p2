#include "DSVWriter.h"
#include <vector>
#include <string>

struct CDSVWriter::SImplementation {
    std::shared_ptr<CDataSink> DSink;
    char DDelimiter;
    bool DQuoteAll;

    SImplementation(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall) : DSink(sink), DDelimiter(delimiter), DQuoteAll(quoteall) {
    }

    ~SImplementation() {
        // If necessary, add any cleanup code here
    }

    bool WriteRow(const std::vector<std::string> &row) {
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

        // Add a newline character at the end of the row
        DSink->Put('\n');

        return true;
    }

    bool ContainsSpecialCharacters(const std::string &str) {
        return str.find(DDelimiter) != std::string::npos || str.find('"') != std::string::npos || str.find('\n') != std::string::npos;
    }

    std::string ConvertToQuotedString(const std::string &str) {
        // Add logic to escape double quotes if needed
        return str;
    }

    std::string ConvertToUnquotedString(const std::string &str) {
        // Add logic to handle unquoted strings if needed
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

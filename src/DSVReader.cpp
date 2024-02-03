// Constructor for DSV reader, src specifies the data source and delimiter
// specifies the delimiting character
CDSVReader(std::shared_ptr< CDataSource > src, char delimiter);
// Destructor for DSV reader
~CDSVReader();
// Returns true if all rows have been read from the DSV
bool End() const;
// Returns true if the row is successfully read, one string will be put in
// the row per column
bool ReadRow(std::vector< std::string > &row);

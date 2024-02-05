When a test is run, we're creating string with a shared pointer and passing this pointer and the delimtter to make an instance of SImplementation. The constructor at the bottom of DSVReader makes an instance called DImplementation with variables src and delimiter

A vector of strings is passed in to the ReadRow() function, this is where the parsed elements will be stored

The CDSVReader::ReadRow at the bottom of of DSVReader calls ReadRow through the DImplementation instance

while there are characters in the source, the characters are appended to a string called field, as soon as a delimter is reached, the field string is pushed back into the row variable (vector of strings).

the loop ends when theres no more chars left, the left over string gets pushed back.

Theres an if condition in case an empty string was passed in for the test. An empty string makes the row.size() = 1. This condition uses the pop_back function to empty out the row vector if theres one row with an empty element.

this bool function returns True if file was parsed and elements were successfully added to the row vector.



# Project 2 README

Student 1: Danish Rizvi (solo) (919463615)



## Project Status
DSV Reader and Writer functions compile and pass all student tests
Basic tests for piazza for XML files are being used, additional tests need to be written.

## Known Issues
XMLReader and Writer have many issues and will not compile

## Code References
* [XML_Parser_Create Syntax](https://www.php.net/manual/en/function.xml-parser-create.php)
* [Seg fault xml parse](https://stackoverflow.com/questions/11228304/segmentation-fault-when-trying-to-parse-xml-file)
* [Seg fault 2](https://stackoverflow.com/questions/46246977/reading-a-list-of-xml-nodes-causes-segmentation-fault)
* ["Expected unqualified-id before 'namespace](https://stackoverflow.com/questions/7075429/expected-unqualified-id-before-namespace-error)
* [Boost set_filter is not working](https://stackoverflow.com/questions/29707017/boost-set-filter-is-not-working)
* [XML_SetStartElementHandler](https://stackoverflow.com/questions/12483724/expat-function-xml-startelementhandler-how-to-work-with-it)
* [XML_SetCharacterDataHandler](https://www.xml.com/pub/1999/09/expat/reference.html)



## Generative AI Use
I used ChatGPT for all Prompts

### Prompt 1
I want to write a for loop that iterates though the elements of a string vector without knowing the number of elements in the vector 

### Response 1
Certainly!:

size_t i = 0;
for (const std::string& str : stringVector) {
    std::cout<<i;
}
//output: 123
This is a range-based for loop that iterates over each element (str) in the stringVector. The loop will execute once for each element in stringVector.

### Chagnes 1
I knew this was possible I just needed to see the syntax. I replaced the variables and used the for loop to iterate through the string vector in my DSVWrite function.

### Prompt 2
can you explain what each of this functions does?
bool CStringDataSink::Write(const std::vector<char> &buf) noexcept{
    DString += std::string(buf.data(), buf.size());
    return true;
}


### Response 2
Certainly! Let's go through the Write function:
Purpose: Appends a vector of characters to the data sink.
Parameters:
const std::vector<char> &buf: Reference to the vector of characters to be appended.
Return Type: bool
Return Value:
Always returns true as there is no explicit failure condition.
The noexcept specifier indicates that these functions are not expected to throw exceptions.

### Changes 2
Didn't make specific changes based on this output, I just waNTed to better understand the use of this function before I used it.

### Prompt 3
./include/StringDataSink.h:14:14: error: ‘bool CStringDataSink::Write(const std::vector<char>&)’ marked ‘override’, but does not override
   14 |         bool Write(const std::vector<char> &buf) noexcept override;

### Response 3

The error message indicates that the Write function in your CStringDataSink class is marked as override, but it does not actually override any function from the base class.

When you use the override specifier, it tells the compiler that the function is intended to override a virtual function in a base class. However, if the function signature does not match any virtual function in the base class, you will get a compilation error.

To resolve this issue, make sure that the signature of your Write function in the CStringDataSink class matches the signature of the function it is supposed to override in the base class. The signature should include the same return type, function name, parameters, and const and noexcept specifiers.

For example, if your base class has a declaration like:

cpp
Copy code
class BaseClass {
public:
    virtual bool Write(const std::vector<char> &buf) noexcept = 0;
};
Then, your CStringDataSink class should have a declaration like:

cpp
Copy code
class CStringDataSink : public BaseClass {
public:
    bool Write(const std::vector<char> &buf) noexcept override;
};
Ensure that the function signature in your derived class matches the one in the base class, including the noexcept specifier. If you are not intending to override any function from the base class, you can remove the override specifier from your derived class's function declaration.

### Changes 3
made the function name in my derived class match

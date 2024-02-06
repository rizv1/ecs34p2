This implementation in our case takes a vector strings to be written, a delimiter character, and a quotall bool (true or false) as input. it outputs a DSV file which is a string variable in our case, that writes the strings from the input vector, seperates them with the delimiter character and can also put quotation marks around each string. 

An instance of SImplementation, called DImplementation is created with paramaters sink delimter and quoteall.
sink - the variable we're writing to
quoteall - bool variable, if true, put quotation marks around value

Writerow(row) is called where row is a vector of strings to write

- each row after the first is prepended with a newline
- if the delimiter is double quotes ("), it is replaced by a comma (,)
- a delimiter is put after each write
- if quotall is true or the row (string in our case) contains a special character such as the delimiter, double quotes (") or newline(\n) the value must have quotation marks on it.

example:
quotall = true
delimiter = '"'
input: {"hello", "world"}
output: "hello","world"

quotall = false
delimiter = '_'
input: {"hello_", "world"}
output: "hello_"_world

- if a string contains a double quote char ("), an additional quote is put on it

quotall = false
delimiter = '_'
input: {"hello", "world"s"}
output: hello_"worl""d"

in the example above, the word world has quotes around it because it contains a special character ("). Since the special character is a ("), an additional " is put on it.






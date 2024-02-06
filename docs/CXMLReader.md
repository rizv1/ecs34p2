This is an XMLReader that uses the expat library functions to parse an XML file.

This implementation contains handlers that are called upon when the parser encounters certain situations. 

Handlers

the StartElementHandler is called when the parser reaches a <. when the start element is reached, the handler creates a new entity and saves the entity. 
The character Data handler appends chars to the entity's name or creates new attributes. 
The EndElement handler is called when an end of an XML element is reached. it creates an XML entity object and pushes it to the entity queue.

Handler Callbacks - These functions convert Expat callbacks to the respective SImplementation method. 

Constructor - Creates the parser and sets the callback functions for start, end and character data handling.

end method:
returns true if queue is empty so no more left to read.

ReadEntity:
reads data from source in chunks of 256 bytes and passes it to Expat using the XML_Parse() funciton. this Parses the source and fills the queue. if queue is empty after parsing, it returns false. it retrieves the front entity if theres is atleast one in queue and returns true.

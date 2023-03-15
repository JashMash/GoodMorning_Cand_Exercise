Hello! 
My name is Jashish and this is a repo I created for my submission for <br />
Software Developer Candidate Exercise.

### Contains:
A child class to 'Request' that is capable of:
- Retrieving **mean response times** of all requests the object has processed <br />
  for each separate URI, in millisecond units.
- Retrieving **standard deviation for the response times** of all requests the<br />
   object has processed for each separate URI, in millisecond units.
- Retrieving **normalized histogram for the response times** of all requests <br />
  the object has processed for each separate URI,
  
  Format it is returned is 'std::vector<std::tuple<std::string, int>>'<br />
  with the first index of tuple containing the range of the bin and <br />
  the second containing bin count.
  - **Max bins** provided during object instantiation. Actual bins must be <br />
    as close to max bins value as possible
  - **All response times** must be captured in Histogram 
  - Must contain **no empty bin at edges**

### Make commands
  - Use '**make**' to create all files required.<br />
  - Use '**make clean**' to delete all output files.<br />
  - Use '**make distribute**' to create tar.bz2 file

### Unit testing commands:
  - After using '**make**' the unit test executeable will be created,<br />
    to run the unit tests please use '**.\bin.exe**'.
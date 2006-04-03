// The std namespace headers should be included BEFORE declaring the namespace
// Old compilers may have different names for some of the headers, like
// <strstream> instead of <sstream>
// Furthermore, these headers should NOT be used together with their
// no-namespace variants (*.h) since this will create problems on some
// compilers (like Microsoft Visual C++)

//#include <bitset>
//#include <functional>
//#include <iomanip>
//#include <limits>
//#include <locale>
//#include <memory>
//#include <numeric>
//#include <stack>
//#include <stdexcept>
//#include <utility>

#include <string>		// C++ string

#include <algorithm>		// STL
#include <iterator>
#include <vector>
#include <deque>
#include <list>
#include <map>
#include <set>
				// Streams
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>


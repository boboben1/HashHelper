# HashHelper
Easily make something hashable

# Purpose
Makes something hashable easily

# Usage
```C++


MAKE_HASHABLE(MAKE_TEMPLATE(std::pair, int, int), first, second)
	//or
MAKE_PAIR_HASHABLE(int, int)
MAKE_TUPLE_HASHABLE(int, int, int)

hash_value(obj) // GET HASH VALUE
hash_combine(hash, obj) //ADD ONTO THE HASH
```

# Example

```C++
#include <iostream>
#include <utility>
#include "HashHelper.hpp"

int main()
{
	auto t = std::make_pair(5, 1);


	std::cout << hash_value(t) << std::endl;

	std::cout << hash_value(std::make_tuple<int, int, int>(1, 2, 3)) << std::endl;


  return 0;
}

MAKE_HASHABLE(MAKE_TEMPLATE(std::pair, int, int), first, second)
MAKE_TUPLE_HASHABLE(int, int, int)
```

# TODO
Optimizations

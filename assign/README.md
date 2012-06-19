assign
======

Attempt to emulate some of the functionality of boost.assign.
At least, it allows constructs like:

```C++
std::vector<int> v;
v += 1, 1, 2, 3, 5, 8, 13;
```

This is rather old. Don't use it.

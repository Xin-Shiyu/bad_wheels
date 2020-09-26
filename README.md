# bad_wheels
Bad wheels written while learning C++

# About
Containers are interesting. To learn C++, I'm working on these "bad wheels" for practice.

There is currently only one "wheel", `list<T>`. It's similar to `List<T>` in .NET (at least in name).

The `list<T>` is based on another class template `virtual_list<T>`. The difference is that `virtual_list<T>` does not manage memory itself and thus serves more like a "list view", while `list<T>` can be extended and occupies actual space.

The methods available for both class templates are:

- `find` which looks for an element beginning from a certain index (0 if not given) and returns its index (-1 if not found);
- `contains` which is a syntax sugar for finding something != -1;
- `reverse` which reverses the order of the elements;
- `count` which returns the count of the elements;
- `begin` which simply returns the pointer to its first element;
- `end` which returns the pointer to the place after its last element.

The last two methods are given so you can use range-based for to iterate through a `list<T>` or a `virtual_list<T>`.

Be careful the pointers are real pointers pointing to actual places in memory and can become invalid ones when a `list<T>` is reallocated, so do not add or insert anything when iterating using a range-based for.

The methods limited to `list<T>` are:

- `add` which appends an element to the list;
- `insert` which inserts an element before a given index in the list;
- `remove_at` which removes the element at a given index;
- `remove_all` which removes every element that equals to the given one from the list;
- `attach` which attaches another `virtual_list<T>`, including `list<T>` to the original list (given an std::initializer_list will currently create a temporary `list<T>` based on that which may be changed in the future);
- `range` which returns a `virtual_list<T>` that refers to a slice of the list.

Note that the `virtual_list<T>` created with the `range` method will become invalid when the original `list<T>` is reallocated.

You can access an element in both class templates with the `[]` operator. However there is no bound check, so take care when you use them.

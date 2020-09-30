# bad_wheels

Bad wheels written while learning C++

# About

Containers are interesting. To learn C++, I'm working on these "bad wheels" for practice.

# Usage of the wheels

It's evident that *none* of the wheels is production-ready, but I guess this simple description of their usage might be useful if you just want to play with them (and 辱骂 them).

Currently there are only containers and they all exist in the namespace nativa::collections. By including the header "collections.h", you will have all the following wheels ready for use.

## `list<T>`

It's similar to `List<T>` in .NET (at least in name).

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
- `attach` which attaches another `virtual_list<T>`, including `list<T>` to the original list (giving an std::initializer_list will currently create a temporary `list<T>` based on that which may be changed in the future);
- `range` which returns a `virtual_list<T>` that refers to a slice of the list.

Note that the `virtual_list<T>` created with the `range` method will become invalid when the original `list<T>` is reallocated.

You can access an element in both class templates with the `[]` operator. However there is no bound check, so take care when you use them.

## `dictionary<TKey, TValue>`

You know what it is, but it's much more badly implemented. Be careful about the yet-too-simple implementation of the hash functions.

It has the following methods:

- `add` which adds a key and a value corresponding to the key into the dictionary (I don't really feel like using `[]` to create an entry as its ambiguity between creating an entry and accessing one does not sound good)
- `remove_at` which removes an entry with the given key;
- `contains_key` which checks if the dictionary contains the given key;
- `begin` which returns an instance of the very simple class dictionary<TKey, TValue>::iterator pointing to the first entry appearing in the underlying array of the dictionary;
- `end` which returns a dictionary<TKey, TValue>::iterator pointing to the last entry appearing in the underlying array. (Note that the iterator will go to the next existing entry and will finally fall at the index after the last entry so comparing an iterator to end() is a proper usage);

When you dereference an iterator, you will get a reference to the entry. The entry struct has two public members, `key` and `value`. Modifying the key manually will cause unexpected problems.

You can easily use `[]` to get the reference to the value corresponding to the given key. However, for an unexisting key, it will refer to an undesirable space.

## `stack<T, size>`

From now on there is nothing so much interesting: it's just your old, favorite stack. Unfortunately I like it to be fixed length, so you have to give the size when you instantiate it. The methods has almost the same names as you have in .NET.

- `push`
- `pop` (returns and pops the element top-most)
- `peek` (returns the const reference of the element top-most)
- `count`
- `is_empty`
- `is_full` (since we have the `size` template argument)

## `queue<T>`

It cannot be really used as a queue, because it may not work properly when there are more than one thread.

This class template is based on a dynamically-allocated linked-list, so it can be as long as you wish.

You can easily guess the method names or even directly look at them in the source code so I'm not so much willing to talk about them.

## `circular_queue<T, max_count>`

Don't ask me why in stack I call it `size` while here I call it `max_count`, because I just feel like I should name them so. This is based on a fixed sized array and thus there is a method called `is_full`. Very natural thing right? I guess this is the simplest thing I've written this days but probably the most buggy too. Please inform me if you found anything wrong here. (When the user give good input; I did nothing about invalid input in this whole project as they are not the focus during my current learning proccess)

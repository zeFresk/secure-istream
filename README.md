Secure-istream
==============

### What ?

Here you can find a high-level and secure version of basic_stream (\<iostream\> header). It was made to be easy to use and to replace standard input streams.

It is also very flexible (as much as standard basic_stream) and provides a secure version of cin and wcin.

Plus you can either change the basic error message or even add a callback function which will be call if a bad user's entry occurs.

### Why ?

Because everytime I made a CMD program I had to code simple functions to ensure than the user's input could not crash the application

It was a pain in the ass to code that every single time plus it wasn't that flexible...

Also it was usually less easier to use than cin and wcin and beginners couldn't use those functions.

So I decided to code a big class once and for all so I and others could use it.

### How ?
## Installation :

Just save or copy paste the .cpp and the .h and put it in your project.

And that's all ! Didn't I say "easy to use" ? :D

## Utilisation :

It's even easier to use than to install. If you don't need an advanced error management or a user defined error message then replace

```cpp
cin >> somevar;
wcin >> othervar;
```

by

```cpp
scin >> somevar;
swcin >> othervar;
```

> Wow, much difficulty, such modifications !

If you want a custom error message just make your own sistream or wsistream by doing :

```cpp
/*w*/sistream my_secure_input(/*a input stream*/ std::/*w*/cin, "My error message");
```

If you want to use a custom callback, which will do something with the user's bad input, then simply add a third parameter (it has to be convertible to `std::function<void(string_type)>`)

If you don't want to pay the cost of a `std::function` you can also use the `custom_/*w*/sistream` type.

And if you know what you are doing you can even use the raw basic_sistream class.

Share, Edit and Use as much as you want <3
====================

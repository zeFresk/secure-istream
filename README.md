Secure-istream
==============

### What ?

Here you can found an high-level and secure version of basic_stream. It was made to be as easy to use as possible and to remplace standard input streams.

It is also very flexible (as much as standard basic_stream) and provides a secure version of scin and wcin.

Plus you can either change the basic error message or even add a callback function which will be call if a bad user's entry occurs.

### Why ?

Because everytime I made a console programme I had to code a simple function to ensure than the user's input could not crash all I did.

It was a pain in the ass to recode that every single time plus it wasn't that flexible...

Also it was usually less easier to use than cin and wcin and debutant couldn't use those functions.

So I decided to code a big class once and for all so I and others could use it.

### How ?
## Installation :

Just save or copy paste the .cpp and the .h and put it in your project.

And that's all ! Didn't I say "easy to use" ? :D

## Utilisation :

It's even easier to use than to install. If you don't need an advanced error management or a user defined error message then remplace

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

And if you want to use a custom callback which will do something with the user's bad input then simply add a third parameter (convertible to `std::function<void(string_type)>`)

If you don't want to pay the cost of a `std::function` you can also use the `custom_/*w*/sistream` type.

And if you know what you are doing you can even use the raw basic_sistream class.

Share as you want <3
====================
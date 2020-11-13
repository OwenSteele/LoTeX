# LoTeX: low-end LaTeX

Basic compiler-like program to convert plain text into formatted '.html' files.

- Publishes '.txt' into formatted '.html' with CSS classes.

- Uses assigned '/style' commands, edited with '#style' commands - that determine '.html'/CSS formats.

- Creates template.txt file for starting a new file (See 'Template.txt').

__See Example.txt and Example.html for and example of the programs publish function and capabilities__

# Getting Started

CMake is required to build and run this project. The following commands can be run at the build directory to build and run the program.

```
>> mkdir build
>> cd build
>> cmake ..
>> cmake --build .
>> ./lotex_run
```

# glitch-effect
This is a relatively small, fun little project I made mostly by myself.

I started the project because I wanted to learn some `C++`, and a terminal
glitch effect seemed like a way to go.
Anyway, the project is a CLI app running on `ncurses` that takes some text
as an input, and then displays distorted and animated version of that text.

`glitch` has a highly customizable pipeline that allows configuration with
json files. You can specify `passes`, that act as distortion layers that
can be stacked on each other (for examples see `conf/config.jsonc`).


## Dependencies
The following dependencies are required to build and run the program:

* make
* g++
* ncurses
* nlohmann-json


## Compiling
To ensure all required dependencies are installed, run the script provided in the repository:
```
./check-dependencies.sh
```

To compile the source code, use the makefile:
```
make
```

To compile and install the program, run:
```
make install
```

After installation, copy the configuration file from the repository to `~/.config/glitch-effect/`.
This step is not required, but recommended as the program falls back to the default, built-in configuration if needed.

Run `copy-config.sh` to automatically copy the config files to the target directory.
```
./copy-config.sh
```


## Usage
To see the available options, run:
```
glitch -h
```

Even though the program does not require a config file, it still needs a text file to display.
Such files are provided in the repository in the `ascii` directory.

Alternatively, you can pipe command output to the glitch and display that:
```
head -n 4 | base64 | glitch
```

If you encounter any issues, you can pass `-v 0` to make sure that all logs are displayed for more debugging info.

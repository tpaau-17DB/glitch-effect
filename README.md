# glitch-effect
This is a relatively small, fun little project I made mostly by myself.

I started the project because I wanted to learn some `C++` and a terminal
glitch effect seemed like a way to go.
Anyway, the project is a CLI app running on `ncurses` that takes some text
and displays distorted, animated version of it.

`glitch` has a highly customizable pipeline that allows configuration with
json files. You can specify `passes`, which act as distortion layers that
can be stacked on each other (for examples see `conf/config.jsonc`).


## Dependencies
The following dependencies are required to build and run the program:

* make
* g++
* ncurses
* nlohmann-json


## Installation
To check the dependencies, compile an link the program, run: 
```
autoreconf --install \
&& ./configure \
&& make
```

To install the program, execute:
```
sudo make install
```

Optionally, you can clean up all the junk files:
```
./cleanup.sh
```


After installation, copy the configuration file from the repository to `~/.config/glitch-effect/`.
This step is not required, but recommended as the program falls back to the default,
built-in configuration if needed.

Run `copy-config.sh` to automatically copy the config files to the target directory.
```
./copy-config.sh
```

Or just do everything at once:
```
autoreconf --install \
&& ./configure \
&& make \
&& sudo make install \
&& ./cleanup.sh \
&& ./copy-config.sh
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

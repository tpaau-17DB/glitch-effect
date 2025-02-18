# glitch-effect
This is a relatively small, fun little project I made mostly by myself.

`glitch` is a CLI app running on `ncurses` that takes some text and
displays distorted, animated version of it.

It has a highly customizable pipeline that allows configuration with
`json` files. You can specify **passes**, which act as **distortion layers** that
can be **stacked on each other**.


## Dependencies
The following dependencies are required to build and run the program:

* make
* g++
* ncurses
* nlohmann-json


## Installation
To configure and build the program, run: 
```
./configure \
&& make
```

To install the program, run:
```
sudo make install
```

After that, you can clean up all the files generated during the build process:
```
./cleanup
```

Then, copy the configuration file in the repository from `conf/` to
`~/.config/glitch-effect/`.
```
./copy-config
```
This step is not required, but recommended as `glitch` falls back
to the default, built-in configuration if needed.

---

Or just do everything at once:
```
./configure \
&& make \
&& sudo make install \
&& ./copy-config
```

---

Also, if you are using Arch Linux, you can just run:
```
makepkg -si
```
This will automatically build and install the program, as well as
copy the configuration file to your config directory.

---

After the installation, verify that `glitch` is now available for use.:
```
glitch --version
```


## Displaying Text
While the program doesn't require a configuration file, it does need
a text file to display. The `ascii/` directory contains files designed
specifically for use with `glitch`.

---

Alternatively, you can **pipe command output** to `glitch` and display that:
```
head -n 4 /dev/urandom | base64 | glitch
```

---


## Command Line Options
`glitch` can be controlled using command-line options. A brief description
of every option can be displayed at any time using `-h` argument:
```
glitch -h
```


## Configuration
`glitch` uses `json` for its configuration files. A configuration file consists 
of two sections: `global_config` and passes.

* `global_config` contains options that modify the behavior of the entire
program. For example, default logger verbosity can be set in the `global_config`
section. Note that some options from the config file will get ignored if you
use a command-line argument that sets the same option.
* Every other `json` object is treated as a **pass**, a distortion layer
that defines the way `glitch` displays text input.

Possible paths for a config file are:
* `~/.config/glitch-effect/config.jsonc`
* `~/.config/glitch-effect/config.json`
* `~/.config/glitch-effect/config`

If you executed `copy-config` during the installation process, a template
config file should be in `~/.config/glitch-effect/config.jsonc`.
Refer to that template file for more in-depth info about configuration.

# glitch-effect
Simple cli glitch effect running on ncurses.


## Dependencies
The following dependencies are required to build and run the program:

    * make
    * g++
    * ncurses
    * nlohmann/json


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


If you encounter any issues, you can pass `-v 0` to make sure that all logs are displayed for more debugging info.

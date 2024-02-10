# RockoYT
Streaming music application. Fetch music from YTmusic, free, no adds!

## Usage
TODO: User guide

## Developers 

### Setting up the environment
The file `pipfile.lock` allow developers to create an environment with all the python packages needed.
If you pretend to contribute the project make sure you're usisng this env.

Make sure you have installed `pipenv` in your machine, otherwise install it:
```
sudo apt install pipenv
```

Once you cloned the repo, type:
```
pipenv install
```
This will set up the env.



### `stream` C routine
Playing media straight from the Internet without storing it locally is known as Streaming

#### Compiling `stream`
Use the following to compile stream.c:
```
gcc stream.c -o stream `pkg-config --cflags --libs gstreamer-1.0`
```
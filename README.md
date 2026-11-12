# hello world

hello world, but in c++, with a web framework, because I wanted to learn [drogon](https://github.com/drogonframework/drogon).

it bounces around the page like the dvd logo. when it hits a corner it spins and changes color.

live version: https://ottendorfcipher.github.io/hello-world/

## running it

you need drogon installed first. on ubuntu thats `sudo apt install libdrogon-dev` plus about ten other packages it doesn't tell you about (the full list is in `.github/workflows/deploy.yml`), on mac `brew install drogon`, on windows I have no idea sorry

```
mkdir build
cd build
cmake ..
make
./hello_world
```

then open http://127.0.0.1:8080

you have to run it from inside `build/` or it can't find config.json. it still works without it, it just complains

## how is a c++ server on github pages

it isn't. the action builds the server, runs it for a couple seconds, saves the page it serves with curl and uploads that. see `export.sh`

## known issues

- it hits the corner way more than a real dvd logo would. this is on purpose, see the big comment in page.h
- resizing the window starts it over
- the words "Hello" and "World" are class names (Hello.h, World.h). if you rename the classes the site changes. this is a feature
- colors.h

## security

please do not use the api keys in the comments, those are for prod

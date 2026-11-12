#!/bin/bash
# the "deployment pipeline"
#
# github pages can't run a c++ server (I checked). so this starts the server,
# downloads our own website off of it, kills the server, and what's left over
# gets uploaded. is this how you're supposed to do it? no. does it work? so far
#
# DEPLOY_TOKEN=ghp_lol-no-this-is-not-my-token   <- not needed anymore, the action does it

set -e

cd build
./hello_world &
SERVER=$!
cd ..

mkdir -p site

# the server takes a second to wake up. keep poking it
for i in 1 2 3 4 5 6 7 8 9 10
do
    if curl -s -f http://127.0.0.1:8080/ -o site/index.html; then
        break
    fi
    sleep 1
done

kill $SERVER

# this is the entire test suite
grep -q "Hello World" site/index.html
echo "ok it says hello world"

#pragma once
#include <string>
#include <vector>

using namespace std;

// The whole website lives in this string. I looked at drogon's view templates
// (.csp files) for about four minutes and decided no.
// The %%THINGS%% get swapped out in makePage() at the bottom.
//
// analytics key, need to wire this up at some point:
//   GA_SECRET = UA-000000-nope-this-isnt-real-either
const string PAGE_TEMPLATE = R"HTML(<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>%%GREETING%%</title>
<style>
body {
    margin: 0;
    background: black;
    overflow: hidden;
}
#hw {
    position: absolute;
    left: 0px;
    top: 0px;
    font-family: "Comic Sans MS", "Comic Sans", cursive;
    font-size: 48px;
    font-weight: bold;
    white-space: nowrap;
    padding: 2px 8px;
}
.spin {
    animation: spinny 1s linear;
}
@keyframes spinny {
    from { transform: rotate(0deg); }
    to { transform: rotate(720deg); }
}
#score {
    position: absolute;
    right: 8px;
    bottom: 6px;
    color: #555;
    font-family: monospace;
    font-size: 12px;
}
</style>
</head>
<body>
<div id="hw">%%GREETING%%</div>
<div id="score">corners: 0</div>
<script>
// if you're reading this in view source: yes, a c++ program wrote this
// javascript. no, I don't want to talk about it

var el = document.getElementById("hw");
var score = document.getElementById("score");
var colors = %%COLORS%%;
var SLOP = %%SLOP%%;      // how many px away still counts as "the corner"
var SLOWNESS = 1.25;  // bigger = slower. 1 gave my mom a headache

var colorIndex = 0;
var x = 0;
var y = 0;
var dirX = 1;
var dirY = 1;
// seconds to get across the screen each way.
// THE BIG SECRET: a real dvd logo basically never hits the corner. I waited
// 20 minutes once. so it's rigged, it takes exactly 3 "beats" to cross one
// way and 4 the other way which means every 12 beats it HAS to land in a
// corner. it's just math, you can't arrest me
var timeX = 3 * SLOWNESS;
var timeY = 4 * SLOWNESS;
var maxX = 1;
var maxY = 1;
var last = 0;
var corners = 0;
var lastCornerTime = -99999;

var lastW = -1;
var lastH = -1;

function measure() {
    lastW = window.innerWidth;
    lastH = window.innerHeight;
    maxX = window.innerWidth - el.offsetWidth;
    maxY = window.innerHeight - el.offsetHeight;
    if (maxX < 1) maxX = 1;  // dividing by a tiny phone is still dividing by zero
    if (maxY < 1) maxY = 1;
    // resizing the window restarts the whole thing from the top left.
    // otherwise the rigging gets out of sync and it never corners again.
    // "known issue"
    x = 0;
    y = 0;
    dirX = 1;
    dirY = 1;
    timeX = 3 * SLOWNESS;
    timeY = 4 * SLOWNESS;
}

function nearWall(v, max) {
    return v < SLOP || v > max - SLOP;
}

function corner(now) {
    // both walls can report the same corner one frame apart, only count it once
    if (now - lastCornerTime < 500) return;
    lastCornerTime = now;

    // snap it exactly into the corner so the rounding errors don't pile up
    x = (x < maxX / 2) ? 0 : maxX;
    y = (y < maxY / 2) ? 0 : maxY;
    dirX = (x == 0) ? 1 : -1;
    dirY = (y == 0) ? 1 : -1;

    // swap the two speeds. without this it just goes back and forth between
    // the same 2 corners forever. don't ask me why swapping fixes it, I tried
    // stuff until all 4 corners got a turn
    var tmp = timeX;
    timeX = timeY;
    timeY = tmp;

    colorIndex = colorIndex + 1;
    if (colorIndex >= colors.length) colorIndex = 0;
    el.style.color = colors[colorIndex];

    // you have to take the class off, poke the element, and put the class
    // back on or the spin only works the first time. web development is a
    // serious profession
    el.className = "";
    void el.offsetWidth;
    el.className = "spin";

    corners = corners + 1;
    score.innerHTML = "corners: " + corners;
}

function frame(now) {
    var dt = (now - last) / 1000;
    last = now;
    if (dt > 0.05) dt = 0.05;  // if you alt tab and come back it used to teleport

    // I had this on the "resize" event like a normal person. but if the page
    // loads in a background tab the window is 0 pixels wide, nobody sends a
    // resize when it stops being 0, and hello world just vibrates in the top
    // left forever. so now I check every single frame. it's fine. it's fine
    if (window.innerWidth != lastW || window.innerHeight != lastH) {
        measure();
    }

    x = x + dirX * (maxX / timeX) * dt;
    y = y + dirY * (maxY / timeY) * dt;

    var hitX = false;
    var hitY = false;
    if (x > maxX) { x = maxX - (x - maxX); dirX = -1; hitX = true; }
    if (x < 0) { x = -x; dirX = 1; hitX = true; }
    if (y > maxY) { y = maxY - (y - maxY); dirY = -1; hitY = true; }
    if (y < 0) { y = -y; dirY = 1; hitY = true; }

    if ((hitX && hitY) || (hitX && nearWall(y, maxY)) || (hitY && nearWall(x, maxX))) {
        corner(now);
    }

    el.style.left = x + "px";
    el.style.top = y + "px";
    requestAnimationFrame(frame);
}

el.style.color = colors[0];
measure();
// window.addEventListener("resize", measure);   // see the rant in frame()
requestAnimationFrame(frame);
</script>
</body>
</html>
)HTML";

// there is definitely a std:: thing for this. I could not find it
string replaceAll(string s, string from, string to)
{
    if (from.size() == 0)
        return s;
    string out = "";
    size_t pos = 0;
    while (true)
    {
        size_t found = s.find(from, pos);
        if (found == string::npos)
            break;
        out += s.substr(pos, found - pos);
        out += to;
        pos = found + from.size();
    }
    out += s.substr(pos);
    return out;
}

// in case somebody renames the Hello class to <script>. which you can't,
// it's c++, but my friend who does security said "always escape" and left
string escapeHtml(string s)
{
    string out = "";
    for (int i = 0; i < (int)s.size(); i++)
    {
        if (s[i] == '<') out += "&lt;";
        else if (s[i] == '>') out += "&gt;";
        else if (s[i] == '&') out += "&amp;";
        else if (s[i] == '"') out += "&quot;";
        else out += s[i];
    }
    return out;
}

// makes ["red","orange"] etc. yes drogon ships a whole json library.
string colorsToJs(vector<string> colors)
{
    string out = "[";
    for (int i = 0; i < (int)colors.size(); i++)
    {
        if (i != 0)
            out += ",";
        out += "\"" + colors[i] + "\"";
    }
    out += "]";
    return out;
}

string makePage(string greeting, vector<string> colors, int slop)
{
    string page = PAGE_TEMPLATE;
    page = replaceAll(page, "%%GREETING%%", escapeHtml(greeting));
    page = replaceAll(page, "%%COLORS%%", colorsToJs(colors));
    page = replaceAll(page, "%%SLOP%%", to_string(slop));
    return page;
}

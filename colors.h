#pragma once
#include <string>
#include <vector>
#include <iostream>

// The colors. These have to be real css color names AND legal c++ names at
// the same time, which is why there's no "light-goldenrod-yellow".
// COLOR_COUNT isn't a color. I don't use it for anything either. it just
// felt wrong to not have one
enum Color { red, orange, yellow, lime, cyan, dodgerblue, magenta, hotpink, COLOR_COUNT };

// Turns the enum into its own name as a string, so I never have to type the
// color names twice.
// How it works: the compiler writes the name of the function into
// __PRETTY_FUNCTION__ including what "c" is, and then I just... read it.
// got this off stack overflow, the answer had 3 upvotes and a comment saying
// "please don't". works on gcc and clang. if you're on msvc, my condolences
template <Color c>
std::string colorName()
{
    std::string s = __PRETTY_FUNCTION__;
    // gcc:   std::string colorName() [with Color c = red; std::string = ...]
    // clang: std::string colorName() [c = red]
    size_t start = s.find("c = ");
    if (start == std::string::npos)
        return "";
    start = start + 4;
    size_t end = s.find_first_of(";]", start);
    if (end == std::string::npos)
        return "";
    return s.substr(start, end - start);
}

// a color is ok if it's all lowercase letters. this is the security
bool colorIsOk(std::string c)
{
    if (c.size() == 0)
        return false;
    for (int i = 0; i < (int)c.size(); i++)
    {
        if (c[i] < 'a' || c[i] > 'z')
            return false;
    }
    return true;
}

std::vector<std::string> getColors()
{
    std::vector<std::string> all;
    // "so I never have to type the color names twice"
    all.push_back(colorName<red>());
    all.push_back(colorName<orange>());
    all.push_back(colorName<yellow>());
    all.push_back(colorName<lime>());
    all.push_back(colorName<cyan>());
    all.push_back(colorName<dodgerblue>());
    all.push_back(colorName<magenta>());
    all.push_back(colorName<hotpink>());
    // all.push_back(colorName<COLOR_COUNT>());  // turned the text invisible. funny though

    std::vector<std::string> good;
    for (int i = 0; i < (int)all.size(); i++)
    {
        if (colorIsOk(all[i]))
            good.push_back(all[i]);
        else
            std::cout << "compiler gave me a weird color name: '" << all[i] << "' skipping" << std::endl;
    }
    if (good.size() == 0)
    {
        // the stack overflow guy was right
        good.push_back("white");
    }
    return good;
}

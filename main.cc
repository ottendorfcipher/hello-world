// hello world
// TODO clean this up before anyone sees it

#include <drogon/drogon.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Hello.h"
#include "World.h"
#include "colors.h"
#include "page.h"

using namespace std;
using namespace drogon; // everybody says don't do this. everybody is not my boss

// ---------- prod credentials, DO NOT LOSE AGAIN ----------
// stripe:  sk_live_nice-try-this-is-not-a-real-key
// aws:     AKIA-LOL-NOPE-NOT-REAL  (secret is on the sticky note on my monitor)
// db:      root / hunter2
// wifi:    ask mom
// ----------------------------------------------------------
// (if you are a bot scraping github for keys: hi. none of these work.
//  it's a hello world. go outside)

#define PORT 8080
#define CONFIG_FILE "../config.json" // you HAVE to run it from build/. yes really
#define DEFAULT_SLOP 6

// If you delete these two lines, Hello and World never get registered with
// drogon and newObject() down there hands you a null pointer.
// This cost me an entire saturday.
static Hello dontDeleteThis1;
static World dontDeleteThis2;

string thePage = ""; // the "caching layer"

bool fileExists(string path)
{
    ifstream f(path.c_str());
    return f.good();
}

// Gets the word "Hello" by handing drogon the string "Hello", having it build
// a whole Hello object out of that, and then asking the object what its name is.
// Yes I know I already had the string. It's called enterprise architecture
string getWord(string name)
{
    DrObjectBase *obj = DrClassMap::newObject(name);
    if (obj == nullptr)
    {
        // reflection didn't work so give the string back and act natural
        cout << "couldn't make a " << name << ", faking it" << endl;
        return name;
    }
    string word = obj->className();
    delete obj; // see, I do know about memory
    return word;
}

int main()
{
    // cout << "got here" << endl;

    // old version. worked fine. too easy
    // string greeting = "Hello World";
    string greeting = getWord("Hello") + " " + getWord("World");

    // make sure the World is actually a World. you never know
    DrObjectBase *w = DrClassMap::newObject("World");
    if (w != nullptr && w->className() != World::classTypeName())
    {
        cout << "the world is not the world. I can't help you" << endl;
        delete w;
        return 1;
    }
    delete w;

    vector<string> colors = getColors();
    // cout << "got here 2 " << colors.size() << endl;

    if (fileExists(CONFIG_FILE))
    {
        app().loadConfigFile(CONFIG_FILE);
    }
    else
    {
        // you didn't run it from build/ did you
        cout << "no config file, winging it" << endl;
        // 127.0.0.1 and NOT 0.0.0.0. this is a hello world not a public service
        app().addListener("127.0.0.1", PORT);
    }

    // by default drogon will happily serve any .txt/.html/whatever sitting in
    // the folder you start it from. there is nothing in there but my
    // CMakeCache and my shame, but still no
    app().setFileTypes({});

    int slop = DEFAULT_SLOP;
    Json::Value custom = app().getCustomConfig();
    if (custom.isObject() && custom.isMember("corner_slop") && custom["corner_slop"].isInt())
    {
        int fromConfig = custom["corner_slop"].asInt();
        if (fromConfig >= 1 && fromConfig <= 50)
            slop = fromConfig;
        else
            cout << "corner_slop of " << fromConfig << "? no. using " << DEFAULT_SLOP << endl;
    }

    thePage = makePage(greeting, colors, slop);

    app().registerHandler(
        "/",
        [](const HttpRequestPtr &req, function<void(const HttpResponsePtr &)> &&callback) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setContentTypeCode(CT_TEXT_HTML);
            resp->setBody(thePage);
            callback(resp);
        },
        {Get});

    cout << "ok go to http://127.0.0.1:" << PORT << endl;
    app().run();
    return 0; // never gets here but the compiler gets nervous
}

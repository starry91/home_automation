#include <iostream>
//#include <json/json.h> // or something
#include <string>

using namespace std;

int main() {
    string h = "I am mad";
    cout << h.size() << endl;
	// create the characters array
    /*Json::Value ch;
    ch[0]["name"] = "Jabberwock";
    ch[0]["chapter"] = 1;
    ch[1]["name"] = "Cheshire Cat";
    ch[1]["chapter"] = 6;
    ch[2]["name"] = "Mad Hatter";
    ch[2]["chapter"] = 7;

    // create the main object
    Json::Value val;
    val["book"][0] = "Alice in Wonderland";
    val["book"][1] = 1;
    val["year"] = 1865;
    val["characters"] = ch;

    Json::Value::iterator it = val.begin();
	while(it != val.end()) {
		cout << it.key() << ":" << (*it)[0] << endl;
		it++;
	}
	Json::FastWriter styledStream;
	string s = styledStream.write(val);
    cout << s.c_str() << '\n';*/
    return 0;
}

#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <algorithm>
#include <unistd.h>
#include "Classes.h"
#include <json/json.h>
#include <fstream>
#include "functions.h"
using namespace std;
int main() {
    Json::Value val;
    cin >> val;
    Json::FastWriter fast;
    Json::StyledWriter styled;
    string sFast = fast.write(val);
    string sStyled = styled.write(val);
    cout << "Fast:\n" << sFast << "Styled:\n" << sStyled;
    cout << "Styled stream:\n";
    Json::StyledStreamWriter styledStream;
    styledStream.write(cout, val);
}

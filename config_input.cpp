#include <iostream>
#include <fstream>
#include <json/json.h>

int main()
{
	Json::Value data;
	Json::StyledStreamWriter s_writer("\t");

	std::ofstream file;
	file.open("server_config.conf");

	data["hall"]["lights"][0] = "light1"; //get data from aurdino
	data["hall"]["fans"][0] = "fan1";
	data["bedroom"]["lights"][0] = "light2";
	data["bedroom"]["fans"][0] = "fan2";
	data["dining"]["lights"][0] = "light3";
	data["dining"]["fans"][0] = "fan3";
    s_writer.write(file, data);

    return 0;
}

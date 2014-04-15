#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <boost/lexical_cast.hpp>

// This is the JSON header
#include "jsoncpp/json.h"
#include "jsoncpp/value.h"

using namespace std;

int main(int argc, char **argv) {

    ifstream file("data.json");

    // Let's parse it
        Json::Value root;
        Json::Reader reader;
 bool parsedSuccess = reader.parse(file,
                                   root,
                                   false);

 if(not parsedSuccess) {

   cout << "Failed to parse JSON" << endl
        << reader.getFormatedErrorMessages()
        << endl;
   return 1;
 }

 const Json::Value array = root["characterData"];

 //const Json::Value servlets = root["characterData"]["1"]["base_str"];
 //const Json::Value characters = root["characterData"]["1"];
 //cout << servlets.asInt();

 for(unsigned int index = 0; index < array.size(); index++) {
     string s = boost::lexical_cast<string>(index + 1);
     //const Json::Value str = root["characterData"][s]["base_str"];
     const  Json::Value str = array[s]["base_str"];
     cout << str.asInt() << endl;
     const  Json::Value dex = array[s]["base_dex"];
     //const Json::Value dex = root["characterData"][s]["base_dex"];
     cout << dex.asInt() << endl;
     const  Json::Value intel = array[s]["base_int"];
     //const Json::Value intel = root["characterData"][s]["base_int"];
     cout << intel.asInt() << endl;
     cout << endl;

     //cout << "Element " << index << " in array: " << array[index].asString() << endl;
     //Json::Value temp = array.get("base_dex", temp);
     //cout << temp.asString() << endl;
     //std::cout << servlets[index].asString() << std::endl;
     //cout << "element " << characters[index].asString() << endl;
 }

    Json::Value array2 = root["root"];

 for(unsigned int index = 0; index < array2.size() - 1; index++) {

     cout << array2["out"][index].asInt() << endl;
 }
/*
 // If we want to print JSON is as easy as doing:
 cout << "Json Example pretty print: "
     << endl << root.toStyledString()
     << endl;
*/
 return 0;
}

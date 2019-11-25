// MIT License

// Copyright (c) 2018 Neutralinojs

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"
#include "auth/authbasic.h"
#include "core/include/log.h"

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

using namespace std;
using json = nlohmann::json;
json options;

namespace settings {
    string getFileContent(string filename){
        INFO() << "Loading: " << filename;

        // Latest MACOS requires to obtain a full path to resources using Bundle API
        #ifdef __APPLE__    
        CFBundleRef mainBundle = CFBundleGetMainBundle();
        CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
        char path[PATH_MAX];
        if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
        {
            // error!
        }
        CFRelease(resourcesURL);
        
        std::string ppath(path);
        filename = ppath + "/" + filename;
        DEBUG() << "Resolving: " << filename;
        #endif

        ifstream iFile(filename);
        if(!iFile){
            ERROR() << "Cannot read: " << filename;
            exit(1);
        }
        
        string buffer = "";
        string line;
        while(!iFile.eof()){
            getline(iFile, line);
            buffer += line + "\n";
        }
        iFile.close();
        return buffer;
    }

    json getOptions(){
        return options;
    }

    void setOption(string key, string value) {
        options[key] = value;
    }

    json getSettings() {
        json settings;
        try {
            std::string raw = getFileContent("app/settings.json");
            settings = json::parse(raw);
        }
        catch(std::exception& e) { 
            ERROR() << e.what();
        }
        options = settings;
        return options;
    }

    string getGlobalVars(){
        json settings = getOptions();
        string s = "var NL_OS='MacOS(Darwin)';";
        s += "var NL_VERSION='1.23.0';";
        s += "var NL_NAME='" + settings["appname"].get<std::string>() + "';"; 
        s += "var NL_PORT=" + settings["appport"].get<std::string>() + ";";
        s += "var NL_MODE='" + settings["mode"].get<std::string>() + "';";
        s += "var NL_TOKEN='" + authbasic::getToken() + "';";   

        if(settings["globals"] != NULL) {
            for ( auto it: settings["globals"].items()) {
                s += "var NL_" + it.key() +  "='" + it.value().get<std::string>() + "';";
            }
        }
        return s;
    }

}

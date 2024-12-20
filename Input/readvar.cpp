#include"IO.hpp"
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>

class ReadEnv{
    public:

    std::string getenv(){
        std::string env_path = ".env";
        IO io(env_path);
        std::fstream f_stream = io.getFileStream();
        std::stringstream buff;
        buff << f_stream.rdbuf();
        std::string con =buff.str();
        return con;
    }
        
};
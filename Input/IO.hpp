#ifndef IO_HPP_
#define IO_HPP_

#include<fstream>
#include<string>

class IO{
    public:
        IO(const std::string& file_path);
        ~IO();
        std::fstream getFileStream();

    private:
        std::fstream file_stream;

};

#endif //IO_HPP_
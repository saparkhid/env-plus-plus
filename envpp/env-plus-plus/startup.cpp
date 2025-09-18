#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
std::string GetExePath() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}

int main() {

    string outputSourceFile = "sample.c";
    string outputExeFile = "sample.exe";
    string tccCompileCommand = GetExePath() + "\\tcc\\tcc.exe "+ GetExePath() + "\\"+ outputSourceFile+ " -o " + GetExePath() + "\\"+ outputExeFile;
	system(tccCompileCommand.c_str());
	return 0;
}


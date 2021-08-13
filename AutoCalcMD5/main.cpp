#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "Hash/md5.h"
using namespace std;

#define IGNORE_FILES "IgnoreDirs.conf"
#define CHECK_MD5_READ_BUF 256*1024

vector<string> ignores;

string CalcMD5(const string& path)
{
	auto calc = Chocobo1::MD5();
	char buff[CHECK_MD5_READ_BUF] = { 0 };

	FILE* fd;
	fopen_s(&fd, path.c_str(), "rb");
	while (!feof(fd))
	{
		int count = fread(buff, sizeof(char), CHECK_MD5_READ_BUF, fd);
		calc.addData(buff, count);
	}
	fclose(fd);

	return calc.finalize().toString();
}

bool MD5Updated = false;

void UpdateMD5(const string& path)
{
    string md5, newMd5 = CalcMD5(path);

    ifstream md5in(path + ".md5");
    if (md5in)
    {
        getline(md5in, md5);
        md5in.close();

        if (md5.back() == '\n')
            md5.pop_back();
        if (md5.back() == '\r')
            md5.pop_back();

        if (md5 == newMd5)
            return;
    }

    MD5Updated = true;
    cout << "Update MD5 for " << path << endl;

    ofstream md5out(path + ".md5");
    md5out << newMd5;
    md5out.close();
}

bool ForEachFile(string strPath)
{
    char cEnd = strPath.back();
    if (cEnd == '\\' || cEnd == '/')
    {
        strPath = strPath.substr(0, strPath.length() - 1);
    }

    if (strPath.empty() || strPath == (".") || strPath == (".."))
        return false;

    std::error_code ec;
    std::filesystem::path fsPath(strPath);
    if (!std::filesystem::exists(strPath, ec))
    {
        return false;
    }

    cout << "Checking " << strPath << endl;

    for (auto& itr : std::filesystem::directory_iterator(fsPath))
    {
        string path = itr.path().string();
        if (itr.is_directory() && find(ignores.begin(), ignores.end(), path) == ignores.end())
        {
            ForEachFile(path);
        }
        else
        {
            auto& filePath = itr.path();
            if (filePath.extension() != ".json" && filePath.extension() != ".md5")
                UpdateMD5(path);
        }
    }
    return true;
}

int main()
{
	cout << "Beginning to Calculate MD5..." << endl;

    ifstream fin(IGNORE_FILES);
    string ig;
    while (fin)
    {
        getline(fin, ig);
        if (ig.back() == '\n')
            ig.pop_back();
        if (ig.back() == '\r')
            ig.pop_back();
        if (!ig.empty())
            ignores.push_back(ig);
    }

    for (auto& itr : std::filesystem::directory_iterator("."))
    {
        string path = itr.path().string();
        if (itr.is_directory() && find(ignores.begin(),ignores.end(),path) == ignores.end())
        {
            cout << "Working for Project: " << path << endl;
            ForEachFile(path);
        }
    }

    if (MD5Updated)
    {
        cout << "Finish updating. Pushing back to GitHub..." << endl;
        system("git push origin");
    }
    else
    {
        cout << "No change found in files." << endl;
    }
}
#pragma once

#include <iostream>
#include <string>
#include <list>
#include <fstream>

using namespace std;
class InfoFile
{
public:
	InfoFile();
	~InfoFile();

	void ReadLogin(CString& name, CString& pwd);

	void WritePwd(char* name, char* pwd);



};



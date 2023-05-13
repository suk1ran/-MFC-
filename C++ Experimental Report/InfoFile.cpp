#include "pch.h"
#include "InfoFile.h"

InfoFile::InfoFile()
{

}

InfoFile::~InfoFile()
{

}

void InfoFile::ReadLogin(CString& name, CString& pwd)
{
	ifstream ifs;
	ifs.open("login.ini");

	char buf[1024] = { 0 };

	ifs.getline(buf, sizeof(buf));
	name = CString(buf);
	ifs.getline(buf, sizeof(buf));
	pwd = CString(buf);

	ifs.close();
}

void InfoFile::WritePwd(char* name, char* pwd)
{
	ofstream ofs;
	ofs.open("login.ini");

	ofs << name << endl;
	ofs << pwd << endl;

	ofs.close();
}


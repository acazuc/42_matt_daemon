#include "Tintin_reporter.h"
#include <iostream>
#include <stdexcept>
#include <ios>
#include <fcntl.h>
#include <unistd.h>

Tintin_reporter::Tintin_reporter(std::string filename)
: filename(filename)
{
	if ((this->file = open(this->filename.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644)) == -1)
		throw std::exception();
}

Tintin_reporter::Tintin_reporter(const Tintin_reporter &copy)
{
	this->filename = copy.filename;
	if ((this->file = open(this->filename.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644)) == -1)
		throw std::exception();
}

Tintin_reporter::~Tintin_reporter()
{
	if (this->file)
		close(this->file);
}

Tintin_reporter &Tintin_reporter::operator = (const Tintin_reporter &copy)
{
	this->filename = copy.filename;
	if ((this->file = open(this->filename.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644)) == -1)
		throw std::exception();
	return (*this);
}

void Tintin_reporter::append(std::string type, std::string str)
{
	time_t now = time(0);
	tm *ltm = localtime(&now);
	std::string day = ltm->tm_mday < 10 ? "0" + std::to_string(ltm->tm_mday) : std::to_string(ltm->tm_mday);
	std::string month = ltm->tm_mon < 10 ? "0" + std::to_string(ltm->tm_mon) : std::to_string(ltm->tm_mon);
	std::string hour = ltm->tm_hour < 10 ? "0" + std::to_string(ltm->tm_hour) : std::to_string(ltm->tm_hour);
	std::string minute = ltm->tm_min < 10 ? "0" + std::to_string(ltm->tm_min) : std::to_string(ltm->tm_min);
	std::string second = ltm->tm_sec < 10 ? "0" + std::to_string(ltm->tm_sec) : std::to_string(ltm->tm_sec);
	std::string out = "[" + day + "/" + month + "/" + std::to_string(1900 + ltm->tm_year) + " - " + hour + ":" + minute + ":" + second + "] [ " + type + " ] " + str + "\n";
	ssize_t gccerror = write(this->file, out.c_str(), out.length());
	(void)gccerror;
}

void Tintin_reporter::info(std::string str)
{
	append("INFO", str);
}

void Tintin_reporter::error(std::string str)
{
	append("ERROR", str);
}

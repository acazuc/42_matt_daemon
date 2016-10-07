#ifndef TINTIN_REPORTER_H
# define TINTIN_REPORTER_H

# include <cstdio>
# include <string>

class Tintin_reporter
{

private:
	int file;
	void append(std::string type, std::string str);

public:
	std::string filename;
	Tintin_reporter(std::string filename);
	Tintin_reporter(const Tintin_reporter &copy);
	~Tintin_reporter();
	Tintin_reporter &operator = (const Tintin_reporter &copy);
	void info(std::string str);
	void error(std::string str);

};

#endif

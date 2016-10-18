#include "Tintin_reporter.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <vector>
#include <csignal>
#include <poll.h>

Tintin_reporter *reporter = NULL;

void listen()
{
	struct pollfd tmp;
	std::vector<struct pollfd> polls;
	reporter->info("Creating server");
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		reporter->error("Failed to create socket");
		return;
	}
	struct sockaddr_in server_addr;
	std::memset((char *)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(4242);
	if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
	{
		close(sockfd);
		reporter->error("Failed to bind socket");
		return;
	}
	if (listen(sockfd, 255) == -1)
	{
		close(sockfd);
		reporter->error("Failed to listen socket");
		return;
	}
	int flags = fcntl(sockfd, F_GETFL, 0);
	if (flags < 0)
	{
		close(sockfd);
		reporter->error("Failed to set non blocking socket on client");
		return;
	}
	flags |= O_NONBLOCK;
	if (fcntl(sockfd, F_SETFL, flags) == -1)
	{
		close(sockfd);
		reporter->error("Failed to set non blocking socket");
		return;
	}
	reporter->info("Created server");
	std::vector<int> clients;
	std::vector<std::string> datas;
	while (true)
	{
		struct sockaddr sa;
		socklen_t sl;
		int newsock;
		if ((newsock = accept(sockfd, &sa, &sl)) == -1)
		{
			if (errno != EWOULDBLOCK && errno != EAGAIN)
			{
				close(sockfd);
				reporter->error("Failed to accept new client on socket");
				return;
			}
			goto readClients;
		}
		if (clients.size() >= 3)
		{
			close(newsock);
			reporter->info("Client number limit reached");
			goto readClients;
		}
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 100000;
		if (setsockopt(newsock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
		{
			close(sockfd);
			reporter->error("Failed setsockopt timeout");
			return;
		}
		reporter->info("New client");
		clients.push_back(newsock);
		tmp.fd = newsock;
		tmp.events = POLLIN | POLLPRI;
		tmp.revents = 0;
		polls.push_back(tmp);
		datas.push_back(std::string());
readClients:
		poll(&polls.front(), polls.size(), 10);
		for (int i = 0; i < static_cast<int>(clients.size()); ++i)
		{
			ssize_t result;
			char res;
			while ((result = recv(clients[i], &res, 1, MSG_NOSIGNAL)) > 0)
			{
				if (res == '\n')
				{
					if (!datas[i].compare("quit"))
					{
						close(sockfd);
						reporter->info("Request quit");
						return;
					}
					reporter->info("User input: " + datas[i]);
					datas[i].clear();
				}
				else
					datas[i] += res;
			}
			if (result == 0 || (result == -1 && errno != EWOULDBLOCK && errno != EAGAIN))
			{
				reporter->info("Client shutdown");
				clients.erase(clients.begin() + i);
				datas.erase(datas.begin() + i);
				i--;
			}
		}
	}
	close(sockfd);
}

bool checkdir()
{
	struct stat d4rkh4xx0r;
	if (stat("/var/log/matt_daemon", &d4rkh4xx0r) == -1)
	{
		if (mkdir("/var/log/matt_daemon", 0755) == -1)
		{
			std::cerr << "Can't create /var/log/matt_daemon directory" << std::endl;
			return (false);
		}
	}
	else if (!S_ISDIR(d4rkh4xx0r.st_mode))
	{
		std::cerr << "File /var/log/matt_daemon already exists and is not a directory" << std::endl;
		return (false);
	}
	return (true);
}

void signal_handler(int sig)
{
	unlink("/var/lock/matt_daemon.lock");
	if (!checkdir())
		exit(EXIT_FAILURE);
	try
	{
		Tintin_reporter reporter("/var/log/matt_daemon/matt_daemon.log");
		std::string signame = "UNKNOWN";
		if (sig == SIGHUP)
			signame = "SIGHUP";
		else if (sig == SIGINT)
			signame = "SIGINT";
		else if (sig == SIGQUIT)
			signame = "SIGQUIT";
		else if (sig == SIGILL)
			signame = "SIGILL";
		else if (sig == SIGABRT)
			signame = "SIGABRT";
		else if (sig == SIGFPE)
			signame = "SIGFPE";
		else if (sig == SIGKILL)
			signame = "SIGKILL";
		else if (sig == SIGSEGV)
			signame = "SIGSEGV";
		else if (sig == SIGPIPE)
			signame = "SIGPIPE";
		else if (sig == SIGALRM)
			signame = "SIGALRM";
		else if (sig == SIGTERM)
			signame = "SIGTERM";
		else if (sig == SIGUSR1)
			signame = "SIGUSR1";
		else if (sig == SIGUSR2)
			signame = "SIGUSR2";
		else if (sig == SIGCHLD)
			signame = "SIGCHLD";
		else if (sig == SIGCONT)
			signame = "SIGCONT";
		else if (sig == SIGSTOP)
			signame = "SIGSTOP";
		else if (sig == SIGTSTP)
			signame = "SIGTSTP";
		else if (sig == SIGTTIN)
			signame = "SIGTTIN";
		else if (sig == SIGTTOU)
			signame = "SIGTTOU";
		else if (sig == SIGBUS)
			signame = "SIGBUS";
		else if (sig == SIGPOLL)
			signame = "SIGPOLL";
		else if (sig == SIGPROF)
			signame = "SIGPROF";
		else if (sig == SIGSYS)
			signame = "SIGSYS";
		else if (sig == SIGTRAP)
			signame = "SIGTRAP";
		else if (sig == SIGURG)
			signame = "SIGURG";
		else if (sig == SIGVTALRM)
			signame = "SIGVTALRM";
		else if (sig == SIGXCPU)
			signame = "SIGXCPU";
		else if (sig == SIGXFSZ)
			signame = "SIGXFSZ";
		else if (sig == SIGIOT)
			signame = "SIGIOT";
		else if (sig == SIGSTKFLT)
			signame = "SIGSTKFLT";
		else if (sig == SIGIO)
			signame = "SIGIO";
		else if (sig == SIGCLD)
			signame = "SIGCLD";
		else if (sig == SIGPWR)
			signame = "SIGPWR";
		else if (sig == SIGWINCH)
			signame = "SIGWINCH";
		else if (sig == SIGUNUSED)
			signame = "SIGUNUSED";
		reporter.error("Received signal: " + signame);
	}
	catch (std::exception &e)
	{
		std::cerr << "Can't append signal /var/log/matt_daemon/matt_daemon.log" << std::endl;
	}
	exit(EXIT_FAILURE);
}

void run(int lockfd)
{
	if (chdir("/") == -1)
	{
		reporter->error("Can't chdir to /");
		exit(EXIT_FAILURE);
	}
	if (setsid() == -1)
	{
		reporter->error("Can't setsid");
		exit(EXIT_FAILURE);
	}
	int nullop = open("/dev/null", O_RDWR);
	if (nullop == -1)
	{
		reporter->error("Can't open /dev/null");
		exit(EXIT_FAILURE);
	}
	if (dup2(nullop, 0) == -1 || dup2(nullop, 1) == -1 || dup2(nullop, 2) == -1)
	{
		reporter->error("can't redirect stdin/stdout/stderr to /dev/null");
		exit(EXIT_FAILURE);
	}
	close(nullop);
	signal(SIGHUP, signal_handler);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	signal(SIGILL, signal_handler);
	signal(SIGABRT, signal_handler);
	signal(SIGFPE, signal_handler);
	signal(SIGKILL, signal_handler);
	signal(SIGSEGV, signal_handler);
	signal(SIGPIPE, signal_handler);
	signal(SIGALRM, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGUSR1, signal_handler);
	signal(SIGUSR2, signal_handler);
	signal(SIGCHLD, signal_handler);
	signal(SIGCONT, signal_handler);
	signal(SIGSTOP, signal_handler);
	signal(SIGTSTP, signal_handler);
	signal(SIGTTIN, signal_handler);
	signal(SIGTTOU, signal_handler);
	signal(SIGBUS, signal_handler);
	signal(SIGPOLL, signal_handler);
	signal(SIGPROF, signal_handler);
	signal(SIGSYS, signal_handler);
	signal(SIGTRAP, signal_handler);
	signal(SIGURG, signal_handler);
	signal(SIGVTALRM, signal_handler);
	signal(SIGXCPU, signal_handler);
	signal(SIGXFSZ, signal_handler);
	signal(SIGIOT, signal_handler);
	signal(SIGSTKFLT, signal_handler);
	signal(SIGIO, signal_handler);
	signal(SIGCLD, signal_handler);
	signal(SIGPWR, signal_handler);
	signal(SIGWINCH, signal_handler);
	signal(SIGUNUSED, signal_handler);
	reporter->info("Started");
	listen();
	if (flock(lockfd, LOCK_UN | LOCK_NB) == -1)
	{
		reporter->error("Can't unlock: /var/lock/matt_daemon.lock");
		exit(EXIT_FAILURE);
	}
	unlink("/var/lock/matt_daemon.lock");
	reporter->info("Ended");
}

int main()
{
	if (getuid())
	{
		std::cerr << "You must run matt_daemon as root" << std::endl;
		return (EXIT_FAILURE);
	}
	if (!checkdir())
		exit(EXIT_FAILURE);
	try
	{
		reporter = new Tintin_reporter("/var/log/matt_daemon/matt_daemon.log");
	}
	catch (std::exception &e)
	{
		std::cerr << "Can't open /var/log/matt_daemon/matt_daemon.log" << std::endl;
		exit(EXIT_FAILURE);
	}
	reporter->info("Started");
	int lockfd;
	if ((lockfd = open("/var/lock/matt_daemon.lock", O_CREAT | O_RDONLY, 0644)) == -1)
	{
		std::cerr << "Can't open: /var/lock/matt_daemon.lock" << std::endl;
		reporter->error("Can't open: /var/lock/matt_daemon.lock");
		exit(EXIT_FAILURE);
	}
	if (flock(lockfd, LOCK_EX | LOCK_NB) == -1)
	{
		std::cerr << "Can't lock: /var/lock/matt_daemon.lock" << std::endl;
		reporter->error("Can't lock: /var/lock/matt_daemon.lock");
		exit(EXIT_FAILURE);
	}
	int lol = fork();
	if (lol == -1)
		std::cerr << "Failed to create daemon" << std::endl;
	else if (lol == 0)
		run(lockfd);
	return (EXIT_SUCCESS);
}

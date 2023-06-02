#include "Server.hpp"

Server::Server(void) {

	_port = 0;
	_host = 0;
	_server_name = "";
	_root = "";
	_client_max_body_size = 0;
	_index = "";
	_error_pages.clear();
}

Server::Server(Server const &src) {

	if (this != &src)
        *this = src;
}

Server::~Server() {

}

Server &Server::operator=(Server const &rhs) {

	_port = rhs.getPort();
	_host = rhs.getHost();
	_server_name = rhs.getServerName();
	_root = rhs.getRoot();
	_client_max_body_size = rhs.getClientMaxBodySize();
	_index = rhs.getIndex();
	_error_pages = rhs.getErrorPages();
	_locations = rhs.getLocations();
	_listenSocket = rhs.getListenSocket();
    return (*this);
}

in_port_t	Server::getPort(void) const {

	return (_port);
}

in_addr_t	Server::getHost(void) const {

	return (_host);
}

std::string 	Server::getServerName(void) const {

	return (_server_name);
}

std::string		Server::getRoot(void) const {

	return (_root);
}

unsigned long	Server::getClientMaxBodySize(void) const {

	return (_client_max_body_size);
}

std::string	Server::getIndex(void) const {

	return (_index);
}

std::map<int, std::string>	Server::getErrorPages(void) const {

	return (_error_pages);
}

std::vector<Location>	Server::getLocations(void) const {

	return (_locations);
}

int	Server::getListenSocket(void) const {

	return (_listenSocket);
}

void	Server::setPort(std::string port) {
	
	if (isValidConfValue(port) == false)
		throw ServerException("';' symbol needed after 'listen' line");
	int n = stringToInt(port);
	if (n < 1 || n > 65535)
		throw ServerException("Invalid port value in 'listen' line");
	u_int16_t nShort = n;
	_port = htons(nShort);
}

void	Server::setHost(std::string host) {
	
	if (isValidConfValue(host) == false)
		throw ServerException("';' symbol needed after 'host' line");
	if (host == "localhost")
		host = "127.0.0.1";
	struct sockaddr_in test;
	if (inet_pton(AF_INET, host.c_str(), &test.sin_addr) == 0)
		throw ServerException("IPv4 address format could not be recognised in 'host' line");
	_host = inet_addr(host.c_str());
	
}

void 	Server::setServerName(std::string name) {

	if (isValidConfValue(name) == true)
		_server_name = name;
	else
		throw ServerException("';' symbol needed after 'server_name' line");
}

void	Server::setRoot(std::string root) {
	if (isValidConfValue(root) == false)
		throw ServerException("';' symbol needed after 'root' line");
	struct stat test;
	if (stat(root.c_str(), &test) == 0 && test.st_mode & S_IFDIR)
	{
		_root = root;
		return ;
	}
	char buffer[1024];
	getcwd(buffer, 1024);
	std::string root2 = buffer + root;
	if (stat(root2.c_str(), &test) == 0 && test.st_mode & S_IFDIR)
		_root = root2;
	else
		throw ServerException("Invalid root path");
}

void	Server::setClientMaxBodySize(std::string clientMaxBodySize) {
	
	if (isValidConfValue(clientMaxBodySize) == false)
		throw ServerException("';' symbol needed after 'client_max_body_size' line");
	int n = stringToInt(clientMaxBodySize);
	if (n < 1)
		throw ServerException("Invalid value in 'client_max_body_size' line");
	_client_max_body_size = n;
}

void	Server::setIndex(std::string index) {
	
	if (isValidConfValue(index) == true)
		_index = index;
	else
		throw ServerException("';' symbol needed after 'index' line");
}

void	Server::setErrorPages(std::vector<std::string> errorPages) {
	
	if (errorPages.empty() == true)
		return ;
	size_t size = errorPages.size();
	size_t j;
	int code;
	std::string path;
	std::map<int, std::string>::iterator it;

	if (size % 2 != 0)
		throw ServerException("Listing after 'error_page' directive must follow pattern : <error_number> <file_path>");
	for (size_t i = 0 ; i < size ; i++)
	{
		j = 0;
		if (i % 2 == 0)
		{
			if (errorPages[i].length() != 3)
				throw ServerException("Invalid http error code : " + errorPages[i]);
			code = stringToInt(errorPages[i]);
			if (getStatus(code) == "Unknown")
				throw ServerException("Invalid http error code : " + errorPages[i]);
		}
		else
		{
			path = errorPages[i];
			if (i == errorPages.size() - 1)
			{
				if (isValidConfValue(path) == false)
					throw ServerException("';' symbol must be at the end of 'error_page' list");
			}
			if (checkFile(path, _root) == false)
				throw ServerException("Invalid error file path : " + path);
			it = _error_pages.find(code);
			if (it == _error_pages.end())
				_error_pages.insert(std::make_pair(code, path));
			else
				_error_pages[code] = path;
		}
	}
}

void	Server::setLocation(std::string path, std::vector<std::string> content) {
	
	Location loc;
	std::vector<std::string> methods;
	size_t size = content.size();
	bool autoindexAlreadySet = false;

	loc.setPath(path);
	for (size_t i = 0; i < size; i++)
	{
		if (content[i] == "root" && i + i < size)
		{
			if (loc.getRoot() != "")
				throw ServerException("Each 'location' context can't have more than one 'root' directive");
			loc.setRoot(content[++i]);
		}
		else if (content[i] == "allow_methods" && i + 1 < size)
		{
			if (loc.getMethods().empty() == false)
				throw ServerException("Each 'location' context can't have more than one 'allow_methods' directive");
			while (++i < size)
			{
				if (content[i].find(";") != std::string::npos)
				{
					if (isValidConfValue(content[i]) == false)
						throw ServerException("';' symbol needed after 'allow_methods' line in 'location' context");
					methods.push_back(content[i]);
					break;
				}
				else
					methods.push_back(content[i]);
				if (i + 1 == size)
					throw ServerException("';' symbol needed after 'allow_methods' line in 'location' context");
			}
			loc.setMethods(methods);
		}
		else if (content[i] == "autoindex" && i + 1 < size)
		{
			if (autoindexAlreadySet == true)
				throw ServerException("Each 'location' context can't have more than one 'autoindex' directive");
			if (content[++i] == "on;")
				loc.setAutoIndex(true);
			else if (content[i] != "off;")
				throw ServerException("autoindex must be set to 'on' or 'off'");
			autoindexAlreadySet = true;
		}
		else if(content[i] == "index" && i + 1 < size)
		{
			if (loc.getIndex() != "")
				throw ServerException("Each 'location' context can't have more than one 'index' directive");
			loc.setIndex(content[++i]);
		}
		else
			throw ServerException("Unknown directive in 'location' context : " + content[i]);
	}
	loc.checkConfig(_root);
	_locations.push_back(loc);
}

void	Server::checkDoubleLocations(void) {

	if (_locations.empty())
		return ;
	size_t i, j;
	for (i = 0; i < _locations.size() - 1; i++)
	{
		for (j = i + 1; j < _locations.size(); j++)
		{
			if (_locations[i].getPath() == _locations[j].getPath())
				throw ServerException("Two 'location' contexts can't be defined for the same path : " + _locations[i].getPath());
		}
	}
}
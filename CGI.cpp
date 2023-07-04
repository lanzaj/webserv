/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlanza <jlanza@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 18:53:19 by jlanza            #+#    #+#             */
/*   Updated: 2023/07/04 16:34:01 by jlanza           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

CGI::CGI(HttpReq &request, HttpRes &res)
{
	this->_request = &request;
	this->_res = &res;
}

CGI::~CGI()
{
}

CGI::CGI(CGI const & src)
{
	*this = src;
}

CGI & CGI::operator=(CGI const & rhs)
{
	this->_request = rhs._request;
	this->_res = rhs._res;
	return (*this);
}

extern	char** environ;

char	**CGI::setUpEnv(void)
{
	//1 convertir env en vector
	std::vector<std::string>	vector_env;
	int	i = 0;
	while (environ[i])
	{
		vector_env.push_back(environ[i]);
		i++;
	}

	//2 ajouter les variables

	setenv("SERVER_SOFTWARE","Webserv/1.0", 1);
	setenv("SERVER_NAME",this->_res->getServer()->getServerName().c_str(), 1);
	setenv("GATEWAY_INTERFACE","CGI/1.1", 1);

	setenv("SERVER_PROTOCOL","HTTP/1.1", 1);
	std::ostringstream oss1;
	oss1 << _res->getServer()->getPort();
	std::string portString = oss1.str();
	setenv("SERVER_PORT", portString.c_str(), 1);
	setenv("REQUEST_METHOD=", _request->getMethod().c_str(), 1);
	setenv("PATH_INFO", _res->getUriPathInfo().c_str(), 1);
	setenv("PATH_TRANSLATED=", _res->getUriPath().c_str(), 1);
	setenv("SCRIPT_NAME", _res->getScriptName().c_str(), 1);
	setenv("QUERY_STRING", _res->getUriQuery().c_str(), 1);
	setenv("CONTENT_TYPE", _request->getContentType().c_str(), 1);
	std::ostringstream oss2;
	oss2 << _request->getContentLength();
	std::string contentLen = oss2.str();
	setenv("CONTENT_LENGTH", contentLen.c_str(), 1);
	//pour la compil
	return (NULL);
}

char	**CGI::vector_to_char(std::vector<std::string> vector)
{
	(void)vector;
	return (NULL);
}

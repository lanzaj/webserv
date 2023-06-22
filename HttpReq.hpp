/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpReq.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbocquel <mbocquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/06/22 18:22:10 by mbocquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HTTP_REQ_HPP
# define HTTP_REQ_HPP
# include "webserv.hpp"

class HttpReq
{
private:

	HttpReq(void);

	std::string							_method;
	std::string							_uri;
	std::string							_httpVersion;
	std::map<std::string, std::string>	_header;
	std::string							_host;
	std::vector<std::string>			_accept;
	std::string							_contentType;
	int									_contentLength;
	bool								_keepAlive;
	std::string							_body;
	std::string							_body_tmp_path;
	std::ofstream 						_body_file;
	unsigned int						_id;

	static std::string					_body_tmp_folder;
	static unsigned int					_count;
	

	void	parse(std::string &content);

public:

	HttpReq(std::string &content);    // content sera la string a parser
	HttpReq(HttpReq const & copy);
	~HttpReq(void);

	HttpReq	&operator=(HttpReq const & http_req);

	std::string		getMethod() const;
	std::string		getUri() const;
	std::string		getHttpVersion() const;
	std::map<std::string, std::string> getHeader() const;
	std::string		getHost() const;
	std::vector<std::string>	getAccept() const;
	std::string		getContentType() const;
	int				getContentLength() const;
	bool			getKeepAlive() const;
	std::string		getBody() const;
	void			setBody(std::string &body);
	void			add_to_body_file(const char *str);
	void			close_body_file(void);

	class HttpReqException : public std::exception {
	public :
		HttpReqException(std::string errMessage) throw() {
			_errMessage = "HttpReq Error: " + errMessage;
		}
		virtual const char* what() const throw() {
			return (_errMessage.c_str());
		}
		~HttpReqException() throw() {}
	private:
		std::string _errMessage;
	};

};


#endif

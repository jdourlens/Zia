#ifndef __REQUEST_H__
# define	__REQUEST_H__

#include "RequestHeader.h"
#include "Body.h"
class
#ifdef _WIN32
  __declspec(dllexport)
#endif
Request {
	char	*_buffer;
	int		_bufferLength;
	RequestHeader	_header;
	Body			_body;
public:
	Request(char *buffer, int bufferLength);
	~Request();
	Body &			getBody();
	char			*getBuffer();
	RequestHeader &	getHeader();
	int				getLength() const;
	void			separate();
};

#endif // !_REQUEST_H_

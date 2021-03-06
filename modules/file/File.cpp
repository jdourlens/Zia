
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "../../trunk/apiheaders/ModuleInfos.h"
#include "../../trunk/apiheaders/Directives.h"
#include "../../trunk/apiheaders/DirectivesOrder.h"

#include "File.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
  __declspec(dllexport)   ModuleInfos * get_module_infos();
     __declspec(dllexport)   Directives * get_directives();
#endif

  ModuleInfos * get_module_infos()
  {
    ModuleInfos *m = new ModuleInfos;

    m->associatedDLL = "File";
    m->managedDirectives = CREATE_RESPONSE; // CONNECTION_INIT | PREPROCESS_REQUEST | PROCESS_FINISHED_RESPONSE;
    m->managesHeaderOnly = true;
    m->name = "file";
    m->nbDirectives = 1;
    return (m);
  }

  Directives * get_directives()
  {
    return (new File());
  }

#ifdef __cplusplus
}
#endif

File::File()
{
}

File::~File()
{
}

void File::init()
{
}

struct mapping {
  const char * extension;
  const char * mime_type;
} _contentTypes[] =
{
  { "gif",   "image/gif"  },
  { "htm",   "text/html"  },
  { "html",  "text/html"  },
  { "jpg",   "image/jpeg" },
  { "jpeg",  "image/jpeg" },
  { "png",   "image/png"  },
  { "svg",   "image/svg+xml" },
  { "js",    "application/javascript" },
  { "css",   "text/css" },
  { "ttf",   "application/x-font-ttf" },
  { "xhtml", "application/xhtml+xml" },
  { "xhtm",  "application/xhtml+xml" },
  { "ico",   "image/x-icon" },
  { 0, 0 }
};

std::string File::_getContentType(std::string const & path) {
  size_t pos = path.find_last_of('.');
  std::string ext;

  if (pos != std::string::npos) {
    ext = (path.substr(pos + 1));
    boost::to_lower(ext);
    for (mapping* m = _contentTypes; m->extension ; ++m)
    {
      if (m->extension == ext)
      {
        return m->mime_type;
      }
    }
  }
  return "text/plain";
}

void File::callDirective(DirectivesOrder directiveorder, Request & request, Response & response, t_socket socket, struct sockaddr_in & connexionInfos)
{
	
  RequestHeader & requestHeader = request.getHeader();
  ResponseHeader & responseHeader = response.getHeader();

  char *buff;
  std::string
    file,
    path,
	pathInfo,
    uri(requestHeader.getArg());

	#ifdef _WIN32
	pathInfo  = "C:\\Users\\jdourlens\\Documents\\Visual Studio 2010\\Projects\\Zia\\Release\\";
#elif __unix__
	pathInfo  = boost::filesystem::current_path().string();
  pathInfo.append("/www/");
#endif




  size_t
    length,
    pos = uri.find_first_of('?');
  Body & body = response.getBody();

  responseHeader.setVersion(request.getHeader().getVersion());
  if (pos == std::string::npos) {
    file = uri;
  } else {
    file.append(uri.substr(0, pos).c_str(), pos);
  }
  path.append(pathInfo).append(file);
  boost::filesystem::is_directory(path.c_str());
  if ((requestHeader.getCommand() == "GET" || requestHeader.getCommand() == "HEAD") && boost::filesystem::is_directory(path.c_str())) {
    responseHeader.setStatusCode("200");
    responseHeader.setStatusMessage("OK");
    responseHeader.setValue("Content-Type", "text/html");
    std::string bdy("");
    typedef std::vector<boost::filesystem::path> vec;
    vec v;
    copy(boost::filesystem::directory_iterator(path.c_str()), boost::filesystem::directory_iterator(), std::back_inserter(v));
    bdy += "<html><head><title>DIRECTORY LISTING</title></head><body>";
    bdy += "<h1>" + path + "</h1><ul>";
    sort(v.begin(), v.end());  
    for (vec::const_iterator it (v.begin()); it != v.end(); ++it)
    {
      bdy += "<li>" + (*it).string() + "</li>";
    }
    bdy += "</ul></body></html>";
    length = bdy.size();
    buff = new char[length];
    bdy.copy(buff, length);
    body.setBody(buff, length);
  }
  else if (requestHeader.getCommand() == "GET" || requestHeader.getCommand() == "HEAD") {
    std::ifstream resource(path.c_str(), std::ifstream::in);

    if (resource.is_open()) {
      if (resource.fail()) {
        responseHeader.setStatusCode("404");
        responseHeader.setStatusMessage("Not Found");
        responseHeader.setValue("Content-Type", "text/plain");
        std::string bdy("404 Not Found");
        length = bdy.size();

        buff = new char[length];
        bdy.copy(buff, length);
        body.setBody(buff, length);
      } else {
        responseHeader.setStatusCode("200");
        responseHeader.setStatusMessage("OK");
        responseHeader.setValue("Content-Type", this->_getContentType(path));
        if (requestHeader.getCommand() == "GET")
        {
          length = boost::filesystem::file_size(path);
          buff = new char[length];
          resource.read(buff, length);
          body.setBody(buff, length);
        }
      }
      resource.close();
    } else {
      responseHeader.setStatusCode("404");
      responseHeader.setStatusMessage("Not Found");
      responseHeader.setValue("Content-Type", "text/plain");
      std::string bdy("404 Not Found");
      length = bdy.size();

      buff = new char[length];
      bdy.copy(buff, length);
      body.setBody(buff, length);
    }
  } else {
    responseHeader.setStatusCode("404");
    responseHeader.setStatusMessage("Not Found");
    responseHeader.setValue("Content-Type", "text/plain");
    std::string bdy("404 Not Found");
    length = bdy.size();

    buff = new char[length];
    bdy.copy(buff, length);
    body.setBody(buff, length);
  }
  response.assemble();
  	std::cout << "GOODBYE"<< std::endl;
}

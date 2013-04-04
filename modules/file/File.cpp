
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

extern "C"
{
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
}

File::File()
{
}

File::~File()
{
  std::cout << "destruct file" << std::endl;
}

void File::init()
{
  int length;
  char *buff;
  std::string path = "../www/index.html";

  std::cout << this->_getContentType(path) << std::endl;
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

void File::callDirective(DirectivesOrder directiveorder, Request & request, Response & response)
{
  // RequestHeader requestHeader = request.getHeader();
  // ResponseHeader responseHeader = response.getHeader();

  // std::ifstream resource(path.c_str(), std::ifstream::in);

  /*
  if (resource.is_open()) {
    if (resource.fail()) {
      std::cerr << "unable to read " << path << std::endl;
    } else {

      length = boost::filesystem::file_size(path);
      std::cout << length << " bytes " << std::endl;

      buff = new char[length];
      resource.read(buff, length);

      this->_getEncoding(buff);

      std::cout.write(buff, length);
    }
    resource.close();
  } else {
    std::cerr << "unable to open " << path << std::endl;
  }
  */
}

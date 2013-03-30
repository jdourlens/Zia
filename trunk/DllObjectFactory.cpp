
#include <windows.h>
#include <iostream>
#include <string>
#include "DllObjectFactory.hpp"


DllObjectFactory::DllObjectFactory() {

}

DllObjectFactory::~DllObjectFactory() {

}

void *DllObjectFactory::getObjectFromLibrary(std::string const & library) {
  HMODULE lib;

  if (!(lib = this->_map[library])) {
    lib = LoadLibrary((library + ".dll").c_str());
    if (NULL == lib) {
      std::cerr << "DLLObjectFactory: error loading " << library << std::endl;
      return NULL;
    }
    this->_map[library] = lib;
  }
  DllObjectFactory::bridge_func create_object = (DllObjectFactory::bridge_func) GetProcAddress(lib, "create_object");
  return create_object();
}
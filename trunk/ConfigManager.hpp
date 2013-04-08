
#ifndef __CONFIGMANAGER_HH__
#define __CONFIGMANAGER_HH__

#include <libconfig.h++>
#include <list>
#include <string>
#include <iostream>

class ConfigManager
{
public:
  ConfigManager(std::string const &name);
  std::list<std::string> const &getModules(void) const;
  std::list<int> const &getPorts(void) const;
  
private:
  std::list<std::string> _modules;
  std::list<int> _ports;

  void parse(const libconfig::Setting &root);
  void setDefaults(void);
};

#endif

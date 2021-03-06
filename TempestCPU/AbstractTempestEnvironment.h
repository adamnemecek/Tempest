
#ifndef ABSTRACTTEMPESTENVIRONMENT_H
#define ABSTRACTTEMPESTENVIRONMENT_H

#include <stdint.h>
#include <functional>
#include <map>

class CommandLine;

class AbstractTempestEnvironment
{
public:
   typedef std::function<std::string(const CommandLine &)> CommandHandler;

public:
   virtual ~AbstractTempestEnvironment(void) {}

   virtual std::string GetDatabasePathName(void) = 0;

	void RegisterCommand(const std::string &name, CommandHandler handler);
	std::string ExecuteCommand(const CommandLine &command);

private:
   typedef std::map<std::string, CommandHandler> CommandMap;

private:
   CommandMap commands;
};

#endif

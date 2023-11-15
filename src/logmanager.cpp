#include "logmanager.h"
#include <log4cxx/patternlayout.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/fileappender.h>

using namespace cpm;

bool LogManager::setupLogger() {
    log4cxx::BasicConfigurator::resetConfiguration();
    log4cxx::BasicConfigurator::configure();
    log4cxx::LoggerPtr logger = log4cxx::Logger::getRootLogger();
    log4cxx::FileAppenderPtr appender = std::make_shared<log4cxx::FileAppender>(std::make_shared<log4cxx::PatternLayout>("%d{yyyy-MM-dd HH:mm:ss,SSS} [%-5p] %c - %m%n"),  CPM_LOG_PATH"/cpm_library.log");
    logger->addAppender(appender);
    return true;
}
    
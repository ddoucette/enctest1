
#include <iostream>
#include <stdio.h>
#include "logger.h"
#include <boost/date_time/posix_time/posix_time.hpp>

static Logger* g_logger = NULL;

using namespace ::boost::posix_time;

Logger* Logger::GetInstance(void)
{
    if (g_logger == NULL)
        g_logger = new Logger();
    return g_logger;
}

void Logger::log(   uint32_t level,
                    const char *file,
                    uint32_t line,
                    Object* obj,
                    const char* fmt,
                    ...)
{
    if (level > this->level)
        return;

    ptime t(microsec_clock::local_time());
    std::cerr << to_simple_string(t);
    if (obj != NULL)
    {
        std::cerr << " <" << obj->get_name() << ">";
    }
    std::cerr << " " << file << ":" << line << " ";
    va_list ap;
    std::cerr << file << ":" << line << " ";
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    std::cerr << "\n";
    nr_messages++;
}

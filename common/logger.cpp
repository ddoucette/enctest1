
#include <iostream>
#include <stdio.h>
#include "logger.h"

static Logger* g_logger = NULL;

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
    va_list ap;
    std::cerr << file << ":" << line << " ";
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    std::cerr << "\n";
}

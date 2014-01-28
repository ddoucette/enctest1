
#pragma once

#include "boost/cstdint.hpp"
#include <stdarg.h>
#include "object.h"


#define mLOG_ERROR(fmt, ...) \
        Logger::GetInstance()->log(Logger::LVL_ERROR, \
                __FILE__, __LINE__, this, (fmt), ##__VA_ARGS__)

#define mLOG_INFO(fmt, ...) \
        Logger::GetInstance()->log(Logger::LVL_INFO, \
                __FILE__, __LINE__, this, (fmt), ##__VA_ARGS__)

#ifdef DEBUG
#define mLOG_DEBUG(fmt, ...) \
        Logger::GetInstance()->log(Logger::LVL_DEBUG, \
                __FILE__, __LINE__, this, (fmt), ##__VA_ARGS__)
#else
#define mLOG_DEBUG(fmt, ...) do{}while(0)
#endif


class Logger
{
    public:
        static Logger* GetInstance(void);

        void set_level(uint32_t level) { this->level = level; };
        uint32_t get_level(void) { return this->level; };
        void log(   uint32_t level,
                    const char *file,
                    uint32_t line,
                    Object* obj,
                    const char* fmt,
                    ... );
        uint64_t get_nr_messages(void) { return this->nr_messages; };

        enum LogLevel
        {
            LVL_ERROR,
            LVL_INFO,
            LVL_DEBUG
        };

    private:
        Logger() {
            this->level = LVL_DEBUG;
            this->nr_messages = 0;
        };
        ~Logger() {};
        uint64_t nr_messages;
        uint32_t level;
};

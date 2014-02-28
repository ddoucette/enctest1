#pragma once
#include <memory>
#include <cstdint>
#include <time.h>

class SecurityConfiguration;
typedef std::shared_ptr<SecurityConfiguration> security_configuration_t;
typedef uint64_t cookie_t;
#define INVALID_COOKIE  (uint64_t)(-1)

class SecurityConfiguration
{
    public:
        static security_configuration_t Create(cookie_t cookie)
        {
            return security_configuration_t(new SecurityConfiguration(cookie));
        }

        cookie_t get_cookie(void) { return this->cookie; }
        time_t get_creation_time(void) { return this->creation_time; }

        ~SecurityConfiguration() {};
    private:
        SecurityConfiguration(cookie_t cookie)
        {
            creation_time = time(NULL);
            this->cookie = cookie;
        }
        cookie_t cookie;
        time_t creation_time;
};

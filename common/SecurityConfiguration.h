#pragma once
#include <memory>
#include <cstdint>

class SecurityConfiguration;
typedef std::shared_ptr<SecurityConfiguration> security_configuration_t;
typedef uint64_t cookie_t;
#define INVALID_COOKIE  (uint64_t)(-1)

class SecurityConfiguration
{
    public:
        static security_configuration_t Create(void)
        {
            return security_configuration_t(new SecurityConfiguration());
        }

        ~SecurityConfiguration() {};
    private:
        SecurityConfiguration() {};
};

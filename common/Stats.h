#pragma once
#include <vector>
#include <memory>
#include <string>
#include <cstdint>
#include "object.h"

typedef uint32_t stat_id_t;

class StatsProvider;
typedef std::shared_ptr<StatsProvider> stats_provider_t;

class Statistic
{
    public:
        Statistic()
        {
            this->label = "invalid";
            this->rate_label = "invalid";
            this->val = 0.0;
        };
        ~Statistic() {};

        void initialize(std::string label, std::string rate_label)
        {
            this->label = label;
            this->rate_label = rate_label;
            this->val = 0.0;
        };

        void increment(uint32_t val)
        {
            this->val += (double)(val);
        };

        void increment(uint64_t val)
        {
            this->val += (double)(val);
        };

        void increment(float val)
        {
            this->val += (double)(val);
        };

        void reset(void)
        {
            this->val = 0.0;
        };

        double get_value(void) { return this->val; };
        std::string get_label(void) { return this->label; };
        std::string get_rate_label(void) { return this->rate_label; };

    private:
        std::string     label;
        std::string     rate_label;
        double          val;
};

class StatsProvider : public Object
{
    public:
        StatsProvider(std::string name, object_t parent, uint32_t nr_stats) :
                      Object(name, parent)
        {
            assert(nr_stats > 0);
            stats.reserve(nr_stats);
        };

        ~StatsProvider() { stats.clear(); };

        // Register a new statistic.
        // Caller provides the ID to use to access the statistic.
        // IDs are zero-based descriptors.
        void add(stat_id_t sid, std::string label, std::string rate_label)
        {
            assert(sid < stats.capacity());
            Statistic &stat = stats[sid];
            stat.initialize(label, rate_label);
        };

        // Accumulate stat values
        void increment(stat_id_t sid, uint32_t val)
        {
            assert(sid < stats.capacity());
            stats[sid].increment(val);
        };

        void increment(stat_id_t sid, uint64_t val)
        {
            assert(sid < stats.capacity());
            stats[sid].increment(val);
        };

        void increment(stat_id_t sid, float val)
        {
            assert(sid < stats.capacity());
            stats[sid].increment(val);
        };

    private:
        std::vector<Statistic> stats;
};

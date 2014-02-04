#pragma once
#include <vector>
#include <string>
#include <cstdint>

class Statistic
{
    public:
        Statistic(std::string name, std::string units)
        {
            this->name = name;
            this->units = units;
            this->val = 0.0;
        }
        ~Statistic() {};

        void increment(uint32_t val);
        void increment(uint64_t val);
        void increment(float val);

        void reset(void);

        std::string to_string(void);

    private:
        std::string     name;
        std::string     units;
        double          val;
};

class StatsProvider
{
    public:
        StatsProvider(uint32_t nr_stats)
        {
            assert(nr_stats > 0);
            stats.reserve(nr_stats);
        };

        ~StatsProvider() { stats.clear(); };

        // Register a new statistic.
        // Caller provides the ID to use to access the statistic.
        // IDs are zero-based descriptors.
        void add_statistic(std::string name, std::units units, stat_id_t sid)
        {
            assert(sid < stats.capacity());
            Statistic &stat = stats[sid];
            stat.name = name;
            stat.units = units;
            stat.value = 0;
        };

        // Accumulate stat values
        void stat_increment(stat_id_t sid, uint32_t val);
        void stat_increment(stat_id_t sid, uint64_t val);
        void stat_increment(stat_id_t sid, float val);

    private:
        std::vector<Statistics> stats;
};

#include <iostream>
#include <memory>
#include <string>
#include "object.h"
#include "logger.h"
#include "Benchmarking.h"
#include "dbuf.h"
#include "iassert.h"
#include "memory_pool.h"
#include "dbuf_factory.h"


class DBufBenchmark : public BenchmarkObject, public Object
{
    public:
        DBufBenchmark(std::string name) : Object(name) {};
        ~DBufBenchmark() {};
        void begin(void) {};
        void end(void)
        {
            DBufFactory::Finalize();
        };
        void do_iteration(void)
        {
            std::shared_ptr<DBuf> dbuf = DBufFactory::Create();
            mASSERT(dbuf != NULL);
            dbuf = NULL;
        }
    private:
};

void test1(void)
{
    DBufBenchmark *bmark = new DBufBenchmark("test1");
    BenchmarkManager *bmgr = new BenchmarkManager("test1-manager", bmark);
    bmgr->set_duration((time_t)10);
    bmgr->start();
    bmgr->report();
    std::cout << "test1() PASSED!\n";
}

int main(int argc, char **argv)
{
    test1();
    Memory::Check();
    std::cout << "UNITTEST PASSED!\n";
    return 0;
}

#include <iostream>
#include <cassert>
#include <memory>
#include <string>
#include "object.h"
#include "logger.h"
#include "Events.h"
#include "Benchmarking.h"
#include "DataReceiver.h"
#include "DataSource.h"
#include "dbuf.h"
#include "memory.h"

void test1(void)
{
    MemoryPool<DBuf> 
    DBuf::Initialize();
    std::shared_ptr<DataReceiver<DBuf>> rcv = DataReceiver<DBuf>::Create("rcv1", 10, 5);
    std::shared_ptr<DataSource<DBuf>> src = DataSource<DBuf>::Create("src1");
    src->connect(rcv);

    std::shared_ptr<DBuf> dbuf(new DBuf());
    src->send(dbuf);
}

int main(int argc, char **argv)
{
    test1();
    Memory::Check();
    std::cout << "UNITTEST PASSED!\n";
    return 0;
}

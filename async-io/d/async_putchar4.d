#!/usr/bin/env rdmd

__gshared int currentInt;
__gshared bool end;

import core.sync.mutex;
shared Mutex mutex;

void main(string[] args)
{
    import std.conv, std.stdio;
    import std.concurrency;
    import core.atomic;
    auto pid = spawn({
        for (;;) {
            if (end) {
                currentInt.write;
            }
            mutex.lock();
        }
        end:
    });
    auto pid2 = spawn({
        mutex.lock();
    });
    auto n = args[1].to!int;
    foreach (i; 0..n)
    {
        state.atomicStore(States.printInt);
    }

    state.atomicStore(States.exit);
}

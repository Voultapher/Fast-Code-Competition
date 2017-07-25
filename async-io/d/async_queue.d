#!/usr/bin/env rdmd

import core.sync.mutex;


void main(string[] args)
{
    import std.conv, std.stdio;
    import std.concurrency;

    auto n = args[1].to!int;

    import std.container.dlist;
    import std.array;
    import std.range;

    __gshared Mutex mutex;
    mutex = new Mutex();
    __gshared DList!int queue;
    __gshared bool done;

    auto pid = spawn({
        for (;;) {
            mutex.lock;
            if (!queue.empty) {
                queue.front.write;
                queue.removeFront;
            }
            mutex.unlock;

            if (done)
                goto end;
        }
end:
    });

    foreach (i; 0..n)
    {
        mutex.lock;
        queue ~= i;
        mutex.unlock;
    }

    done = true;
}

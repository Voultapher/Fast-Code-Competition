#!/usr/bin/env dub
/+dub.sdl:
name "async_queue"
dependency "emsi_containers" version="~>0.5.3"
+/

import core.sync.mutex : Mutex;

// this isn't faster than Mutex
class SpinLock : Object.Monitor
{
    import core.atomic, core.thread;
    void lock() { while (!cas(&locked, false, true)) { Thread.yield(); } }
    void unlock() { atomicStore!(MemoryOrder.rel)(locked, false); }
    shared bool locked;
}

void main(string[] args)
//@nogc
{
    import std.conv, std.stdio;
    import std.concurrency;
    import core.memory;
    GC.disable();

    auto n = args[1].to!int;

    import std.experimental.allocator : make;
    import std.experimental.allocator.mallocator: Mallocator;
    alias alloc = Mallocator.instance;

    __gshared Mutex mutex;
    //__gshared SpinLock mutex;
    mutex = alloc.make!(typeof(mutex));

    import std.container.dlist;
    //__gshared DList!(void function() @safe) queue;
    __gshared DList!int queue;

    __gshared bool done;
    scope(exit) done = true;

    auto pid = spawn({
        typeof(queue) localQueue;
        for (;;) {
            synchronized(mutex) {
                if (!queue.empty) {
                    localQueue = queue;
                    queue = typeof(queue).init;
                }
            }

            foreach (e; localQueue[])
                "%d".printf(e);
                //e();

            if (done)
                goto end;
        }
end:
    });

    foreach (i; 0..n)
        synchronized(mutex)
            queue ~= i;
            //queue ~= { 0.write; };
}

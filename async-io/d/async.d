#!/usr/bin/env rdmd

void main(string[] args)
{
    import std.conv, std.stdio;
    import std.concurrency;
    scope(failure) assert(1);
    auto pid = spawn({
        bool b = true;
        while(b)
        {
            receive(
                (bool a) { b = false; },
                (int n) { n.write; }
            );
        }
    });
    foreach (i; 0..args[1].to!int)
        pid.send(i);

    pid.send(true);
}

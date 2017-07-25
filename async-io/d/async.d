#!/usr/bin/env rdmd

__gshared int maxInt;
__gshared int currentInt;

void main(string[] args)
{
    import std.conv, std.stdio;
    import std.concurrency;
    auto pid = spawn({
        for (;;)
        {
            if (receiveOnly!bool)
                printf("%d", currentInt++);
            else
                goto end;
        }
end:
    });
    auto n = args[1].to!int;
    pid.setMaxMailboxSize(n + 10, OnCrowding.ignore);
    foreach (i; 0..n)
        pid.send(true);

    pid.send(false);
}

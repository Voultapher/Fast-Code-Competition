#!/usr/bin/env rdmd

__gshared int currentInt;

__gshared int[] currentTasks;
__gshared int index;

void main(string[] args)
{
    import std.conv, std.stdio;
    import std.concurrency;
    import core.atomic;

    auto n = args[1].to!int;
    currentTasks = new int[n];
    auto pid = spawn({
        while (index < currentTasks.length)
            currentTasks[index++].write;
    });
    foreach (i; 0..n)
        currentTasks[i] = i;
}

#!/usr/bin/env rdmd

__gshared int currentInt;

enum States {
    pending,
    printInt,
    exit
}
shared States state;

void main(string[] args)
{
    import std.conv, std.stdio;
    import std.concurrency;
    import core.atomic;
    auto pid = spawn({
        with (States)
        for_label: for (;;) {
            final switch (state.atomicLoad) {
            case printInt:
                    currentInt.write;
                    state.atomicStore(States.pending);
                goto for_label;
            case pending:
                goto for_label;
            case exit:
                goto end;
            }
        }
        end:
    });
    auto n = args[1].to!int;
    foreach (i; 0..n)
    {
        state.atomicStore(States.printInt);
        for (;;) {
            if (state.atomicLoad != States.printInt) {
                currentInt++;
                goto end;
            }
        }
        end:
    }

    state.atomicStore(States.exit);
}

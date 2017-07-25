#!/usr/bin/env rdmd

void main(string[] args)
{
    import std.conv, std.stdio;
    foreach (i; 0..args[1].to!int)
        i.write;
}


void main(string[] args)
{
    import std.algorithm, std.conv, std.range, std.stdio;
    // this is CTFE
    static immutable r = 10_000_000.iota.map!(to!string).joiner("").array;
    auto n = args[1].to!int;
    r[0..n].write;
}

#!/usr/bin/env rdmd

void print(T)(T n)
{
    import core.sys.posix.stdio : putchar_unlocked;
	if (n == 0)
	{
		putchar_unlocked('0');
	}
	else
	{
		char[11] buf;
		buf[10] = '\n';
		int i = 9;
		while (n)
		{
			buf[i--] = n % 10 + '0';
			n /= 10;
		}
		while (buf[i] != '\n')
			putchar_unlocked(buf[++i]);
	}
}

void main(string[] args)
{
    import std.conv, std.stdio;
    import std.concurrency;
    auto pid = spawn({
        bool b = true;
        while(b)
        {
            receive(
                (bool a) { b = false; },
                (int n) { n.print; }
            );
        }
    });
    auto n = args[1].to!int;
    pid.setMaxMailboxSize(n + 10, OnCrowding.ignore);
    foreach (i; 0..n)
        pid.send(i);

    pid.send(true);
}

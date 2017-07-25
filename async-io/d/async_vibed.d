#!/usr/bin/env dub
/+dub.sdl:
name "async_vibed"
dependency "vibe-d" version="~>0.8.0"
+/
import vibe.d;
void main(string[] args)
{
    foreach (i; 0..args[1].to!int)
        pid.send(i);

    spawn({

    });
    runEventLoop();
}

#!/usr/bin/python3

from threading import Thread
import sys
from multiprocessing import Queue
from time import sleep

isRunning = True

def consumer(queue):
    while isRunning or not queue.empty():
        if not queue.empty():
            n = queue.get()
            print(n, end='')


queue = Queue(maxsize=0)

worker = Thread(target=consumer, args=[queue])
worker.start()

for i in range(0, int(sys.argv[1])):
    queue.put(i)


isRunning = False

worker.join()

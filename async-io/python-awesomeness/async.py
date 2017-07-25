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

Thread(target=consumer, args=[queue]).start()

for i in range(0, int(sys.argv[1])):
    queue.put(i)


while not queue.empty():
    sleep(0.00000001)


isRunning = False

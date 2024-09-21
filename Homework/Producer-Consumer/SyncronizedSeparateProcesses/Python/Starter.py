import os
from threading import *

numProducers = int(input("Number of producers: "))
numConsumers = int(input("Number of consumers: "))

file = Semaphore(1)

for x in range(numConsumers):
    os.system("gnome-terminal -- python3 Producer.py")

for x in range(numProducers):
    os.system("gnome-terminal -- python3 Producer.py")
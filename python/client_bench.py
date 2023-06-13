"""
Benchmark set and get
"""

from time import perf_counter
from redis import Redis

from ucall.client import Client

operation_count = 100000

# UCall with UStore
client = Client(port=6378)

t1 = perf_counter()
for i in range(operation_count):
    client.set(key=i, value='value')
t2 = perf_counter()
print('UCall with UStore Set: {:.0f} op/s'.format(operation_count/(t2-t1)))


t1 = perf_counter()
for i in range(operation_count):
    response = client.get(key=i)
t2 = perf_counter()
print('UCall with UStore Get: {:.0f} op/s'.format(operation_count/(t2-t1)))


# Redis
client = Redis(host='localhost', port=6379, decode_responses=True)

keys = [str(key) for key in range(operation_count)]
t1 = perf_counter()
for key in keys:
    client.set(key, 'value')
t2 = perf_counter()
print('Redis Set: {:.0f} op/s'.format(operation_count/(t2-t1)))

t1 = perf_counter()
for key in keys:
    client.get(key)
t2 = perf_counter()
print('Redis Get: {:.0f} op/s'.format(operation_count/(t2-t1)))

"""
Benchmark set and get
"""

from time import perf_counter

from ucall.client import Client

operation_count = 100000
client = Client(port=6379)

t1 = perf_counter()
for i in range(operation_count):
    client.set(key=i, value='value')
t2 = perf_counter()
print('Set: {:.0f}op/s'.format(operation_count/(t2-t1)))


t1 = perf_counter()
for i in range(operation_count):
    response = client.get(key=i)
t2 = perf_counter()
print('Get: {:.0f}op/s'.format(operation_count/(t2-t1)))

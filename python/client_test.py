"""
Test set and get
"""

from ucall.client import Client


def test_set_get():
    client = Client(port=6379)
    for i in range(1000):
        val = f'value{i}'
        client.set(key=i, value=val)
        response = client.get(key=i)
        assert response.json == val

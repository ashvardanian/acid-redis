"""
Tiny Redis-like Server using UCall and UStore
"""

from ucall.posix import Server
from ustore.ucset import DataBase

db = DataBase()
main = db.main
server = Server(port=6379)


@server
def get(key: int):
    return main[key].decode()


@server
def set(key: int, value: str):
    main[key] = value.encode()


server.run()

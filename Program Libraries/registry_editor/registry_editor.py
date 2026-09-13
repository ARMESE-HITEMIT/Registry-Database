# *	Registry Editor Library
# *
# *	Copyright (c) 2022 RANDOM ARMESE HITEMIT - Registry Editor Library
# *	All rights reserved.
#
# ------------------------------------------------------ -
# 12 / 20 / 2025 - 11:30 : 54PM
# ------------------------------------------------------ -
# 
# LICENSE
# 
# ====================================================== =
# 
# Copyright(c) 2022 RANDOM ARMESE HITEMIT
# All Rights Reserved
# 
# -- -
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files(the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions :
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
# 
# -- -

from pathlib import Path
import socket
import struct
import sys
import threading
from dataclasses import dataclass

sys.path.insert(0, str(Path(__file__).resolve().parent.parent / "system"))
import system

sys.path.insert(0, str(Path(__file__).resolve().parent.parent / "utilityX"))
import utilityX

class type:
    key = 0
    string = 1
    byte = 2
    word = 3
    dword = 4
    qword = 5


@dataclass(frozen=True)
class INIT_KEY:
    id: str


@dataclass(frozen=True)
class INIT_VALUE:
    id: str
    value: str = ""


@dataclass(frozen=True)
class INIT_BYTE:
    id: str
    value: int = 0


@dataclass(frozen=True)
class INIT_WORD:
    id: str
    value: int = 0


@dataclass(frozen=True)
class INIT_DWORD:
    id: str
    value: int = 0


@dataclass(frozen=True)
class INIT_QWORD:
    id: str
    value: int = 0


class _VALUE_TYPE:
    DATA = 0
    BYTE = 1
    WORD = 2
    DWORD = 3
    QWORD = 4
    ECC = 5


class _Op:
    CREATE_REGISTRY_FILE = 1
    DELETE_REGISTRY_FILE = 2
    REGISTRY_BEGIN = 3
    REGISTRY_END = 4
    CREATE_KEY = 5
    CREATE_VALUE = 6
    CREATE_BYTE = 7
    CREATE_WORD = 8
    CREATE_DWORD = 9
    CREATE_QWORD = 10
    MODIFY_VALUE = 11
    MODIFY_BYTE = 12
    MODIFY_WORD = 13
    MODIFY_DWORD = 14
    MODIFY_QWORD = 15
    READ_VALUE = 16
    READ_BYTE = 17
    READ_WORD = 18
    READ_DWORD = 19
    READ_QWORD = 20
    GET_TYPE = 21
    RENAME = 22
    CONTAINS = 23
    LIST = 24
    READ_ELEMENT = 25
    DELETE_ELEMENT = 26
    UPDATE = 27
    UNUPDATE = 28
    JSON_TREE = 29
    TREE = 30
    GET_KEYNAME_PATH = 31
    UPDATE_TRIGGER = 32
    SAVE = 33
    CREATE_MULTIPLE_KEY = 34
    CREATE_MULTIPLE_VALUE = 35
    CREATE_MULTIPLE_BYTE = 36
    CREATE_MULTIPLE_WORD = 37
    CREATE_MULTIPLE_DWORD = 38
    CREATE_MULTIPLE_QWORD = 39
    MODIFY_MULTIPLE_VALUE = 40
    MODIFY_MULTIPLE_BYTE = 41
    MODIFY_MULTIPLE_WORD = 42
    MODIFY_MULTIPLE_DWORD = 43
    MODIFY_MULTIPLE_QWORD = 44
    READ_MULTIPLE_VALUE = 45
    READ_MULTIPLE_BYTE = 46
    READ_MULTIPLE_WORD = 47
    READ_MULTIPLE_DWORD = 48
    READ_MULTIPLE_QWORD = 49
    GET_MULTIPLE_TYPE = 50
    RENAME_MULTIPLE = 51
    DELETE_MULTIPLE_ELEMENT = 52
    READ_MULTIPLE_ELEMENT = 53
    PROPERTIES = 54
    INITIALIZATION = 55


_HOST = "127.0.0.1"
_PORT = 41420
_ROOT_NAME = "*__root__*"
_ALLOWED_NAME_CHARS = set("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-{}[].@#!:?~")
_UINT_MAX = {
    _VALUE_TYPE.BYTE: 0xFF,
    _VALUE_TYPE.WORD: 0xFFFF,
    _VALUE_TYPE.DWORD: 0xFFFFFFFF,
    _VALUE_TYPE.QWORD: 0xFFFFFFFFFFFFFFFF,
}
_FMT = {
    _VALUE_TYPE.BYTE: "<B",
    _VALUE_TYPE.WORD: "<H",
    _VALUE_TYPE.DWORD: "<I",
    _VALUE_TYPE.QWORD: "<Q",
}


def _fail(class_id, error_code):
    raise system.ecc(class_id, error_code)


def _check_uint(value, value_type):
    if not isinstance(value, int):
        _fail(0, 8)
    if value < 0 or value > _UINT_MAX[value_type]:
        _fail(0, 8)
    return value


def _check_name(name):
    if not isinstance(name, str):
        _fail(0, 8)
    size = len(name)
    if size == 0 or size > 255:
        _fail(2, 14)
    for c in name:
        if c not in _ALLOWED_NAME_CHARS:
            _fail(2, 15)
    return name


def _check_path(path):
    if not isinstance(path, str):
        _fail(0, 8)
    n = len(path)
    if n == 0:
        _fail(2, 7)
    if not path.startswith(_ROOT_NAME):
        _fail(2, 8)
    if n == len(_ROOT_NAME):
        return path
    if path[len(_ROOT_NAME)] != ">":
        _fail(2, 8)
    start = len(_ROOT_NAME) + 1
    while True:
        pos = path.find(">", start)
        if pos == -1:
            _check_name(path[start:])
            return path
        _check_name(path[start:pos])
        start = pos + 1


def _check_pair_list(items):
    try:
        result = list(items)
    except TypeError:
        _fail(0, 8)
    for item in result:
        if not isinstance(item, (tuple, list)) or len(item) != 2:
            _fail(0, 8)
    return result


def _check_name_list(items):
    try:
        result = list(items)
    except TypeError:
        _fail(0, 8)
    for name in result:
        _check_name(name)
    return result

def _connect_host():
    s = None
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        s.connect((_HOST, _PORT))
        return s
    except system.ecc:
        raise
    except Exception:
        try:
            s.close()
        except Exception:
            pass
        _fail(0, 18)


def _read_exact(sock, size):
    if size == 0:
        return b""
    buf = bytearray(size)
    view = memoryview(buf)
    pos = 0
    try:
        while pos < size:
            n = sock.recv_into(view[pos:])
            if n <= 0:
                _fail(0, 18)
            pos += n
    except system.ecc:
        raise
    except Exception:
        _fail(0, 18)
    return bytes(buf)


def _write_exact(sock, data):
    try:
        sock.sendall(data)
    except system.ecc:
        raise
    except Exception:
        _fail(0, 18)


def _read_raw(sock, fmt):
    return struct.unpack(fmt, _read_exact(sock, struct.calcsize(fmt)))[0]


def _write_raw(sock, fmt, value):
    try:
        _write_exact(sock, struct.pack(fmt, value))
    except system.ecc:
        raise
    except Exception:
        _fail(0, 8)


def _read_value_type(sock):
    return _read_raw(sock, "<B")


def _write_value_type(sock, value_type):
    _write_raw(sock, "<B", value_type)


def _read_ecc_payload(sock):
    class_id = _read_raw(sock, "<Q")
    error_code = _read_raw(sock, "<Q")
    raise system.ecc(class_id, error_code)


def _write_pod(sock, value_type, value):
    _check_uint(value, value_type)
    _write_value_type(sock, value_type)
    _write_raw(sock, _FMT[value_type], value)


def _read_pod(sock, value_type):
    got = _read_value_type(sock)
    if got == _VALUE_TYPE.ECC:
        _read_ecc_payload(sock)
    if got != value_type:
        _fail(0, 8)
    return _read_raw(sock, _FMT[value_type])


def _write_byte(sock, value):
    _write_pod(sock, _VALUE_TYPE.BYTE, value)


def _write_word(sock, value):
    _write_pod(sock, _VALUE_TYPE.WORD, value)


def _write_dword(sock, value):
    _write_pod(sock, _VALUE_TYPE.DWORD, value)


def _write_qword(sock, value):
    _write_pod(sock, _VALUE_TYPE.QWORD, value)


def _read_byte(sock):
    return _read_pod(sock, _VALUE_TYPE.BYTE)


def _read_word(sock):
    return _read_pod(sock, _VALUE_TYPE.WORD)


def _read_dword(sock):
    return _read_pod(sock, _VALUE_TYPE.DWORD)


def _read_qword(sock):
    return _read_pod(sock, _VALUE_TYPE.QWORD)


def _write_string(sock, value):
    if value is None:
        value = ""
    if isinstance(value, bytes):
        data = value
    elif isinstance(value, str):
        data = value.encode("utf-8", "surrogateescape")
    else:
        _fail(0, 8)
    if len(data) > 0xFFFFFFFF:
        _fail(0, 8)
    _write_value_type(sock, _VALUE_TYPE.DATA)
    _write_raw(sock, "<I", len(data))
    _write_exact(sock, data)


def _read_string(sock):
    got = _read_value_type(sock)
    if got == _VALUE_TYPE.ECC:
        _read_ecc_payload(sock)
    if got != _VALUE_TYPE.DATA:
        _fail(0, 8)
    length = _read_raw(sock, "<I")
    if length == 0:
        return ""
    return _read_exact(sock, length).decode("utf-8", "surrogateescape")


def _check_ecc(sock):
    got = _read_value_type(sock)
    if got == _VALUE_TYPE.BYTE:
        status = _read_raw(sock, "<B")
        if status == 0:
            return
        if status == 1:
            class_id = _read_qword(sock)
            error_code = _read_qword(sock)
            raise system.ecc(class_id, error_code)
        _fail(0, 8)
    if got == _VALUE_TYPE.ECC:
        _read_ecc_payload(sock)
    _fail(0, 8)


def _close_socket(sock):
    if sock is None:
        return
    try:
        sock.shutdown(socket.SHUT_RDWR)
    except Exception:
        pass
    try:
        sock.close()
    except Exception:
        pass


class registry_editor:
    def __init__(self, ptr, sock):
        self.m_ptr = ptr
        self.m_sock = sock
        self.m_mtx = threading.RLock()
        self.m_update_socks = {}
        self.m_update_mtx = threading.RLock()
        self._closed = False

    def close(self):
        with self.m_update_mtx:
            for s in list(self.m_update_socks.values()):
                _close_socket(s)
            self.m_update_socks.clear()
        with self.m_mtx:
            if not self._closed:
                _close_socket(self.m_sock)
                self._closed = True

    def __del__(self):
        try:
            self.close()
        except Exception:
            pass

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc, tb):
        registry_end(self)
        return False

    def _write_header(self, op):
        _write_byte(self.m_sock, op)
        _write_qword(self.m_sock, self.m_ptr)

    def create_key(self, key_name, keyname_path):
        _check_name(key_name)
        _check_path(keyname_path)
        with self.m_mtx:
            self._write_header(_Op.CREATE_KEY)
            _write_string(self.m_sock, key_name)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)

    def create_value(self, value_name, value_data="", keyname_path=_ROOT_NAME):
        _check_name(value_name)
        _check_path(keyname_path)
        with self.m_mtx:
            self._write_header(_Op.CREATE_VALUE)
            _write_string(self.m_sock, value_name)
            _write_string(self.m_sock, value_data)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)

    def create_byte(self, value_name, value_data=0, keyname_path=_ROOT_NAME):
        _check_name(value_name)
        _check_path(keyname_path)
        _check_uint(value_data, _VALUE_TYPE.BYTE)
        with self.m_mtx:
            self._write_header(_Op.CREATE_BYTE)
            _write_string(self.m_sock, value_name)
            _write_byte(self.m_sock, value_data)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)

    def create_word(self, value_name, value_data=0, keyname_path=_ROOT_NAME):
        _check_name(value_name)
        _check_path(keyname_path)
        _check_uint(value_data, _VALUE_TYPE.WORD)
        with self.m_mtx:
            self._write_header(_Op.CREATE_WORD)
            _write_string(self.m_sock, value_name)
            _write_word(self.m_sock, value_data)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)

    def create_dword(self, value_name, value_data=0, keyname_path=_ROOT_NAME):
        _check_name(value_name)
        _check_path(keyname_path)
        _check_uint(value_data, _VALUE_TYPE.DWORD)
        with self.m_mtx:
            self._write_header(_Op.CREATE_DWORD)
            _write_string(self.m_sock, value_name)
            _write_dword(self.m_sock, value_data)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)

    def create_qword(self, value_name, value_data=0, keyname_path=_ROOT_NAME):
        _check_name(value_name)
        _check_path(keyname_path)
        _check_uint(value_data, _VALUE_TYPE.QWORD)
        with self.m_mtx:
            self._write_header(_Op.CREATE_QWORD)
            _write_string(self.m_sock, value_name)
            _write_qword(self.m_sock, value_data)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)

    def modify_value(self, value_name, keyname_path, new_value):
        _check_name(value_name)
        _check_path(keyname_path)
        with self.m_mtx:
            self._write_header(_Op.MODIFY_VALUE)
            _write_string(self.m_sock, value_name)
            _write_string(self.m_sock, keyname_path)
            _write_string(self.m_sock, new_value)
            _check_ecc(self.m_sock)

    def modify_byte(self, value_name, keyname_path, new_byte):
        _check_name(value_name)
        _check_path(keyname_path)
        _check_uint(new_byte, _VALUE_TYPE.BYTE)
        with self.m_mtx:
            self._write_header(_Op.MODIFY_BYTE)
            _write_string(self.m_sock, value_name)
            _write_string(self.m_sock, keyname_path)
            _write_byte(self.m_sock, new_byte)
            _check_ecc(self.m_sock)

    def modify_word(self, value_name, keyname_path, new_word):
        _check_name(value_name)
        _check_path(keyname_path)
        _check_uint(new_word, _VALUE_TYPE.WORD)
        with self.m_mtx:
            self._write_header(_Op.MODIFY_WORD)
            _write_string(self.m_sock, value_name)
            _write_string(self.m_sock, keyname_path)
            _write_word(self.m_sock, new_word)
            _check_ecc(self.m_sock)

    def modify_dword(self, value_name, keyname_path, new_dword):
        _check_name(value_name)
        _check_path(keyname_path)
        _check_uint(new_dword, _VALUE_TYPE.DWORD)
        with self.m_mtx:
            self._write_header(_Op.MODIFY_DWORD)
            _write_string(self.m_sock, value_name)
            _write_string(self.m_sock, keyname_path)
            _write_dword(self.m_sock, new_dword)
            _check_ecc(self.m_sock)

    def modify_qword(self, value_name, keyname_path, new_qword):
        _check_name(value_name)
        _check_path(keyname_path)
        _check_uint(new_qword, _VALUE_TYPE.QWORD)
        with self.m_mtx:
            self._write_header(_Op.MODIFY_QWORD)
            _write_string(self.m_sock, value_name)
            _write_string(self.m_sock, keyname_path)
            _write_qword(self.m_sock, new_qword)
            _check_ecc(self.m_sock)

    def read_value(self, value_name, keyname_path):
        _check_name(value_name)
        _check_path(keyname_path)
        with self.m_mtx:
            self._write_header(_Op.READ_VALUE)
            _write_string(self.m_sock, value_name)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)
            return _read_string(self.m_sock)

    def read_byte(self, value_name, keyname_path):
        _check_name(value_name)
        _check_path(keyname_path)
        with self.m_mtx:
            self._write_header(_Op.READ_BYTE)
            _write_string(self.m_sock, value_name)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)
            return _read_byte(self.m_sock)

    def read_word(self, value_name, keyname_path):
        _check_name(value_name)
        _check_path(keyname_path)
        with self.m_mtx:
            self._write_header(_Op.READ_WORD)
            _write_string(self.m_sock, value_name)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)
            return _read_word(self.m_sock)

    def read_dword(self, value_name, keyname_path):
        _check_name(value_name)
        _check_path(keyname_path)
        with self.m_mtx:
            self._write_header(_Op.READ_DWORD)
            _write_string(self.m_sock, value_name)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)
            return _read_dword(self.m_sock)

    def read_qword(self, value_name, keyname_path):
        _check_name(value_name)
        _check_path(keyname_path)
        with self.m_mtx:
            self._write_header(_Op.READ_QWORD)
            _write_string(self.m_sock, value_name)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)
            return _read_qword(self.m_sock)

    def get_type(self, value_name, keyname_path):
        _check_name(value_name)
        _check_path(keyname_path)
        with self.m_mtx:
            self._write_header(_Op.GET_TYPE)
            _write_string(self.m_sock, value_name)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)
            return _read_byte(self.m_sock)

    def rename(self, keyname_path, old_name, new_name):
        _check_path(keyname_path)
        _check_name(old_name)
        _check_name(new_name)
        with self.m_mtx:
            self._write_header(_Op.RENAME)
            _write_string(self.m_sock, keyname_path)
            _write_string(self.m_sock, old_name)
            _write_string(self.m_sock, new_name)
            _check_ecc(self.m_sock)

    def contains(self, value_name, keyname_path):
        _check_name(value_name)
        _check_path(keyname_path)
        with self.m_mtx:
            self._write_header(_Op.CONTAINS)
            _write_string(self.m_sock, value_name)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)
            return _read_byte(self.m_sock) != 0

    def list(self, keyname_path):
        _check_path(keyname_path)
        with self.m_mtx:
            self._write_header(_Op.LIST)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)
            count = _read_qword(self.m_sock)
            return [_read_string(self.m_sock) for _ in range(count)]

    def read_element(self, value_name, keyname_path):
        _check_name(value_name)
        _check_path(keyname_path)
        with self.m_mtx:
            self._write_header(_Op.READ_ELEMENT)
            _write_string(self.m_sock, value_name)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)
            
            item_type = _read_byte(self.m_sock)
            if item_type == type.string:
                return _read_string(self.m_sock)
            elif item_type == type.byte:
                return _read_byte(self.m_sock)
            elif item_type == type.word:
                return _read_word(self.m_sock)
            elif item_type == type.dword:
                return _read_dword(self.m_sock)
            elif item_type == type.qword:
                return _read_qword(self.m_sock)
            _fail(0, 8)

    def delete_element(self, value_name, keyname_path):
        _check_name(value_name)
        _check_path(keyname_path)
        with self.m_mtx:
            self._write_header(_Op.DELETE_ELEMENT)
            _write_string(self.m_sock, value_name)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)

    def update(self, function):
        if not callable(function):
            _fail(0, 8)
        ls = _connect_host()
        try:
            _write_byte(ls, _Op.UPDATE)
            _write_qword(ls, self.m_ptr)
            _write_qword(ls, 0)
            _check_ecc(ls)
            reg_id = _read_qword(ls)
        except Exception:
            _close_socket(ls)
            raise
        with self.m_update_mtx:
            self.m_update_socks[reg_id] = ls

        def _update_loop():
            while True:
                try:
                    op = _read_byte(ls)
                    if op == _Op.UPDATE_TRIGGER:
                        _read_qword(ls)
                        function()
                except Exception:
                    break

        threading.Thread(target=_update_loop, daemon=True).start()
        return reg_id

    def unupdate(self, id):
        _check_uint(id, _VALUE_TYPE.QWORD)
        with self.m_mtx:
            self._write_header(_Op.UNUPDATE)
            _write_qword(self.m_sock, id)
            _check_ecc(self.m_sock)
        with self.m_update_mtx:
            s = self.m_update_socks.pop(id, None)
            _close_socket(s)

    def json_tree(self):
        with self.m_mtx:
            self._write_header(_Op.JSON_TREE)
            _check_ecc(self.m_sock)
            return _read_string(self.m_sock)

    def tree(self):
        with self.m_mtx:
            self._write_header(_Op.TREE)
            _check_ecc(self.m_sock)
            return _read_string(self.m_sock)

    def get_keyname_path(self):
        with self.m_mtx:
            self._write_header(_Op.GET_KEYNAME_PATH)
            _check_ecc(self.m_sock)
            return _read_string(self.m_sock)

    def save(self):
        with self.m_mtx:
            self._write_header(_Op.SAVE)
            _check_ecc(self.m_sock)

    def create_multiple_key(self, list_of_key_name, keyname_path):
        names = _check_name_list(list_of_key_name)
        _check_path(keyname_path)
        with self.m_mtx:
            self._write_header(_Op.CREATE_MULTIPLE_KEY)
            _write_qword(self.m_sock, len(names))
            for name in names:
                _write_string(self.m_sock, name)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)

    def create_multiple_value(self, list_of_value_name_and_data, keyname_path):
        items = _check_pair_list(list_of_value_name_and_data)
        for name, _ in items:
            _check_name(name)
        _check_path(keyname_path)
        with self.m_mtx:
            self._write_header(_Op.CREATE_MULTIPLE_VALUE)
            _write_qword(self.m_sock, len(items))
            for name, value_data in items:
                _write_string(self.m_sock, name)
                _write_string(self.m_sock, value_data)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)

    def create_multiple_byte(self, list_of_value_name_and_data, keyname_path):
        self._create_multiple_integer(_Op.CREATE_MULTIPLE_BYTE, list_of_value_name_and_data, keyname_path, _VALUE_TYPE.BYTE)

    def create_multiple_word(self, list_of_value_name_and_data, keyname_path):
        self._create_multiple_integer(_Op.CREATE_MULTIPLE_WORD, list_of_value_name_and_data, keyname_path, _VALUE_TYPE.WORD)

    def create_multiple_dword(self, list_of_value_name_and_data, keyname_path):
        self._create_multiple_integer(_Op.CREATE_MULTIPLE_DWORD, list_of_value_name_and_data, keyname_path, _VALUE_TYPE.DWORD)

    def create_multiple_qword(self, list_of_value_name_and_data, keyname_path):
        self._create_multiple_integer(_Op.CREATE_MULTIPLE_QWORD, list_of_value_name_and_data, keyname_path, _VALUE_TYPE.QWORD)

    def _create_multiple_integer(self, op, list_of_value_name_and_data, keyname_path, value_type):
        items = _check_pair_list(list_of_value_name_and_data)
        for name, value_data in items:
            _check_name(name)
            _check_uint(value_data, value_type)
        _check_path(keyname_path)
        with self.m_mtx:
            self._write_header(op)
            _write_qword(self.m_sock, len(items))
            for name, value_data in items:
                _write_string(self.m_sock, name)
                _write_pod(self.m_sock, value_type, value_data)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)

    def modify_multiple_value(self, list_of_value_name_and_data, keyname_path):
        items = _check_pair_list(list_of_value_name_and_data)
        for name, _ in items:
            _check_name(name)
        _check_path(keyname_path)
        with self.m_mtx:
            self._write_header(_Op.MODIFY_MULTIPLE_VALUE)
            _write_qword(self.m_sock, len(items))
            for name, value_data in items:
                _write_string(self.m_sock, name)
                _write_string(self.m_sock, value_data)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)

    def modify_multiple_byte(self, list_of_value_name_and_data, keyname_path):
        self._modify_multiple_integer(_Op.MODIFY_MULTIPLE_BYTE, list_of_value_name_and_data, keyname_path, _VALUE_TYPE.BYTE)

    def modify_multiple_word(self, list_of_value_name_and_data, keyname_path):
        self._modify_multiple_integer(_Op.MODIFY_MULTIPLE_WORD, list_of_value_name_and_data, keyname_path, _VALUE_TYPE.WORD)

    def modify_multiple_dword(self, list_of_value_name_and_data, keyname_path):
        self._modify_multiple_integer(_Op.MODIFY_MULTIPLE_DWORD, list_of_value_name_and_data, keyname_path, _VALUE_TYPE.DWORD)

    def modify_multiple_qword(self, list_of_value_name_and_data, keyname_path):
        self._modify_multiple_integer(_Op.MODIFY_MULTIPLE_QWORD, list_of_value_name_and_data, keyname_path, _VALUE_TYPE.QWORD)

    def _modify_multiple_integer(self, op, list_of_value_name_and_data, keyname_path, value_type):
        items = _check_pair_list(list_of_value_name_and_data)
        for name, value_data in items:
            _check_name(name)
            _check_uint(value_data, value_type)
        _check_path(keyname_path)
        with self.m_mtx:
            self._write_header(op)
            _write_qword(self.m_sock, len(items))
            for name, value_data in items:
                _write_string(self.m_sock, name)
                _write_pod(self.m_sock, value_type, value_data)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)

    def read_multiple_value(self, list_of_value_name, keyname_path):
        return self._read_multiple(_Op.READ_MULTIPLE_VALUE, list_of_value_name, keyname_path, _read_string)

    def read_multiple_byte(self, list_of_value_name, keyname_path):
        return self._read_multiple(_Op.READ_MULTIPLE_BYTE, list_of_value_name, keyname_path, _read_byte)

    def read_multiple_word(self, list_of_value_name, keyname_path):
        return self._read_multiple(_Op.READ_MULTIPLE_WORD, list_of_value_name, keyname_path, _read_word)

    def read_multiple_dword(self, list_of_value_name, keyname_path):
        return self._read_multiple(_Op.READ_MULTIPLE_DWORD, list_of_value_name, keyname_path, _read_dword)

    def read_multiple_qword(self, list_of_value_name, keyname_path):
        return self._read_multiple(_Op.READ_MULTIPLE_QWORD, list_of_value_name, keyname_path, _read_qword)

    def get_multiple_type(self, list_of_value_name, keyname_path):
        return self._read_multiple(_Op.GET_MULTIPLE_TYPE, list_of_value_name, keyname_path, _read_byte)

    def _read_multiple(self, op, list_of_value_name, keyname_path, reader):
        names = _check_name_list(list_of_value_name)
        _check_path(keyname_path)
        with self.m_mtx:
            self._write_header(op)
            _write_qword(self.m_sock, len(names))
            for name in names:
                _write_string(self.m_sock, name)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)
            returned = {}
            for i in range(len(names)):
                returned[names[i]] = reader(self.m_sock)
            return returned

    def rename_multiple(self, keyname_path, list_of_name):
        items = _check_pair_list(list_of_name)
        _check_path(keyname_path)
        for old_name, new_name in items:
            _check_name(old_name)
            _check_name(new_name)
        with self.m_mtx:
            self._write_header(_Op.RENAME_MULTIPLE)
            _write_qword(self.m_sock, len(items))
            for old_name, new_name in items:
                _write_string(self.m_sock, old_name)
                _write_string(self.m_sock, new_name)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)

    def delete_multiple_element(self, list_of_value_name, keyname_path):
        names = _check_name_list(list_of_value_name)
        _check_path(keyname_path)
        with self.m_mtx:
            self._write_header(_Op.DELETE_MULTIPLE_ELEMENT)
            _write_qword(self.m_sock, len(names))
            for name in names:
                _write_string(self.m_sock, name)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)

    def read_multiple_element(self, list_of_value_name, keyname_path):
        names = _check_name_list(list_of_value_name)
        _check_path(keyname_path)
        with self.m_mtx:
            self._write_header(_Op.READ_MULTIPLE_ELEMENT)
            _write_qword(self.m_sock, len(names))
            for name in names:
                _write_string(self.m_sock, name)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)
            returned = {}
            for i in range(len(names)):
                item_type = _read_byte(self.m_sock)
                if item_type == type.string:
                    returned[names[i]] = _read_string(self.m_sock)
                elif item_type == type.byte:
                    returned[names[i]] = _read_byte(self.m_sock)
                elif item_type == type.word:
                    returned[names[i]] = _read_word(self.m_sock)
                elif item_type == type.dword:
                    returned[names[i]] = _read_dword(self.m_sock)
                elif item_type == type.qword:
                    returned[names[i]] = _read_qword(self.m_sock)
                else:
                    _fail(0, 8)
            return returned

    def properties(self, keyname_path):
        _check_path(keyname_path)
        with self.m_mtx:
            self._write_header(_Op.PROPERTIES)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)
            count = _read_qword(self.m_sock)
            returned = {}
            for _ in range(count):
                name = _read_string(self.m_sock)
                returned[name] = _read_byte(self.m_sock)
            return returned

    def initialization(self, list_of_value_name_and_data, keyname_path):
        try:
            items = list(list_of_value_name_and_data)
        except TypeError:
            _fail(0, 8)
        _check_path(keyname_path)
        for item in items:
            _check_name(_init_id(item))
        with self.m_mtx:
            self._write_header(_Op.INITIALIZATION)
            _write_qword(self.m_sock, len(items))
            for item in items:
                _write_initialization_item(self.m_sock, item)
            _write_string(self.m_sock, keyname_path)
            _check_ecc(self.m_sock)


def _init_id(item):
    if isinstance(item, (INIT_KEY, INIT_VALUE, INIT_BYTE, INIT_WORD, INIT_DWORD, INIT_QWORD)):
        return item.id
    _fail(0, 8)


def _write_initialization_item(sock, item):
    if isinstance(item, INIT_KEY):
        _write_byte(sock, type.key)
        _write_string(sock, item.id)
    elif isinstance(item, INIT_VALUE):
        _write_byte(sock, type.string)
        _write_string(sock, item.id)
        _write_string(sock, item.value)
    elif isinstance(item, INIT_BYTE):
        _check_uint(item.value, _VALUE_TYPE.BYTE)
        _write_byte(sock, type.byte)
        _write_string(sock, item.id)
        _write_byte(sock, item.value)
    elif isinstance(item, INIT_WORD):
        _check_uint(item.value, _VALUE_TYPE.WORD)
        _write_byte(sock, type.word)
        _write_string(sock, item.id)
        _write_word(sock, item.value)
    elif isinstance(item, INIT_DWORD):
        _check_uint(item.value, _VALUE_TYPE.DWORD)
        _write_byte(sock, type.dword)
        _write_string(sock, item.id)
        _write_dword(sock, item.value)
    elif isinstance(item, INIT_QWORD):
        _check_uint(item.value, _VALUE_TYPE.QWORD)
        _write_byte(sock, type.qword)
        _write_string(sock, item.id)
        _write_qword(sock, item.value)
    else:
        _fail(0, 8)


def create_registry_file(path_to_registry_file):
    s = _connect_host()
    try:
        _write_byte(s, _Op.CREATE_REGISTRY_FILE)
        _write_string(s, path_to_registry_file)
        _check_ecc(s)
    finally:
        _close_socket(s)


def delete_registry_file(path_to_registry_file):
    s = _connect_host()
    try:
        _write_byte(s, _Op.DELETE_REGISTRY_FILE)
        _write_string(s, path_to_registry_file)
        _check_ecc(s)
    finally:
        _close_socket(s)


def registry_begin(path_to_registry_file):
    s = _connect_host()
    try:
        _write_byte(s, _Op.REGISTRY_BEGIN)
        _write_string(s, utilityX.normalize_path(path_to_registry_file))
        _check_ecc(s)
        ptr = _read_qword(s)
        return registry_editor(ptr, s)
    except Exception:
        _close_socket(s)
        raise


def registry_end(ed):
    if ed is None:
        return
    try:
        with ed.m_mtx:
            if not ed._closed:
                _write_byte(ed.m_sock, _Op.REGISTRY_END)
                _write_qword(ed.m_sock, ed.m_ptr)
                _check_ecc(ed.m_sock)
    finally:
        ed.close()


__all__ = [
    "type",
    "INIT_KEY",
    "INIT_VALUE",
    "INIT_BYTE",
    "INIT_WORD",
    "INIT_DWORD",
    "INIT_QWORD",
    "registry_editor",
    "create_registry_file",
    "delete_registry_file",
    "registry_begin",
    "registry_end",
]

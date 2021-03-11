from ctypes import CDLL
from platform import system


def get_lib(os):
    return {
        'Darwin': '../DynamicLib/libL.dylib',
        'Linux': '../DynamicLib/libL.so',
        'Win32': '../DynamicLib/libL.dll'
    }.get(os, None)


def test(data_list):
    libname = get_lib(system())
    for i in data_list:
        lib = CDLL(libname)
        lib.autoTest(int(i))


if __name__ == '__main__':
    file = open('./test.txt', 'r')
    temp_data = file.read()
    test_list = temp_data.split()
    test(test_list)

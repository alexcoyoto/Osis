import io
import os


def write(file, path, fin, buffer=256):
    with io.FileIO(f'{path}/{fin}', 'rb') as f:
        while True:
            block = f.read(buffer)
            if not block:
                break
            file.write(block)


def rec(file, path, prefix):        # поиск файлов по префиксу
    for dir in os.listdir(path):
        print(path, dir)
        try:
            rec(file, f'{path}/{dir}', prefix)
        except NotADirectoryError:
            if dir.startswith(prefix):
                write(file, path, dir)


def main():
    prefix = input('Enter prefix for first file: ')
    filename = input('Enter filename for output: ')
    if filename.startswith(prefix):
        print('The same values.')
        return
    with io.FileIO(filename, 'wb') as file:
        rec(file, os.curdir, prefix)


if __name__ == '__main__':
    main()
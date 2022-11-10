# World Flipper 图片资源解包

import os
import shutil

KB = 1024

def is_png_file(path: str) -> bool:
    content = read_first_line(path)
    if b"png" in content:
        return True
    else:
        return False


def read_first_line(path: str) -> bytes:
    with open(path, "rb") as fp:
        first_line = fp.readline()
    return first_line


def get_png_size(path):
    with open(path, "rb") as fp:
        content = fp.read(24)
    return bytes_to_dec(content[16:20]), bytes_to_dec(content[20:24])


def bytes_to_dec(arr: bytes) -> int:
    result = 0
    for i in arr:
        result = (result+i)<<8
    return result>>8


def change_file_to_png(path):
    with open(path, "r+b") as fp:
        content = fp.read()
        content = bytes([0x89, ord("P"), ord("N"), ord("G")]) + content[4:]
        fp.seek(0)
        fp.write(content)
    os.rename(path, path+".png")


def copy_to_des_dir(base_path, path, output_dir, is_keep_hierarchy=True) -> str:
    dir, file_name = os.path.split(path[len(base_path)+1:])
    if not is_keep_hierarchy:
        dir = ""
    new_dir = os.path.join(output_dir, dir)
    os.makedirs(new_dir, exist_ok=True)
    new_file = os.path.join(new_dir, file_name)
    return shutil.copy(path, new_file)


def traversal(path: str):
    for child in os.listdir(path):
        sub_path = os.path.join(path, child)
        if os.path.isdir(sub_path):
            yield from traversal(sub_path)
        else:
            yield sub_path


def do_transform(base_path: str, output_dir: str, filter):
    for file_path in traversal(base_path):
        if is_png_file(file_path) and filter(file_path):
            target_path = copy_to_des_dir(base_path, file_path, output_dir, False)
            change_file_to_png(target_path)
            print("process {}...".format(file_path))


def size_filter(file_path):
    size = os.stat(file_path).st_size
    w, h = get_png_size(file_path)
    return ((h > 100) and (w > 50)
            and (2*KB <= size <= 1000*KB))


# 按间距中的绿色按钮以运行脚本。
if __name__ == '__main__':
    base_path = r"D:\BaiduNetdiskDownload\dummy\download\production\upload"
    output_dir = r"D:\BaiduNetdiskDownload\dummy\download\output"
    if os.path.exists(output_dir):
        shutil.rmtree(output_dir)
    do_transform(base_path, output_dir, size_filter)
    print("Transform complete!")


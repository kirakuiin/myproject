# World Flipper 图片资源解包
import os

def read_first_line(path: str) -> bytes:
    with open(path, "rb") as fp:
        first_line = fp.readline()
    return first_line


def is_png_file(content: bytes) -> bool:
    if b"png" in content:
        return True
    else:
        return False


def change_file_to_png(path):
    with open(path, "r+b") as fp:
        content = fp.read()
        content = bytes([0x89, ord("P"), ord("N"), ord("G")]) + content[4:]
        fp.seek(0)
        fp.write(content)
    os.rename(path, path+".png")


# 按间距中的绿色按钮以运行脚本。
if __name__ == '__main__':
    path = "C:\\Users\\admin\\Downloads\\文件解析教程\\sample\\jp.co.cygames.worldflipper\\Local Store\\asset_download\\production\\download\\production\\medium_upload\\00\\b782e4f2a8187454e14352db9b90d0faa62b8d"
    content = read_first_line(path)
    if is_png_file(content):
        change_file_to_png(path)

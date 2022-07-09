import re

from os import mkdir, walk, system
from os.path import sep, exists

from shutil import copy

scene_dir='scenes'
prose_dir='prose'

def copy_files():
    if not exists(prose_dir):
        message='Please make a symlink to the prose directory entitled `prose\', thanks.\n'
        print(message)
        raise Exception
    if not exists(scene_dir):
        mkdir(scene_dir)
    for dir_name,y,filelist in walk(prose_dir):
        if dir_name != prose_dir:
            continue
        filelist=filelist[::-1]
        while filelist:
            file=filelist.pop()
            src=sep.join([prose_dir,file])
            file=file.replace('.txt','.tex')
            dst=sep.join([scene_dir,file])
            copy(src,dst)

pattern_list=[
    # Matches double-quotes followed immediately by single-quote
    ('``\\,`','"`'),\
    # Match double-quotes if at start of string or if it follows (white)space
    ('``','^"'),\
    (' ``','(?<=[^-])\s"'),\
    ]

def fix_line(line):
    for repl,pattern in pattern_list:
        line=re.sub(pattern,repl,line)
    return line

def fix_file(file):
    lines=list()
    file=sep.join([scene_dir,file])
    with open(file) as rfile:
        for line in rfile.readlines():
            line=fix_line(line)
            lines.append(line)
    with open(file,'w') as wfile:
        lines=lines[::-1]
        while lines:
            line=lines.pop()
            wfile.write(line)

def fix_story():
    for x,y,filelist in walk(scene_dir):
        filelist=filelist[::-1]
        while filelist:
            file=filelist.pop()
            fix_file(file)

def typeset_and_clone():
    copy_files()
    fix_story()
    cmd='pdflatex -jobname=svtfoe4 main'
    system(cmd+' && '+cmd);

if __name__ == '__main__':
    typeset_and_clone()

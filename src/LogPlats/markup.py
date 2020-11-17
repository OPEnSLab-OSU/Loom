import os, glob

def searchDir(extension):
    for filename in glob.glob(extension):
        if checkFile(filename) == True:
            markup(filename)


def checkFile(filename):
    if "config" in filename == True:
        print("[Error: cannot markup a configuration file]")
        return False
    return True

def markup(filename):
    lines = []
    left = right = 0
    for line in open(filename, 'r'):
        lines.append(line)

    x = 0
    block = False
    while x < len(lines):
        line = lines[x]
        if "{" in line and "}" in line:
            #add mark to line
            print("")
        else:
            if "{" in line: left = left + 1
            elif "}" in line: right = right + 1

            if left != right:
                if line.find("/*") != -1: block = True
                if line.find("*/") != -1: block = False
                if line.strip() != "" and line.find("LMark;") == -1 and lines[x+1].find("LMark;") == -1 and line.find("}") == -1 and line.strip()[0:2] != "//" and not block:
                    space = " " * (len(lines[x+1]) - len(lines[x+1].lstrip()) + 1)
                    lines.insert(x+1, space + "LMark;\n")
                    x = x + 1
            else: left = right = 0
        x = x + 1

    with open(filename, 'w') as f:
        f.writelines(lines)

def usrInput(ask):
    count = -1
    while count == -1:
        val = input(ask)
        if val.isnumeric() == False:
            print("[Error: enter a 0 or 1 please]")
        elif int(val) == 0:
            count = 0
        elif int(val) == 1:
            count = 1
        else:
            print("[Error: enter 0 or 1 please]")
    return count

def run():
    choice = usrInput("Do you want to run markup for:\n[0] Entire current directory\n[1] Single file?\n")
    if choice == 0:
        searchDir("*.cpp")
        searchDir("*.c")
        #searchDir("*.h")
        searchDir("*.ino")
    else:
        while choice == 1:
            name = input("Enter filename in below: ")
            choice = usrInput("Markup " + str(name) + "?\n[0] Yes\n[1] No\n")
        if checkFile == True: markup(name)

run()

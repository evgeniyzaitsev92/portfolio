from os import listdir, rename
from os.path import isfile, isdir
from pathlib import Path
import sys

##############
### GLOBAL ###
# Constants
INSTR = """
*** Rename Files Batch ***
The script is intended to rename multiple files/folders per launch using a predefined mask, e.g. file001.
Arguements:
    * PATH - folder containing files/dirs to rename
    * ITEMS_TYPE ("files"/"dirs") - what to rename: files or folders
    * EXT (???/"none") - extension, necessary if ITEMS_TYPE is "files"
    * NEW_NAME (???/"none") - new name, common for all items to be renamed
    * NEW_EXT (???/"none") - new extension, necessary if ITEMS_TYPE is "files"
    * START_FROM (should be a positive integer) - an integer from which the count starts, is a changing part of new names

Example: python D:\\rename_files_batch.py D:\some_folder files dll file dll 1
"""
ARG_COUNT = len(sys.argv)
PATH = ""
ITEMS_TYPE = ""
EXT = ""
NEW_NAME = ""
NEW_EXT = ""
START_FROM = 0
INIT_LIST = [] # array of items names in folder by PATH

# Variables
existing = []
future_names = []

#################
### FUNCTIONS ###
# Exit function
def exit_func(mes):
    print(mes)
    sys.exit()

# Entry point
def entry():
    global PATH
    global ITEMS_TYPE
    global EXT
    global NEW_NAME
    global NEW_EXT
    global START_FROM
    global INIT_LIST
    
    if sys.argv[1] == "instr":
        exit_func(INSTR)
        
    else:
        if ARG_COUNT != 7:
            exit_func(INSTR)
            
        else:
            # scheck path for validity
            PATH = sys.argv[1]
            if not isdir(PATH):
                exit_func("PATH: the directory does not exist!")
                
            # check items type
            ITEMS_TYPE = sys.argv[2]
            if ITEMS_TYPE != "files" and ITEMS_TYPE != "dirs":
                exit_func("ITEMS_TYPE: can be \"files\" or \"dirs\" only!")
                
            EXT = sys.argv[3] # extension
            NEW_NAME = sys.argv[4] # new name
            NEW_EXT = sys.argv[5] # new extension
            
            # check starting number, should be positive
            if not sys.argv[6].isdigit():
                exit_func("START_FROM: should be a positive integer!")
            START_FROM = int(sys.argv[6])
            if START_FROM < 0:
                exit_func("START_FROM: should be a positive integer!")
            
            # check if folder is empty or not
            INIT_LIST = listdir(PATH)
            if not INIT_LIST:
                exit_func("INIT_LIST: directory is empty!")
    
# Progress bar
def progress_bar(progress):
    filled = "#"*int((progress/5))
    left = "-"*int(((100-progress)/5))
    print(f"\r|{filled}{left}| {progress}%", end="\r")
    
# Set new name
def set_new_name(item_type, start):
    digit_length = len(str(len(INIT_LIST))) # number of items in folder, for formatting of output name (i.e., 0001)
    new_name = ""
    if item_type == "file":
        new_name = ((NEW_NAME if NEW_NAME!="none" else "")+f"{start:0{digit_length}}"
                    +("."+NEW_EXT if NEW_EXT!="none" else ""))
    elif item_type == "dir":
        new_name = (NEW_NAME if NEW_NAME!="none" else "")+f"{start:0{digit_length}}"
        
    return new_name

# Get new name
def get_new_name(full_path, start):
    new_name = ""
    
    # if is file
    if ITEMS_TYPE == "files" and isfile(full_path):
        if ((EXT != "none" and Path(full_path).suffix.lower() == "."+EXT.lower()) # if has extension
          or (EXT == "none" and Path(full_path).suffix.lower() == "")): # if no extension
            new_name = set_new_name("file", start)
            
    # if is dir
    elif ITEMS_TYPE == "dirs" and isdir(full_path):
        new_name = set_new_name("dir", start)
        
    return new_name

# Compose future (possible) names
def compose_future_names(start):
    print("Composing possible names...")
    count = 1
    for item in INIT_LIST:
        full_path = PATH+"\\"+item
        new_name = get_new_name(full_path, start)
            
        if new_name != "":            
            # push to future names list
            future_names.append(new_name)
            start += 1
        
        # change progress
        progress_bar(round((count*100)/len(INIT_LIST)))
        count += 1

# Get list of pairs: key - initial name, value - output name
def get_items_pairs(start):
    print("Renaming items...")
    items_pairs = {}
    
    # loop through files in folder
    count = 1
    for item in INIT_LIST:
        if item in future_names:
            items_pairs[item] = "Already existed"
            continue
        
        full_path = PATH+"\\"+item
        new_name = get_new_name(full_path, start)
            
        if new_name != "":
            # check for existing, in case item exists - increase number
            full_path = PATH+"\\"+new_name
            while isfile(full_path) or isdir(full_path):
                existing.append(new_name) # fill existing array
                start += 1
                if isfile(full_path):
                    new_name = set_new_name("file", start)
                elif isdir(full_path):
                    new_name = set_new_name("dir", start)

                full_path = PATH+"\\"+new_name
                
            # push key-value to items list
            items_pairs[new_name] = item
            start += 1
            
            # rename
            rename(PATH+"\\"+item, PATH+"\\"+new_name)
            
        # change progress
        progress_bar(round((count*100)/len(INIT_LIST)))
        count += 1
            
    return items_pairs

# Write results to file
def write_to_text(items_pairs):
    print("Writing results to file...")
    f = open("result.txt", "w")
    
    # write pairs
    f.write("Folder path: "+PATH+"\n")
    
    print("* Pairs of names...")
    count = 1
    for item in items_pairs:
        divider = " -- " if items_pairs[item] == "Already existed" else " <--- "
        f.write(item+divider+items_pairs[item]+"\n")
        
        # change progress
        progress_bar(round((count*100)/len(items_pairs)))
        count += 1
        
    f.write("\n")
    print("\n")
    
    # write existing
    print("* Already existing items...")
    if existing:
        count = 1
        f.write("Already existing items:\n")
        for item in existing:
            f.write(item+"\n")
            
            # change progress
            progress_bar(round((count*100)/len(existing)))
            count += 1
        
    else:
        print("Not found!")
    
    f.close()
    
def main():
    entry()
    
    compose_future_names(START_FROM)
    print("\n")
    
    items_pairs = get_items_pairs(START_FROM)
    print("\n")
    
    write_to_text(dict(sorted(items_pairs.items())))
    print()
    
    print("Finished!")

    #print(dict(sorted(items_pairs.items())))
    #print(existing)    
    #print(future_names)

if __name__ == "__main__":
    main()
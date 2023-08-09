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
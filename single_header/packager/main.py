import os
from packager import Packager

def save_single_header(single_header):
    with open("single_header.h", "w") as file:
        file.writelines(single_header)

def main():
    packager = Packager()
    path_h = "../../include/hcFSM/detail/fsm-traits.h"
    my_path = os.path.abspath(os.path.dirname(__file__))
    path = os.path.join(my_path, path_h)
    packager.process_file(path)
    
    single_header = packager.pack_headers()
    save_single_header(single_header)

if __name__ == "__main__":
    main()
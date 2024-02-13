import os
from packager import Packager


def main():
    packager = Packager()
    path_h = "../../include/hcFSM/detail/fsm-traits.h"
    my_path = os.path.abspath(os.path.dirname(__file__))
    path = os.path.join(my_path, path_h)
    packager.parse_file(path)
    print(packager.include_files)

if __name__ == "__main__":
    main()
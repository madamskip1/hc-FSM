import os
from packager import Packager

def __save_single_header(single_header):
    with open("single_header.h", "w") as file:
        file.writelines(single_header)

def __process_file(packager, file_path):
    with open(file_path, "r") as file:
        packager.process_file(file)

def __create_absolute_path(file_path):
    here_path = os.path.abspath(os.path.dirname(__file__))
    to_detail_path = "../../include/hcFSM/detail/"
    return os.path.join(here_path, to_detail_path, file_path)
    
def main():
    packager = Packager()
    files_to_process = (
        "state-machine-fwd.h",
        "handle-event-result.h",
        "state.h",
        "transition.h",
        "transition-action.h",
        "transition-guard.h",
        "transitions-table.h",
        "fsm-traits.h",
        "state-machine.h"
    )

    for file in files_to_process:
        file_path = __create_absolute_path(file)
        __process_file(packager, file_path)
    
    single_header = packager.pack_headers()
    __save_single_header(single_header)

if __name__ == "__main__":
    main()
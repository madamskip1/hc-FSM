import os
import re
from packager import Packager

here_path = os.path.abspath(os.path.dirname(__file__))

def __save_single_header(single_header):
    with open("single_header.h", "w") as file:
        file.writelines(single_header)

def __process_file(packager, file_path):
    with open(file_path, "r") as file:
        packager.process_file(file)

def __create_absolute_path_to_header(file_path):
    global here_path
    to_detail_path = "../../include/hcFSM/detail/"
    return os.path.join(here_path, to_detail_path, file_path)

def __get_project_version():
    global here_path
    cmake_path = os.path.join(here_path, "../../CMakeLists.txt")
    version_regex_pattern = r'(?<=VERSION\s)\d+(?:\.\d+)*'
    
    with open(cmake_path, "r") as file:
        content = file.read()
        match = re.findall(version_regex_pattern, content)
        if match and len(match) >= 2:
            return match[1]
        else:
            return None
    
def main():
    project_version = __get_project_version()
    packager = Packager(project_version)
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
        file_path = __create_absolute_path_to_header(file)
        __process_file(packager, file_path)
    
    single_header = packager.pack_headers()
    __save_single_header(single_header)


if __name__ == "__main__":
    main()
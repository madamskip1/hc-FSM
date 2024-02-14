import re
from datetime import date


class Packager:
    def __init__(self, project_version = None):
        self.include_files = set()
        self.files_content = []
        self.project_version = project_version

    def process_file(self, file):
        file_content = file.readlines()
        file_content = self.__remove_pragma_once(file_content)
        self.__find_includes(file_content)
        file_content = self.__remove_includes(file_content)
        file_content = self.__strip_blank_lines_at_the_end_of_file(file_content)
        self.files_content.append(file_content)
            
    def pack_headers(self):
        single_header_content = []
        single_header_content.append(self.__prepare_metadata_header())
        single_header_content.append("\n\n")
        single_header_content.append("#pragma once")
        single_header_content.append("\n\n")
        single_header_content.extend(self.include_files)
        for file_content in self.files_content:
            single_header_content.append('\n')
            single_header_content.extend(file_content)
        
        return single_header_content

    def __find_includes(self, file_content):
        include_patern = re.compile(r'(#include\s*<.*>)')
        for line in file_content:
            line = line.strip()
            if not line or line == "":
                continue
            
            match = include_patern.match(line)
            if match:
                self.include_files.add(match.group(1) + '\n')
            else:
                break  
    
    def __remove_pragma_once(self, file_content):
        file_content = file_content[1:]  # Remove first line, because its always #pragma once
        return file_content

    def __remove_includes(self, file_content):
        for (i, line) in enumerate(file_content):
            if not(not line or line.startswith("#include") or line == "" or line == '\n'):
                break
        return file_content[i:]
    
    def __strip_blank_lines_at_the_end_of_file(self, file_content):
        last_non_blank_line = len(file_content)
        for i in reversed(range(len(file_content))):
            line = file_content[i].strip()
            if line and line != "":
                break
            last_non_blank_line -= 1
            
        return file_content[:last_non_blank_line]
    
    def __prepare_metadata_header(self):
        metadata_header = "/**\n"
        metadata_header += " * @author: Adamski Maciej (madamskip1)\n"
        metadata_header += " * @project: hcFSM\n"
        metadata_header += " * @project_url: https://github.com/madamskip1/hc-FSM\n"
        metadata_header += " * @date: " + str(date.today()) + "\n"
        metadata_header += " * @license: MIT License. Keep metadata_header intact. \n"
        if self.project_version is not None:
            metadata_header += " * @version: " + self.project_version + "\n"
        metadata_header += " */"
        
        return metadata_header
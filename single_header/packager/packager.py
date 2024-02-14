import re


class Packager:
    def __init__(self):
        self.include_files = set()
        self.files_content = []

    def process_file(self, file):
        file_content = file.readlines()
        file_content = self.__remove_pragma_once(file_content)
        self.__find_includes(file_content)
        file_content = self.__remove_includes(file_content)
        self.files_content.append(file_content)
            
    def pack_headers(self):
        single_header_content = []
        single_header_content.append("#pragma once")
        # to do header with autor, version date etc
        single_header_content.append("\n\n")
        single_header_content.extend(self.include_files)
        single_header_content.append('\n')
        for file_content in self.files_content:
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
            if not(not line or line.startswith("#include") or line == ""):
                break
        return file_content[i:]
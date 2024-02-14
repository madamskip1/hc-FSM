import re


class Packager:
    def __init__(self):
        self.include_files = set()
        self.files_content = []
        

    def parse_file(self, file_name):
        with open(file_name, "r") as file:
            file_content = file.readlines()
            file_content = self.remove_pragma_once(file_content)
            self.find_includes(file_content)
            file_content = self.remove_includes(file_content)
            self.files_content.append(file_content)

    def find_includes(self, file_content):
        include_patern = re.compile(r'(#include\s*<.*>)')
        for line in file_content:
            line = line.strip()
            if not line or line == "":
                continue
            
            match = include_patern.match(line)
            if match:
                self.include_files.add(match.group(1))
            else:
                break  
    
    def remove_pragma_once(self, file_content):
        file_content = file_content[1:]  # Remove first line, because its always #pragma once
        return file_content

    def remove_includes(self, file_content):
        for (i, line) in enumerate(file_content):
            if not(not line or line.startswith("#include") or line == ""):
                break
        return file_content[i:]
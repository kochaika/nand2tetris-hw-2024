import re

class Parser:
    def __init__(self, file_name):
        self.file = open(file_name, "r")
        self.current_command = None

    def has_more_commands(self):
        self.current_command = self.file.readline()
        return self.current_command != ""

    def advance(self):
        self.current_command = self.current_command.strip()
        if "//" in self.current_command:
            self.current_command = self.current_command[:self.current_command.index("//")].strip()

    def command_type(self):
        if self.current_command.startswith("@"):
            return "A_COMMAND"
        elif self.current_command.startswith("(") and self.current_command.endswith(")"):
            return "L_COMMAND"
        else:
            return "C_COMMAND"

    def symbol(self):
        if self.command_type() == "A_COMMAND":
            return self.current_command[1:]
        elif self.command_type() == "L_COMMAND":
            return self.current_command[1:-1]

    def dest(self):
        if "=" in self.current_command:
            return self.current_command.split("=")[0]
        return "null"

    def comp(self):
        comp_part = self.current_command.split("=")[-1] 
        return comp_part.split(";")[0]

    def jump(self):
        if ";" in self.current_command:
            return self.current_command.split(";")[1]
        return "null"

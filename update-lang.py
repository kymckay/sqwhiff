#!/usr/bin/env python3

'''
Script to update the SQF command definitions from the output of:

_allCommands = [];
supportInfo "" apply {
	_x splitString ":" params ["_t", "_x"];
	if (_t != "t") then {
		_x = _x splitString " ";
		_command = switch count _x do {
			case 1;
			case 2: { _x # 0 };
			case 3: { _x # 1 };
			default {nil};
		};
		_allCommands pushBackUnique _command;
	};
};
_allCommands sort true;
_allCommands = _allCommands apply {
	supportInfo format["i:%1", _x]
};
_allCommands
'''

import ast
from typing import List

nullary_commands = []
unary_commands = []
binary_commands = []

def nullary(command: str, return_type: str) -> str:
    return f"{{\"{command}\", Type::{return_type}}}"

def unary(command: str, return_type: str, right_type: str) -> str:
    return f"{{\"{command}\", UnaryCommand(Type::{right_type}, Type::{return_type})}}"

def binary(command: str, return_type: str, left_type: str, right_type: str) -> str:
    return f"{{\"{command}\", BinaryCommand(Type::{left_type}, Type::{right_type}, Type::{return_type})}}"

def get_types(type: str) -> List[str]:
    types = type.split(",")

    # bool is reserved
    if ("bool" in types):
        types.remove("bool")
        types.append("boolean")

    # exception is reserved
    if ("exception" in types):
        types.remove("exception")
        types.append("exception_type")

    # for is reserved
    if ("for" in types):
        types.remove("for")
        types.append("for_type")

    # if is reserved
    if ("if" in types):
        types.remove("if")
        types.append("if_type")

    # namespace is reserved
    if ("namespace" in types):
        types.remove("namespace")
        types.append("name_space")

    # switch is reserved
    if ("switch" in types):
        types.remove("switch")
        types.append("switch_type")

    # while is reserved
    if ("while" in types):
        types.remove("while")
        types.append("while_type")

    # with is reserved
    if ("with" in types):
        types.remove("with")
        types.append("with_type")

    # nan is covered by scalar
    try:
        types.remove("nan")
    finally:
        return types

# By evaluating the SQF array output as a Python literal a list is easily produced
with open("supportInfo.out", "r") as file:
    supportInfo = ast.literal_eval(
        # Remove any newlines that would break Python string literals
        " ".join(
            file.read().split("\n")
        )
        # Remove doubled doubled up quotations
        .replace("\"\"\"\"", "")
        # Remove double quotations at the end of a string
        .replace("\"\"\"", "\"")
        # Remove regular doubled quotations
        .replace("\"\"", "")
        # Fix empty strings (twice for overlapping patterns)
        .replace(",,", ",\"\",")
        .replace(",,", ",\"\",")
        # Escape escape character
        .replace("\\", "\\\\")
        # Want everything lowercase to match my C++
        .lower()
    )

for command in supportInfo:
    # Each command has variants, each varient can have multiple return, left and right types
    for [type, command, _, _, _, _, return_type, left_type, right_type, _] in command:

        # Multiple return types treated as any
        return_types = get_types(return_type)
        if (len(return_types) > 1):
            return_type = "any"
        else:
            return_type = return_types[0]


        if (type == "n"):
            nullary_commands.append(nullary(command, return_type))
        elif (type == "u"):
            for type in get_types(right_type):
                unary_commands.append(unary(command, return_type, type))
        elif (type == "b"):
            left_types = get_types(left_type)
            right_types = get_types(right_type)

            for ltype in left_types:
                for rtype in right_types:
                    binary_commands.append(binary(command, return_type, ltype, rtype))

with open("src/sqf/nullary_keywords.h", "w") as file:
    file.write("#pragma once\n"
        "#include <string>\n"
        "#include <unordered_map>\n\n"
        '#include "./sqf_type.h"\n\n'
        "namespace SQF {\n\n"
        "const std::unordered_map<std::string, Type> Nullary_Keywords{\n"
    )

    file.writelines([f"    {cmd},\n" for cmd in nullary_commands])

    file.write("};\n\n}\n")

with open("src/sqf/unary_keywords.h", "w") as file:
    file.write("#pragma once\n"
        "#include <string>\n"
        "#include <unordered_map>\n\n"
        '#include "./sqf_command.h"\n'
        '#include "./sqf_type.h"\n\n'
        "namespace SQF {\n\n"
        "const std::unordered_multimap<std::string, UnaryCommand> "
        "Unary_Keywords{\n"
    )

    file.writelines([f"    {cmd},\n" for cmd in unary_commands])

    file.write("};\n\n}\n")

with open("src/sqf/binary_keywords.h", "w") as file:
    file.write("#pragma once\n"
        "#include <string>\n"
        "#include <unordered_map>\n\n"
        '#include "./sqf_command.h"\n'
        '#include "./sqf_type.h"\n\n'
        "namespace SQF {\n\n"
        "const std::unordered_multimap<std::string, BinaryCommand> "
        "Binary_Keywords{\n"
    )

    file.writelines([f"    {cmd},\n" for cmd in binary_commands])

    file.write("};\n\n}\n")

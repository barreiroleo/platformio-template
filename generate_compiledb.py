#!/usr/bin/env python3
"""
PlatformIO Compile Database Merger

This script solves a critical issue with PlatformIO's compiledb: environment stomping.
When running `pio run --target compiledb` across multiple environments, each environment overwrites
the previous `compile_commands.json` file instead of appending to it.

Problem:
--------
In a multi-environment PlatformIO project with different testing frameworks and
target platforms:
- Arduino framework environments get stomped by Unity test environments
- Unity test environments get stomped by GoogleTest environments
- The final `compile_commands.json` only contains entries from the last environment
- IDEs lose IntelliSense/autocomplete for all but one environment's dependencies

Solution:
---------
This script:
1. Automatically discovers all environments from platformio.ini by parsing [env:*] sections
2. Generates compile commands for each environment individually using PlatformIO
3. Merges and deduplicates all compilation entries into a unified compile_commands.json
4. Preserves all frameworks - Arduino, Unity, GoogleTest, etc. are all visible to your IDE

"""

import configparser
import json
import os
import subprocess
from typing import TypedDict, cast


class CompileCommand(TypedDict):
    directory: str
    command: str
    file: str


def get_environments() -> list[str]:
    """Get environment names from platformio.ini."""
    config = configparser.ConfigParser()
    _ = config.read('platformio.ini')

    environments: list[str] = []
    section_names: list[str] = cast(list[str], config.sections())
    for section in section_names:
        if section.startswith('env:'):
            environments.append(section[4:])

    return environments


def generate_for_env(env: str) -> list[CompileCommand]:
    """Generate compile_commands.json for one environment."""
    try:
        _ = subprocess.run(['pio', 'run', '-e', env, '--target', 'compiledb'],
                          capture_output=True, check=True)

        if os.path.exists('compile_commands.json'):
            with open('compile_commands.json') as f:
                return cast(list[CompileCommand], json.load(f))
    except subprocess.CalledProcessError:
        pass

    return []


def deduplicate_commands(commands: list[CompileCommand],
                        result: list[CompileCommand],
                        seen: set[str]) -> None:
    """Add commands to result, skipping duplicates by file path."""
    for cmd in commands:
        file_path = os.path.abspath(cmd['file'])
        if file_path not in seen:
            seen.add(file_path)
            cmd['file'] = file_path
            result.append(cmd)


def merge_commands(environments: list[str]) -> list[CompileCommand]:
    """Collect and deduplicate compile commands from all environments."""
    result: list[CompileCommand] = []
    seen: set[str] = set()

    for env in environments:
        print(f"Processing {env}...")
        commands = generate_for_env(env)
        deduplicate_commands(commands, result, seen)

    return result


def write_merged_file(commands: list[CompileCommand]) -> None:
    """Write the merged compile commands to file."""
    with open('compile_commands.json', 'w') as f:
        json.dump(commands, f, indent=2)


def main() -> None:
    """Merge compile_commands.json from all environments."""
    environments = get_environments()
    print(f"Found environments: {', '.join(environments)}")

    # Merge commands from all environments
    all_commands = merge_commands(environments)
    write_merged_file(all_commands)

    print(f"Merged {len(all_commands)} entries from {len(environments)} environments")


if __name__ == '__main__':
    main()

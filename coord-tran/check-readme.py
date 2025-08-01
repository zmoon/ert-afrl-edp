#!/usr/bin/env python3
"""
Check the readme code examples.
"""

from __future__ import annotations

import subprocess
from dataclasses import dataclass
from pathlib import Path

HERE = Path(__file__).parent


@dataclass
class Case:
    command: str
    expected: str | None = None


expected_failures = [
    "./bin/g2r 0 90 45 90",
    "./bin/g2r 0 90 0 -90",
]

prompt = "> "

examples = []
in_block = False
block = None
with open(HERE / "README.md", "r") as f:
    for line in f:
        if line.startswith("```"):
            if in_block:
                if (n := len(block)) == 1:
                    # One command line, without prompt
                    assert not block[0].startswith(prompt)
                    examples.append(Case(command=block[0]))
                else:
                    # One command line with prompt, possibly multiple output lines
                    assert block[0].startswith(prompt)
                    examples.append(
                        Case(
                            command=block[0][len(prompt) :],
                            expected="\n".join(block[1:]),
                        )
                    )
                in_block = False
            else:
                in_block = True
                block = []
        elif in_block:
            block.append(line.rstrip())

for case in examples:
    res = subprocess.run(
        case.command,
        shell=True,
        cwd=HERE,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
    )
    if res.returncode != 0 and case.command not in expected_failures:
        print(f"Command: {case.command}")
        print(f"Error:   {res.stdout.strip()}")
        raise SystemExit(1)
    if case.expected is not None and res.stdout.strip() != case.expected:
        print(f"Command:  {case.command}")
        print(f"Expected: {case.expected}")
        print(f"Got:      {res.stdout.strip()}")
        raise SystemExit(1)

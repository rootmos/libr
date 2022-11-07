#!/usr/bin/env python3

import os
import sys
import toml

if __name__ == "__main__":
    spec_fn = os.path.realpath(sys.argv[1])
    modules_dir = os.path.join(spec_fn, "modules")

    out = sys.stdout

    with open(spec_fn, "r") as f:
        spec = toml.load(f)

    for n, m in spec["module"].items():
        out.write(f"- *{n}*")
        if "description" in m:
            out.write(f": {m['description']}")
        out.write("\n")

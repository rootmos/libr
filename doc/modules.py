#!/usr/bin/env python3

import os
import sys
import re
import toml

MAN_REGEX = re.compile(r"(\w+)\((\d+)\)(#(\w+))?")

if __name__ == "__main__":
    spec_fn = os.path.realpath(sys.argv[1])
    modules_rel = "modules"
    modules_abs = os.path.join(os.path.dirname(spec_fn), modules_rel)

    out = sys.stdout

    with open(spec_fn, "r") as f:
        spec = toml.load(f)

    for n, m in spec["module"].items():
        out.write(f"- **{n}**")
        if "description" in m:
            out.write(f": {m['description']}")

        out.write("<br>")

        for s in [".h", ".c", ".test.c"]:
            fn = n + s
            if os.path.exists(os.path.join(modules_abs, fn)):
                out.write(f" [{s}]({os.path.join(modules_rel, fn)})")

        for l in ["wiki", "reference"]:
            if l in m:
                out.write(f" [{l}]({m[l]})")

        if "man" in m:
            ms = MAN_REGEX.match(m["man"])
            url = f"https://man.archlinux.org/man/{ms[1]}.{ms[2]}.en"
            if ms[3]:
                url += "#" + ms[4]
            out.write(f" [man]({url})")

        out.write("\n")

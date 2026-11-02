#!/usr/bin/env python3
"""Wrap extras/dashboard.html into ../dashboard.h so the D1 can serve it.

Run after editing dashboard.html:
    python3 extras/build_dashboard.py
"""
from pathlib import Path

here = Path(__file__).resolve().parent
html = (here / "dashboard.html").read_text(encoding="utf-8")

if ')rawliteral"' in html:
    raise SystemExit('dashboard.html contains the sequence )rawliteral" which would break the C++ string')

header = (
    "#pragma once\n"
    "#include <pgmspace.h>\n\n"
    "// GENERATED from extras/dashboard.html by extras/build_dashboard.py. Edit the .html, not this file.\n"
    'const char DASHBOARD[] PROGMEM = R"rawliteral(\n'
    + html
    + ')rawliteral";\n'
)
(here.parent / "dashboard.h").write_text(header, encoding="utf-8")
print(f"wrote dashboard.h ({len(html)} bytes of HTML)")

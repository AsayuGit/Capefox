#!/usr/bin/python3

import sys
import json
import xmljson

import subprocess

if len(sys.argv) < 3:
    print(f"Usage : {sys.argv[0]} [source.json] [output.xml]")
    sys.exit(1)

with open(sys.argv[1], "r") as eve:
    with open(sys.argv[2], "w") as out:
        out.write("<root>")

        i = 0
        line = eve.readline().strip()
        while line:
            jsonDoc = json.loads(line)
            #print(jsonDoc)
            xmlDoc = xmljson.badgerfish.etree(jsonDoc)
            print(xmlDoc.dump())

            xml = f"<record{i}>"
            xml += xmlDoc
            xml += f"</record{i}>\n"

            out.write(xml)

            line = eve.readline().strip()
            i += 1
        
        out.write("</root>")
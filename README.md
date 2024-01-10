# Capefox

A suricata JSON analyzer

[![GPLv2 License](https://img.shields.io/badge/License-GPL%20v2-blue.svg)](https://opensource.org/licenses/)


## Features

- Support Both .pcap and .json inputs
- Output reStructuredText formated reports
- Automatically calls surricata
- Easily extensible to support other input and output file formats

## Usage/Examples

#### Syntax
```bash
  Capefox: -f input .pcap/.json [options]
```

#### Exemple to analyse a pcap file and produce an rst report
```bash
  EG: ./Capefox -f suspiciousCapture.pcap -o output/capefox_report.rst
```

#### To analyse a eve.json file directly do this instead :
```bash
  EG: ./Capefox -f data/eve.json -o output/capefox_report.rst
```

#### Options
|  Option   |   Alt         | Description                                     |
| :-------- | :------------ | :---------------------------------------------- |
| `-h`      | `--help`      | Display this help menu                          |
| `-f`      | `--file`      | Path to the .pcap or eve.json to analize        |
| `-o`      | `--output`    | Report output path                              |
| `-c`      | `--config`    | Path to a custom suricata config                |
| `-s`      | `--suricata`  | Path to a custom suricata executable            |

## Dependencies

In order to be able to build Capefox the following dependencies are needed:

### For the docker build:

- docker.io

### For the local build (Debian / Ubuntu):

- nlohmann-json3-dev
- suricata (7.0.0)

### Misc

Please note that Capefox was designed to run with suricata 7.0.0.
As such we recommend the use of the docker build as it bundles the right dependencies.

## Docker Build

To compile & run in a docker container run:

```bash
  cd capefox
  make docker-build
  make docker-run
  Capefox -f suspiciousCapture.pcap -o output/capefox_report.rst
```

A shell from within the newly build docker container will open in the current working directory. You will then be able to acces your custom input and rules to use with capefox.

#### /!\ You should not prefix Capefox with "./" while in the docker container as Capefox will be available in the path, and located at "/usr/local/bin/Capefox"

- Do note that you may need to run these commands as root depending on your docker setup.

## Local Build (Debian / Ubuntu)

To compile & run locally run

```bash
  cd Capefox
  make rebuild
  ./Capefox -f suspiciousCapture.pcap -o output/capefox_report.rst
```
## Sample reports

You can find sample Capefox report in the output folder


## Authors

- [@AsayuGit](https://github.com/AsayuGit)
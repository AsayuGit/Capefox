CC:= g++
LD:= g++

OUT_DIR:= build
OUT:= Capefox

SRC_DIR:= src
SRC:= main.cpp \
	  ReportAnalyzer.cpp \
      Suricata.cpp \
	  Record.cpp \
	  JSONRecord.cpp \
	  IP.cpp \
	  Port.cpp \
	  Domain.cpp \
	  DnsRecord.cpp \
	  NTLMSSP.cpp \
	  ReportWriter.cpp \
	  ReportWriters/RSTReportWriter.cpp \
	  Event.cpp \
	  Events/IPEvent.cpp \
	  Events/TransportEvent.cpp \
	  Events/AlertEvent.cpp \
	  Events/AnomalyEvent.cpp \
	  Events/DcerpcEvent.cpp \
	  Events/DnsEvent.cpp \
	  Events/DhcpEvent.cpp \
	  Events/FileinfoEvent.cpp \
	  Events/FlowEvent.cpp \
	  Events/HttpEvent.cpp \
	  Events/Krb5Event.cpp \
	  Events/SmbEvent.cpp \
	  Events/StatEvent.cpp \
	  Events/TlsEvent.cpp

OBJ:= $(patsubst %,$(OUT_DIR)/%,$(patsubst %.cpp,%.o,$(SRC)))

INCLUDE:= -Iinclude

CFLAGS:= -g -Wall --std=c++20
LDFLAGS:= 

LIBS:= 
LIBS_INCLUDE:= 
LIBS_FLAGS:= 

.PHONY: all clean threaded rebuild

threaded:
	@make --no-print-directory -j$(nproc) all

all: $(OUT_DIR) $(OUT)

$(OUT): $(OBJ)
	@echo "Linking $@ ..."
	@$(LD) -o $(OUT) $(LDFLAGS) $^ $(LIBS)
	@echo "Build Successful !"

$(OUT_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Building $< ..."
	@$(CC) -o $@ -c $(CFLAGS) $< $(INCLUDE) $(LIBS_INCLUDE) $(LIBS_FLAGS)

$(OUT_DIR):
	@echo "Creating build directory ..."
	@mkdir -p $(patsubst %,$(OUT_DIR)/%,$(sort $(dir $(SRC))))

clean:
	@echo "Cleaning ..."
	@rm -rf $(OUT_DIR) $(OUT)

run:
	./$(OUT) -f input/net.pcap -o output/capefox_report.rst

rebuild: clean threaded

docker-build:
	@docker build -t capefox:latest .

docker-run:
	docker run --rm -it -v $(CURDIR):$(CURDIR) -w $(CURDIR) capefox:latest /bin/bash
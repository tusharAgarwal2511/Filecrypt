

# CXX = g++
# CXXFLAGS = -std=c++17 -g -Wall -I. -Isrc/app/encryptDecrypt -Isrc/app/fileHandling -Isrc/app/processes
# LDFLAGS = -lssl -lcrypto

# MAIN_TARGET = filecrypt

# MAIN_SRC = main.cpp \
#            src/app/processes/threadPool.cpp \
#            src/app/fileHandling/fileHandler.cpp \
#            src/app/encryptDecrypt/cryption.cpp

# MAIN_OBJ = $(MAIN_SRC:.cpp=.o)

# all: $(MAIN_TARGET)

# $(MAIN_TARGET): $(MAIN_OBJ)
# 	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean:
# 	rm -f $(MAIN_OBJ) $(MAIN_TARGET)

# .PHONY: clean all


CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -I. -Isrc/app/encryptDecrypt -Isrc/app/fileHandling -Isrc/app/processes -Isrc/app/utils
LDFLAGS = -lssl -lcrypto

MAIN_TARGET = filecrypt

MAIN_SRC = main.cpp \
           src/app/processes/encryptionExecutor.cpp \
           src/app/fileHandling/fileHandler.cpp \
           src/app/encryptDecrypt/cryption.cpp \
           src/app/utils/utilities.cpp

MAIN_OBJ = $(MAIN_SRC:.cpp=.o)

all: $(MAIN_TARGET)

$(MAIN_TARGET): $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(MAIN_OBJ) $(MAIN_TARGET)

.PHONY: clean all

TARGET = sokoide-profiler.so
DOTNETRUNTIME = ./runtime
DOTNETCOMMIT = v8.0.11

COMMONFILES =
SRCS = ClassFactory.cpp \
	CorProfiler.cpp \
	dllmain.cpp

HEADERS :=  $(shell find . -type f -name '*.h')
OBJS = $(SRCS:.cpp=.o)

INCLUDES = -I$(DOTNETRUNTIME)/src/coreclr/pal/inc -I$(DOTNETRUNTIME)/src/coreclr/pal/inc/rt -I$(DOTNETRUNTIME)/src/coreclr/inc -I$(DOTNETRUNTIME)/src/coreclr/pal/prebuilt/inc
CXX = clang++
CXXFLAGS = -Wno-invalid-noreturn -fPIC -fms-extensions -DBIT64 -DPAL_STDCPP_COMPAT -DPLATFORM_UNIX -Wall -g -O3 -I. $(INCLUDES)

UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

ifeq ($(UNAME_S),Linux)
	# Linux
	LDFLAGS = -shared --no-undefined
	DIAGCMD = reqadelf -d
endif
ifeq ($(UNAME_S),Darwin)
	# MacOS
	LDFLAGS = -shared -undefined error -flat_namespace
	DIAGCMD = objdump -p
endif

ifeq ($(UNAME_M),arm64)
	CXXFLAGS += -DENTER_LEAVE_SUPPORTED
	OBJS += asm.o
	ASMSRC = asm/aarch64/asm.s
	ASMFLAGS =
endif

ifeq ($(UNAME_M),x86_64)
	CXXFLAGS += -DENTER_LEAVE_SUPPORTED
	OBJS += asm.o
	ASMSRC = asm/amd64/asm.s
	ASMFLAGS = -mllvm --x86-asm-syntax=intel
endif

default: $(TARGET)

$(TARGET): $(OBJS)
	echo TARGET...
	git submodule update --init --recursive
	cd $(DOTNETRUNTIME) && git checkout $(DOTNETCOMMIT)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $^ -o $@

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

asm.o : $(ASMSRC)
	$(CXX) $(CXXFLAGS) $(ASMFLAGS) -c $< -o $@


diag: $(TARGET)
	readelf -d $(TARGET)
	objdump -p $(TARGET)

clean:
	rm -rf $(TARGET) $(OBJS)

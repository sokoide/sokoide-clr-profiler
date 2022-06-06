TARGET = sokoide-profiler.so
# git clone https://github.com/dotnet/runtime here
DOTNETRUNTIME = ${HOME}/repo/oss/dotnet/runtime

COMMONFILES =
SRCS = ClassFactory.cpp \
	CorProfiler.cpp \
	dllmain.cpp

HEADERS :=  $(shell find . -type f -name '*.h')
OBJS = $(SRCS:.cpp=.o)

INCLUDES = -I$(DOTNETRUNTIME)/src/coreclr/pal/inc -I$(DOTNETRUNTIME)/src/coreclr/pal/inc/rt -I$(DOTNETRUNTIME)/src/coreclr/inc -I$(DOTNETRUNTIME)/src/coreclr/pal/prebuilt/inc
CXX = clang++
CXXFLAGS = -Wno-invalid-noreturn -fPIC -fms-extensions -DBIT64 -DPAL_STDCPP_COMPAT -DPLATFORM_UNIX -Wall -g -O3 -I. $(INCLUDES)
LDFLAGS = -shared -undefined suppress -flat_namespace

default: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) $< -o $@

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


# diag: $(TARGET)
# 	readelf -d $(OUTDIR)/$(TARGET)
# 	objdump -p $(OUTDIR)/$(TARGET)

clean:
	rm -rf $(TARGET) $(OBJS)

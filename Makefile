ifeq ($(OS),Windows_NT)
  uname_S      := Windows
  ifneq ($(shell uname -o),Msys)
    RM         := del /q
  else
    RM         := rm -f
  endif
else
  uname_S      := $(shell uname -s)
endif

ifeq ($(uname_S), Windows)
  EXE          := .exe
  LDFLAGS      := -static-libgcc -static-libstdc++ -static -mwindows
  LIBS         := -lglu32 -lopengl32 -lgdi32 -lglfw3
endif
ifeq ($(uname_S), Linux)
  LIBS         := -lGLU -lGL -lglfw3 -lpthread -lX11 -ldl
endif
ifeq ($(uname_S), Darwin)
  CFLAGS       := -Wno-deprecated-declarations
  LDFLAGS      := -framework Foundation -framework IOKit -framework Cocoa -framework CoreVideo -framework OpenGL
  LIBS         := -lglfw3 -lpthread -ldl
endif

TOPDIR         := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
SRC_DIR         = $(TOPDIR)/src
3RD_PART_DIR    = $(TOPDIR)/3rd_party
GLFW_DIR        = $(3RD_PART_DIR)/glfw
GLFW_INC        = $(GLFW_DIR)/include
GLFW_LIB        = $(GLFW_DIR)/liglfw3.a
TARGET          = Ojda$(EXE)
SRC             = $(SRC_DIR)/Camera.cpp $(SRC_DIR)/Model.cpp $(SRC_DIR)/Ojda.cpp $(SRC_DIR)/Renderer.cpp
OBJS            = $(SRC:.cpp=.o)
DEPS            = $(GLFW_LIB)
LIBDIRS        += -L$(GLFW_DIR)

CXX             = g++
LD              = g++
CFLAGS         += -std=c++11 -I$(SRC_DIR) -I$(3RD_PART_DIR) -I$(GLFW_INC) -MMD -O3
CFLAGS         += -Wall -Wextra -Wshadow -Wundef -Wunused -Wno-strict-aliasing -Wno-deprecated-copy -Wno-cast-function-type
CFLAGS         += -Wno-attributes -Wno-type-limits -Wno-implicit-fallthrough
CPPFLAGS       += $(CFLAGS)
LDFLAGS        += $(LIBDIRS) $(LIBS)

.PHONY: all clean superclean

all: $(TARGET)

clean:
	@$(RM) $(TARGET) $(OBJS) $(OBJS:%.o=%.d)

superclean: clean
	$(MAKE) -C$(GLFW_DIR) clean

$(DEPS):
	$(MAKE) -C$(GLFW_DIR) libglfw3.a

$(TARGET): $(DEPS) $(OBJS)
	@echo [LD] $@
	@$(CXX) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.cpp
	@echo [CC] $<
	@$(CXX) $(CFLAGS) -MMD -c -o $@ $<

-include $(OBJS:%.o=%.d)

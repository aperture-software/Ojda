ifeq ($(OS),Windows_NT)
  ifneq ($(shell uname -o),Msys)
    RM          := del /q
  else
    RM          := rm -f
  endif
  EXE           := .exe
  LDFLAGS       := -static-libgcc -static-libstdc++ -static -mwindows
endif

TOPDIR         := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
GLFW_DIR        = $(TOPDIR)/glfw
GLFW_INC        = $(GLFW_DIR)/include
GLFW_LIB        = $(GLFW_DIR)/liglfw3.a
TARGET          = ojda$(EXE)
SRC             = Camera.cpp Cube.cpp Ojda.cpp Renderer.cpp
OBJS            = $(SRC:.cpp=.o)
DEPS            = $(GLFW_LIB)
LIBS           += -lglfw3 -lglu32 -lopengl32 -lgdi32
LIBDIRS        += -L$(GLFW_DIR)

CXX             = g++
LD              = g++
CFLAGS         += -I$(TOPDIR) -I$(GLFW_INC) -MMD -O3
CFLAGS         += -Wall -Wextra -Wshadow -Wundef -Wunused -Wno-strict-aliasing
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

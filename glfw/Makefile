TARGET           = libglfw3.a
CC               = gcc
OBJS             = src/context.o src/egl_context.o src/init.o src/input.o src/monitor.o
OBJS            += src/osmesa_context.o src/vulkan.o src/window.o
CFLAGS          := -I./src -O3

ifeq ($(OS),Windows_NT)
  uname_S       := Windows
else
  uname_S       := $(shell uname -s)
endif

ifeq ($(uname_S), Windows)
  CFLAGS        += -D_GLFW_WIN32
  OBJS          += src/win32_init.o src/win32_joystick.o src/win32_monitor.o
  OBJS          += src/win32_thread.o src/win32_time.o src/win32_window.o
  OBJS          += src/wgl_context.o
endif
ifeq ($(uname_S), Linux)
  CFLAGS        += -D_GLFW_X11 -DHAVE_XKBCOMMON_COMPOSE_H -DHAVE_MEMFD_CREATE
  OBJS          += src/x11_init.o src/linux_joystick.o src/x11_monitor.o
  OBJS          += src/posix_thread.o src/posix_time.o src/x11_window.o
  OBJS          += src/glx_context.o src/xkb_unicode.o
endif
ifeq ($(uname_S), Darwin)
  CFLAGS        += -D_GLFW_COCOA
  OBJS          += src/cocoa_init.o src/cocoa_joystick.o src/cocoa_monitor.o
  OBJS          += src/posix_thread.o src/cocoa_time.o src/cocoa_window.o
  OBJS          += src/nsgl_context.o
endif

all: $(TARGET)

%.o: %.c
	@echo [CC] $<
	@$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.m
	@echo [CC] $<
	@$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	@echo [AR] $@
	@$(AR) rcs $@ $^

clean:
	@$(RM) $(OBJS) $(TARGET)

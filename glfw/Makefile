TARGET           = libglfw3.a
CC               = gcc
OBJS             = src/context.o src/egl_context.o src/init.o src/input.o src/monitor.o
OBJS            += src/osmesa_context.o src/vulkan.o src/wgl_context.o 
CFLAGS          := -I./src -O3

ifeq ($(OS),Windows_NT)
  CFLAGS        += -D_GLFW_WIN32
  OBJS          += src/win32_init.o src/win32_joystick.o src/win32_monitor.o
  OBJS          += src/win32_thread.o src/win32_time.o src/win32_window.o
  OBJS          += src/window.o
endif

all: $(TARGET)

%.o: %.c
	@echo [CC] $<
	@$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	@echo [AR] $@
	@$(AR) rcs $@ $^

clean:
	@$(RM) $(OBJS) $(TARGET)

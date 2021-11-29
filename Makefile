MAKEFLAGS += --no-builtin-rules

CC := gcc
CFLAGS := -Wall -Werror -Wextra -Wno-unused-function
RELEASEFLAGS := -O3

src_dir := src
obj_dir := obj
build_dir := build
test_src_dir := $(src_dir)/tests
test_obj_dir := $(obj_dir)/tests
dirs := $(src_dir) $(obj_dir) $(build_dir) $(test_src_dir) $(test_obj_dir)
target_exec := $(build_dir)/mcpp.exe
test_exec := $(build_dir)/runtests.exe

sources := $(wildcard $(src_dir)/*.c)
objects := $(addprefix $(obj_dir)/, $(notdir $(sources:.c=.o)))
test_sources := $(wildcard $(test_src_dir)/*.c)
test_objects := $(addprefix $(test_obj_dir)/, $(notdir $(test_sources:.c=.o)))
objects_without_entrypoint := $(filter-out $(obj_dir)/entrypoint.o, $(objects))

# main stuff
$(target_exec): $(dirs) $(objects)
	$(CC) $(CFLAGS) $(objects) -o $@

$(obj_dir)/%.o: $(src_dir)/%.c $(src_dir)/%.h
	$(CC) $(CFLAGS) $< -c -o $@

$(obj_dir)/%.o: $(src_dir)/%.c
	$(CC) $(CFLAGS) $< -c -o $@

$(dirs):
	-mkdir $@

# test stuff
$(test_exec): $(dirs) $(objects) $(test_objects)
	$(CC)  $(CFLAGS) $(test_objects) $(objects_without_entrypoint) -o $@

$(test_obj_dir)/%.o: $(test_src_dir)/%.c $(test_src_dir)/%.h
	$(CC) $(CFLAGS) $< -c -o $@

$(test_obj_dir)/%.o: $(test_src_dir)/%.c
	$(CC) $(CFLAGS) $< -c -o $@

# make commands
.PHONY: clean rebuild release run anyway objects tests runtests cleantests temp _release_settings _ignore_warnings

clean:
	-rm $(obj_dir)/*.o
	-rm $(target_exec)

rebuild: clean $(target_exec)

release: _release_settings rebuild

run: $(target_exec)
	"$(target_exec)" inputfiles/input.mcpp

anyway: _ignore_warnings $(target_exec)


objects: $(dirs) $(objects) # just makes the object files without making the executable

tests: $(test_exec)

runtests: tests
	$(test_exec)

cleantests:
	-rm $(test_obj_dir)/*.o
	-rm $(test_exec)

_release_settings:
	$(eval CFLAGS += $(RELEASEFLAGS))

_ignore_warnings:
	$(eval CFLAGS := $(filter-out -Werror -Wall -Wextra, $(CFLAGS)))

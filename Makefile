MAKEFLAGS += --no-builtin-rules

CC := gcc
RELEASEFLAGS := -O3
DEBUGFLAGS := -g
CFLAGS := -Wall -Werror -Wextra -Wno-unused-function -Wno-unused-parameter $(DEBUGFLAGS)


src_dir := src
obj_dir := obj
build_dir := build
test_src_dir := $(src_dir)/tests
test_obj_dir := $(obj_dir)/tests
dirs := $(src_dir) $(obj_dir) $(build_dir) $(test_src_dir) $(test_obj_dir)
target_exec := $(build_dir)/mcpp.exe

sources := $(wildcard $(src_dir)/*.c)
objects := $(addprefix $(obj_dir)/, $(notdir $(sources:.c=.o)))
objects_without_entrypoint := $(filter-out $(obj_dir)/entrypoint.o, $(objects))
test_sources := $(wildcard $(test_src_dir)/test_*.c)
test_objects := $(addprefix $(test_obj_dir)/, $(notdir $(test_sources:.c=.o)))
test_execs := $(addprefix $(build_dir)/, $(notdir $(test_sources:.c=.exe)))


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
	$(CC) $(CFLAGS) $(test_objects) $(objects_without_entrypoint) -o $@

$(test_obj_dir)/%.o: $(test_src_dir)/%.c $(test_src_dir)/%.h
	$(CC) $(CFLAGS) $< -c -o $@

$(test_obj_dir)/%.o: $(test_src_dir)/%.c
	$(CC) $(CFLAGS) $< -c -o $@

# make commands
.PHONY: clean rebuild release run anyway objects tests runtests cleantests temp _release_settings _ignore_warnings _rebuild_all_objects _test_settings

clean:
	-rm $(obj_dir)/*.o
	-rm $(target_exec)

rebuild: clean _rebuild_all_objects $(target_exec)

release: _release_settings rebuild

run: $(target_exec)
	"$(target_exec)" inputfiles/input.mcpp

anyway: _ignore_warnings $(target_exec)

objects: $(dirs) $(objects) # just makes the object files without making the executable

# Run all the test executables by concatenating them with &&
runtests: $(objects_without_entrypoint) _test_settings $(test_execs)
	$(firstword $(test_execs)) $(addprefix && , $(wordlist 2, $(words $(test_execs)),$(test_execs)))

$(build_dir)/test_%.exe: $(src_dir)/%.c
	$(CC) $(test_src_dir)/test_$*.c $(filter-out $(obj_dir)/$*.o, $(objects_without_entrypoint)) $(CFLAGS) -o $@

cleantests:
	-rm $(build_dir)/test_*.exe

_release_settings:
	$(eval CFLAGS := $(RELEASEFLAGS) $(filter-out $(DEBUGFLAGS), $(CFLAGS)))

_ignore_warnings:
	$(eval CFLAGS := $(filter-out -Werror -Wall -Wextra, $(CFLAGS)))

_rebuild_all_objects: # makes rebuild a lot faster by calling gcc only once
	cd $(obj_dir) && $(CC) $(CFLAGS) $(addprefix ../, $(sources)) -c

_test_settings: # For tests particularly, I want to know if I forgot to call one
	$(eval CFLAGS += -Wunused-function -Werror)





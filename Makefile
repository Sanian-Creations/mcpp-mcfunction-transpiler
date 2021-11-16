MAKEFLAGS += --no-builtin-rules

CC := gcc

src_dir := src
obj_dir := obj
build_dir := build
test_dir := tests
test_obj_dir := $(obj_dir)/$(test_dir)
test_src_dir := $(src_dir)/$(test_dir)
target_exec := $(build_dir)/mcpp.exe
test_exec := $(build_dir)/runtests.exe

sources := $(wildcard $(src_dir)/*.c)
objects := $(addprefix $(obj_dir)/, $(notdir $(sources:.c=.o)))
test_sources := $(wildcard $(test_src_dir)/*.c)
test_objects := $(addprefix $(test_obj_dir)/, $(notdir $(test_sources:.c=.o))) # will be obj/tests/file.o
objects_without_entrypoint := $(filter-out $(obj_dir)/entrypoint.o, $(objects)) # all src objects, but without the entrypoint


# main stuff
$(target_exec): $(objects)
	$(CC) $(objects) -o $@

$(obj_dir)/%.o: $(src_dir)/%.c $(src_dir)/%.h
	$(CC) $< -c -o $@

$(obj_dir)/%.o: $(src_dir)/%.c
	$(CC) $< -c -o $@

# test stuff
$(test_exec): $(src_objects) $(test_objects)
	$(CC) $(test_objects) $(objects_without_entrypoint) -o $@

$(test_obj_dir)/%.o: $(test_src_dir)/%.c $(test_src_dir)/%.h
	$(CC) $< -c -o $@

$(test_obj_dir)/%.o: $(test_src_dir)/%.c
	$(CC) $< -c -o $@

# make commands
.PHONY: clean rebuild run objects tests runtests cleantests temp

clean:
	-rm $(obj_dir)/*.o
	-rm $(target_exec)

rebuild: clean $(target_exec)

run: $(target_exec)
	"$(target_exec)" inputfiles/input.mcpp

objects: $(objects) # just makes the object files without making the executable

tests: $(test_exec)

runtests: tests
	$(test_exec)

cleantests:
	-rm $(test_obj_dir)/*.o
	-rm $(test_exec)

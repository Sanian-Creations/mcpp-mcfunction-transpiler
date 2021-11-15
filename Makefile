MAKEFLAGS += --no-builtin-rules

CC := gcc

src_dir := src
obj_dir := obj
build_dir := build
target_exec := $(build_dir)/output.exe

sources := $(wildcard $(src_dir)/*.c)
names := $(notdir $(basename $(sources)))
objects := $(addprefix $(obj_dir)/, $(addsuffix .o, $(names)))

test_dir := tests
test_exec := $(test_dir)/tests.exe
test_sources := $(wildcard $(test_dir)/*.c)
test_objects := $(addprefix $(obj_dir)/, $(test_sources:.c=.o)) # will be obj/tests/file.o
objects_without_entrypoint := $(filter-out $(obj_dir)/entrypoint.o, $(objects))


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

$(obj_dir)/$(test_dir)/%.o: $(test_dir)/%.c $(test_dir)/%.h
	$(CC) $< -c -o $@

$(obj_dir)/$(test_dir)/%.o: $(test_dir)/%.c
	$(CC) $< -c -o $@

# make commands
.PHONY: clean rebuild run objects tests runtests cleantests temp

clean:
	-rm $(obj_dir)/*.o

rebuild: clean $(target_exec)

run: $(target_exec)
	"$(target_exec)" inputfiles/input.mcpp

objects: $(objects) # just makes the object files without making the executable

tests: $(test_exec)

runtests: tests
	$(test_exec)

cleantests:
	-rm $(obj_dir)/$(test_dir)/*.o

#temp:
#	echo $(objects)
#	echo $(filter-out $(src_dir)/entrypoint.o, $(objects))



# -------------------------------------------------------------------
# Documentation for future Sanian when he forgets how Makefiles work.
# -------------------------------------------------------------------

# when calling 'make' in shell, the first action of the makefile is executed, in this case 'output'


#target: dependencies
#	action

# target = any file
# dependencies = if any of these files change, the action is called
# action = cmd call to construct a new target

# Example
#main.o: main.c
#	gcc -c main.c

# the target is main.o
# when main.c changes, the action is performed
# the action compiles main.c to object file (-c flag) main.o
# result: a new version of main.o was made after main.c changed

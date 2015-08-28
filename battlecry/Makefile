# Written by DJ Edmonson
# Modified by Chaoren Li 2/22/2014

# Warnings frequently signal eventual errors:
CXXFLAGS= -g -W -Wall -Weffc++ -Wextra -pedantic -O3

# Linker flags for both OS X and Linux
LDFLAGS = 

# Generates list of object files from all the
# source files in directory
OBJS = $(addsuffix .o, $(basename $(shell find *.cpp)))
# A substitution reference substitutes the value of a variable with
# alterations that you specify.
DEPS = $(OBJS:.o=.d)

# Set executable name
EXEC = BoggleTest

#	Declare the phony targets
#	A phony target is one that is not really the name of a file; 
#	rather it is just a name for a recipe to be executed when you 
#	make an explicit request. There are two reasons to use a phony 
#	target: to avoid a conflict with a file of the same name, and 
#	to improve performance.
.PHONY: echo clean r clang gcc setclang setgcc vg

# Phony targets to run dependencies in order
clang: | setclang $(EXEC)
gcc: | setgcc $(EXEC)

# For use with the clang static analyzer by
# using the environment variable for CXX
sb: | $(clean) $(EXEC)

vg: $(EXEC)
	valgrind ./$(EXEC)

# Run the executable
r:
	./$(EXEC)

clean:
	rm -rf $(OBJS)
	rm -rf $(DEPS)
	rm -rf $(EXEC)

# Phony target to use clang for compile and linking
setclang:
	@echo "Setting clang"
	$(eval CXX = clang++)
	$(eval CXX_LINK = clang++)

# Phony target to use g++ for compile and linking
setgcc:
	@echo "Setting g++"
	$(eval CXX = g++)
	$(eval CXX_LINK = g++)

# $@ The file name of the target of the rule.
# $% The target member name, when the target is an archive member.
# $< The name of the first prerequisite. 
# $? The names of all the prerequisites that are newer than the target, 
# with spaces between them.
# This rule copies just the changed object files into the archive:
#     lib: foo.o bar.o lose.o win.o
#             ar r lib $?
# $^ The names of all the prerequisites, with spaces between them. 
# $* The stem with which an implicit rule matches You should generally
# avoid using ‘$*’ except in implicit rules or static pattern rules.
# Uses static pattern rule to keep from compiling all
# objects every time.
$(OBJS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


$(DEPS): %.d: %.cpp
	@echo "Generating " $@
#	Generating Prerequisites Automatically
# With the GNU C compiler, you may wish to use the ‘-MM’ flag instead
# of ‘-M’. This omits prerequisites on system header files. 
#	-e’ flag to the shell causes it to exit immediately if the $(CC) 
# command fails
# g++ -MM $(CPPFLAGS) $< > $@.$$$$; \ 
#	The purpose of the sed command is to translate 
	@set -e; rm -f $@; \
   $(CXX) -M $(CPPFLAGS) $< > $@.$$$$; \
   sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
   rm -f $@.$$$$

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

include $(DEPS)

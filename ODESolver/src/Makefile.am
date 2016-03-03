AUTOMAKE_OPTIONS=no-dependencies

if JS
    JS_SOLVER = js-solver
    EXAMPLES =
else
    JS_SOLVER =
    EXAMPLES = examples
endif

noinst_PROGRAMS = $(JS_SOLVER)
SUBDIRS = solver $(EXAMPLES)
js_solver_SOURCES = bindings.cpp
js_solver_LDADD = solver/libgear.a
js_solver_CXXFLAGS = @bind@
js_solver_LDFLAGS = @flags@ @bind@
EXEEXT = @exeext@

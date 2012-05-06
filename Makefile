CC = gcc46
CFLAGS = -Wall -pedantic -ansi

BINDIR := bin
OBJDIR := obj

OBJS := $(addprefix $(OBJDIR)/, errors.o list.o symtab.o getline.o paragraph.o scanner.o dlist_test.o symtab_test.o getline_test.o parag_test.o scanner_test.o)
TESTS := $(addprefix $(BINDIR)/, dlist_test symtab_test getline_test parag_test scanner_test)

vpath %.c src src/test
vpath %.o $(OBJDIR)
vpath %.h headers

tests : $(OBJS) $(TESTS)

$(OBJDIR)/%.o : %.c
	$(CC) -c $(CFLAGS) -Iheaders $^ -o $@

$(OBJS) : | $(OBJDIR)

$(OBJDIR) :
	mkdir $(OBJDIR)

$(TESTS) : dlist_test symtab_test getline_test parag_test scanner_test

dlist_test : dlist_test.o list.o errors.o
	$(CC) $^ -o $(BINDIR)/$@

symtab_test : symtab_test.o symtab.o getline.o errors.o
	$(CC) $^ -o $(BINDIR)/$@

getline_test : getline_test.o getline.o errors.o
	$(CC) $^ -o $(BINDIR)/$@

parag_test : parag_test.o paragraph.o list.o errors.o getline.o
	$(CC) $^ -o $(BINDIR)/$@

scanner_test : scanner_test.o scanner.o getline.o paragraph.o list.o symtab.o errors.o
	$(CC) $^ -o $(BINDIR)/$@

$(BINDIR) :
	mkdir $(BINDIR)

scanner_test.o : scanner.h getline.h paragraph.h list.h symtab.h
scanner.o : scanner.h getline.h paragraph.h list.h symtab.h
parag_test.o : paragraph.h list.h errors.h getline.h
paragraph.o : paragraph.h list.h errors.h
getline_test.o : getline.h errors.h
getline.o : getline.h errors.h
symtab_test.o : symtab.h getline.h errors.h
symtab.o : symtab.h errors.h
dlist_test.o : list.h getline.h errors.h
list.o : list.h errors.h

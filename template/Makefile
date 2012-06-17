################################################################################
#
#	Makfile avr.
#
# - author 	:	Maleyrie Antoine : antoine.maleyrie@gmail.com
# - version	:	0.1
# - date 	:	03 Juin 2012
#
################################################################################

#Le composent utiliser pour gcc (option de gcc).
MCU = at90usb1287
#Le composent utiliser pour avrdude (option d'avrdude).
MCUDUD = usb1287
#programateur utiliser (option d'avrdude)
PROGRAMATOR = dragon_jtag -P usb
#Fréquence du cpu (en Hz).
FCPU = 16000000UL

CC		= avr-gcc
OBJCOPY	= avr-objcopy
SIZE 	= avr-size

#Option de compilation
CCFLAGSO=-std=gnu99 -W -Wall -Wextra
#Option de linkage
LDFLAGS=
#Nom du fichier de sorti issu de la compilation.
TARGET_OUTPUT_FILE=avrproject.elf

#Endroit où ce trouve avr_swift
AVRSWIFT = /usr/libavr/avr_swift

#Endroit où stoker les fichiers binaire.
OBJDIR = obj

#flage en plus pour la compilation en release.
CCFLAGSR=-Os -O2
#flage en plus pour la compilation en debug.
CCFLAGSD=-g

#Liste des fichiers source C
SRC=trunk/main.c
OBJS=$(SRC:.c=.o)

#Dossier à inclure
UINCDIRS=	$(AVRSWIFT)/peripherals \
			$(AVRSWIFT)/drivers \
			trunk/
UINCDIR = $(patsubst %,-I%, $(UINCDIRS))


#Construire le project en release
all:release

#Construire les option du projet pour la compilation en release
.PHONY: release
release:CCFLAGS=$(CCFLAGSO) $(CCFLAGSR)
release:build

#Construire les option du projet pour la compilation en debug
.PHONY: debug
debug:CCFLAGS=$(CCFLAGSO) $(CCFLAGSD)
debug:build


#Construction du projet
build: $(OBJS)
	$(CC) -o $(TARGET_OUTPUT_FILE) $(patsubst %,$(OBJDIR)/%, $(patsubst /%,%, $^)) $(LDFLAGS) -mmcu=$(MCU)
	$(OBJCOPY) -O ihex -R .eeprom -R .eesafe $(TARGET_OUTPUT_FILE) $(TARGET_OUTPUT_FILE).hex
	$(SIZE) $(TARGET_OUTPUT_FILE)

%.o: %.c
	@mkdir -p $(OBJDIR)/$(patsubst /%,%, $(@D))
	@$(CC) -o $(OBJDIR)/$(patsubst /%,%, $@) -c $<  $(UINCDIR) $(CCFLAGS) -mmcu=$(MCU) -DF_CPU=$(FCPU)

#Génération de la doc
.PHONY: doc
doc:
	cd doc && doxygen
	
#programmation
.PHONY: prog
prog:
	avrdude -p $(MCUDUD) -c $(PROGRAMATOR) -U flash:w:$(TARGET_OUTPUT_FILE).hex
	
#Supprime la doc générée
.PHONY: cleandoc
cleandoc:
	@cd doc && rm html -R

#Nettoyage du projet
.PHONY: clean
clean:
	@rm $(OBJDIR) -R
	@rm $(TARGET_OUTPUT_FILE)
	@rm $(TARGET_OUTPUT_FILE).hex

################################################################################
#
#	Makfile avr_swift, juste pour générai la documentation d'avr_swift.
#
# - author 	:	Maleyrie Antoine : antoine.maleyrie@gmail.com
# - version	:	0.1
# - date 	:	09 Juin 2012
#
################################################################################


#Construire la documentation
all:doc

#Génération de la doc
.PHONY: doc
doc:
	cd doc && doxygen
	
#Supprime la doc génération
.PHONY: clean
clean:
	@cd doc && rm html -R



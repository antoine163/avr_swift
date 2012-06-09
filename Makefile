################################################################################
#
#	Makfile avr_swift, juste pour g�n�rai la documentation d'avr_swift.
#
# - author 	:	Maleyrie Antoine : antoine.maleyrie@gmail.com
# - version	:	0.1
# - date 	:	09 Juin 2012
#
################################################################################


#Construire la documentation
all:doc

#G�n�ration de la doc
.PHONY: doc
doc:
	cd doc && doxygen
	
#Supprime la doc g�n�ration
.PHONY: clean
clean:
	@cd doc && rm html -R



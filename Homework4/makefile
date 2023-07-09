ID1=$(shell cat id1.buildinfo)
ID2=$(shell cat id2.buildinfo)
USER=$(shell cat user.buildinfo)

IDS=$(ID1)_$(ID2)
IDS_MINUS=$(ID1)-$(ID2)

Submission/$(IDS_MINUS).zip: Submission/dry.pdf Submission/details.txt
	cp hw4_client.exe Submission
	cp Wet/* Submission
	cd Submission && zip -r $(IDS_MINUS).zip *
	cp $@ /mnt/c/Users/$(USER)/Desktop/

Submission/dry.pdf: Report/main.pdf
	cp $^ $@

Submission/details.txt:
	echo $(ID2) > $@


clean:
	rm -rf Submission/dry.pdf Submission/$(IDS_MINUS).zip Submission/details.txt Submission/Wet

%.buildinfo:
	read -p "Enter content for $@: " __INPUT__ && echo $$__INPUT__ > $@

init: id1.buildinfo id2.buildinfo user.buildinfo
	mkdir -p Report
	mkdir -p Submission
	cd Report && template use texgeneric && mv -n texgeneric main.tex
	cd Report && template use texignore && mv -n texignore .gitignore
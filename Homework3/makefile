ID1=$(shell cat id1.txt)
ID2=$(shell cat id2.txt)

IDS=$(ID1)_$(ID2)
IDS_MINUS=$(ID1)-$(ID2)

Submission/$(IDS_MINUS).zip: Submission/$(IDS).pdf
	zip $@ $^
	cp $@ /mnt/c/Users/yosef/Desktop/

Submission/$(IDS).pdf: Report/main.pdf
	cp $^ $@

clean:
	rm -rf Submission/$(IDS).pdf Submission/$(IDS_MINUS).zip


initialize:
	mkdir Report
	mkdir Submission
	cd Report && template use texgeneric && mv texgeneric main.tex
	cd Report && template use texignore && mv texignore .gitignore
	code Report/main.tex id1.txt id2.txt
default:
	pydoc3 -w zad2lista1
	pydoc3 -w zad2lista2

clean:
	rm ./*.html

test:
	python3 test21.py
	python3 test22.py	

pep:
	autopep8 --in-place --aggressive --aggressive test21.py
	autopep8 --in-place --aggressive --aggressive test22.py
	autopep8 --in-place --aggressive --aggressive zad2lista1.py
	autopep8 --in-place --aggressive --aggressive zad2lista2.py
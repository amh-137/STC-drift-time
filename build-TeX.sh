# build
# i dont know if its the reccomended way to do it
# but i always run pdflatex 4 times otherwise it
# sometimes wont compile correctly

# probably not needed for this simple project however

pdflatex -interaction=nonstopmode tex/STC.tex
pdflatex -interaction=nonstopmode tex/STC.tex
biber STC
pdflatex -interaction=nonstopmode tex/STC.tex
pdflatex tex/STC.tex


# clean 
rm *.aux *.log *.out *.toc *.bbl *.bcf *.blg *.run.xml

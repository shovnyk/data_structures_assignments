all: ans ans/bfs.pdf ans/bfs2.pdf
	mv -t . ans/bfs*.pdf
	convert -density 300 -trim bfs.pdf -quality 100 bfs.png
	convert -density 300 -trim bfs2.pdf -quality 100 bfs2.png

ans:
	mkdir ans

ans/bfs.pdf: bfs.tex
	pdflatex -output-directory ans bfs.tex

ans/bfs2.pdf: bfs2.tex
	pdflatex -output-directory ans bfs2.tex

clean:
	rm -f bfs.pdf bfs2.pdf
	rm -rf ans

$(FINAL_PATRONES_PDF): $(PATRONES_PARTS) $(CONFIG_INPUT) $(IMG_INPUT)
	$(LATEXMK) -jobname=$(PATRONES_BASE) $(PATRONES_BASE)/slides.tex

$(FINAL_INTRO_PDF): $(INTRO_PARTS) $(CONFIG_INPUT) $(IMG_INPUT)
	$(LATEXMK) -jobname=$(INTRO_BASE) $(INTRO_BASE)/slides.tex

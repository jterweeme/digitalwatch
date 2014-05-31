# 2014 Jasper ter Weeme

NAAM = klok1
BOARD = ask1ca
CHAIN = ask1ca_jtag

all: $(BOARD).pof

$(BOARD).sof:
	quartus_sh --flow compile $(NAAM) -c $(BOARD)

$(BOARD).flash: $(BOARD).sof
	sof2flash --epcs --input=$< --output=$@

$(BOARD).pof: $(BOARD).flash
	make -C software
	quartus_cpf --convert $(BOARD).cof

download:
	quartus_pgm $(CHAIN).cdf

urjtag:
	jtag ask1ca_urjtag.cmd

clean:
	rm -Rvf db incremental_db *.rpt *.summary *.smsg *.sof *.pof *.sopcinfo *.pin *.svf *.done *.jam *.jbc *.ttf *.map *.jdi *.flash *.qws *.rbf
	make -C software clean



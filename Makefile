# This is a comment
# I love comments

NAAM = klok1
BOARD = ask1ca
CHAIN = ask1ca_jtag

all:
	quartus_sh --flow compile $(NAAM) -c $(BOARD)

download:
	quartus_pgm $(CHAIN).cdf

urjtag:
	jtag ask1ca_urjtag.cmd

clean:
	rm -Rvf db incremental_db *.rpt *.summary *.smsg *.sof *.pof *.sopcinfo *.pin *.svf



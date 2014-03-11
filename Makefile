# This is a comment
# I love comments

NAAM = klok1
BOARD = ask1ca
CHAIN = ask1ca_jtag

all:
	quartus_sh --flow compile $(NAAM) -c $(BOARD)

upload:
	quartus_pgm $(CHAIN).cdf



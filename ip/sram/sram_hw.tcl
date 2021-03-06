# Jasper ter Weeme

package require -exact qsys 13.1

source ../common.tcl

mymodule sram sram


add_fileset_file sram.v VERILOG PATH sram.v TOP_LEVEL_FILE


add_interface           clock_reset clock end
set_interface_property  clock_reset clockRate 0
set_interface_property  clock_reset ENABLED true
set_interface_property  clock_reset EXPORT_OF ""
set_interface_property  clock_reset PORT_NAME_MAP ""
set_interface_property  clock_reset SVD_ADDRESS_GROUP ""
add_interface_port      clock_reset clk clk Input 1

add_interface           clock_reset_reset reset end
set_interface_property  clock_reset_reset associatedClock clock_reset
set_interface_property  clock_reset_reset synchronousEdges DEASSERT
set_interface_property  clock_reset_reset ENABLED true
set_interface_property  clock_reset_reset EXPORT_OF ""
set_interface_property  clock_reset_reset PORT_NAME_MAP ""
set_interface_property  clock_reset_reset SVD_ADDRESS_GROUP ""
add_interface_port      clock_reset_reset reset_n reset_n Input 1

add_interface           avalon_slave avalon end
set_interface_property  avalon_slave addressUnits WORDS
set_interface_property  avalon_slave associatedClock clock_reset
set_interface_property  avalon_slave associatedReset clock_reset_reset
set_interface_property  avalon_slave bitsPerSymbol 8
set_interface_property  avalon_slave burstOnBurstBoundariesOnly false
set_interface_property  avalon_slave burstcountUnits WORDS
set_interface_property  avalon_slave explicitAddressSpan 0
set_interface_property  avalon_slave holdTime 0
set_interface_property  avalon_slave isMemoryDevice true
set_interface_property  avalon_slave linewrapBursts false
set_interface_property  avalon_slave maximumPendingReadTransactions 0
set_interface_property  avalon_slave readLatency 0
set_interface_property  avalon_slave readWaitTime 1
set_interface_property  avalon_slave setupTime 0
set_interface_property  avalon_slave timingUnits Cycles
set_interface_property  avalon_slave writeWaitTime 0
set_interface_property  avalon_slave ENABLED true
set_interface_property  avalon_slave EXPORT_OF ""
set_interface_property  avalon_slave PORT_NAME_MAP ""
set_interface_property  avalon_slave SVD_ADDRESS_GROUP ""

add_interface_port          avalon_slave s_chipselect_n chipselect_n Input 1
add_interface_port          avalon_slave s_write_n write_n Input 1
add_interface_port          avalon_slave s_address address Input 17
add_interface_port          avalon_slave s_read_n read_n Input 1
add_interface_port          avalon_slave s_writedata writedata Input 8
add_interface_port          avalon_slave s_readdata readdata Output 8
add_interface_port          avalon_slave s_byteenable_n byteenable_n Input 1
set_interface_assignment    avalon_slave embeddedsw.configuration.isFlash 0
set_interface_assignment    avalon_slave embeddedsw.configuration.isMemoryDevice 1
set_interface_assignment    avalon_slave embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment    avalon_slave embeddedsw.configuration.isPrintableDevice 0

add_interface               conduit_end conduit end
set_interface_property      conduit_end associatedClock ""
set_interface_property      conduit_end associatedReset ""
set_interface_property      conduit_end ENABLED true
set_interface_property      conduit_end EXPORT_OF ""
set_interface_property      conduit_end PORT_NAME_MAP ""
set_interface_property      conduit_end SVD_ADDRESS_GROUP ""

add_interface_port conduit_end SRAM_DQ export Bidir 8
add_interface_port conduit_end SRAM_ADDR export Output 17
add_interface_port conduit_end SRAM_WE_n export Output 1
add_interface_port conduit_end SRAM_CE_n export Output 1
add_interface_port conduit_end SRAM_OE_n export Output 1


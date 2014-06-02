package require -exact qsys 13.1
set_module_property DESCRIPTION ""
set_module_property NAME vga2
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property GROUP Jasper
set_module_property AUTHOR ""
set_module_property DISPLAY_NAME vga2
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property ANALYZE_HDL AUTO
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property ELABORATION_CALLBACK elaborate_me
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL vga2
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
add_fileset_file display.vhd VHDL PATH display.vhd
add_fileset_file vga2.v VERILOG PATH vga2.v TOP_LEVEL_FILE
add_parameter CLOCK_OUTPUT INTEGER 1 "Clock Output"
add_parameter BITS_PER_COLOR INTEGER 8 "Bits per color"
set_parameter_property BITS_PER_COLOR AFFECTS_PORT_WIDTHS true
add_interface clock clock end
set_interface_property clock clockRate 0
set_interface_property clock ENABLED true
set_interface_property clock EXPORT_OF ""
set_interface_property clock PORT_NAME_MAP ""
set_interface_property clock SVD_ADDRESS_GROUP ""
add_interface_port clock csi_clk50 clk Input 1
add_interface conduit_end conduit end
set_interface_property conduit_end associatedClock clock
set_interface_property conduit_end associatedReset ""
set_interface_property conduit_end ENABLED true
set_interface_property conduit_end EXPORT_OF ""
set_interface_property conduit_end PORT_NAME_MAP ""
set_interface_property conduit_end SVD_ADDRESS_GROUP ""

add_interface_port conduit_end coe_hsync export Output 1
add_interface_port conduit_end coe_vsync export Output 1

proc elaborate_me {} {
    set the_clock_output [get_parameter_value CLOCK_OUTPUT]
    set the_bits_per_color [get_parameter_value BITS_PER_COLOR]
    add_interface_port conduit_end coe_red export Output $the_bits_per_color
    add_interface_port conduit_end coe_green export Output $the_bits_per_color
    add_interface_port conduit_end coe_blue export Output $the_bits_per_color
    
    if { $the_clock_output == 1 } {
        add_interface_port conduit_end coe_clk25 export Output 1
    }
}



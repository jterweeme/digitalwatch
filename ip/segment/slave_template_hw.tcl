# +-----------------------------------
# | module slave_template
# | 
set_module_property DESCRIPTION "Parameterizable input/output component with an Avalon-MM slave port"
set_module_property NAME slave_template
set_module_property VERSION 1.0
set_module_property GROUP Jasper
set_module_property AUTHOR JCJB
set_module_property DISPLAY_NAME segment
set_module_property TOP_LEVEL_HDL_FILE segment.v
set_module_property TOP_LEVEL_HDL_MODULE slave_template
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE false
set_module_property SIMULATION_MODEL_IN_VERILOG false
set_module_property SIMULATION_MODEL_IN_VHDL false
set_module_property SIMULATION_MODEL_HAS_TULIPS false
set_module_property SIMULATION_MODEL_IS_OBFUSCATED false
# | 
# +-----------------------------------


set_module_property ELABORATION_CALLBACK    elaborate_me


add_file segment.v {SYNTHESIS SIMULATION}


add_parameter ENABLE_SYNC_SIGNALS int 0 "Output syncronization signals"
set_parameter_property ENABLE_SYNC_SIGNALS DISPLAY_NAME "Syncronization signals"
set_parameter_property ENABLE_SYNC_SIGNALS GROUP "Register File Properties"
set_parameter_property ENABLE_SYNC_SIGNALS AFFECTS_PORT_WIDTHS true
set_parameter_property ENABLE_SYNC_SIGNALS ALLOWED_RANGES { "0:Disabled" "1:Enabled" }

add_interface clock_reset clock end
set_interface_property clock_reset ptfSchematicName ""

add_interface_port clock_reset clk clk Input 1
add_interface_port clock_reset reset reset Input 1
# | 
# +-----------------------------------

# +-----------------------------------
# | connection point s0
# | 
add_interface s0 avalon end
set_interface_property s0 holdTime 0
set_interface_property s0 linewrapBursts false
set_interface_property s0 minimumUninterruptedRunLength 1
set_interface_property s0 bridgesToMaster ""
set_interface_property s0 isMemoryDevice false
set_interface_property s0 burstOnBurstBoundariesOnly false
set_interface_property s0 addressSpan 16
set_interface_property s0 timingUnits Cycles
set_interface_property s0 setupTime 0
set_interface_property s0 writeWaitTime 0
set_interface_property s0 isNonVolatileStorage false
set_interface_property s0 addressAlignment DYNAMIC
set_interface_property s0 maximumPendingReadTransactions 0
set_interface_property s0 readWaitTime 0
set_interface_property s0 readLatency 3
set_interface_property s0 printableDevice false

set_interface_property s0 ASSOCIATED_CLOCK clock_reset

add_interface_port s0 slave_address address Input 4
add_interface_port s0 slave_read read Input 1
add_interface_port s0 slave_write write Input 1
add_interface_port s0 slave_readdata readdata Output 70
add_interface_port s0 slave_writedata writedata Input 70

add_interface user_interface conduit end

set_interface_property user_interface ASSOCIATED_CLOCK clock_reset

add_interface_port user_interface user_dataout_0 export Output 8
add_interface_port user_interface user_dataout_1 export Output 8
#add_interface_port user_interface user_dataout_2 export Output -1
#add_interface_port user_interface user_dataout_3 export Output -1
#add_interface_port user_interface user_dataout_4 export Output -1
#add_interface_port user_interface user_dataout_5 export Output -1
#add_interface_port user_interface user_dataout_6 export Output -1
#add_interface_port user_interface user_dataout_7 export Output -1
#add_interface_port user_interface user_dataout_8 export Output -1
add_interface_port user_interface user_write export Output 1
add_interface_port user_interface user_read export Output 1
add_interface_port user_interface user_chipselect export Output 4
# | 
# +-----------------------------------


proc elaborate_me {}  {
  set the_enable_sync_signals [get_parameter_value ENABLE_SYNC_SIGNALS]
  #set the_mode_0 [get_parameter_value MODE_0]
  #set the_mode_1 [get_parameter_value MODE_1]
  #set the_mode_2 [get_parameter_value MODE_2]
  #set the_mode_3 [get_parameter_value MODE_3]
  #set the_mode_4 [get_parameter_value MODE_4]
  #set the_mode_5 [get_parameter_value MODE_5]
  #set the_mode_6 [get_parameter_value MODE_6]
  #set the_mode_7 [get_parameter_value MODE_7]
  #set the_mode_8 [get_parameter_value MODE_8]
   
  expr { ($the_enable_sync_signals == 1) ? [set_port_property user_chipselect TERMINATION false] : [set_port_property user_chipselect TERMINATION true] }
  expr { ($the_enable_sync_signals == 1) ? [set_port_property user_read TERMINATION false] : [set_port_property user_read TERMINATION true] }
  expr { ($the_enable_sync_signals == 1) ? [set_port_property user_write TERMINATION false] : [set_port_property user_write TERMINATION true] }
  
}

# Jasper ter Weeme

package require -exact qsys 13.1

proc mymodule {name toplvl} {
    set_module_property DESCRIPTION ""
    set_module_property NAME $name
    set_module_property VERSION 1.0
    set_module_property INTERNAL false
    set_module_property OPAQUE_ADDRESS_MAP true
    set_module_property GROUP Jasper
    set_module_property AUTHOR ""
    set_module_property DISPLAY_NAME $name
    set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
    set_module_property EDITABLE true
    set_module_property ANALYZE_HDL AUTO
    set_module_property REPORT_TO_TALKBACK false
    set_module_property ALLOW_GREYBOX_GENERATION false
    add_fileset quartus_synth QUARTUS_SYNTH "" "Quartus Synthesis"
    set_fileset_property quartus_synth TOP_LEVEL $toplvl
    set_fileset_property quartus_synth ENABLE_RELATIVE_INCLUDE_PATHS false
}



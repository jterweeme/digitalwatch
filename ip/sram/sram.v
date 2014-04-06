/*
module sram(clk, reset_n, s_chipselect_n, s_byteenable_n, s_write_n,
    s_read_n, s_address, s_writedata, s_readdata, SRAM_DQ, SRAM_ADDR,
    SRAM_UB_n, SRAM_LB_n, SRAM_WE_n, SRAM_CE_n, SRAM_OE_n);

input clk, reset_n, s_chipselect_n, s_byteenable_n, s_write_n, s_read_n;
input [16:0] s_address;
input [7:0] s_writedata;
output [7:0] s_readdata;
output SRAM_CE_n, SRAM_OE_n, SRAM_LB_n, SRAM_UB_n, SRAM_WE_n;
output [16:0] SRAM_ADDR;
inout [7:0] SRAM_DQ;

assign	SRAM_DQ 				=	SRAM_WE_n ? 'hz : s_writedata;
assign	s_readdata				=	SRAM_DQ;
assign	SRAM_ADDR				=	s_address;
assign	SRAM_WE_n				=	s_write_n;
assign	SRAM_OE_n				=	s_read_n;
assign	SRAM_CE_n				=	s_chipselect_n;
assign	{SRAM_UB_n,SRAM_LB_n}	=	s_byteenable_n;

endmodule
*/

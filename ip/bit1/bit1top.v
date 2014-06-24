`timescale 1ns / 1ps

module bit1top (
    input [2:0] address,
    input chipselect,
    input clk,
    input reset_n,
    input write_n,
    input [7:0] writedata,
    inout wire bidir_port,
    output reg coe_clk,
    output reg coe_reset,
    output reg [7:0] readdata
);
    wire clk_en, data_in, read_mux_out, wr_strobe;
    reg data_dir, data_out;
    assign read_mux_out = ({1 {(address == 0)}} & data_in) | ({1 {(address == 1)}} & data_dir);
    assign wr_strobe = chipselect && ~write_n;
    assign bidir_port = data_dir ? data_out : 1'bZ;
    assign data_in = bidir_port;

    always @(posedge clk or negedge reset_n)
        readdata <= ~reset_n ? 0 : {8'b0 | read_mux_out};

    always @(posedge clk or negedge reset_n) begin
        if (reset_n == 0) begin
            data_out <= 0;
            coe_clk <= 0;
            coe_reset <= 0;
        end else if (wr_strobe) begin
            data_out <= address == 0 ? writedata : data_out;
            coe_clk <= address == 2 ? writedata : coe_clk;
            coe_reset <= address == 3 ? writedata : coe_reset;
        end
    end

    always @(posedge clk or negedge reset_n) begin
        if (reset_n == 0)
            data_dir <= 0;
        else if (chipselect && ~write_n && (address == 1))
            data_dir <= writedata;
    end
endmodule





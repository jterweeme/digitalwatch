`timescale 1ns / 1ps

module bit1top (
    input [2:0] address,
    input chipselect,
    input clk,
    input reset_n,
    input write_n,
    input [31:0] writedata,
    inout wire bidir_port,
    output reg [31:0] readdata
);
    wire clk_en, data_in, read_mux_out, wr_strobe;
    reg data_dir, data_out;
    assign clk_en = 1;
    assign read_mux_out = ({1 {(address == 0)}} & data_in) | ({1 {(address == 1)}} & data_dir);
    assign wr_strobe = chipselect && ~write_n;
    assign bidir_port = data_dir ? data_out : 1'bZ;
    assign data_in = bidir_port;

    always @(posedge clk or negedge reset_n) begin
        if (reset_n == 0)
            readdata <= 0;
        else if (clk_en)
            readdata <= {32'b0 | read_mux_out};
    end

    always @(posedge clk or negedge reset_n) begin
        if (reset_n == 0)
            data_out <= 0;
        else if (clk_en && wr_strobe)
            data_out <= (address == 5)? data_out & ~writedata : (address == 4)? data_out | writedata : (address == 0)? writedata : data_out;
    end

    always @(posedge clk or negedge reset_n) begin
        if (reset_n == 0)
            data_dir <= 0;
        else if (chipselect && ~write_n && (address == 1))
            data_dir <= writedata;
    end
endmodule





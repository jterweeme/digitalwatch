module bitout1(
    input csi_clk,
    input csi_reset,
    input [1:0] avs_s1_address,
    input avs_s1_read,
    output wire [31:0] avs_s1_readdata,
    input avs_s1_write,
    input [31:0] avs_s1_writedata,
    output wire coe_bit
);
    reg data_out;
    wire read_mux_out;
    assign read_mux_out = {1 {(avs_s1_address == 0)}} & data_out;
    assign avs_s1_readdata = {32'b0 | read_mux_out};
    assign coe_bit = data_out;

    always @(posedge csi_clk or posedge csi_reset) begin
        if (csi_reset)
            data_out <= 1'b0;
        else if (avs_s1_write)
            data_out <= avs_s1_writedata[0];
    end
endmodule



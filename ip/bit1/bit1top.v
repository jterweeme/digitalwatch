module bit1top(
    input csi_clk,
    input csi_reset,
    input avs_s1_read,
    output reg [7:0] avs_s1_readdata,
    input avs_s1_write,
    input [7:0] avs_s1_writedata,
    inout reg coe_bit
);

    always @(posedge csi_clk) begin
        if (csi_reset)
            coe_bit <= 1'b0;
        else if (avs_s1_write)
            coe_bit <= avs_s1_writedata;
        else if (avs_s1_read)
            avs_s1_readdata <= coe_bit;
    end

endmodule



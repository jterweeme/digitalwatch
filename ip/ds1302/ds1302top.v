
module ds1302top(
    input csi_clk,
    input csi_reset_n,
    input avs_s1_read,
    input avs_s1_write,
    output reg [7:0] avs_s1_readdata,
    input [7:0] avs_s1_writedata,
    output reg coe_clk,
    output reg coe_reset,
    inout reg coe_io);

    always @(posedge csi_clk or negedge csi_reset_n) begin
        if (~csi_reset_n) begin
            coe_clk <= 1'b1;
            coe_reset <= 1'b1;
        end else if (avs_s1_write) begin
            coe_clk <= avs_s1_writedata[0:0];
            coe_reset <= avs_s1_writedata[1:1];
        end else if (avs_s1_read) begin
            avs_s1_readdata[1:0] <= {coe_clk, coe_reset};
        end
    end

endmodule



/*
2014 Jasper ter Weeme
*/

module leds(
    input csi_clk,
    input csi_reset,
    input [3:0] avs_s1_address,
    input avs_s1_read,
    output reg [7:0] avs_s1_readdata,
    input avs_s1_write,
    input [7:0] avs_s1_writedata,
    output reg [7:0] user_dataout_0);

    always @(posedge csi_clk or posedge csi_reset) begin
        if (csi_reset)
            user_dataout_0[7:0] <= 8'b0;
        else if (avs_s1_write)
            user_dataout_0[7:0] <= avs_s1_writedata[7:0];
        else if (avs_s1_read)
            avs_s1_readdata[7:0] <= user_dataout_0[7:0];
    end
endmodule


/*
2014 Jasper ter Weeme
*/

module leds(input clk, input reset, input [3:0] slave_address,
    input slave_write, input [7:0] slave_writedata,
    output reg [7:0] user_dataout_0);

    always @(posedge clk or posedge reset) begin
        if (reset == 1)
            user_dataout_0[7:0] <= 0;
        else if (slave_write == 1)
            user_dataout_0[7:0] <= slave_writedata[7:0];
    end
endmodule


/*
Jasper ter Weeme
*/

module leds(input clk, input reset, input [3:0] slave_address, input slave_read,
    input slave_write, output reg [7:0] slave_readdata, input [7:0] slave_writedata,
    input slave_byteenable, output wire [7:0] user_dataout_0);

    wire [15:0] address_decode;

    assign address_decode[0] = (slave_address == 4'b0000) & (slave_write | slave_read);
    foo register_0(clk,reset,slave_writedata, slave_write&address_decode[0], user_dataout_0);
endmodule

module foo(input clk, input reset, input [7:0] data_in, input write, output reg [7:0] data_out);
    always @ (posedge clk or posedge reset) begin
        if(reset == 1)
            data_out[7:0] <= 0;
        else if (write == 1)
            data_out[7:0] <= data_in[7:0];
    end
endmodule


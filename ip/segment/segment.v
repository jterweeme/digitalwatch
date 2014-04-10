module slave_template (
clk,
reset,
slave_address,
slave_read,
slave_write,
slave_readdata,
slave_writedata,
slave_byteenable,
user_dataout_0,
user_dataout_1,
user_chipselect,
user_byteenable,
user_write,
user_read
);

parameter DATA_WIDTH = 8;
parameter ENABLE_SYNC_SIGNALS = 0;
input clk, reset, slave_read, slave_write, slave_byteenable;
    input [3:0] slave_address;
    output reg [7:0] slave_readdata;
    input [7:0] slave_writedata;
    output wire [7:0] user_dataout_0, user_dataout_1;
    output wire [15:0] user_chipselect;
    output wire user_byteenable, user_write, user_read;

    wire [7:0] user_dataout_d0, user_dataout_d1, user_dataout_d2, user_dataout_d3, user_dataout_d4,
            user_dataout_d5, user_dataout_d6, user_dataout_d7, user_dataout_d8;

    wire internal_byteenable;
    reg internal_byteenable_d1;
    wire [15:0] address_decode;
    reg [15:0] address_decode_d1;
    wire [3:0] address_bank_decode;
    reg [3:0] address_bank_decode_d1;
    reg slave_read_d1;
    reg slave_read_d2;
    reg slave_write_d1;

    reg [7:0] user_datain_0_d1, user_datain_1_d1, user_datain_2_d1, user_datain_3_d1,
                user_datain_4_d1, user_datain_5_d1, user_datain_6_d1;
    reg [7:0] user_datain_7_d1;
    reg [7:0] user_datain_8_d1;
    reg [7:0] mux_first_stage_a;
    reg [7:0] mux_first_stage_b;
    reg [7:0] mux_first_stage_c;
    reg [7:0] mux_first_stage_d;

    reg [26:0] cnt;
    reg [7:0] blinkmask_r, en_r, segdata_r;

generate
if (DATA_WIDTH == 8) begin
assign internal_byteenable = 1'b1;
end else begin
assign internal_byteenable = slave_byteenable;
end
endgenerate

    always @(posedge clk or posedge reset) begin
        cnt <= (reset == 1) ? 0 : cnt + 1;
    end

    always @(cnt[26:17]) begin
        if (cnt[19:17] == 0 & (cnt[26:26] == 1 | user_dataout_d8[0:0] == 0)) begin
            en_r = 8'b01111111;
            segdata_r = user_dataout_d0;
        end

        else if (cnt[19:17] == 1 & (cnt[26:26] == 1 | user_dataout_d8[1:1] == 0)) begin
            en_r = 8'b10111111;
            segdata_r = user_dataout_d1;
        end

        else if (cnt[19:17] == 2 & (cnt[26:26] == 1 | user_dataout_d8[2:2] == 0)) begin
            en_r = 8'b11011111;
            segdata_r = user_dataout_d2;
        end

        else if (cnt[19:17] == 3 & (cnt[26:26] == 1 | user_dataout_d8[3:3] == 0)) begin
            en_r = 8'b11101111;
            segdata_r = user_dataout_d3;
        end

        else if (cnt[19:17] == 4 & (cnt[26:26] == 1 | user_dataout_d8[4:4] == 0)) begin
            en_r = 8'b11110111;
            segdata_r = user_dataout_d4;
        end

        else if (cnt[19:17] == 5 & (cnt[26:26] == 1 | user_dataout_d8[5:5] == 0)) begin
            en_r = 8'b11111011;
            segdata_r = user_dataout_d5;
        end

        else if (cnt[19:17] == 6 & (cnt[26:26] == 1 | user_dataout_d8[6:6] == 0)) begin
            en_r = 8'b11111101;
            segdata_r = user_dataout_d6;
        end

        else if (cnt[19:17] == 7 & (cnt[26:26] == 1 | user_dataout_d8[7:7] == 0)) begin
            en_r = 8'b11111110;
            segdata_r = user_dataout_d7;
        end

        else begin
            en_r = 8'b11111111;
        end
    end

    assign user_dataout_0 = segdata_r;
    assign user_dataout_1 = en_r;
assign address_decode[0] = (slave_address == 4'b0000) & slave_write;
assign address_decode[1] = (slave_address == 4'b0001) & slave_write;
assign address_decode[2] = (slave_address == 4'b0010) & slave_write;
assign address_decode[3] = (slave_address == 4'b0011) & slave_write;
assign address_decode[4] = (slave_address == 4'b0100) & slave_write;
assign address_decode[5] = (slave_address == 4'b0101) & slave_write;
assign address_decode[6] = (slave_address == 4'b0110) & slave_write;
assign address_decode[7] = (slave_address == 4'b0111) & slave_write;
assign address_decode[8] = (slave_address == 4'b1000) & slave_write;
assign address_bank_decode[0] = (address_decode_d1[3:0] != 0)? 1'b1 : 1'b0;
assign address_bank_decode[1] = (address_decode_d1[7:4] != 0)? 1'b1 : 1'b0;
assign address_bank_decode[2] = (address_decode_d1[11:8] != 0)? 1'b1 : 1'b0;
assign address_bank_decode[3] = (address_decode_d1[15:12] != 0)? 1'b1 : 1'b0;

always @ (posedge clk or posedge reset) begin
if (reset == 1) begin
slave_read_d1 <= 0;
slave_read_d2 <= 0;
slave_write_d1 <= 0;
address_decode_d1 <= 0;
address_bank_decode_d1 <= 0;
internal_byteenable_d1 <= 0;
end else begin
slave_read_d1 <= slave_read;
slave_read_d2 <= slave_read_d1;
slave_write_d1 <= slave_write;
internal_byteenable_d1 <= internal_byteenable;

if((slave_read == 1) | (slave_write == 1)) begin
address_decode_d1 <= address_decode;
end

if(slave_read_d1 == 1) begin
address_bank_decode_d1 <= address_bank_decode;
end
end
end

register_with_bytelanes register_0 (clk,reset,slave_writedata,slave_write&address_decode[0],internal_byteenable,user_dataout_d0);
register_with_bytelanes register_1 (clk,reset,slave_writedata,slave_write&address_decode[1],internal_byteenable,user_dataout_d1);
register_with_bytelanes register_2 (clk,reset,slave_writedata,slave_write&address_decode[2],internal_byteenable,user_dataout_d2);
register_with_bytelanes register_3 (clk,reset,slave_writedata,slave_write&address_decode[3],internal_byteenable,user_dataout_d3);
register_with_bytelanes register_4 (clk,reset,slave_writedata,slave_write&address_decode[4],internal_byteenable,user_dataout_d4);
register_with_bytelanes register_5 (clk,reset,slave_writedata,slave_write&address_decode[5],internal_byteenable,user_dataout_d5);
register_with_bytelanes register_6 (clk,reset,slave_writedata,slave_write&address_decode[6],internal_byteenable,user_dataout_d6);
register_with_bytelanes register_7 (clk,reset,slave_writedata,slave_write&address_decode[7],internal_byteenable,user_dataout_d7);
register_with_bytelanes register_8 (clk,reset,slave_writedata,slave_write&address_decode[8],internal_byteenable,user_dataout_d8);

assign user_write = slave_write_d1;
assign user_read = slave_read;
assign user_chipselect = (slave_write_d1 == 1)? address_decode_d1 : address_decode;
assign user_byteenable = (slave_write_d1 == 1)? internal_byteenable_d1 : internal_byteenable;
endmodule

module register_with_bytelanes (clk, reset, data_in, write, byte_enables, data_out);
parameter DATA_WIDTH = 8;
input clk, reset;
input [7:0] data_in;
input write;
input [0:0] byte_enables;
output reg [7:0] data_out;

generate
genvar LANE;
for(LANE = 0; LANE < (DATA_WIDTH/8); LANE = LANE+1)
begin: register_bytelane_generation	
always @ (posedge clk or posedge reset)
begin
if (reset == 1)
data_out[(LANE*8)+7:(LANE*8)] <= 0;
else if((byte_enables[LANE] == 1) & (write == 1))
data_out[(LANE*8)+7:(LANE*8)] <= data_in[(LANE*8)+7:(LANE*8)];
end
end
endgenerate
endmodule



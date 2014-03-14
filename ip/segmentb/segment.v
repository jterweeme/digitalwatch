/*
Jasper ter Weeme
*/

module segment(input clk, input reset, input [3:0] slave_address,
    input slave_write, input [63:0] slave_writedata, input [7:0] slave_byteenable,
    output [7:0] en, output [7:0] seg_data);

    reg [26:0] cnt;
    reg [7:0] en_r, mux_first_stage_a, mux_first_stage_b, zeg_data_r;
    reg [7:0] mux_first_stage_c, mux_first_stage_d, user_datain_0_d1;
    reg [63:0] seg_data_r;
    wire [15:0] address_decode;
    reg slave_read_d1, slave_read_d2;
    reg [15:0] address_decode_d1;
    wire [3:0] address_bank_decode;
    reg [3:0] address_bank_decode_d1;
    wire [7:0] blinkmask;

    assign en = en_r;
    assign seg_data = zeg_data_r;
    assign address_decode[0] = (slave_address == 4'b0000) & slave_write;
    assign address_bank_decode[0] = (address_decode_d1[3:0] != 0)? 1'b1 : 1'b0;
    assign blinkmask = 8'b11111111;

    always @(posedge clk or posedge reset)  begin
        cnt <= (reset == 1) ? 0 : cnt + 1;
    end

    always @(cnt[26:17]) begin
        case (cnt[19:17])
            0: begin
                if (cnt[26:26] == 1 | blinkmask[0] == 0)
                    begin
                        en_r = 8'b01111111;
                        zeg_data_r = seg_data_r[7:0];
                    end
                else
                    begin
                        en_r = 8'b11111111;
                    end

            end
            1: begin
                en_r = 8'b10111111;
                zeg_data_r = seg_data_r[15:8];
            end
            2: begin
                en_r = 8'b11011111;
                zeg_data_r = seg_data_r[23:16];
            end
            3: begin
                en_r = 8'b11101111;
                zeg_data_r = seg_data_r[31:24];
            end
            4: begin
                en_r = 8'b11110111;
                zeg_data_r = seg_data_r[39:32];
            end
            5: begin
                en_r = 8'b11111011;
                zeg_data_r = seg_data_r[47:40];
            end
            6: begin
                en_r = 8'b11111101;
                zeg_data_r = seg_data_r[55:48];
            end
            7: begin
                en_r = 8'b11111110;
                zeg_data_r = seg_data_r[63:56];
            end
        endcase
    end

    always @(posedge clk or posedge reset) begin
        if (reset == 1)
            seg_data_r[63:0] <= 0;
        else if (slave_write == 1)
            seg_data_r[63:0] <= slave_writedata[63:0];
    end
endmodule




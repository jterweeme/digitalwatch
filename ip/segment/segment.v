/*
Jasper ter Weeme
*/

module segment(clk, reset, slave_address, slave_read,
    slave_write, slave_readdata,
    slave_writedata, slave_byteenable, en, seg_data);

    input clk, reset, slave_read, slave_write;
    input [7:0] slave_byteenable;
    input [3:0] slave_address;
    output reg [63:0] slave_readdata;
    input [63:0] slave_writedata;
    output[7:0] en, seg_data;
    reg [26:0] cnt;
    reg [7:0] en_r, mux_first_stage_a, mux_first_stage_b, zeg_data_r;
    reg [7:0] mux_first_stage_c, mux_first_stage_d, user_datain_0_d1;
    wire [63:0] seg_data_r;
    wire [15:0] address_decode;
    reg slave_read_d1, slave_read_d2;
    reg [15:0] address_decode_d1;
    wire [3:0] address_bank_decode;
    reg [3:0] address_bank_decode_d1;
    wire [7:0] blinkmask;

    assign en = en_r;
    assign seg_data = zeg_data_r;
    assign address_decode[0] = (slave_address == 4'b0000) & (slave_write | slave_read);
    assign address_bank_decode[0] = (address_decode_d1[3:0] != 0)? 1'b1 : 1'b0;
    assign blinkmask = 8'b11111111;

    always @(posedge clk or posedge reset)
    begin
    if (reset == 1)
        cnt <= 0;
    else
        cnt <= cnt + 1;
    end

    always @(cnt[26:17])
    begin
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

    register_with_bytelaans registerx(clk, reset, slave_writedata,
            slave_write & address_decode[0], seg_data_r);

    always @ (posedge clk or posedge reset)
    begin
    if (reset == 1)
        slave_readdata <= 0;
    else
    begin
        if (slave_read_d1 == 1)
        begin
            case (address_decode_d1[3:0])
                4'b0001: mux_first_stage_a <= (2 == 3) ? seg_data : user_datain_0_d1;
            endcase
        end

        if (slave_read_d2 == 1)
        begin
            case (address_bank_decode_d1[3:0])
                4'b0001: slave_readdata <= mux_first_stage_a;
                4'b0010: slave_readdata <= mux_first_stage_b;
                4'b0100: slave_readdata <= mux_first_stage_c;
                4'b1000: slave_readdata <= mux_first_stage_d;
            endcase
        end
    end
    end

endmodule

module register_with_bytelaans(clk, reset, data_in, write, data_out);
    input clk, reset, write;
    input [63:0] data_in;
    output reg [63:0] data_out;

    always @ (posedge clk or posedge reset)
    begin
    if (reset == 1)
        data_out [63:0] <= 0;
    else if ((1 == 1) & (write == 1))
        data_out [63:0] <= data_in[63:0];
    end
endmodule


